#include <stdio.h>
#include <ctype.h>
#include <cstdint>
#include <cstring>
#include "common.h"

// Process command line arguments
bool process_args(int argc, char** argv, char* key, char* data, bool* c_format)
{
	size_t keyarg_idx = 1;

	// Check for -c format option
	if (argc != 3)
	{
		if ((argc == 4) && !strcmp("-c", argv[1]))
		{
			*c_format = true;
			keyarg_idx = 2;
		}
		else
			return false;
	}

	// Validate arguments
	if (!ascii_to_m128i(key, argv[keyarg_idx]) ||
		!ascii_to_m128i(data, argv[keyarg_idx + 1]))
	{
		return false;
	}

	return true;
}

// Read ascii hex string into m128i byte buffer format
bool ascii_to_m128i(char* out, const char* ascii)
{
	int i = strlen(ascii) - 2;
	size_t j = 0;

	memset(out, 0, 16);

	for (; i >= 0; i -= 2)
	{
		uint32_t c = 0;

		// Use sscanf_s when compiling in Visual Studio
#ifdef  _MSC_VER
		if (sscanf_s(&ascii[i], "%2x", &c) == EOF)
#else
		if (sscanf(&ascii[i], "%2x", &c) == EOF)
#endif
			return false;

		out[j++] = c & 0xff;
	}

	if (i == -1)
	{
		uint32_t c = 0;

#ifdef  _MSC_VER
		if (sscanf_s(&ascii[0], "%01x", &c) == EOF)
#else
		if (sscanf(&ascii[0], "%01x", &c) == EOF)
#endif
			return false;

		out[j++] = c & 0xff;
	}

	return true;
}

void print_m128i(const char* value, bool c_format)
{
	const char* format = (c_format) ? "\\x%.2x" : "%.2x ";

	// Print uint128 format
	printf("0x");

	for (size_t i = 16; i != 0; i--)
		printf("%.2x", value[i - 1] & 0xff);

	putchar('\n');

	// Print byte buffer format
	for (size_t i = 0; i < 16; i++)
		printf(format, value[i] & 0xff);

	putchar('\n');
}
