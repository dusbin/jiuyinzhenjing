/**		
 * @file    	sms4.h
 * @date: 		2016/03/09	
 * @brief  	    SMS4加解密头文件，包含三个函数：
 *              1. sms4_set_key
 *                  设置KEY
 *              2. sms4_encrypt
 *                  加密
 *              3. sms4_decrypt
 *                  解密
 * @author:		xhtian
 * 修改履历：	001)[修改原因1]		
 * 修改作者：	001)修改者 YYYY/MM/DD		
 * @version   	1.0
 * Copyright(c) All Right Reserved, Copyright(c) 2016.		
 */	
#ifndef __SMS4_H__
#define __SMS4_H__

/**
 * @brief 设置密钥
 * 		@param [in/out] unsigned int * key 密钥，长度128bits
 * @return void 无
 * @note  
 * @see   
 */
void sms4_set_key(unsigned int* key);

/**
 * @brief SMS4加密
 * 		@param [in/out] char * in 输入数据
 * 		@param [in/out] int in_len 输入数据长度，以字节计
 * 		@param [in/out] char * out 输出数据
 * 		@param [in/out] int out_len 输出数据长度，以字节计
 * @return void 无
 * @note  以128bits为一个加密单元，剩余不足128bits的数据不加密
 * @see   
 */
void sms4_encrypt(char* in, int in_len, char* out,int out_len);

/**
 * @brief SMS4解密
 * 		@param [in/out] char * in 输入数据
 * 		@param [in/out] int in_len 输入数据长度，以字节计
 * 		@param [in/out] char * out 输出数据
 * 		@param [in/out] int out_len 输出数据长度，以字节计
 * @return void 无
 * @note  以128bits为一个解密单元，剩余不足128bits的数据不解密
 * @see   
 */
void sms4_decrypt(char* in, int in_len, char* out,int out_len);

// 传输数据加密
void sms4_data_encrypt(
    char* input, 
    unsigned int input_len, 
    char* output, 
    unsigned int output_len);
// 传输数据解密
void sms4_data_decrypt(
    char* input, 
    unsigned int input_len, 
    char* output, 
    unsigned int output_len);


#endif

