// MiSTer_MVC2: Main_MiSTer with a loop that runs the ARM build of Marvel vs.
// Capcom 2 under the MVC2 core, or under the user's Pinedra core loaded as
// "MVC2 Pinedra" (an .mgl's setname). It keeps the OSD and the pads (passed to
// the game through shared memory), turns the MVC2 core's frame reader on, and
// goes back to the menu core when the game ends.

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sched.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "cfg.h"
#include "file_io.h"
#include "fpga_io.h"
#include "frame_timer.h"
#include "input.h"
#include "menu.h"
#include "offload.h"
#include "osd.h"
#include "user_io.h"
#include "video.h"
#include "support.h"
#include "mvc2_joy_shm.h"

const char *version = "$VER:" VDATE;

volatile sig_atomic_t g_child_pid = -1;

namespace {

constexpr const char *kCoreName  = "MVC2";
constexpr const char *kGLName    = "MVC2 Pinedra";
constexpr const char *kHome      = "/media/fat/games/mvc2";
constexpr const char *kGame      = "/media/fat/games/mvc2/mvc2";
constexpr const char *kGameGL    = "/media/fat/games/mvc2/mvc2-pinedra";
constexpr const char *kTestRun   = "/tmp/mvc2-run";
constexpr const char *kLogDir    = "/media/fat/games/mvc2/logs";
constexpr const char *kLog       = "/media/fat/games/mvc2/logs/wrapper.log";
constexpr const char *kRunLog    = "/media/fat/games/mvc2/logs/last-run.log";
constexpr off_t       kCtrlPhys  = 0x3A000000;

constexpr int kBitEnable  = 9;
constexpr int kBitRestart = 20;
constexpr int kBitExit    = 22;

FILE *g_log;
const char *g_mode;     // --arcade or --dc, from the .mgl's <file path="arcade"> or "dc"; else mvc2.cfg's mode
volatile uint32_t *g_ctrl;
MisterJoyShm *g_joy;
volatile sig_atomic_t g_signal;
bool g_gl;
int g_vt_fd = -1;

void logf(const char *fmt, ...)
{
	if (!g_log) return;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	fprintf(g_log, "[%ld.%03ld] ", (long)ts.tv_sec, ts.tv_nsec / 1000000);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(g_log, fmt, ap);
	va_end(ap);
	fputc('\n', g_log);
	fflush(g_log);
}

void on_signal(int sig)
{
	g_signal = sig;
	pid_t child = (pid_t)g_child_pid;
	if (child > 0) kill(child, SIGTERM);
}

void ctrl_open()
{
	int fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
	if (fd < 0) return;
	void *p = mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, kCtrlPhys);
	close(fd);
	if (p != MAP_FAILED) g_ctrl = (volatile uint32_t *)p;
}

// no frame: the core shows black until the game posts one
void ctrl_clear()
{
	if (g_ctrl) g_ctrl[0] = 0;
}

// keep fbcon from drawing into fb0, where the game puts its frames
void console_graphics(bool on)
{
	if (g_vt_fd < 0) g_vt_fd = open("/dev/tty0", O_RDWR | O_CLOEXEC);
	if (g_vt_fd >= 0 && ioctl(g_vt_fd, KDSETMODE, on ? KD_GRAPHICS : KD_TEXT))
		logf("KDSETMODE %d failed: %s", on, strerror(errno));
}

void joy_open()
{
	int fd = open(MVC2_JOY_SHM_PATH, O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
	if (fd < 0) return;
	if (ftruncate(fd, sizeof(MisterJoyShm)) == 0)
	{
		void *p = mmap(nullptr, sizeof(MisterJoyShm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (p != MAP_FAILED) g_joy = (MisterJoyShm *)p;
	}
	close(fd);
	if (!g_joy) return;
	memset(g_joy, 0, sizeof(*g_joy));
	g_joy->magic = MVC2_JOY_SHM_MAGIC;
	g_joy->version = MVC2_JOY_SHM_VERSION;
}

// Under Pinedra's core the OSD asks for the game's own buttons, in the game's order (bits 4-13), then a second A1 and
// A2 (14, 15), in place of the core's names (A B X Y L R Select Start L2 R2 L3 R3): Main_MiSTer's override, as an
// MRA's <buttons names>. A pad not defined yet takes them from its menu map, by name or by place (gamepad_defaults)
// the same: Y X L over B A R as LP HP A1 over LK HK A2, Select the coin (the menu map has no L2, R2, L3 or R3).
// Their maps have their own name, as a map by Pinedra's names gives its bits other buttons.
char g_buttons[]  = "LP,HP,A1,LK,HK,A2,Coin,Start,Test,Service,2nd A1,2nd A2";
char g_defaults[] = "Y,X,L,B,A,R,Select,Start,,,,";
constexpr const char *kMapName = "MVC2 Buttons";

// the game's order but Start before Coin, and the second A1 and A2 as A1 and A2
uint32_t pinedra_to_mvc2(uint32_t m)
{
	return (m & 0x33FFu) | (m >> 1 & 0x400) | (m << 1 & 0x800) | (m >> 8 & 0x40) | (m >> 6 & 0x200);
}

// A pad's maps by Pinedra's names (MVC2 Pinedra's: its buttons, its advanced map's button masks, and a key map as it
// is) copied once to the game's buttons, the old ones only read: Y X L B A R Select Start L3 R3 L2 R2 as LP HP A1 LK
// HK A2 Coin Start Test Service and the second A1 and A2
void maps_move()
{
	static const uint8_t from[] = { 7, 6, 8, 5, 4, 9, 10, 11, 14, 15, 12, 13 };
	union
	{
		uint32_t map[NUMBUTTONS];
		advancedButtonMap adv[ADVANCED_MAP_MAX];
		uint16_t jk[1024];
	} was, now;
	DIR *d = opendir(getFullPath(CONFIG_DIR "/inputs"));
	if (!d) return;
	size_t n = strlen(kGLName);
	while (struct dirent *e = readdir(d))
	{
		size_t l = strlen(e->d_name);
		if (l < n + 7 || l > 200 || strncasecmp(e->d_name, kGLName, n)) continue;
		const char *rest = e->d_name + n, *end = e->d_name + l - 7;
		int size;
		if (!strncmp(rest, "_input_", 7) && !strcmp(end, "_v3.map")) size = sizeof(was.map);
		else if (!strncmp(rest, "_input_", 7) && !strcmp(end, "_jk.map")) size = sizeof(was.jk);
		else if (!strncmp(rest, "_advanced_input_", 16) && !strcmp(end, "_v1.map")) size = sizeof(was.adv);
		else continue;
		char old_name[256], new_name[256];
		snprintf(old_name, sizeof(old_name), "inputs/%s", e->d_name);
		snprintf(new_name, sizeof(new_name), "inputs/%s%s", kMapName, rest);
		if (FileLoadConfig(new_name, nullptr, 0) > 0 || FileLoadConfig(old_name, nullptr, 0) != size ||
		    FileLoadConfig(old_name, &was, size) != size)
			continue;
		now = was;
		if (size == (int)sizeof(was.map))
			for (int i = 0; i < 12; i++) now.map[4 + i] = was.map[from[i]];
		if (size == (int)sizeof(was.adv))
			for (int a = 0; a < ADVANCED_MAP_MAX; a++)
			{
				now.adv[a].button_mask &= ~0xFFF0u;
				for (int i = 0; i < 12; i++)
					if (was.adv[a].button_mask >> from[i] & 1) now.adv[a].button_mask |= 0x10u << i;
			}
		if (FileSaveConfig(new_name, &now, size) == size)
			logf("buttons: %s copied to the game's buttons as %s", old_name, new_name);
		else
			FileDeleteConfig(new_name);
	}
	closedir(d);
}

// The user's Pinedra Config settings (pinedra/settings/all.cfg) that Pinedra's
// settings.sh gives its games: the CRT visible area and the logo, to the driver
// through its environment, and the size the game opens at for that area.
void pinedra_settings()
{
	int area = 0;
	bool splash = true;
	FILE *f = fopen("/media/fat/pinedra/settings/all.cfg", "re");
	if (f)
	{
		char line[128];
		while (fgets(line, sizeof(line), f))
		{
			int v;
			if (sscanf(line, "crt_area=%d", &v) == 1 && v >= 50 && v <= 99) area = v;
			else if (!strncmp(line, "splash=0", 8)) splash = false;
		}
		fclose(f);
	}
	if (area)
	{
		char v[16];
		snprintf(v, sizeof(v), "%d", area);
		setenv("PND_SAFE", v, 1);
		snprintf(v, sizeof(v), "%dx%d", ((640 * area + 50) / 100) & ~1, ((480 * area + 50) / 100) & ~1);
		setenv("MVC2_PND_SIZE", v, 1);
	}
	if (!splash) setenv("PND_SPLASH", "0", 1);
	logf("pinedra: area %d, logo %d", area, splash);
}

void joy_close()
{
	if (g_joy) munmap(g_joy, sizeof(MisterJoyShm));
	g_joy = nullptr;
	unlink(MVC2_JOY_SHM_PATH);
}

void game_stop()
{
	ctrl_clear();
	if (!g_gl) user_io_status_set("[9]", 0);
	console_graphics(false);
}

[[noreturn]] void back_to_menu()
{
	logf("back to menu");
	game_stop();
	joy_close();
	input_switch(0);
	sync();
	if (fpga_load_rbf_no_restart("menu.rbf"))
	{
		logf("menu.rbf failed to load");
		reboot(1);
	}
	const char *menu = getFullPath("MiSTer");
	if (g_log) fclose(g_log);
	execl(menu, menu, "menu.rbf", "", nullptr);
	reboot(1);
	for (;;) pause();
}

// msg: up to four lines
[[noreturn]] void fail(const char *msg, unsigned seconds = 5)
{
	logf("error: %s", msg);
	OsdSetTitle(kCoreName, 0);
	OsdSetSize(8);
	OsdEnable(0);
	for (int i = 0; i < 8; i++) OsdWrite(i, "", 0, 0);
	OsdWrite(1, " Marvel vs. Capcom 2", 0, 0);
	char line[64];
	for (int row = 3; *msg && row < 7; row++)
	{
		size_t n = strcspn(msg, "\n");
		snprintf(line, sizeof(line), "%.*s", (int)n, msg);
		OsdWrite(row, line, 0, 0);
		msg += n + (msg[n] == '\n');
	}
	OsdUpdate();
	sleep(seconds);
	OsdDisable();
	back_to_menu();
}

// The game's first start unpacking its data (the arcade's zip, a disc image),
// shown as Main's ProgressMessage shows a load: the name and a bar, here with
// the size. Drawn again each second, or the message's own timeout hides it.
int g_unpack_step = -1;
uint64_t g_unpack_drawn;

uint64_t now_ms()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void unpack_show()
{
	static const char glyph[] = { '\x8C', '\x8E', '\x8F', '\x90', '\x91', '\x7F' };
	const int cells = 27, n = (int)sizeof(glyph), steps = cells * n - 1;
	if (!g_joy || !g_joy->unpack_kb_total) return;
	uint32_t total = g_joy->unpack_kb_total, done = g_joy->unpack_kb;
	if (done > total) done = total;
	int step = (int)((uint64_t)done * steps / total);
	uint64_t now = now_ms();
	if (g_unpack_step == steps || (step == g_unpack_step && now - g_unpack_drawn < 1000)) return;
	bool first = g_unpack_step < 0;
	g_unpack_step = step;
	g_unpack_drawn = now;

	char name[28], bar[cells + 1] = {};
	snprintf(name, sizeof(name), "%s", (const char *)g_joy->unpack_name);
	for (int i = 0; i <= step / n; i++) bar[i] = i < step / n ? '\x7F' : glyph[step % n];
	char text[160];
	snprintf(text, sizeof(text), "\n\n %s\n %s\n\n %u of %u MB\n Only at the first start", name, bar,
		(unsigned)(done >> 10), (unsigned)(total >> 10));
	InfoMessage(text, step < steps ? 2000 : 500, "Unpacking");
	if (first || step == steps) logf("unpacking %s: %u of %u KB", name, (unsigned)done, (unsigned)total);
}

// Main's video poll, every 500 ms as user_io_poll runs it: the core's mode read
// and acted on (vsync_adjust, vscale_mode, the filters), and the S-Video and
// composite encoder set for it (vga_mode=svideo or cvbs)
void video_poll()
{
	static uint64_t next;
	static uint32_t width, height;
	uint64_t now = now_ms();
	if (now < next) return;
	next = now + 500;
	video_mode_adjust();
	const VideoInfo &vi = current_video_info;
	if (vi.width == width && vi.height == height) return;
	width = vi.width;
	height = vi.height;
	logf("video %ux%u%s, %.2f Hz, %.3f MHz, vga_mode %s", vi.width, vi.height, vi.interlaced ? "i" : "",
		vi.vtime ? 100000000.0 / vi.vtime : 0.0, vi.ptime ? vi.ctime * 100.0 / vi.ptime : 0.0,
		cfg.vga_mode[0] ? cfg.vga_mode : "rgb");
}

pid_t game_start()
{
	if (g_joy) g_joy->unpack_kb_total = 0;
	g_unpack_step = -1;
	int log = open(kRunLog, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
	pid_t pid = fork();
	if (pid == 0)
	{
		prctl(PR_SET_PDEATHSIG, SIGTERM);
		cpu_set_t all;
		CPU_ZERO(&all);
		CPU_SET(0, &all);
		CPU_SET(1, &all);
		sched_setaffinity(0, sizeof(all), &all);
		signal(SIGINT, SIG_DFL);
		signal(SIGTERM, SIG_DFL);
		signal(SIGHUP, SIG_DFL);

		int null = open("/dev/null", O_RDONLY);
		if (null >= 0) dup2(null, 0);
		if (log >= 0)
		{
			dup2(log, 1);
			dup2(log, 2);
		}
		if (chdir(kHome)) _exit(126);
		setenv("MVC2_JOY_SHM", MVC2_JOY_SHM_PATH, 1);
		setenv("MVC2_MISTER", "1", 1);
		if (g_gl) setenv("MVC2_VIDEO", "pinedra", 1);
		// a test binary can stand in for the game until the next boot
		char alt[256] = {};
		FILE *f = fopen(kTestRun, "r");
		if (f)
		{
			if (fgets(alt, sizeof(alt), f)) alt[strcspn(alt, "\r\n")] = 0;
			fclose(f);
		}
		const char *exe = alt[0] ? alt : g_gl ? kGameGL : kGame;
		execl(exe, exe, alt[0] ? nullptr : g_mode, nullptr);
		_exit(127);
	}
	if (log >= 0) close(log);
	return pid;
}

// the pads read about twice a frame (the game reads them once a frame); more often only costs the game's CPU
const useconds_t kPollUs = 8000;

// runs the OSD and the pads until the game exits; returns its wait status
int game_wait(pid_t pid, bool *restart, bool *quit)
{
	int status = 0;
	for (;;)
	{
		pid_t rc = waitpid(pid, &status, WNOHANG);
		if (rc == pid) return status;
		if (rc < 0 && errno != EINTR) return -1;

		if (is_fpga_ready(1))
		{
			video_poll();
			frame_timer();
			input_poll(0);
			if (g_joy)
			{
				uint32_t mask[MVC2_JOY_PLAYERS];
				input_get_joy_mask(mask, MVC2_JOY_PLAYERS);
				for (int i = 0; i < MVC2_JOY_PLAYERS; i++)
					g_joy->joy_mask[i] = g_gl ? pinedra_to_mvc2(mask[i]) : mask[i];
			}
		}
		unpack_show();
		HandleUI();
		OsdUpdate();

		// the Pinedra core's OSD and status bits are its own
		uint32_t t = g_gl ? 0 : user_io_status_trigger_take();
		if (t & ((1u << kBitRestart) | (1u << kBitExit)))
		{
			if (t & (1u << kBitRestart)) *restart = true;
			if (t & (1u << kBitExit)) *quit = true;
			logf("%s requested", *quit ? "exit" : "restart");
			kill(pid, SIGTERM);
		}
		usleep(kPollUs);
	}
}

}  // namespace

int main(int argc, char *argv[])
{
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(1, &set);
	sched_setaffinity(0, sizeof(set), &set);

	offload_start();
	fpga_io_init();

	mkdir(kLogDir, 0755);
	g_log = fopen(kLog, "we");
	logf("MiSTer_MVC2 %s, core %s, %s", version + 5, argc > 1 ? argv[1] : "?", argc > 2 ? argv[2] : "no mgl");

	if (!is_fpga_ready(1))
	{
		logf("FPGA not ready");
		return 1;
	}

	FindStorage();
	user_io_init(argc > 1 ? argv[1] : "", argc > 2 ? argv[2] : nullptr);
	g_gl = !strcasecmp(user_io_get_core_name(), kGLName);
	if (!g_gl && strcasecmp(user_io_get_core_name(), kCoreName))
	{
		logf("core is %s, not %s", user_io_get_core_name(), kCoreName);
		back_to_menu();
	}
	for (int i = 0; i < mgl_get()->count; i++)
	{
		const char *p = mgl_get()->item[i].path;
		if (!strcasecmp(p, "arcade")) g_mode = "--arcade";
		else if (!strcasecmp(p, "dc")) g_mode = "--dc";
	}
	mgl_get()->done = 1;

	struct sigaction sa = {};
	sa.sa_handler = on_signal;
	sigaction(SIGINT, &sa, nullptr);
	sigaction(SIGTERM, &sa, nullptr);
	sigaction(SIGHUP, &sa, nullptr);

	if (g_gl && !FileExists(kTestRun, 0) && !FileExists(kGameGL, 0))
		fail(" Pinedra version: coming soon");
	if (!g_gl && !FileExists(kGame, 0)) fail(" Missing games/mvc2/mvc2");
	if (g_gl)
		for (int i = 0; i < 256; i++)
			if (user_io_get_confstr(i)) logf("confstr %d: %s", i, user_io_get_confstr(i));

	if (!g_gl)
	{
		ctrl_open();
		if (!g_ctrl) fail(" Cannot map the frame control");
		ctrl_clear();
	}
	joy_open();
	if (g_gl)
	{
		set_ovr_buttons(g_buttons, 0);
		set_ovr_buttons(g_defaults, 1);
		set_ovr_buttons(g_defaults, 2);
		parse_buttons();  // the names in the autofire message too
		input_set_map_name(kMapName);
		maps_move();
		pinedra_settings();
		menu_core_entry = 1;  // the OSD's way out: Pinedra's core has no Exit
	}
	console_graphics(true);
	if (!g_gl) user_io_status_set("[9]", 1);
	input_set_joy_passthrough(1);

	// the OSD loop stays on CPU 0, the game gets both
	CPU_ZERO(&set);
	CPU_SET(0, &set);
	sched_setaffinity(0, sizeof(set), &set);

	for (;;)
	{
		pid_t pid = game_start();
		if (pid < 0) fail(" Cannot start the game");
		g_child_pid = pid;
		logf("game pid %d", pid);

		bool restart = false, quit = false;
		int status = game_wait(pid, &restart, &quit);
		g_child_pid = -1;

		if (WIFEXITED(status)) logf("game exit %d", WEXITSTATUS(status));
		else if (WIFSIGNALED(status)) logf("game signal %d", WTERMSIG(status));
		ctrl_clear();

		if (g_signal)
		{
			logf("wrapper signal %d", (int)g_signal);
			back_to_menu();
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) == 127) fail(" Cannot start the game");
		// main_mister.c's 10 + PICK_: the data missing or unreadable (logs/last-run.log has the whole of it)
		if (WIFEXITED(status) && WEXITSTATUS(status) == 11)
			fail(" No game data found. Put\n the ROMs in games/mvc2/\n roms/mvsc2, or the disc\n in games/mvc2/disc", 10);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 12)
			fail(" No arcade ROMs in\n games/mvc2/roms/mvsc2\n (MAME's mvsc2.zip)", 10);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 13)
			fail(" No Dreamcast disc in\n games/mvc2/disc", 10);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 14)
			fail(" The disc in games/mvc2/disc\n could not be read, or it\n is not the USA disc", 10);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 15)
			fail(" Could not unpack mvsc2.zip:\n its drive or the SD card\n needs 136 MB free", 10);
		if (!restart || quit) back_to_menu();
	}
}
