/**************************************************************
* @file EncodeBase64.h
* Date: 2018/04/11
* Author: shiqing
* History: 001)[Modify reason 1]
* Editor: 001) Editor YYYY/MM/DD
* All Right Reserved, Copyright(c) Beijing Uxsino Limited 2018.
 * **************************************************************/

char * base64_encode(const unsigned char * bindata, char * base64, int binlength);
int base64_decode(const char * base64, unsigned char * bindata);
