/**************************************************************
* @file License.c
* 创建日期：2017/09/26
* 创建作者：liuyongzhen
* 修改履历：001)[修改原因1]
* 修改作者：001)修改者 YYYY/MM/DD
* All Right Reserved, Copyright(c) Beijing Uxsino Limited 2017.
* **************************************************************/
#include "License.h"
#include "Common.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
#include "stdbool.h"
#include "time.h"
#include "math.h"
PSET g_pair_set;
PAIRKEY g_pair_key;

const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 明文初始置换
char msg_ch[64] = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

// 密钥初始置换
char key_ch[56] = {
    57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12, 4
};

// 扩展置换
char msg_ex[48] = {
    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// 每轮密钥的位移
char key_mov[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// 压缩置换
char key_cmprs[48] = {
    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

// S 盒置换
char s_box[8][4][16] = {
    // S1
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
    // S2    
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
    // S3
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
    // S4
    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
    // S5
    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
    // S6
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
    // S7
    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
    // S8
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

// P 盒置换
char p_box[32] = {
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
    2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};

// 末置换
char last_ch[64] = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};

// hash 置换，将加密后的密文置换为可读明文
char hs_ch[20] = "0123456789abcdef";
char sh_ch[128];

void init_trans() {
    char i;
    for (i = 0; i < 16; i++)
        sh_ch[hs_ch[i]] = i;    // 完成hash转换的对应
}



// 字符转成二进制
void ChToBit(char* dest, char* src, int length) {
    int i, j;
    char t;
    for (i = 0; i < length; i++) {
        for (j = 8, t = src[i]; j > 0; j--) {
            dest[(i << 3) + j] = t & 1;   // 取字符末位
            t >>= 1;
        }
    }
}

// 二进制转成字符
void BitToCh(char* dest, char* src, int length) {
    int i;
    for (i = 0; i < length << 3; i++) {
        dest[i >> 3] <<= 1;
        dest[i >> 3] |= src[i + 1];   // 添加到末位
    }
    dest[length] = 0;
}

// 批置换，以offset为偏移，以count为长度
void BatchSet(char* dest, char* src, char* offset, int count) {
    int i;
    for (i = 0; i < count; i++)
        dest[i + 1] = src[offset[i]];
}

// 得到16轮所需的密钥
void getKeys() {
    char tk[128], bk[72];
    char* ptk = tk;
    int i, j;
    for (i = 0; i < 8; i++)
        key[i] <<= 1; // 跳过奇偶校验位
    ChToBit(bk, key, 8);
    BatchSet(tk, bk, key_ch, 56);
    for (i = 0; i < 16; i++) {
        for (j = 0; j < key_mov[i]; j++, ptk++) {
            ptk[57] = ptk[28];
            ptk[28] = ptk[1];
            // ptk++ 为亮点所在，实质上每一位都没有左移，只是指针右移了
        }
        BatchSet(keyb[i], ptk, key_cmprs, 48);
    }
}

// 将加密后的密文转换为可读的明文
void msgPro(char* dest, char* src) {
    int i, j;
    for (i = 0; i < 16; i++) {
        dest[i] = 0;
        for (j = 1; j <= 4; j++) // 取4位按hash构造一个字符
            dest[i] = (dest[i] << 1) | src[i * 4 + j];
        dest[i] = hs_ch[dest[i]];
    }
    dest[i] = 0;
}

// 将密文转换为真正的密文
void dropMsg(char* dest, char* src) {
    int i;
    for (i = 0; i < 16; i++) { // 为hash的逆运算过程
        dest[i >> 1] = (dest[i >> 1] << 4) | sh_ch[src[i]];
    }
}

// 通用加/解密函数，后面三个参数由宏ENCODE和DECODE提供
void DES(char* pmsg, int st, int cl, int step) {
    int i, row, col;    
    char r[64], rt[48], s[8];
    ChToBit(msgbt, pmsg, 8);
    BatchSet(msgb, msgbt, msg_ch, 64); // 初始置换
    for (; st != cl; st += step) {
        memcpy(rt, msgb + 33, 32);
        BatchSet(r, msgb + 32, msg_ex, 48); // 扩展置换
        for (i = 1; i <= 48; i++)
            r[i] ^= keyb[st][i]; // 异或操作
        // s_box 代替
        for (i = 0; i < 48; i += 6) {
            row = col = 0;
            row = r[i + 1] << 1 | r[i + 6];
            col = (r[i + 2] << 3) | (r[i + 3] << 2) | (r[i + 4] << 1) | r[i + 5];
            s[i / 12] = (s[i / 12] <<= 4) | s_box[i / 6][row][col];
        }
        ChToBit(r, s, 4);
        BatchSet(msgb + 32, r, p_box, 32); // p_box 置换
        for (i = 1; i <= 32; i++)
            msgb[i + 32] ^= msgb[i]; // 异或
        memcpy(msgb + 1, rt, 32);
    }
    memcpy(msgbt + 33, msgb + 1, 32);
    memcpy(msgbt + 1, msgb + 33, 32);
    BatchSet(msgb, msgbt, last_ch, 64); // 末置换
    if (step == 1)
        msgPro(res, msgb); // 使密文可读
    else
        BitToCh(res, msgb, 8); // 转为原明文
}

/* 
 * 功能:使用des算法加密字符串
 * 详细参数:
 *  des_key——密钥
 *  plaintext——要加密的明文
 */    
int DES_encrypt(char* plaintext ,char cw[], char *des_key) {
    int i;

    init_trans();  
    /* 读取参数过程 */
    strcpy(msg, plaintext);
    strcpy(key, des_key);
    getKeys(); /* 得到16轮要用到的密钥 */

    for (i = 0; msg[i]; i += 8) {
        DES(msg + i, ENCODE); // 加密
        strcat(cw,res);
    }

    return 0;
}

/**************************************************************
* 函数名称:    get_json_date
* 输入参数:    无
* 输出参数:    json_date : 获取的license文件中的json数据
* 返 回 值:    ret_val   : FAIL失败 OK 成功
* 修改记录:
* 修 改 人:
* 修改原因:
* **************************************************************/
int get_json_date(char **json_date,char * data)
{
	
	FILE      *jsonfd = NULL;
	int       ret_val = FAIL;
	int     file_size = EMPTY;
	int file_max_size = ZERO;
	int      num_read = ZERO;
	time_t systime = 0;
	char json_systime[MAX_JSON_SYSTIME_LENGTH] = {0};
	char json_file_content[STRING_MAX_LEN] = {0};
	num_read = strlen(data);
	file_max_size = num_read + MAX_JSON_SYSTIME_LENGTH + 1;
	(*json_date) = (char*)malloc(sizeof(char)*file_max_size);
	//don't copy '}'
	strncpy(json_file_content, (char*)data, (num_read - 2) < STRING_MAX_LEN?(num_read - 2):STRING_MAX_LEN);
	memset(*json_date, 0, sizeof(char)*file_max_size);
	systime = time(NULL);
	snprintf(json_systime, sizeof(json_systime) - 1, "\",\"SystermTime\": %d}", systime);
	snprintf(*json_date, (sizeof(char)*file_max_size) - 1, "%s%s", json_file_content, json_systime);

	return OK;

}

/**************************************************************
* 函数名称:    parse_license_configfile 解析license配置文件
* 输入参数:    无
* 输出参数:    license_config_json_date:解析license配置文件数据到该json对象中
* 返 回 值:    ret_val: FAIL失败 OK 成功
* 修改记录:
* 修 改 人:
* 修改原因:
* **************************************************************/
int parse_license_configfile(cJSON **license_config_json_date,char *data)
{
	int     ret_val = FAIL;
	char *json_date = NULL;

	ret_val = get_json_date(&json_date,data);
	if (OK != ret_val)
	{
		fprintf(stderr, "parse_license_configfile: get_json_date fail!\n");
		return ret_val;

	}
	if (NULL == json_date)
	{
		fprintf(stderr, "parse_license_configfile: get license json config file  fail!\n");
		return ret_val;

	}
 	(*license_config_json_date) = cJSON_Parse(json_date);

	free(json_date);
	json_date = NULL;

	return OK;
}

/**************************************************************
* 函数名称:    rsa_encrypt 加密算法
* 输入参数:    n:最小公倍数，e:私钥mw:明文，mLength:明文字符串的长度，
* 输出参数:    cw :密文
* 返 回 值:    ret_val: FAIL失败 OK 成功
* 修改记录:
* 修 改 人:
* 修改原因:
* **************************************************************/
void rsa_encrypt( int n, int e, char *mw, int mLength, int *cw )
{
	int i=0, j=0;
	long temInt = 0;

	for( i=0; i<mLength; i++ )
	{
		temInt = mw[i];
		if( e!=0 )
		{
			for( j=1; j<e; j++ )
			{
				temInt = (long)( temInt * mw[i] )%n;
			}
		}
		else
		{
			temInt = 1;
		}

		cw[i] = (int)temInt;

	}

}

/**************************************************************
* 函数名称:    output_encryptor_licensefile 将密文输出到uxdb.lic文件中
* 输入参数:    license_ciphertext:密文，len :密文长度
* 输出参数:    无
* 返 回 值:    ret_val: FAIL失败 OK 成功
* 修改记录:
* 修 改 人:
* 修改原因:
 * **************************************************************/
int output_encryptor_licensefile(char *license_content, int l_len)
{
	int ret_val = FAIL;
	FILE* encryptor_license_fd = NULL;
	char licFilePath[FILEPATHLEN] = {0};
	char base64[MAX_CIPHER_TEXT_LENGTH] = {0};

	snprintf(licFilePath, sizeof(licFilePath), "%s/%s", UXDB_LICENSE_DEFAULT_INSTALL_PATH, LICENSE_ENCRYTOR_FILE);

	encryptor_license_fd = fopen("user.data","w");
	if (NULL == encryptor_license_fd)
	{
		fprintf(stderr, "output_encryptor_licensefile: create encrytor file fail!\n");
		return ret_val;
	}

	base64_encode(license_content, base64, l_len);
	fprintf(encryptor_license_fd, "%s", base64);

	fclose(encryptor_license_fd);
	encryptor_license_fd = NULL;

	return OK;
}

/**************************************************************
* 函数名称:    encrypt_license_file 对license文件加密
* 输入参数:    无
* 输出参数:    无
* 返 回 值:    ret_val      : FAIL失败 OK 成功
* 修改记录:
* 修 改 人:
* 修改原因:
* **************************************************************/
int encrypt_license_file(PAIRKEY *pair_key,char *data)
{
	int ret_val = FAIL;
	int  *deskey_ciphertext = NULL;
	char  license_ciphertext[MAX_CIPHER_TEXT_LENGTH] = {0};
	char* license_plaintext = NULL;
	int len =0;
	char des_key[RAND_NUM+1] = { 0 };
	char enxrypt_content[MAX_CIPHER_TEXT_LENGTH] = {0};
	char final_license[MAX_CIPHER_TEXT_LENGTH] = {0};
	/* step1:解析配置文件 */
	ret_val = parse_license_configfile(&g_license_config_json_date,data);
	if (NULL == g_license_config_json_date)
	{
		fprintf(stderr, "encrypt_license_file:get license_config_json_date fail!\n");
		return ret_val;
	}
	if (OK != ret_val)
	{
		fprintf(stderr, "encrypt_license_file: parse_license_configfile fail!\n");
		cJSON_Delete(g_license_config_json_date);
		g_license_config_json_date = NULL;
		return ret_val;
	}

	deskey_ciphertext = (int*)malloc(sizeof(char)*MAX_NUM);
	if(NULL == deskey_ciphertext)
	{
		
		fprintf(stderr, "decrypt_license_file malloc fail!\n");
		cJSON_Delete(g_license_config_json_date);
		g_license_config_json_date = NULL;
		return ret_val;
	}
	memset(deskey_ciphertext,0,sizeof(char)*MAX_NUM);

	/* step3:将json对象转化为字符串 */
	license_plaintext = cJSON_Print(g_license_config_json_date);

	/* step4:随机生成进行des加密所需要的key(16位) */
	genRandomString(RAND_NUM+1, des_key);

	/* step5:进行des加密 */
	DES_encrypt(license_plaintext,license_ciphertext,des_key);
	snprintf(enxrypt_content, sizeof(enxrypt_content) - 1, "%s$%d*%d@", license_ciphertext, pair_key->publicKey, pair_key->n);

	/* step6:将des加密使用的key进行rsa加密 */
	rsa_encrypt(pair_key->n, pair_key->privateKey, des_key, RAND_NUM,deskey_ciphertext);

	snprintf(final_license, sizeof(final_license) - 1, "%d+%d+%d+%d+%d+%d+%d+%d+%d+%d+%d+%d+%d+%d+%d+%d&%s",\
			deskey_ciphertext[0], deskey_ciphertext[1], deskey_ciphertext[2], deskey_ciphertext[3],\
			deskey_ciphertext[4], deskey_ciphertext[5], deskey_ciphertext[6], deskey_ciphertext[7],\
			deskey_ciphertext[8], deskey_ciphertext[9], deskey_ciphertext[10], deskey_ciphertext[11],\
			deskey_ciphertext[12], deskey_ciphertext[13], deskey_ciphertext[14], deskey_ciphertext[15],\
			enxrypt_content);
	/* step7:将加密的数据输出到uxdb.lic文件 */
	ret_val = output_encryptor_licensefile(final_license,strlen(final_license));
	if (OK != ret_val)
	{
		fprintf(stderr, "decrypt_license_file get license_config_json_date fail!\n");
		cJSON_Delete(g_license_config_json_date);
		g_license_config_json_date = NULL;
		free(deskey_ciphertext);
		deskey_ciphertext = NULL;
		return ret_val;
	}

	/* step8:资源释放 */
	free(deskey_ciphertext);
	deskey_ciphertext = NULL;
	cJSON_Delete(g_license_config_json_date);
	g_license_config_json_date = NULL;

	return OK;
}

/*
 *Function: encode the bindata base64
 */
char * base64_encode(const unsigned char * bindata, char * base64, int binlength)
{
    int i, j;
    unsigned char current;

    for ( i = 0, j = 0 ; i < binlength ; i += 3 )
    {
        current = (bindata[i] >> 2) ;
        current &= (unsigned char)0x3F;
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
        if ( i + 1 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
        if ( i + 2 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
        base64[j++] = base64char[(int)current];
    }
    base64[j] = '\0';
    return base64;
}

/*
 * 功能：根据时间产生长度为length-1的随机字符串
 */   
int genRandomString(int length, char* ouput)
{
	int flag, i;
	srand((unsigned)time(NULL));
	for (i = 0; i < length - 1; i++)
	{
		flag = rand() % 3;
		switch (flag)
		{
		case 0:
			ouput[i] = 'A' + rand() % 26;
			break;
		case 1:
			ouput[i] = 'a' + rand() % 26;
			break;
		case 2:
			ouput[i] = '0' + rand() % 10;
			break;
		default:
			ouput[i] = 'x';
			break;
		}
	}
	return 0;
}

/*
 * 功能：根据时间随机生成大素数
 */
int GenerateRandPrimeNumber()
{
	long digit = 2;//最小的素数
	int i=0;

	//srand作为rand的种子()内为系统时间
	srand((int)time(0));
	while(1)
    {
		//获取3-10240之间的随机整数
        digit = rand()%MAX_PRIME_NUM + 3;
        while((digit % 2) == 0)
        	digit = rand()%MAX_PRIME_NUM + 3;//随机产生一个奇数

        for(i=2; i <= sqrt(digit); i++)
        {
        	if((digit % i) == 0)  break;
        }

        if(i > sqrt(digit))
        {
        	printf("digit %ld is big prime!\n", digit);
            break;
        }
    }
    return digit;
}

/*
 * 功能：根据随机生成的素数产生公钥和私钥文件
 */
int GeneratePublicKey(PAIRKEY *pair_key)
{
	int result = ERROR_GENERROR;
	unsigned int prime_count = 0;

	PAIR_PRIME g_pair_prime_number;
	//初始化两个素数
	g_pair_prime_number.digit_one = 139;
	g_pair_prime_number.digit_two = 233;

	//生成随机素数对
	g_pair_prime_number.digit_one = GenerateRandPrimeNumber();
	//等待1秒后再随机生成一个素数
	sleep(1);
	g_pair_prime_number.digit_two = GenerateRandPrimeNumber();

	//生成所有小于g_pair_prime_number.digit_one*g_pair_prime_number.digit_two，并且与其互质的数
	prime_count = MakePrivatedKeyd(g_pair_prime_number.digit_one, g_pair_prime_number.digit_two);

	if (prime_count < DENOMINATOR)
	{
		//私钥集合中私钥的数量不能小于规定的分母值4
		return FUNC_ERR;
	}

	result = MakePairkey(g_pair_prime_number.digit_one, g_pair_prime_number.digit_two, (prime_count/DENOMINATOR*MOLECULAR));
	if (GENERATE_OK == result)
	{
		pair_key->privateKey = g_pair_key.privateKey;
		pair_key->publicKey = g_pair_key.publicKey;
		pair_key->n = g_pair_key.n;

		return FUNC_OK;
	}
	else
	{
		return FUNC_ERR;
	}
}

/*
 * 名称：MakePrivatedKeyd
 * 功能：由素数uiP、uiQ生成私钥d
 * 参数：uiP: 素数P; uiQ: 素数Q
 * 返回：私钥d
 * */
unsigned int MakePrivatedKeyd(long uiP, long uiQ)
{
	unsigned int i=0;

	//得到所有与z互质的数(私钥d的集合)
	unsigned int z = (uiP - 1) * (uiQ - 1);
	g_pair_set.size = 0;
	memset(g_pair_set.set, 0, sizeof(g_pair_set.set));

	for(i=0; i<z && (g_pair_set.size < (MAX_NUM - 1)); i++)
	{
		if(FUNC_OK == isPrime(i, z))
		{
			g_pair_set.set[g_pair_set.size++] = i;
		}
	}
	printf("g_pair_set.size=[%d]\n", g_pair_set.size);
	return g_pair_set.size;
}

/*
 * 名称：isPrime
 * 功能：判断两个数是否互质
 * 参数：m: 数a; n: 数b
 * 返回：m、n互质返回true; 否则返回false
 */
int isPrime(unsigned int m, unsigned int n)
{
	unsigned int i = 0;
	int Flag = FUNC_OK;

	if(m<2 || n<2)
		return FUNC_ERR;

	unsigned int tem = (m > n) ? n : m;
	for(i=2; i<=tem && (FUNC_OK == Flag); i++)
	{
		int mFlag = FUNC_OK;
		int nFlag = FUNC_OK;

		if(m % i == 0)
			mFlag = FUNC_ERR;
		if(n % i == 0)
			nFlag = FUNC_ERR;
		if((FUNC_ERR == mFlag) && (FUNC_ERR == nFlag))
			Flag = FUNC_ERR;
	}

	if((FUNC_OK == Flag))
		return FUNC_OK;
	else
		return FUNC_ERR;
}

/*
 * 名称：MakePairKey
 * 功能：生成RSA公、私钥对
 * 参数：uiP: 素数P; uiQ: 素数Q; uiD: 私钥d
 * 返回：错误代码
 * */
int MakePairkey(long uiP, long uiQ, long uiD)
{
	int bFlag = FUNC_OK;
	unsigned int i = 0;
	unsigned int z = (uiP - 1)*(uiQ - 1);
	long private_key = g_pair_set.set[uiD];
	long public_key = 0;

	if(FUNC_ERR == isPrime(z, private_key))
		return ERROR_NOEACHPRIME;

	for(i=2; i<z; i++)
	{
		if(((i*private_key) % z) == 1)
		{
			public_key = i;
			bFlag = FUNC_ERR;
		}
	}
	if(FUNC_OK == bFlag)
		return ERROR_NOPUBLICKEY;

	if(((private_key*public_key) % z) != 1)
		return ERROR_GENERROR;

	//私钥对和公钥对写入对应的文件中
	g_pair_key.privateKey = private_key;
	g_pair_key.publicKey = public_key;
	g_pair_key.n = uiP * uiQ;//最大公约数

	printf("g_pair_key.privateKey=[%d], g_pair_key.publicKey=[%d], g_pair_key.n=[%d]\n", g_pair_key.privateKey, g_pair_key.publicKey, g_pair_key.n);

	return GENERATE_OK;
}
/*
 * Function: read the value in the encrypted license file and encrypted key value of DES
 */
int readLicenseValue(unsigned char *licP, int *desP, PUBLICPAIRKEY *pairkey)
{
	int i = 0;
	int bytes = 0;
	int length = 0;
	FILE *fp = NULL;
	char *desStart = NULL;
	char *desEnd = NULL;
	char *licenseStart = NULL;
	char *licenseEnd = NULL;
	char *PublicKeyStart = NULL;
	char *PublicKeyEnd = NULL;
	char *leastCommonMultipleStart = NULL;
	char *leastCommonMultipleEnd = NULL;
	char desKey[MAX_CIPHER_TEXT_LENGTH] = {0};
	char finalDesKey[MAX_CIPHER_TEXT_LENGTH] = {0};
	char content[MAX_CIPHER_TEXT_LENGTH] = {0};
	char plainText[MAX_CIPHER_TEXT_LENGTH] = {0};
	char publickey[MAX_CIPHER_TEXT_LENGTH] = {0};
	char leastCommonMultiple[MAX_CIPHER_TEXT_LENGTH] = {0};

	fp = fopen(UXDB_LICENSE_FILE_FULL_PATH, "r");
	if (NULL == fp)
	{
		printf("can not access license file (%s).", UXDB_LICENSE_FILE_FULL_PATH);
		return length;
	}

	length = fscanf(fp, "%s", content);
	fclose(fp);

	if (length > 0)
	{
		//decode uxdb.lic content
		bytes = base64_decode(content, plainText);

		//get DES key
		desStart = plainText;
		desEnd = strchr(plainText, '&');
		if (NULL == desEnd) return 0;

		strncpy(desKey, desStart, (desEnd - desStart) < MAX_CIPHER_TEXT_LENGTH?(desEnd - desStart):(MAX_CIPHER_TEXT_LENGTH - 1));
		desStart = desKey;
		length = 1;
		while(desStart && (*desStart != '\0'))
		{
			if (*desStart == '+')
			{
				//length represent des key num
				desP[length - 1] = atoi(finalDesKey);
				memset(finalDesKey, 0, sizeof(finalDesKey));
				i = 0;
				length++;
			}
			else
			{
				finalDesKey[i++] = *desStart;
			}
			desStart++;
		}
		desP[length - 1] = atoi(finalDesKey);

		//get license value
		licenseStart = desEnd + 1;
		if ((NULL == licenseStart) || (*licenseStart == '\0')) return 0;
		licenseEnd = strchr(plainText, '$');
		if (NULL == licenseEnd) return 0;
		strncpy(licP, licenseStart, (licenseEnd - licenseStart) < MAX_CLEAR_TEXT_LENGTH?(licenseEnd - licenseStart):(MAX_CLEAR_TEXT_LENGTH - 1));

		//Get the value of the public key
		PublicKeyStart = licenseEnd + 1;
		if ((NULL == PublicKeyStart) || (*PublicKeyStart == '\0')) return 0;
		PublicKeyEnd = strchr(PublicKeyStart, '*');
		if (NULL == PublicKeyEnd) return 0;
		strncpy(publickey, PublicKeyStart, PublicKeyEnd - PublicKeyStart);
		pairkey->publicKey = atoi(publickey);

		//Get the least common multiple
		leastCommonMultipleStart = PublicKeyEnd + 1;
		if ((NULL == leastCommonMultipleStart) || (*leastCommonMultipleStart == '\0')) return 0;
		leastCommonMultipleEnd = strchr(leastCommonMultipleStart, '@');
		if (NULL == leastCommonMultipleEnd) return 0;
		strncpy(leastCommonMultiple, leastCommonMultipleStart, leastCommonMultipleEnd - leastCommonMultipleStart);
		pairkey->n = atoi(leastCommonMultiple);
	}

	return length;
}
/*
 * Function: base64 decode
 */
int base64_decode(const char * base64, unsigned char * bindata)
{
    int i, j;
    unsigned char k;
    unsigned char temp[4];
    for (i = 0, j = 0; base64[i] != '\0' ; i += 4)
    {
        memset(temp, 0xFF, sizeof(temp));
        for (k = 0 ; k < 64 ; k ++)
        {
            if (base64char[k] == base64[i])
                temp[0]= k;
        }
        for (k = 0 ; k < 64 ; k ++)
        {
            if (base64char[k] == base64[i+1])
                temp[1]= k;
        }
        for (k = 0 ; k < 64 ; k ++)
        {
            if (base64char[k] == base64[i+2])
                temp[2]= k;
        }
        for (k = 0 ; k < 64 ; k ++)
        {
            if (base64char[k] == base64[i+3])
                temp[3]= k;
        }

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
                ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
        if (base64[i+2] == '=')
            break;

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
                ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
        if ( base64[i+3] == '=' )
            break;

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
                ((unsigned char)(temp[3]&0x3F));
    }
    return j;
}
void rsa_decrypt(int n, int d, int *cw, int cLength, char *clearText)
{
	int i=0, j=-1;
	long temInt = 0;

	for(i=0; i < cLength; ++i)
	{
		clearText[i] = 0;
		temInt = cw[i];

		if(d != 0)
		{
			for(j=1; j<d; j++)
			{
				temInt = (long)(temInt * cw[i]) % n;
			}
		}
		else
		{
			temInt = 1;
		}

		clearText[i] = (char)temInt;
	}
}
int DES_decrypt(char* plaintext ,char cw[], char *des_key) {
    int i;

    init_trans();
    // Reading parameter process
    strcpy(msg, plaintext);
    strcpy(key, des_key);
    getKeys(); // Get 16 keys to use

    for (i = 0; msg[i]; i += 16) {
        dropMsg(res, msg + i); // Convert ciphertext to real ciphertext
        DES(res, DECODE); // Decryption
        strcat(cw,res);
    }

    return 0;
}
int output(char * clearText)
{
	FILE *fp;

	fp = fopen(UXDB_CLEAR_LICENSE_FILE_PATH,"w");
	if (NULL == fp)
	{
		printf("can not write license clear text information into %s.", UXDB_CLEAR_LICENSE_FILE_PATH);
		return ERR;
	}
	fprintf(fp,"%s\n",clearText);
	printf("The license file is decrypted successfully!\nPlease check in the (%s) file.\n", UXDB_CLEAR_LICENSE_FILE_PATH);

	fclose(fp);

	return OK;
}


