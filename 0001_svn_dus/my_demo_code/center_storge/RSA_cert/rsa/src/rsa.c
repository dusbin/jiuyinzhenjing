#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>
#include <stdio.h>
#include "rsa.h"
int rsa(){
	BIGNUM *bne = BN_new();
	int ret = BN_set_word(bne,RSA_F4);
	RSA *rsa = RSA_new();
	ret = RSA_generate_key_ex(rsa,1024,bne,NULL);
	if(ret != 1){
		return -1;
	}
	FILE* filename = NULL;
	filename = fopen("private.key","wb");
	PEM_write_RSAPrivateKey(filename, rsa, NULL, NULL, 0, NULL, NULL);
    fclose(filename);
    filename = NULL;
	/* 提取公钥 */
    unsigned char *n_b = (unsigned char *)calloc(RSA_size(rsa), sizeof(unsigned char));
    unsigned char *e_b = (unsigned char *)calloc(RSA_size(rsa), sizeof(unsigned char));
    int n_size = BN_bn2bin(rsa->n, n_b);
    int b_size = BN_bn2bin(rsa->e, e_b);
    RSA *pubrsa = RSA_new();
    pubrsa->n = BN_bin2bn(n_b, n_size, NULL);
    pubrsa->e = BN_bin2bn(e_b, b_size, NULL);
    filename = fopen("public.key", "wb");
    PEM_write_RSA_PUBKEY(filename, pubrsa);
    fclose(filename);
    filename = NULL;
    RSA_free(pubrsa);
	RSA_free(rsa);
	return 0;
}

