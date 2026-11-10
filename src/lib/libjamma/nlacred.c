/* libjamma's credit handling: the coin meters, the per-frame JAMMA read that drives the credit
   service, and the system backup update. */
#include "nlajamma.h"

extern int nlaSystemBackupUpdateFlag;

int nlaVblankOff;
static int meterCount[NLA_CHUTES];

int nlaCoinSwitch1, nlaCoinSwitch2;

void nlaJAMMACreditInit( void )
{
	int i;

	nlaCoinSwitch1 = 0;
	nlaCoinSwitch2 = 0;
	for ( i = 0 ; i < NLA_CHUTES ; i++ ) {
		meterCount[i] = 0;
	}
}

void nlaOldJAMMACoinCounter( void )
{
	int i;
	unsigned int *meter;

	meter = nlaGetCoinMeterCount();
	nlaOutput6149PortD = 0;

	for ( i = 0 ; i < NLA_CHUTES ; i++ ) {
		if ( meterCount[i] > 6 ) {
			nlaOutput6149PortD |= ( 1 << i );
			meterCount[i]--;
		} else if ( meterCount[i] > 0 ) {
			meterCount[i]--;
			meter[i] = 0;
		} else {
			if ( meter[i] != 0 )
				meterCount[i] = 12;
		}
	}
}

void nlaVblankStart( void )
{
	nlaVblankOff = 0;
}

void nlaVblankStop( void )
{
	nlaVblankOff = 1;
}

void nlaVblankFunction( void )
{
	int i;
	unsigned int coinin1[NLA_CHUTES];
	unsigned int coinin2[NLA_CHUTES];

	if ( nlaVblankOff != 0 )
		return;
	if ( nlaCheckPerStatus() == NLA_PER_BUSY )
		return;

	for ( i = 0 ; i < NLA_CHUTES ; i++ ) {
		coinin1[i] = 0;
		coinin2[i] = 0;
	}

	if ( nlaJAMMANodes == 0 ) {
		nlaJAMMAOldRead( 0, &nlaCoinSwitch1, coinin1 );
	} else if ( nlaJAMMANodes == 1 ) {
		nlaJAMMARead( 1, &nlaCoinSwitch1, coinin1 );
	} else if ( nlaJAMMANodes == 2 ) {
		nlaJAMMAReadDouble( 2, &nlaCoinSwitch1, coinin1, 1, &nlaCoinSwitch2, coinin2 );
		nlaCoinSwitch1 &= NLAC_SW_SERVICE1;
		nlaCoinSwitch2 &= NLAC_SW_SERVICE1;
		nlaCoinSwitch1 |= ( nlaCoinSwitch2 << 1 );
		coinin1[1] = coinin2[0];
		coinin1[2] = 0;
		coinin1[3] = 0;
	} else {
		nlaJAMMARead( nlaJAMMAPrimaryNode, &nlaCoinSwitch1, coinin1 );
	}

	if ( nlaTestMode == 0 ) {
		nlaSystemBackupUpdateFlag += nlaCreditServiceRoutine( nlaCoinSwitch1, coinin1 );
		nlaOldJAMMACoinCounter();
	}
	nlaVblankFunctionEnd();
}

static int vblankTime;

void nlaUpdateSystemBackup( void )
{
	int sr_org;
	int flag;

	if ( nlaTestMode != 0 )
		return;

	if ( nlaSystemBackupUpdateFlag != 0 ) {
		flag = nlaCheckWaitROMDMA();
		if ( flag == 1 ) {
			sr_org = get_imask();
			set_imask( 15 );
			nlaUpdateCreditData();
			nlaSystemBackupUpdateFlag = 0;
			set_imask( sr_org );
		}
	}

	vblankTime++;
	if ( ( vblankTime & 15 ) == 0 ) {
		flag = nlaCheckWaitROMDMA();
		if ( flag == 1 )
			nlaUpdateTotalTime();
		else
			vblankTime = 15;
	}
}
