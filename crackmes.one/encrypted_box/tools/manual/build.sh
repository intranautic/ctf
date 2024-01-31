if [ -z $1 ]; then
	BUILD_DIR = "obj-intel64"
else
    BUILD_DIR = "$1"
fi

gcc -g -O0 -Wall -msse2 -msse -march=native -maes -o $BUILD_DIR/inv_aesenc inv_aesenc.cpp common.cpp
gcc -g -O0 -Wall -msse2 -msse -march=native -maes -o $BUILD_DIR/inv_aesdec inv_aesdec.cpp common.cpp
