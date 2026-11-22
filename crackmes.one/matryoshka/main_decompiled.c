// positive sp value has been detected, the output may be wrong!
int __fastcall main(int argc, const char **argv, const char **envp)
{
  __int64 (__fastcall *v3)(); // r14
  char v4; // bl
  unsigned int n511; // ebp
  Stream *Stream; // rax
  int n10; // eax
  int n10_1; // eax
  int Character; // esi
  Stream *Stream_1; // rax
  __int64 v12; // rdx
  char *v13; // r8
  __int64 *v14; // rdx
  int n363736431; // eax
  int v16; // r11d
  unsigned int n0x6B7; // eax
  __int64 v18; // rbx
  int v19; // r10d
  int n375422153; // edx
  char *v21; // r9
  _DWORD *Buffer_2; // r8
  unsigned int v23; // ecx
  int v24; // eax
  __int64 v25; // rcx
  __int64 v26; // rdx
  int v27; // edx
  char *v28; // r8
  __int16 *v29; // rcx
  int v30; // esi
  unsigned int n0x6B7_2; // ebp
  int v32; // r9d
  int n116; // esi
  unsigned int n0x3CD; // eax
  __int64 n0x3CD_1; // r11
  int v36; // edx
  __int64 v37; // rcx
  __int64 v38; // rdx
  __int64 v39; // rdx
  __int64 v40; // rcx
  __int64 v41; // rdx
  __int64 v42; // rcx
  __int64 v43; // rdx
  __int64 v44; // rcx
  int v45; // r8d
  __int64 n511_2; // r9
  __int64 v47; // rdx
  char v48; // cl
  __int64 v49; // rdx
  __int64 v50; // r8
  __int64 v51; // rdx
  __int64 v52; // rcx
  __int64 v53; // rdx
  __int64 v54; // rdx
  int v55; // ecx
  __int64 v56; // rdx
  __int64 v57; // rdx
  int v58; // eax
  unsigned int n0x6B7_1; // ebx
  __int64 v60; // r8
  __int64 v61; // rcx
  char v62; // cl
  __int64 v63; // r8
  __int64 v64; // rcx
  unsigned __int32 v65; // r8d
  __int64 v66; // rcx
  __int64 v67; // rcx
  __int64 v68; // rcx
  unsigned __int32 v69; // r8d
  __int64 v70; // rcx
  __int64 v71; // r8
  __int64 v72; // rcx
  __int64 v73; // rcx
  __int64 v74; // r8
  __int64 v75; // rcx
  __int64 v76; // r8
  __m128i *v77; // rax
  unsigned int v78; // edi
  __m128i v79; // xmm4
  __m128i *v80; // r8
  char *v81; // rcx
  __m128i v82; // xmm0
  __m128i v83; // xmm2
  __m128i v84; // xmm13
  __m128i v85; // xmm11
  __m128i v86; // xmm0
  __m128i v87; // xmm0
  __m128i v88; // xmm12
  __m128i v89; // xmm12
  __m128i v90; // xmm1
  __m128i v91; // xmm14
  __m128i v92; // xmm1
  __m128i v93; // xmm11
  __m128i v94; // xmm11
  __m128i v95; // xmm13
  __m128i v96; // xmm15
  __m128i v97; // xmm2
  __m128i v98; // xmm13
  __m128i v99; // xmm2
  __m128i v100; // xmm14
  __m128i v101; // xmm13
  __m128i v102; // xmm14
  __m128i v103; // xmm2
  __m128i v104; // xmm13
  __m128i v105; // xmm0
  __m128i v106; // xmm13
  __m128i v107; // xmm12
  __m128i v108; // xmm1
  __m128i v109; // xmm12
  __int64 *v110; // r10
  unsigned int i; // r8d
  char v112; // r13
  unsigned int n0x268; // eax
  unsigned int n0x1F; // edi
  int v115; // ebp
  __int64 n0x268_1; // r8
  int v117; // edx
  __int64 v118; // rcx
  __int64 v119; // r8
  __int64 v120; // rcx
  int v121; // r10d
  char v122; // r8
  __int64 n511_3; // r11
  __int64 v124; // rcx
  int v125; // r9d
  __int64 v126; // rax
  __int64 n0x268_2; // rcx
  int v128; // r8d
  __int64 n511_4; // r10
  __int64 v130; // rcx
  char v131; // cl
  __int64 v132; // rcx
  __int64 v133; // rax
  __int64 v134; // rcx
  __int64 v135; // rax
  int v136; // r10d
  __int64 n0x1E; // rax
  int v138; // ecx
  __int64 v139; // rax
  __int64 v140; // rcx
  char v141; // cl
  Stream *Stream_2; // rax
  __m128i v143; // xmm2
  _OWORD *v144; // rcx
  __m128i *Buffer_1; // rax
  __m128i v146; // xmm12
  __m128i v147; // xmm13
  __m128i v148; // xmm1
  __m128i v149; // xmm12
  __m128i v150; // xmm0
  __m128i v151; // xmm12
  __m128i v152; // xmm1
  __m128i v153; // xmm0
  __m128i v154; // xmm12
  __m128i v155; // xmm1
  __m128i v156; // xmm12
  __m128i v157; // xmm0
  __m128i v158; // xmm12
  __m128i v159; // xmm13
  __m128i v160; // xmm12
  __m128i v161; // xmm13
  __m128i v162; // xmm12
  __m128i v163; // xmm12
  __m128i v164; // xmm0
  __m128i v165; // xmm12
  __m128i v166; // xmm1
  __m128i v167; // xmm1
  __m128i v168; // xmm1
  int n116_1; // r10d
  unsigned int n0x493; // r8d
  __int64 n0x493_1; // r11
  unsigned int n0xEF; // ecx
  char v173; // al
  int v174; // ebp
  int v175; // eax
  __int64 v176; // rcx
  int v177; // r8d
  __int64 n269; // rcx
  _BYTE *v179; // r9
  char v180; // al
  char v181; // r10
  char v182; // al
  unsigned int j_1; // r9d
  unsigned int v184; // r8d
  __int64 j; // rsi
  unsigned __int8 n85; // al
  __int64 v187; // rcx
  __int64 n511_5; // r11
  int v189; // r9d
  char v190; // cl
  __int64 n511_6; // rcx
  unsigned int n511_1; // [rsp+24h] [rbp-47E4h]
  unsigned int n0x6B7_3; // [rsp+28h] [rbp-47E0h]
  int n0x3CD_2; // [rsp+30h] [rbp-47D8h]
  int v195; // [rsp+34h] [rbp-47D4h]
  int n0x268_3; // [rsp+38h] [rbp-47D0h]
  unsigned __int8 v197; // [rsp+3Dh] [rbp-47CBh]
  unsigned __int8 v198; // [rsp+3Eh] [rbp-47CAh]
  unsigned __int8 n116_2; // [rsp+3Fh] [rbp-47C9h]
  __int128 n375422153_1; // [rsp+40h] [rbp-47C8h]
  __int128 v201; // [rsp+50h] [rbp-47B8h]
  int v202; // [rsp+60h] [rbp-47A8h]
  unsigned int v203; // [rsp+64h] [rbp-47A4h]
  int v204; // [rsp+68h] [rbp-47A0h]
  int v205; // [rsp+6Ch] [rbp-479Ch]
  __int64 v206; // [rsp+70h] [rbp-4798h]
  __int64 v207; // [rsp+78h] [rbp-4790h]
  __int128 v208; // [rsp+80h] [rbp-4788h]
  __int64 v209; // [rsp+90h] [rbp-4778h]
  int v210; // [rsp+98h] [rbp-4770h]
  int v211; // [rsp+9Ch] [rbp-476Ch]
  _DWORD v212[160]; // [rsp+A0h] [rbp-4768h]
  _BYTE v213[1024]; // [rsp+320h] [rbp-44E8h] BYREF
  _BYTE v214[1720]; // [rsp+720h] [rbp-40E8h] BYREF
  __int64 v215; // [rsp+DD8h] [rbp-3A30h] BYREF
  _BYTE v216[974]; // [rsp+1720h] [rbp-30E8h] BYREF
  __int16 v217; // [rsp+1AEEh] [rbp-2D1Ah] BYREF
  _BYTE v218[608]; // [rsp+2720h] [rbp-20E8h] BYREF
  __int64 v219; // [rsp+2980h] [rbp-1E88h] BYREF
  _DWORD Buffer[1075]; // [rsp+3720h] [rbp-10E8h] BYREF

  ((void (__fastcall *)(int, const char **, const char **))sub_1400026D0)(argc, argv, envp);
  sub_140001560();
  puts("====================================================");
  puts("   Crackme - five nested virtual machines");
  puts("   Goal: find the password.");
  puts("====================================================");
  puts(&Buffer_);
  v3 = off_14000B0F0;
LABEL_2:
  v4 = 0;
  n511 = 0;
  sub_140002710("Enter password: ");
  Stream = (FILE *)((__int64 (__fastcall *)(__int64))v3)(1);
  fflush(Stream);
  while ( 1 )
  {
    n10 = getchar();
    if ( n10 == 10 || n10 == -1 )
      break;
    if ( n10 == 13 )
    {
      n10_1 = getchar();
      Character = n10_1;
      if ( n10_1 != 10 && n10_1 != -1 )
      {
        Stream_1 = (FILE *)((__int64 (__fastcall *)(_QWORD))v3)(0);
        ungetc(Character, Stream_1);
      }
      *((_BYTE *)&v212[32] + n511) = 0;
LABEL_12:
      if ( v4 == 0 )
      {
        v13 = (char *)&unk_14000CF40;
        v14 = (__int64 *)v214;
        n363736431 = 363736431;
        do
        {
          v14 = (__int64 *)((char *)v14 + 1);
          ++v13;
          n363736431 = 1103515245 * n363736431 + 12345;
          *((_BYTE *)v14 - 1) = *(v13 - 1) ^ BYTE2(n363736431);
        }
        while ( v14 != &v215 );
        n511_1 = n511;
        v16 = -127;
        n0x6B7 = 0;
        n375422153_1 = 0;
        v201 = 0;
        do
        {
          v18 = n0x6B7 + 1;
          v19 = byte_14000C220[(unsigned __int8)(v16 ^ v214[n0x6B7])];
          switch ( (char)v19 )
          {
            case 0:
              n375422153 = n375422153_1;
              if ( (_DWORD)n375422153_1 != 0 )
                goto LABEL_13;
              v21 = (char *)&unk_14000C1C0;
              Buffer_2 = Buffer;
              v23 = (DWORD2(n375422153_1) ^ 0xCBFCDF6E) - 1640531527;
              do
              {
                Buffer_2 = (_DWORD *)((char *)Buffer_2 + 1);
                ++v21;
                v24 = v23 ^ HIWORD(v23);
                v23 -= 1640531527;
                LOBYTE(v24) = *(v21 - 1)
                            ^ ((1935289751 * (((unsigned int)(569420461 * v24) >> 15) ^ (569420461 * v24))) >> 24);
                *((_BYTE *)Buffer_2 - 1) = v24;
                n375422153 = (unsigned __int8)v24 + 33 * n375422153;
              }
              while ( Buffer_2 != (_DWORD *)((char *)&Buffer[17] + 3) );
              HIBYTE(Buffer[17]) = 0;
              if ( n375422153 == 375422153 )
              {
                Stream_2 = (FILE *)((__int64 (__fastcall *)(__int64))v3)(1);
                fputs((const char *)Buffer, Stream_2);
                return 0;
              }
              else
              {
                puts(&Buffer_);
                puts("[-] Wrong password.");
                return 1;
              }
            case 1:
              v57 = n0x6B7 + 2;
              n0x6B7 += 6;
              *((_DWORD *)&n375422153_1 + (v214[(unsigned int)v18] & 7)) = *(_DWORD *)&v214[v57];
              goto LABEL_30;
            case 2:
              v54 = n0x6B7 + 2;
              n0x6B7 += 6;
              v55 = *(_DWORD *)&v214[v54];
              v56 = v214[(unsigned int)v18] & 7;
              *((_DWORD *)&n375422153_1 + v56) ^= v55;
              goto LABEL_30;
            case 3:
              v52 = n0x6B7 + 2;
              n0x6B7 += 6;
              v53 = v214[(unsigned int)v18] & 7;
              *((_DWORD *)&n375422153_1 + v53) += *(_DWORD *)&v214[v52];
              goto LABEL_30;
            case 4:
              v50 = n0x6B7 + 2;
              n0x6B7 += 6;
              v51 = v214[(unsigned int)v18] & 7;
              *((_DWORD *)&n375422153_1 + v51) *= *(_DWORD *)&v214[v50];
              goto LABEL_30;
            case 5:
              v47 = n0x6B7 + 2;
              n0x6B7 += 3;
              v48 = v214[v47];
              if ( (v48 & 0x1F) != 0 )
              {
                v49 = v214[(unsigned int)v18] & 7;
                *((_DWORD *)&n375422153_1 + v49) = __ROL4__(*((_DWORD *)&n375422153_1 + v49), v48);
              }
              goto LABEL_30;
            case 6:
              v44 = n0x6B7 + 2;
              n0x6B7 += 3;
              v45 = 0;
              n511_2 = (unsigned __int8)v214[v44];
              if ( (unsigned int)n511_2 < n511_1 )
                v45 = *((unsigned __int8 *)&v212[32] + n511_2);
              *((_DWORD *)&n375422153_1 + (v214[(unsigned int)v18] & 7)) = v45;
              goto LABEL_30;
            case 7:
              n0x6B7 += 2;
              *((_DWORD *)&n375422153_1 + (v214[(unsigned int)v18] & 7)) = n511_1;
              goto LABEL_30;
            case 8:
              v42 = n0x6B7 + 2;
              n0x6B7 += 3;
              v43 = v214[(unsigned int)v18] & 7;
              *((_DWORD *)&n375422153_1 + v43) ^= *((_DWORD *)&n375422153_1 + (v214[v42] & 7));
              goto LABEL_30;
            case 9:
              v40 = n0x6B7 + 2;
              n0x6B7 += 3;
              v41 = v214[(unsigned int)v18] & 7;
              *((_DWORD *)&n375422153_1 + v41) |= *((_DWORD *)&n375422153_1 + (v214[v40] & 7));
              goto LABEL_30;
            case 10:
              v39 = n0x6B7 + 2;
              n0x6B7 += 3;
              *((_DWORD *)&n375422153_1 + (v214[(unsigned int)v18] & 7)) = *((_DWORD *)&n375422153_1 + (v214[v39] & 7));
              goto LABEL_30;
            case 11:
              v37 = n0x6B7 + 2;
              n0x6B7 += 3;
              v38 = v214[(unsigned int)v18] & 7;
              *((_DWORD *)&n375422153_1 + v38) += *((_DWORD *)&n375422153_1 + (v214[v37] & 7));
              goto LABEL_30;
            case 12:
              v27 = v201;
              v28 = (char *)&unk_14000CB60;
              v29 = (__int16 *)v216;
              v30 = -1584311473;
              do
              {
                v29 = (__int16 *)((char *)v29 + 1);
                ++v28;
                v30 ^= ((v30 ^ (unsigned int)(v30 << 13)) >> 17)
                     ^ (v30 << 13)
                     ^ (32 * (((v30 ^ (unsigned int)(v30 << 13)) >> 17) ^ v30 ^ (v30 << 13)));
                *((_BYTE *)v29 - 1) = v30 ^ *(v28 - 1);
              }
              while ( &v217 != v29 );
              v202 = 0;
              n0x6B7_2 = n0x6B7 + 1;
              v32 = v16;
              v204 = v27;
              n116 = 116;
              v205 = 0;
              v206 = 0;
              v207 = 0;
              v197 = v19;
              v203 = v27 ^ 0xC52AFA37;
              n0x3CD = 0;
              break;
            case 13:
              v25 = n0x6B7 + 2;
              n0x6B7 += 3;
              v26 = v214[(unsigned int)v18] & 7;
              *((_DWORD *)&n375422153_1 + v26) >>= v214[v25];
              goto LABEL_30;
            case 14:
              if ( *((_DWORD *)&n375422153_1 + (v214[v18] & 7)) != 0 )
                n0x6B7 += 3;
              else
                n0x6B7 += (char)v214[n0x6B7 + 2] + 3;
              goto LABEL_30;
            default:
              goto LABEL_13;
          }
          do
          {
            n0x3CD_1 = n0x3CD + 1;
            v36 = 101 * (unsigned __int8)((n116 ^ v216[n0x3CD]) - 59);
            switch ( (char)v36 )
            {
              case 0:
                v19 = v197;
                v58 = v202;
                n0x6B7_1 = n0x6B7_2;
                v16 = v32;
                goto LABEL_53;
              case 1:
                v67 = n0x3CD + 2;
                n0x3CD += 6;
                *(&v202 + (v216[(unsigned int)n0x3CD_1] & 7)) = _byteswap_ulong(*(_DWORD *)&v216[v67]);
                goto LABEL_55;
              case 2:
                v64 = n0x3CD + 2;
                n0x3CD += 6;
                v65 = _byteswap_ulong(*(_DWORD *)&v216[v64]);
                v66 = v216[(unsigned int)n0x3CD_1] & 7;
                *(&v202 + v66) ^= v65;
                goto LABEL_55;
              case 3:
                v68 = n0x3CD + 2;
                n0x3CD += 6;
                v69 = _byteswap_ulong(*(_DWORD *)&v216[v68]);
                v70 = v216[(unsigned int)n0x3CD_1] & 7;
                *(&v202 + v70) -= v69;
                goto LABEL_55;
              case 4:
                v61 = n0x3CD + 2;
                n0x3CD += 3;
                v62 = v216[v61];
                if ( (v62 & 0x1F) != 0 )
                {
                  v63 = v216[(unsigned int)n0x3CD_1] & 7;
                  *(&v202 + v63) = __ROR4__(*(&v202 + v63), v62);
                }
                goto LABEL_55;
              case 5:
                v120 = n0x3CD + 2;
                v121 = 0;
                n0x3CD += 3;
                v122 = v216[(unsigned int)n0x3CD_1];
                n511_3 = (unsigned __int8)v216[v120];
                if ( (unsigned int)n511_3 < n511_1 )
                  v121 = *((unsigned __int8 *)&v212[32] + n511_3);
                *(&v202 + (v122 & 7)) = v121;
                goto LABEL_55;
              case 6:
                v118 = n0x3CD + 2;
                n0x3CD += 3;
                v119 = v216[(unsigned int)n0x3CD_1] & 7;
                *(&v202 + v119) = *(&v202 + (v216[v118] & 7)) ^ __ROL4__(*(&v202 + v119), 5);
                goto LABEL_55;
              case 7:
                v124 = n0x3CD + 2;
                n0x3CD += 3;
                *(&v202 + (v216[(unsigned int)n0x3CD_1] & 7)) = *(&v202 + (v216[v124] & 7));
                goto LABEL_55;
              case 8:
                n0x3CD += 2;
                *(&v202 + (v216[(unsigned int)n0x3CD_1] & 7)) = 0;
                goto LABEL_55;
              case 9:
                v77 = (__m128i *)&unk_14000C8E0;
                v78 = v203;
                v79 = (__m128i)xmmword_14000D600;
                v80 = (__m128i *)v218;
                v81 = (char *)&unk_14000CB40;
                do
                {
                  ++v77;
                  ++v80;
                  v82 = _mm_add_epi32(v79, (__m128i)xmmword_14000D620);
                  v83 = _mm_add_epi32(v79, (__m128i)xmmword_14000D630);
                  v84 = _mm_add_epi32(v79, (__m128i)xmmword_14000D640);
                  v85 = _mm_add_epi32(v79, (__m128i)xmmword_14000D650);
                  v79 = _mm_add_epi32(v79, (__m128i)xmmword_14000D680);
                  v86 = _mm_xor_si128(_mm_srli_epi32(v82, 0x10u), v82);
                  v87 = _mm_unpacklo_epi32(
                          _mm_shuffle_epi32(_mm_mul_epu32(v86, (__m128i)xmmword_14000D660), 8),
                          _mm_shuffle_epi32(_mm_mul_epu32(_mm_srli_epi64(v86, 0x20u), (__m128i)xmmword_14000D660), 8));
                  v88 = _mm_xor_si128(_mm_srli_epi32(v83, 0x10u), v83);
                  v89 = _mm_unpacklo_epi32(
                          _mm_shuffle_epi32(_mm_mul_epu32(v88, (__m128i)xmmword_14000D660), 8),
                          _mm_shuffle_epi32(_mm_mul_epu32(_mm_srli_epi64(v88, 0x20u), (__m128i)xmmword_14000D660), 8));
                  v90 = _mm_xor_si128(_mm_srli_epi32(v84, 0x10u), v84);
                  v91 = _mm_srli_epi32(v89, 0xFu);
                  v92 = _mm_unpacklo_epi32(
                          _mm_shuffle_epi32(_mm_mul_epu32(v90, (__m128i)xmmword_14000D660), 8),
                          _mm_shuffle_epi32(_mm_mul_epu32(_mm_srli_epi64(v90, 0x20u), (__m128i)xmmword_14000D660), 8));
                  v93 = _mm_xor_si128(_mm_srli_epi32(v85, 0x10u), v85);
                  v94 = _mm_unpacklo_epi32(
                          _mm_shuffle_epi32(_mm_mul_epu32(v93, (__m128i)xmmword_14000D660), 8),
                          _mm_shuffle_epi32(_mm_mul_epu32(_mm_srli_epi64(v93, 0x20u), (__m128i)xmmword_14000D660), 8));
                  v95 = _mm_srli_epi32(v87, 0xFu);
                  v96 = _mm_srli_epi32(v94, 0xFu);
                  v97 = _mm_unpacklo_epi16(v95, v91);
                  v98 = _mm_unpackhi_epi16(v95, v91);
                  v99 = _mm_unpacklo_epi16(_mm_unpacklo_epi16(v97, v98), _mm_unpackhi_epi16(v97, v98));
                  v100 = _mm_srli_epi32(v92, 0xFu);
                  v101 = _mm_unpacklo_epi16(v100, v96);
                  v102 = _mm_unpackhi_epi16(v100, v96);
                  v103 = _mm_packus_epi16(
                           _mm_and_si128(v99, (__m128i)xmmword_14000D670),
                           _mm_and_si128(
                             _mm_unpacklo_epi16(_mm_unpacklo_epi16(v101, v102), _mm_unpackhi_epi16(v101, v102)),
                             (__m128i)xmmword_14000D670));
                  v104 = v87;
                  v105 = _mm_unpacklo_epi16(v87, v89);
                  v106 = _mm_unpackhi_epi16(v104, v89);
                  v107 = v92;
                  v108 = _mm_unpacklo_epi16(v92, v94);
                  v109 = _mm_unpackhi_epi16(v107, v94);
                  v80[-1] = _mm_xor_si128(
                              v103,
                              _mm_xor_si128(
                                _mm_packus_epi16(
                                  _mm_and_si128(
                                    _mm_unpacklo_epi16(_mm_unpacklo_epi16(v105, v106), _mm_unpackhi_epi16(v105, v106)),
                                    (__m128i)xmmword_14000D670),
                                  _mm_and_si128(
                                    _mm_unpacklo_epi16(_mm_unpacklo_epi16(v108, v109), _mm_unpackhi_epi16(v108, v109)),
                                    (__m128i)xmmword_14000D670)),
                                v77[-1]));
                }
                while ( &unk_14000CB40 != (_UNKNOWN *)v77 );
                v110 = &v219;
                for ( i = 0xBB4E05E2;
                      i != 1262570851;
                      *((_BYTE *)v110 - 1) = *(v81 - 1) ^ ((569420461 * (i ^ HIWORD(i))) >> 15) ^ (-83 * (i ^ BYTE2(i))) )
                {
                  i -= 1640531527;
                  ++v81;
                  v110 = (__int64 *)((char *)v110 + 1);
                }
                v198 = v32;
                v112 = -81;
                n0x268 = 0;
                v212[0] = v78;
                n0x1F = 1;
                n0x6B7_3 = n0x6B7_2;
                v115 = v36;
                break;
              case 10:
                v75 = n0x3CD + 2;
                n0x3CD += 6;
                v76 = v216[(unsigned int)n0x3CD_1] & 7;
                *(&v202 + v76) |= ((unsigned __int8)v216[v75 + 2] << 8)
                                | ((unsigned __int8)v216[v75 + 1] << 16)
                                | ((unsigned __int8)v216[v75] << 24)
                                | (unsigned __int8)v216[v75 + 3];
                goto LABEL_55;
              case 11:
                v73 = n0x3CD + 2;
                n0x3CD += 3;
                v74 = v216[(unsigned int)n0x3CD_1] & 7;
                *(&v202 + v74) = (unsigned int)*(&v202 + v74) >> v216[v73];
                goto LABEL_55;
              case 12:
                v71 = n0x3CD + 2;
                n0x3CD += 3;
                v72 = v216[(unsigned int)n0x3CD_1] & 7;
                *(&v202 + v72) |= *(&v202 + (v216[v71] & 7));
                goto LABEL_55;
              case 13:
                v60 = n0x3CD + 2;
                n0x3CD += 3;
                *(&v202 + (v216[n0x3CD_1] & 7)) += *(&v202 + (v216[v60] & 7));
                goto LABEL_55;
              default:
                goto LABEL_56;
            }
            do
            {
              n0x268_1 = n0x268 + 1;
              v117 = 255 - (unsigned __int8)(v112 ^ v218[n0x268]);
              switch ( v117 )
              {
                case 0:
                  v36 = v115;
                  v32 = v198;
                  n0x6B7_2 = n0x6B7_3;
                  if ( n0x1F != 0 )
                    n0x1F = v212[n0x1F - 1];
                  goto LABEL_111;
                case 1:
                  n0x268 += 5;
                  if ( n0x1F <= 0x1F )
                  {
                    v140 = n0x1F++;
                    v212[v140] = *(_DWORD *)&v218[n0x268_1];
                  }
                  goto LABEL_79;
                case 2:
                  n0x1E = n0x1F - 1;
                  if ( (unsigned int)n0x1E > 0x1E )
                    goto LABEL_104;
                  v138 = v212[n0x1E];
                  v139 = n0x1F++;
                  v212[v139] = v138;
                  n0x268 = n0x268_1;
                  goto LABEL_79;
                case 3:
                  ++n0x268;
                  if ( n0x1F <= 1 )
                    goto LABEL_79;
                  v134 = n0x1F - 1;
                  v135 = n0x1F - 2;
                  v136 = v212[v134];
                  v212[v134] = v212[v135];
                  v212[v135] = v136;
                  goto LABEL_104;
                case 4:
                  if ( n0x1F <= 1 )
                    goto LABEL_104;
                  v133 = n0x1F - 2;
                  v212[v133] ^= v212[--n0x1F];
                  n0x268 = n0x268_1;
                  goto LABEL_79;
                case 5:
                  ++n0x268;
                  if ( n0x1F > 1 )
                  {
                    v132 = n0x1F - 2;
                    v212[v132] += v212[--n0x1F];
                  }
                  goto LABEL_79;
                case 6:
                  n0x268 += 2;
                  v131 = v218[(unsigned int)n0x268_1];
                  if ( (v131 & 0x1F) != 0 && n0x1F != 0 )
                    v212[n0x1F - 1] = __ROL4__(v212[n0x1F - 1], v131);
                  goto LABEL_79;
                case 7:
                  n0x268 += 2;
                  if ( n0x1F <= 0x1F )
                  {
                    n0x268_2 = (unsigned int)n0x268_1;
                    v128 = 0;
                    n511_4 = (unsigned __int8)v218[n0x268_2];
                    if ( (unsigned int)n511_4 < n511_1 )
                      v128 = *((unsigned __int8 *)&v212[32] + n511_4);
                    v130 = n0x1F++;
                    v212[v130] = v128;
                  }
                  goto LABEL_79;
                case 8:
                  if ( n0x1F <= 1 )
                    goto LABEL_104;
                  v126 = n0x1F - 2;
                  v212[v126] |= v212[--n0x1F];
                  n0x268 = n0x268_1;
                  goto LABEL_79;
                case 9:
                  if ( n0x1F != 0 )
                  {
                    ++n0x268;
                    if ( n0x1F > 0x1F )
                      goto LABEL_79;
                    v125 = v212[n0x1F - 1];
                  }
                  else
                  {
                    v125 = 1;
                  }
                  v143 = (__m128i)xmmword_14000D610;
                  v144 = &unk_14000C440;
                  Buffer_1 = (__m128i *)Buffer;
                  do
                  {
                    ++Buffer_1;
                    ++v144;
                    v146 = _mm_add_epi32(v143, (__m128i)xmmword_14000D690);
                    v147 = _mm_add_epi32(v143, (__m128i)xmmword_14000D6B0);
                    v148 = _mm_unpackhi_epi16(v143, v146);
                    v149 = _mm_unpacklo_epi16(v143, v146);
                    v150 = _mm_unpacklo_epi16(v149, v148);
                    v151 = _mm_unpackhi_epi16(v149, v148);
                    v152 = v143;
                    v143 = _mm_add_epi32(v143, (__m128i)xmmword_14000D6C0);
                    v153 = _mm_unpacklo_epi16(v150, v151);
                    v154 = _mm_add_epi32(v152, (__m128i)xmmword_14000D6A0);
                    v155 = _mm_unpacklo_epi16(v154, v147);
                    v156 = _mm_unpackhi_epi16(v154, v147);
                    v157 = _mm_packus_epi16(
                             _mm_and_si128(v153, (__m128i)xmmword_14000D670),
                             _mm_and_si128(
                               _mm_unpacklo_epi16(_mm_unpacklo_epi16(v155, v156), _mm_unpackhi_epi16(v155, v156)),
                               (__m128i)xmmword_14000D670));
                    v158 = _mm_unpacklo_epi8(v157, v157);
                    v159 = _mm_unpackhi_epi8(v157, v157);
                    v160 = _mm_packus_epi16(
                             _mm_and_si128(_mm_mullo_epi16(v158, v158), (__m128i)xmmword_14000D670),
                             _mm_and_si128(_mm_mullo_epi16(v159, v159), (__m128i)xmmword_14000D670));
                    v161 = _mm_sub_epi8(_mm_and_si128(_mm_slli_epi16(v160, 5u), (__m128i)xmmword_14000D6D0), v160);
                    v162 = _mm_add_epi8(v157, v157);
                    v163 = _mm_add_epi8(v162, v162);
                    v164 = _mm_xor_si128(
                             _mm_add_epi8(v161, (__m128i)xmmword_14000D6E0),
                             _mm_add_epi8(_mm_sub_epi8(_mm_add_epi8(v163, v163), v157), (__m128i)xmmword_14000D6F0));
                    v165 = (__m128i)*(v144 - 1);
                    v166 = _mm_add_epi8(v165, v165);
                    v167 = _mm_add_epi8(v166, v166);
                    v168 = _mm_add_epi8(v167, v167);
                    Buffer_1[-1] = _mm_xor_si128(
                                     v164,
                                     _mm_or_si128(
                                       _mm_and_si128(_mm_srli_epi16(v165, 4u), (__m128i)xmmword_14000D700),
                                       _mm_add_epi8(v168, v168)));
                  }
                  while ( &Buffer[292] != (_DWORD *)Buffer_1 );
                  Buffer[292] = -1601173888;
                  v211 = 0;
                  v210 = v125;
                  n0x3CD_2 = n0x3CD_1;
                  v195 = v115;
                  v209 = 0;
                  n116_1 = n116;
                  n0x268_3 = n0x268_1;
                  n0x493 = 0;
                  v208 = 0;
                  break;
                case 10:
                  ++n0x268;
                  n0x1F = (n0x1F == 0) + n0x1F - 1;
                  goto LABEL_79;
                case 11:
                  n0x268 += 5;
                  if ( n0x1F != 0 )
                    v212[n0x1F - 1] ^= *(_DWORD *)&v218[n0x268_1];
                  goto LABEL_79;
                case 12:
                  n0x268 += 5;
                  if ( n0x1F != 0 )
                    v212[n0x1F - 1] += *(_DWORD *)&v218[n0x268_1];
                  goto LABEL_79;
                case 13:
                  n0x268 += 5;
                  if ( n0x1F != 0 )
                    v212[n0x1F - 1] *= *(_DWORD *)&v218[n0x268_1];
                  goto LABEL_79;
                case 14:
                  v141 = v218[n0x268_1];
                  n0x268 += 2;
                  if ( (v141 & 0x1F) != 0 && n0x1F != 0 )
                    v212[n0x1F - 1] = __ROR4__(v212[n0x1F - 1], v141);
                  goto LABEL_79;
                default:
                  goto LABEL_80;
              }
              do
              {
                n0x493_1 = n0x493 + 1;
                n0xEF = *((unsigned __int8 *)Buffer + n0x493);
                v173 = *((_BYTE *)Buffer + n0x493);
                if ( n0xEF > 0xEF )
                  break;
                switch ( n0xEF >> 4 )
                {
                  case 0u:
                    goto LABEL_126;
                  case 1u:
                    n0x493 += 5;
                    *((_DWORD *)&v208 + (v173 & 7)) = _byteswap_ulong(*(_DWORD *)((char *)Buffer + n0x493_1));
                    continue;
                  case 2u:
                    n0x493 += 5;
                    *((_DWORD *)&v208 + (v173 & 7)) ^= _byteswap_ulong(*(_DWORD *)((char *)Buffer + n0x493_1));
                    continue;
                  case 3u:
                    n0x493 += 5;
                    *((_DWORD *)&v208 + (v173 & 7)) *= _byteswap_ulong(*(_DWORD *)((char *)Buffer + n0x493_1));
                    continue;
                  case 4u:
                    v190 = *((_BYTE *)Buffer + n0x493_1);
                    n0x493 += 2;
                    if ( (v190 & 0x1F) != 0 )
                      *((_DWORD *)&v208 + (v173 & 7)) = __ROL4__(*((_DWORD *)&v208 + (v173 & 7)), v190);
                    continue;
                  case 5u:
                    n511_5 = *((unsigned __int8 *)Buffer + n0x493_1);
                    n0x493 += 2;
                    v189 = 0;
                    if ( (unsigned int)n511_5 < n511_1 )
                      v189 = *((unsigned __int8 *)&v212[32] + n511_5);
                    *((_DWORD *)&v208 + (v173 & 7)) = v189;
                    continue;
                  case 6u:
                    n0x493 += 2;
                    *((_DWORD *)&v208 + (v173 & 7)) ^= *((_DWORD *)&v208 + (*((_BYTE *)Buffer + n0x493_1) & 7));
                    continue;
                  case 7u:
                    n0x493 += 2;
                    *((_DWORD *)&v208 + (v173 & 7)) = *((_DWORD *)&v208 + (*((_BYTE *)Buffer + n0x493_1) & 7));
                    continue;
                  case 8u:
                    n0x493 += 2;
                    *((_DWORD *)&v208 + (v173 & 7)) |= *((_DWORD *)&v208 + (*((_BYTE *)Buffer + n0x493_1) & 7));
                    continue;
                  case 9u:
                    n116_2 = n116_1;
                    v174 = v209;
                    v177 = 0;
                    n269 = 0;
                    v179 = v213;
                    do
                    {
                      ++v179;
                      v180 = byte_14000C320[n269] - 3 * n269;
                      v181 = n269++;
                      v182 = v177 ^ byte_14000C207[v181 & 3] ^ v180;
                      v177 += 93;
                      *(v179 - 1) = v182;
                    }
                    while ( n269 != 269 );
                    n116_1 = n116_2;
                    j_1 = 0;
                    v184 = 0;
                    break;
                  case 0xAu:
                    LODWORD(n0x3CD_1) = n0x3CD_2;
                    n116 = n116_1;
                    v115 = v195;
                    LODWORD(n0x268_1) = n0x268_3;
                    v175 = v208;
                    goto LABEL_133;
                  case 0xBu:
                    *((_DWORD *)&v208 + (*((_BYTE *)Buffer + n0x493) & 7)) = 0;
                    goto LABEL_126;
                  case 0xCu:
                    n0x493 += 5;
                    *((_DWORD *)&v208 + (v173 & 7)) += _byteswap_ulong(*(_DWORD *)((char *)Buffer + n0x493_1));
                    continue;
                  case 0xDu:
                    n0x493 += 2;
                    *((_DWORD *)&v208 + (v173 & 7)) >>= *((_BYTE *)Buffer + n0x493_1);
                    continue;
                  case 0xEu:
                    n0x493 += 2;
                    *((_DWORD *)&v208 + (v173 & 7)) += *((_DWORD *)&v208 + (*((_BYTE *)Buffer + n0x493_1) & 7));
                    continue;
                  default:
                    abort();
                }
LABEL_137:
                while ( 1 )
                {
                  for ( j = j_1; ; ++j )
                  {
                    if ( (unsigned int)j > 0x10C )
                      goto LABEL_175;
                    n85 = v213[j];
                    v187 = (unsigned int)(j + 1);
                    if ( n85 == 85 )
                    {
                      j_1 = j + 2;
                      v184 = (unsigned __int8)(v184 * v213[v187]);
                      goto LABEL_137;
                    }
                    if ( n85 <= 0x55u )
                    {
                      switch ( n85 )
                      {
                        case 0x11u:
                          n511_6 = (unsigned __int8)v213[v187];
                          j_1 = j + 2;
                          v184 = 0;
                          if ( (unsigned int)n511_6 < n511_1 )
                            v184 = *((unsigned __int8 *)&v212[32] + n511_6);
                          goto LABEL_137;
                        case 0x22u:
                          j_1 = j + 2;
                          v184 ^= (unsigned __int8)v213[v187];
                          goto LABEL_137;
                        case 0x33u:
                          j_1 = j + 2;
                          v184 = (unsigned __int8)(v213[v187] + v184);
                          goto LABEL_137;
                        case 0x44u:
                          j_1 = j + 2;
                          v184 = (unsigned __int8)(((_BYTE)v184 << (v213[v187] & 7)) | (v184 >> (-(v213[v187] & 7) & 7)));
                          goto LABEL_137;
                        default:
                          goto LABEL_175;
                      }
                    }
                    if ( n85 != 0x99 )
                      break;
                    v184 = 0;
                  }
                  if ( n85 > 0x99u )
                    break;
                  if ( n85 != 102 )
                  {
                    if ( n85 == 0x88 )
                    {
                      j_1 = j + 2;
                      if ( n511_1 != (unsigned __int8)v213[v187] )
                        v174 |= 2u;
                      continue;
                    }
LABEL_175:
                    v174 = -559038737;
                    goto LABEL_125;
                  }
                  j_1 = j + 2;
                  if ( v184 != (unsigned __int8)v213[v187] )
                    v174 |= 1u;
                }
                if ( n85 != 0xFF )
                  v174 = -559038737;
LABEL_125:
                LODWORD(v208) = v174 | v208;
LABEL_126:
                n0x493 = n0x493_1;
              }
              while ( n0x493 <= 0x493 );
              v115 = v195;
              n116 = n116_1;
              v175 = -559038737;
              LODWORD(n0x3CD_1) = n0x3CD_2;
              LODWORD(n0x268_1) = n0x268_3;
LABEL_133:
              v176 = n0x1F++;
              v212[v176] = v175;
LABEL_104:
              n0x268 = n0x268_1;
LABEL_79:
              v112 = 13 * v112 + 5 + v117;
            }
            while ( n0x268 <= 0x268 );
LABEL_80:
            v36 = v115;
            v32 = v198;
            n0x6B7_2 = n0x6B7_3;
            n0x1F = 0xDEADBEEF;
LABEL_111:
            v202 |= n0x1F;
            n0x3CD = n0x3CD_1;
LABEL_55:
            n116 = 17 * n116 + 3 + v36;
          }
          while ( n0x3CD <= 0x3CD );
LABEL_56:
          v19 = v197;
          n0x6B7_1 = n0x6B7_2;
          v16 = v32;
          v58 = -559038737;
LABEL_53:
          LODWORD(n375422153_1) = v58 | n375422153_1;
          n0x6B7 = n0x6B7_1;
LABEL_30:
          v16 = v19 + 33 * v16 + 7;
        }
        while ( n0x6B7 <= 0x6B7 );
      }
LABEL_13:
      puts("[-] Wrong password.");
      puts(&Buffer_);
      goto LABEL_2;
    }
    if ( n511 == 511 )
    {
      v4 = 1;
    }
    else
    {
      v4 |= n10 == 0;
      v12 = n511++;
      *((_BYTE *)&v212[32] + v12) = n10;
    }
  }
  *((_BYTE *)&v212[32] + n511) = 0;
  if ( n511 != 0 || n10 != -1 )
    goto LABEL_12;
  if ( v4 != 0 )
    goto LABEL_13;
  puts(&Buffer_);
  puts("[-] No input received. Bye!");
  return 1;
}
