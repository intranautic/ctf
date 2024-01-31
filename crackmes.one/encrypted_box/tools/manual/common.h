#pragma once
#ifndef COMMON_H
#define COMMON_H

// Process command line arguments
bool process_args(int argc, char** argv, char* key, char* data, bool* c_format);

// Read ascii hex string into m128i byte buffer format
bool ascii_to_m128i(char* out, const char* ascii);

// Print m128i value
void print_m128i(const char* value, bool c_format = false);

#endif