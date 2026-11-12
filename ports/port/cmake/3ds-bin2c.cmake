# a binary file as a C header: cmake -DIN=file -DOUT=header -DNAME=name -P 3ds-bin2c.cmake
file(READ ${IN} hex HEX)
string(LENGTH "${hex}" n)
math(EXPR size "${n} / 2")
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," bytes "${hex}")
string(REGEX REPLACE "(0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,0x..,)" "\\1\n    " bytes "${bytes}")
get_filename_component(src ${IN} NAME)
file(WRITE ${OUT} "/* made from ${src} by port/cmake/3ds-bin2c.cmake */\n"
     "static const unsigned char ${NAME}[] __attribute__((aligned(4))) = {\n    ${bytes}\n};\n"
     "static const unsigned int ${NAME}_size = ${size};\n")