/*Author:chenzw@caep.ac.cn
 * date:2010-05-07
 *
 */


#ifndef __SHCRYPT_H__
#define __SHCRYPT_H__

#define MAX_KEY_CHAR 256

#define LM_SHARE 0x1
#define LM_CLIENT_TRUSTER 0x2
#define LM_SERVER_FILE_AUDIT 0x4

typedef struct{
        //////regist request//////////
        char date[64];
        char finger[128];
        char rnd[128];
        char cardcrypt[128];
        char softcrypt[128];


        ///////license info////////////
        char license_for[1024];
        unsigned long license_mod;
        unsigned long license_count;
        long start_time;
        long end_time;

	unsigned char emerge_cert1[5120];
	unsigned char emerge_cert2[5120];
        ////////license check////////////////
        char license_code[5120];
        char license_sig[1024];

} license_key_t;
int HMemoryEncrypt(unsigned char* out,const unsigned char* m,const int mlen,const unsigned char *key,int klen);
int HMemoryDecrypt(unsigned char* out,const unsigned char* c,const int clen,const unsigned char* key,int klen);
int HFileEncrypt(const char* srcFn,const char* dstFn,const char* key,int keylen);
int HFileDecrypt(const char* srcFn,const char*dstFn,const char* key,int keylen);

int GetMachineFinger(char* buf,int len, char*f ,int flen);
#endif
