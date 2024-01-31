#include <wmmintrin.h>
#include "inverse.h"

// Inverse aesenc
void inv_aesenc(unsigned char* data, const unsigned char* key)
{
	// Load 128-bit input values
	__m128i d = _mm_loadu_si128((__m128i*) data);
	__m128i k = _mm_loadu_si128((__m128i*) key);
	__m128i zero = _mm_setzero_si128();

	// Calculate inverse
	d = _mm_xor_si128(d, k);				// Undo the XOR with key performed at the end of aesenc
	d = _mm_aesimc_si128(d);				// Undo MixColumns
	d = _mm_aesdeclast_si128(d, zero);		// Undo ShiftRows and SubBytes

	// Store result
	_mm_storeu_si128((__m128i*) data, d);
}

// Inverse aesdec
void inv_aesdec(unsigned char* data, const unsigned char* key)
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
