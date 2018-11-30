#ifndef __LICENSE_H__
#define __LICENSE_H__
#include"Cjson.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
static cJSON *g_license_config_json_date = NULL;
#define LICENSE_CONFIG_FILE  "user.data"
#define LICENSE_ENCRYTOR_FILE "encryptor/output/uxdb.lic"

#define STRING_MAX_LEN (8192)

#define MAX_JSON_SYSTIME_LENGTH (64)
//#define MAX_NUM 63001
#define RAND_NUM 16

typedef struct pPairkey
{
	unsigned int publicKey;
	unsigned int privateKey;
	unsigned int n;
}PAIRKEY;

#define ENCODE 0,16,1    /* 加密用的宏 */
#define DECODE 15,-1,-1    /* 解密用的宏 */

#define FAIL (-1)


#define EMPTY (0)
#define ZERO (0)

char * base64_encode(const unsigned char * bindata, char * base64, int binlength);
//char msg[maxn], key[16], res[32];
char msg[0x8000], key[16], res[32];
char msgb[72], msgbt[72], keyb[18][72];
int encrypt_license_file(PAIRKEY *pair_key,char *data);
#include "CommonOperator.h"
#include "License.h"
#define MAX_NUM 20480      //生成的素数私钥最大数量
#define MAX_PRIME_NUM 10240 //随机生成的素数最大值
#define DES_KEY_TEXT_LENGTH 16
#define MAX_CLEAR_TEXT_LENGTH 10240
#define UXDB_LICENSE_FILE_FULL_PATH "user.data"
#define UXDB_CLEAR_LICENSE_FILE_PATH "user.data.txt"
#define MAX_CIPHER_TEXT_LENGTH 32638
#define maxn 0x8000    /* 理论支持明文长度 */
/*
 * 说明：根据时间随机生成的素数对
 */
typedef struct pair_prime_number{
	long digit_one;
	long digit_two;
}PAIR_PRIME;

/*
 * 说明：保存所有小于(digit_one)*(digit_two)且与其互为质数的数据
 */
typedef struct pKeyset
{
	long set[MAX_NUM];
	unsigned int size;
}PSET;

enum result{
	GENERATE_OK = 0,
	ERROR_NOEACHPRIME,
	ERROR_NOPUBLICKEY,
	ERROR_GENERROR
};

/*
 * 定义分数，主要用在从私钥集合中选择四分之三处的私钥下标值
 */
enum fraction{
	DENOMINATOR = 4,//分母
	MOLECULAR = 3   //分子
};
typedef struct pPublicPairkey
{
	unsigned int publicKey;
	unsigned int n;
}PUBLICPAIRKEY;
int GenerateRandPrimeNumber();
unsigned int MakePrivatedKeyd(long uiP, long uiQ);
int isPrime(unsigned int m, unsigned int n);
int MakePairkey(long uiP, long uiQ, long uiD);
int GeneratePublicKey(PAIRKEY *pair_key);
int readLicenseValue(unsigned char *licP, int *desP, PUBLICPAIRKEY *pairkey);
int base64_decode(const char * base64, unsigned char * bindata);
void rsa_decrypt(int n, int d, int *cw, int cLength, char *clearText);
int DES_decrypt(char* plaintext ,char cw[], char *des_key);
int output(char * clearText);
#endif

