#include<stdio.h>
#include<stdlib.h>
#include "des.h"
int set_file_data(const char *path,char *data){
	FILE *fp = NULL;
	int len = 0;
	fp = fopen(path,"w+");
	len = strlen(data);
	fwrite(data,len,1,fp);
	fclose(fp);
	fp = NULL;
	return 0;
}
int encrypt(char* hkey,const char* inbuf,const int inlen,unsigned char* outbuf,int* outlen){
	int i = 0;
	char res[9] = {0};
	char key[9] = {0};
	char keyb[18][72] = {0};
	strncpy(key,hkey,8);
	getKeys(key, keyb);
	for(i = 0; i<inlen; i += 8)
	{
		DES(inbuf + i, ENCODE, keyb, res); // 加密
		memcpy(outbuf+i, res, 8);
	}
	*outlen = i;
	return 0;
}
int decrypt(char* hkey,const char* inbuf,const int inlen,unsigned char* outbuf,int* outlen){
	int i = 0;
	char res[9] = {0};
	char keyb[18][72] = {0};
	char key[9] = {0};
	strncpy(key,hkey,8);
	getKeys(key, keyb);
	for(i = 0; i<inlen; i += 8)
	{
		DES(inbuf + i, DECODE, keyb, res); // 加密
		memcpy(outbuf+i, res, 8);
	}
	*outlen = strlen(outbuf);
	return 0;	
}
int main(){
	char str[4096] = {0};
	char stren[4096] = {0};
	char strde[4096] = {0};
	int len = 0;
	//PUBHANDLE* hkey;
	char hkey[9]="xianyang";
	strcpy(str,"{\"version\":\"ver:0.0.0.1\",\"private.pem\":\"-----BEGIN RSA PRIVATE KEY-----\nMIIEpAIBAAKCAQEA4brxrPYd8vGj4BQRCtq+TZ23VVNcf3gqy8bqwtkACbnybzlc\nSjU4b7+VNq8wMn8fbRkviS/t2hQgLG4rcYzWDvBvftnG3sNm+MKW3DDbDVuBv7Vf\nMsD2d4Zcu7ZXTnmodKbG2Idv6UXsgn2geH/3KPJ3R7kx0GCJYDk3NJxKsJLmJuIg\nz6FzoF8Wc+tgOtLsHFBrtFduvd93AB2AvxNzTq7nOG2TL7uPEzIngYR5zI6Dg6Yr\n341TxB2Uf4s+dd3gDQSWzK6Rfti8JILyr30XScxp8JFmaY/dNkVdUCi7g4wkLFzW\nabLgjLngMV0S70BCGVS1y0Qu1Sdtp7p4owKY3QIDAQABAoIBABsZkoItWhMcp4vz\nXMhs0oQG90erJmO3TYwBaZXS74o7ZLutq0LNeOURnxxdk++8C03RxD84HdocqHAt\nGdIcEmbgvsH/6zw7QVWEJxBwXRwRbFZTtAYn3G8lFmUC0yB8wpienSjJucnLt0VD\nR8yvsECeO2vE9BIMRPHkbJhIWz8vadyS+rHd7Xdsz0iiPA1Ttf1NiucBdaJ/8sl1\nOEgx/3e/Rac9Hspt7otB+3v6CeZ2iKaTe8q7LHUM6DIYu4VwPcaindKHhsK62Nge\n4wbTc5FE5jyeyr+9/ryKJtWJvHXQFOm+MAUNpGSHZZmf+mSNDYx3/1gRmSbQcQIH\nQ7Y1dRECgYEA/ZrRrCZMq892h5ivgrkl6ylElHm4kqZlpuKWJL3XQ48KQ2H4m5yR\nVNad9t7HFacztBoT542XLYrZmy0RvxRWamTwLetbGG4w0qlbUXrIZxwIg92+2hSy\nwi+xjmVOsgeHVwBL9dMgRYJNZaBYl4oxpzwQPRBarvb6SynQyq578y8CgYEA49y6\nc7kPCw7qKWywdH3ow3ygl7L+we7yzuX0oGqUkU1qKIGkA7F3PjuFQ5LP8PL5OnzU\nLLEAt7iirkJvrxMO1qcn6rrjgQ+P+pBgqXrGT3SuLiAxtNrkLtsUSL7X48GhR7Yu\nfloAAzfaH7C43aOLvQsowZxL+zv19UAVe4pJobMCgYAOp4KUKwZlKzDTNwx1RlOd\nmh3fYoU63CilkxRP/TRSMBLMv/2IqGxxdZFdMZRZePbw4kziJXll5U8bu2f6Hae8\nsCZ80MvLxppFogmn1Kt94c3QKOEdYvLNa8oby1jXyr2UVm0NQGzt91CXncNgxliG\nLF9X1/kVgSoHa9wnyhtZewKBgQCKJGOVjPglyCEGhDJmLX9UJNZnocj5YhgqfxdT\nucngXYKilBYPdyPHwuYEFZmjhmq6nrszbQv2ViWqZ/S1u2Qflq+Ii00qDXlIewdf\nOGPJ//8pT7NS5fG4xnEBuVHBbr6Mb1K4n2+qg83lsIojNOee8IkA9/TIl42B5hwg\nDbKeKQKBgQDDM2OA6Wk/IyhQrtPmXnd7adITe6YNk2tXgHLE9CgW51PQnqL89llK\nvVOTYZIudt/fMEZVtwQmEPmacYiNGSgKHaYb1ZrHpTrrqX0lVpfas2KsGJPXK0Lo\ndJd12kIb6H4TyF72oICVn7XOJktEe0JGSvUmmhGL9vfYOfRQp1LbHA==\n-----END RSA PRIVATE KEY-----\n\",\"cacert.pem\":\"-----BEGIN CERTIFICATE-----\nMIIEhjCCA26gAwIBAgIJAPRNXM/ievtgMA0GCSqGSIb3DQEBBQUAMIGIMQswCQYD\nVQQGEwJDTjEQMA4GA1UECBMHU2hhYW54aTEOMAwGA1UEBxMFWGknYW4xEzARBgNV\nBAoTCnV4c2luby5jb20xDDAKBgNVBAsTA2R6YjETMBEGA1UEAxMKZHV6aGVuZ2Jp\nbjEfMB0GCSqGSIb3DQEJARYQODQ3Mzc5OTYyQHFxLmNvbTAeFw0xODA3MTIwMTI5\nMzhaFw0yMTA3MTEwMTI5MzhaMIGIMQswCQYDVQQGEwJDTjEQMA4GA1UECBMHU2hh\nYW54aTEOMAwGA1UEBxMFWGknYW4xEzARBgNVBAoTCnV4c2luby5jb20xDDAKBgNV\nBAsTA2R6YjETMBEGA1UEAxMKZHV6aGVuZ2JpbjEfMB0GCSqGSIb3DQEJARYQODQ3\nMzc5OTYyQHFxLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOG6\n8az2HfLxo+AUEQravk2dt1VTXH94KsvG6sLZAAm58m85XEo1OG+/lTavMDJ/H20Z\nL4kv7doUICxuK3GM1g7wb37Zxt7DZvjCltww2w1bgb+1XzLA9neGXLu2V055qHSm\nxtiHb+lF7IJ9oHh/9yjyd0e5MdBgiWA5NzScSrCS5ibiIM+hc6BfFnPrYDrS7BxQ\na7RXbr3fdwAdgL8Tc06u5zhtky+7jxMyJ4GEecyOg4OmK9+NU8QdlH+LPnXd4A0E\nlsyukX7YvCSC8q99F0nMafCRZmmP3TZFXVAou4OMJCxc1mmy4Iy54DFdEu9AQhlU\ntctELtUnbae6eKMCmN0CAwEAAaOB8DCB7TAdBgNVHQ4EFgQUtODZB4UBwe1U4dGA\nq5N4AgjwPn4wgb0GA1UdIwSBtTCBsoAUtODZB4UBwe1U4dGAq5N4AgjwPn6hgY6k\ngYswgYgxCzAJBgNVBAYTAkNOMRAwDgYDVQQIEwdTaGFhbnhpMQ4wDAYDVQQHEwVY\naSdhbjETMBEGA1UEChMKdXhzaW5vLmNvbTEMMAoGA1UECxMDZHpiMRMwEQYDVQQD\nEwpkdXpoZW5nYmluMR8wHQYJKoZIhvcNAQkBFhA4NDczNzk5NjJAcXEuY29tggkA\n9E1cz+J6+2AwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQUFAAOCAQEAFr16IdXE\n/nVQ4B5Mtht6fZC+2oN/Izbi29MFmImrKHAMBft+so/8m47IF14p13O9hBZx7TbK\nbxgWQsg6qCRj3KB2KjKbveBfNp4FUQparUmkCxKV0tGKzbnRY1clR7bLng7bYNWu\nM12PN8l3QqUEh7B/X+xy1PPIrDSGMhsDoaxNbJyK0xq8DGqdOt69tlgD4J6WpnoO\nNAu4ugsI8JliTpyWN99dZClFvgZ0gBfzdT+N2Sz4lzkbn7gSng9jSeSWtVNn9kuH\n7u0Y7Kq8YOjw6Qs0NsUOHnIG1S2yGc6yaBsZxJuZabJcwdMlSwP8sV4N0UQxeftg\nOtFZB/x+QMqSHA==\n-----END CERTIFICATE-----\n\"}");
	printf("str = %s\n",str);
	encrypt(hkey,str,strlen(str),stren,&len);
	//printf("stren = %s\n",stren);
	printf("--------------------------------------------\n--------------------------------------------\n--------------------------------------------\n--------------------------------------------\n");
	set_file_data("endata",stren);
	decrypt(hkey,stren,strlen(str),strde,&len);
	//set_file_data("endata",stren);
	printf("strde = %s\n",strde);
	printf("strlen = %d  %d\n",strlen(str),strlen(strde));
	
	return 0;
}

