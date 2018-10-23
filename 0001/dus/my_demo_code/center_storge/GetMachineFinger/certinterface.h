/*
 * certinterface.h
 *
 *  Created on: 2009-10-12
 *      Author: root
 */

#ifndef CERTINTERFACE_H_
#define CERTINTERFACE_H_
#include "errdefine.h"
RSA * prikey_new(RSA *rsa);
void prikey_free(RSA *rsa);
int prikey_init(char *fileORstring,RSA **rsa,int flag);
int prikey_signature(RSA *rsa,char *data,int datalen,char*signret,int *signlen);


X509 * cert_new(X509 *x509);
void cert_free(X509 *x509);
int cert_init(char *fileORstring,X509 **x509,int flag);
int cert_verify(X509 *x509,char *data,int datalen,char*signret,int signlen);


int xa_hash_data(char *data,int datalen,char*hashret,int *hashlen);

#endif /* CERTINTERFACE_H_ */
