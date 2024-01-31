# Manual tools

These are the tools used for manually solving input data for aesenc and aesdec

# Build

Build both tools using:
```
./build.sh [out_dir]
```

Where out_dir is an optional directory to put the executables (default is obj-intel64)

The tools can also be built invidually like so:
```
gcc -g -O0 -Wall -msse2 -msse -march=native -maes -o inv_aesenc inv_aesenc.cpp common.cpp
```

# Usage

Both tools follow the same usage

```
inv_aesenc [-c] <key_u128> <data_u128>

Finds value such that 'aesenc value, key_u128' == data_u128
 
eg: inv_aesenc -c a16f50c4d1bfe9d0a8d01d9776210139 6d65ed24c22914c98e33d4cb8ec85c19
 
-c				Print in C format: \xa1\xb2\xc3...
				(default is space-separated bytes: a1 b2 c3...)

key_u128		Key as u128 integer (ie. little-endian vs. stored bytes)
data_u128		Result from aesenc instruction as u128 integer
```
