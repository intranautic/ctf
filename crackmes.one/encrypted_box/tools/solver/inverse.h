#pragma once
#ifndef INVERSE_H
#define INVERSE_H

// Inverse aesenc
void inv_aesenc(unsigned char* data, const unsigned char* key);

// Inverse aesdec
void inv_aesdec(unsigned char* data, const unsigned char* key);

#endif // INVERSE_H