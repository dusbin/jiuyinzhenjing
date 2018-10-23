/**		
 * @file    	sms4.c
 * @date: 		2016/03/09	
 * @brief  		SMS4加解密算法实现	
 * @author:		xhtian
 * 修改履历：	001)[修改原因1]		
 * 修改作者：	001)修改者 YYYY/MM/DD		
 * @version   	1.0
 * Copyright(c) All Right Reserved, Copyright(c) 2016.		
 */	
#include "sms4.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#define SMS4_INLINE _inline
#else
#define SMS4_INLINE inline
#endif

static unsigned int sms4_key[4];    /**< KEY*/
static unsigned char sbox[] = {     /**< S-box*/
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
    0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,
    0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,
    0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,
    0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,
    0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,
    0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,
    0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,
    0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,
    0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,
    0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,
    0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,
    0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,
    0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,
    0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
    0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48
};

static unsigned int CK[] = {
    0x00070e15,0x1c232a31,0x383f464d,0x545b6269,
    0x70777e85,0x8c939aa1,0xa8afb6bd,0xc4cbd2d9,
    0xe0e7eef5,0xfc030a11,0x181f262d,0x343b4249,
    0x50575e65,0x6c737a81,0x888f969d,0xa4abb2b9,
    0xc0c7ced5,0xdce3eaf1,0xf8ff060d,0x141b2229,
    0x30373e45,0x4c535a61,0x686f767d,0x848b9299,
    0xa0a7aeb5,0xbcc3cad1,0xd8dfe6ed,0xf4fb0209,
    0x10171e25,0x2c333a41,0x484f565d,0x646b7279
};

static unsigned int FK[] = {
    0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc
};

static unsigned int en_rk[32];
static unsigned int de_rk[32];

#define LEFT_RSHIFT_32B(v, r)   \
    (((v) << (r)) | ((v) >> (32 - (r))))

#define SMS4_BYTE4TOUINT(a0, a1, a2, a3)    \
    (((a0) & 0xff) | (((a1) << 8) & 0xff00) | (((a2) << 16) & 0xff0000) |(((a3) << 24) & 0xff000000))

#define SMS4_UINTTOBYTE4(A, a0, a1, a2, a3)    \
    do {                            \
        (a0) = (A);                 \
        (a1) = ((A) >> 8);          \
        (a2) = ((A) >> 16);         \
        (a3) = ((A) >> 24);         \
    } while (0);

static SMS4_INLINE unsigned int sms4_calc_rk(unsigned int A)
{
    unsigned int B;
    unsigned char a0, a1, a2, a3;
    unsigned char b0, b1, b2, b3;
    unsigned int rk;

    a0 = (unsigned char)(A & 0x000000ff);
    a1 = (unsigned char)((A & 0x0000ff00) >> 8);
    a2 = (unsigned char)((A & 0x00ff0000) >> 16);
    a3 = (unsigned char)((A & 0xff000000) >> 24);
    b0 = sbox[a0];
    b1 = sbox[a1];
    b2 = sbox[a2];
    b3 = sbox[a3];

    B = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
    rk = B ^ (LEFT_RSHIFT_32B(B, 13)) ^ (LEFT_RSHIFT_32B(B, 23));
    return (rk);
}

static SMS4_INLINE unsigned int _sms4_calc_lt(unsigned int A)
{
    unsigned int B;
    unsigned int C;
    unsigned char a0, a1, a2, a3;

    a0 = (unsigned char)(A & 0x000000ff);
    a1 = (unsigned char)((A & 0x0000ff00) >> 8);
    a2 = (unsigned char)((A & 0x00ff0000)>>16);
    a3 = (unsigned char)((A & 0xff000000)>>24);

    B = sbox[a0] | sbox[a1] << 8 | sbox[a2] << 16 | sbox[a3] << 24;
    C = B^(LEFT_RSHIFT_32B(B, 2))^(LEFT_RSHIFT_32B(B, 10))^(LEFT_RSHIFT_32B(B, 18))^(LEFT_RSHIFT_32B(B, 24));

    return C;
}

static SMS4_INLINE unsigned int _sms4_func_F(
    unsigned int x0, 
    unsigned int x1, 
    unsigned int x2, 
    unsigned int x3, 
    unsigned int rk)
{
    return (x0 ^ _sms4_calc_lt(x1 ^ x2 ^ x3 ^ rk));
}

static SMS4_INLINE void _sms4_crypt(unsigned int*in, unsigned int* out, unsigned int* rk)
{
    unsigned int X[36];
    int i;

    X[0] = in[0];
    X[1] = in[1];
    X[2] = in[2];
    X[3] = in[3];

    for (i = 0; i < 32; i++)
    {
        X[i+4] = _sms4_func_F(X[i], X[i+1], X[i+2], X[i+3], rk[i]);
    }
    out[0] = X[35];
    out[1] = X[34];
    out[2] = X[33];
    out[3] = X[32];
}

static SMS4_INLINE void sms4_key_ex()
{
    int i;
    unsigned int K[36];
    unsigned int A;

    K[0] = sms4_key[0] ^ FK[0];
    K[1] = sms4_key[1] ^ FK[1];
    K[2] = sms4_key[2] ^ FK[2];
    K[3] = sms4_key[3] ^ FK[3];

    for (i = 0; i < 32; i++)
    {
        A = K[i+1] ^ K[i+2] ^ K[i+3] ^ CK[i];
        K[i+4] = K[i] ^ sms4_calc_rk(A);
        en_rk[i] = K[i+4];
        de_rk[31-i] = en_rk[i];
    }
}

void sms4_set_key(unsigned int* key)
{
    int i;

    assert(key);

    for (i = 0; i < 4; i++)
    {
        sms4_key[i] = key[i];
    }
    sms4_key_ex();
}

void sms4_encrypt( char* in, int in_len, char* out,int out_len )
{
    int count;
    int is_left;
    int i;
    unsigned int X[4], Y[4];
    int index;

    assert(in && out);
    assert(in_len == out_len);

    count = in_len / 16;
    is_left = in_len % 16;

    for (i = 0; i < count; i++)
    {
        index = i * 16;
        X[0] = SMS4_BYTE4TOUINT(in[index+0], in[index+1], in[index+2], in[index+3]);
        X[1] = SMS4_BYTE4TOUINT(in[index+4], in[index+5], in[index+6], in[index+7]);
        X[2] = SMS4_BYTE4TOUINT(in[index+8], in[index+9], in[index+10], in[index+11]);
        X[3] = SMS4_BYTE4TOUINT(in[index+12], in[index+13], in[index+14], in[index+15]);
        _sms4_crypt(X, Y, en_rk);
        SMS4_UINTTOBYTE4(Y[0], out[index+0], out[index+1], out[index+2], out[index+3]);
        SMS4_UINTTOBYTE4(Y[1], out[index+4], out[index+5], out[index+6], out[index+7]);
        SMS4_UINTTOBYTE4(Y[2], out[index+8], out[index+9], out[index+10], out[index+11]);
        SMS4_UINTTOBYTE4(Y[3], out[index+12], out[index+13], out[index+14], out[index+15]);
    }
    if (is_left)
    {
        memcpy(out+in_len-is_left, in+in_len-is_left, is_left);
    }
}

void sms4_decrypt( char* in, int in_len, char* out,int out_len )
{
    int count;
    int is_left;
    int i;
    unsigned int X[4], Y[4];
    int index;

    assert(in && out);
    assert(in_len == out_len);

    count = in_len / 16;
    is_left = in_len % 16;

    for (i = 0; i < count; i++)
    {
        index = i * 16;
        X[0] = SMS4_BYTE4TOUINT(in[index+0], in[index+1], in[index+2], in[index+3]);
        X[1] = SMS4_BYTE4TOUINT(in[index+4], in[index+5], in[index+6], in[index+7]);
        X[2] = SMS4_BYTE4TOUINT(in[index+8], in[index+9], in[index+10], in[index+11]);
        X[3] = SMS4_BYTE4TOUINT(in[index+12], in[index+13], in[index+14], in[index+15]);
        _sms4_crypt(X, Y, de_rk);
        SMS4_UINTTOBYTE4(Y[0], out[index+0], out[index+1], out[index+2], out[index+3]);
        SMS4_UINTTOBYTE4(Y[1], out[index+4], out[index+5], out[index+6], out[index+7]);
        SMS4_UINTTOBYTE4(Y[2], out[index+8], out[index+9], out[index+10], out[index+11]);
        SMS4_UINTTOBYTE4(Y[3], out[index+12], out[index+13], out[index+14], out[index+15]);
    }
    if (is_left)
    {
        memcpy(out+in_len-is_left, in+in_len-is_left, is_left);
    }
}


void sms4_data_encrypt(
    char* input, 
    unsigned int input_len, 
    char* output, 
    unsigned int output_len)
{
    sms4_encrypt(input, input_len, output, output_len);
}

void sms4_data_decrypt(
    char* input, 
    unsigned int input_len, 
    char* output, 
    unsigned int output_len)
{
    assert(input_len == output_len);

    sms4_decrypt(input, input_len, output, output_len);
}

