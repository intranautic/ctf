/**************************************
*
* inv_aesdec
* Find inverse of aesdec instruction
* 
* === Usage ===
*
* inv_aesdec [-c] <key_u128> <data_u128>
* 
* Finds value such that 'aesdec value, key_u128' == data_u128
* 
* eg: inv_aesdec -c a16f50c4d1bfe9d0a8d01d9776210139 6d65ed24c22914c98e33d4cb8ec85c19
* 
* -c			Print in C format: \xa1\xb2\xc3...
*				(default is space-separated bytes: a1 b2 c3...)
*
* key_u128		Key as u128 integer (ie. little-endian vs. stored bytes)
* data_u128		Result from aesdec instruction as u128 integer
*
* === Build ===
*
* build.sh [out_dir]
*
* Or on its own:
* gcc -g -O0 -Wall -msse2 -msse -march=native -maes -o inv_aesdec inv_aesdec.cpp common.cpp
* 
* For more info, see:
* https://blog.nindalf.com/posts/implementing-aes/
* https://stackoverflow.com/questions/32297088/how-to-implement-aes128-encryption-decryption-using-aes-ni-instructions-and-gcc
* https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf
* https://www.cs.virginia.edu/~cr4bd/3330/S2018/simdref.html
* https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
*
***************************************/

#include <stdio.h>
#include <wmmintrin.h>
#include "common.h"

// Inverse aesdec
void inv_aesdec(char *data, const char *key)
{
	// Load 128-bit input values
	__m128i d = _mm_loadu_si128((__m128i*) data);
	__m128i k = _mm_loadu_si128((__m128i*) key);
	__m128i zero = _mm_setzero_si128();

	// Calculate inverse
	d = _mm_xor_si128(d, k);				// Undo the XOR with key performed at the end of aesdec
	d = _mm_aesdeclast_si128(d, zero);		// Undo InvMixColumns
	d = _mm_aesenc_si128(d, zero);
	d = _mm_aesenclast_si128(d, zero);		// Undo InvShiftRows and InvSubBytes

	// Store result
	_mm_storeu_si128((__m128i*) data, d);
}

const char USAGE[] =
"inv_aesdec [-c] <key_u128> <data_u128>\n\n"
"Finds input such that 'aesenc input, key_u128' = data_u128\n\n"
"eg: inv_aesdec -c a16f50c4d1bfe9d0a8d01d9776210139 6d65ed24c22914c98e33d4cb8ec85c19\n\n"
"-c\t\t\tPrint in C format: \\xa1\\xb2\\xc3...\n"
"\t\t\t(default is space-separated bytes: a1 b2 c3...)\n\n"
"key_u128\t\tKey as u128 integer (ie. little-endian vs. stored bytes)\n"
"data_u128\t\tResult from aesenc instruction as u128 integer\n";

int error()
{
	puts(USAGE);
	return -1;
}

int main(int argc, char **argv)
{
	char key[16], data[16];
	bool c_format = false;
	
	if (!process_args(argc, argv, key, data, &c_format))
		return error();	

	// Perform inverse aesdec
	inv_aesdec(data, key);

	// Print result
	print_m128i(data, c_format);

	return 0;
}
