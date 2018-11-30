#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
typedef unsigned char byte;
const int BYTE_BITS = 8;
const int INT_BIT_SIZE = sizeof (int)*8;
const char* bytes_to_binary_str(byte* v,int byte_count,char* out,int out_size){
	int idx = 0;
	for(int i=0;i < byte_count;i++){
		if(i>0) out[idx++] = '_';
		for(int j = 0;j < BYTE_BITS && i * BYTE_BITS + j < out_size;j++){
			out[idx++] = '0' +((v[i]>>(BYTE_BITS -j -1))&1);
		}
	}
	out[idx] = 0;
	return out;
}
const char* int_to_binary_str(int v,char* out,int out_size){
	int idx = 0;
	for(int i = 0;i < INT_BIT_SIZE && i < out_size;i++){
		if(i > 0 && i % BYTE_BITS == 0){
			out[idx++] = '_';
		}
		out[idx++] = '0' +((v >> (INT_BIT_SIZE -i - 1))&1);
	}
	out[idx] = 0;
	return out;
}
int int_to_zigzag(int n){
	return (n << 1) ^ (n >> 31);
}
int zigzag_to_int(int n){
	return (((unsigned int)n)>>1)^ -(n&1);
}
int write_to_buffer(int zz,byte* buf,int size){
	int ret = 0;
	for(int i = 0;i < size;i++){
		if((zz &(~0x7f)) == 0){
			buf[i] = (byte)zz;
			ret = i + 1;
			break;
		}else{
			buf[i] = (byte)(( zz&0x7f ) | 0x80);
			zz = ((unsigned int)zz) >> 7;
		}
	}
	return ret;
}
int read_from_buffer(byte* buf,int max_size){
	int ret = 0;
	int offset = 0;
	for(int i= 0;i < max_size;i++,offset+=7){
		byte n = buf[i];
		if((n & 0x80) != 0x80){
			ret |= (n << offset);
			break;
		}else{
			ret |= ((n&0x7f)<<offset);
		}
	}
	return ret;
}
int main(){
	int a[] = {(int)(-pow(2L,32)),-1000,-1,0,1,1000,(int)(pow(2L,31) - 1L)};
	for(int i = 0;i < sizeof(a)/sizeof(int);i++){
		int n = a[i];
		char str[INT_BIT_SIZE + INT_BIT_SIZE/BYTE_BITS +1];
		int_to_binary_str(n,str,sizeof(str));
		
		int zz = int_to_zigzag(n);
		char str_zz[INT_BIT_SIZE + INT_BIT_SIZE/BYTE_BITS +1];
		int_to_binary_str(zz,str_zz,sizeof(str_zz));
		
		byte write_buffer[5];
		int to_write_size = write_to_buffer(zz,write_buffer,sizeof(write_buffer));
		char str_write[to_write_size * BYTE_BITS + to_write_size +1];
		bytes_to_binary_str(write_buffer,to_write_size,str_write,sizeof(str_write));
		int read_num = read_from_buffer(write_buffer,to_write_size);
		int last_num = zigzag_to_int(read_num);
		printf("%11d[%s] ==to-zigzag==> %6d[%s]  ===to-buf==> %s\n",n,str,zz,str_zz,str_write);
	}
	return 0;
}


 