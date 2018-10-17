#include <iostream>
using namespace std;
 
//实现memcpy库函数
void* memcpy(void *dst, const void *src, size_t count){
    // 容错处理
    if(dst == NULL || src == NULL){
        return NULL;
    }
    unsigned char *pdst = (unsigned char *)dst;
    const unsigned char *psrc = (const unsigned char *)src;
    //判断内存是否重叠
    bool flag1 = (pdst >= psrc && pdst < psrc + count);
    bool flag2 = (psrc >= pdst && psrc < pdst + count);
    if(flag1 || flag2){
        cout<<"内存重叠"<<endl;
        return NULL;
    }
    // 拷贝
    while(count--){
        *pdst = *psrc;
        pdst++;
        psrc++;
    }
    return dst;
}
//实现memmove库函数
void* memmove(void *dst, const void *src, size_t count){
    // 容错处理
    if(dst == NULL || src == NULL){
        return NULL;
    }
    unsigned char *pdst = (unsigned char *)dst;
    const unsigned char *psrc = (const unsigned char *)src;
    //判断内存是否重叠
    bool flag1 = (pdst >= psrc && pdst < psrc + count);
    bool flag2 = (psrc >= pdst && psrc < pdst + count);
    if(flag1 || flag2){
        // 倒序拷贝
        while(count){
            *(pdst+count-1) = *(psrc+count-1);
            count--;
        }//while
    }
    else{
        // 拷贝
        while(count--){
            *pdst = *psrc;
            pdst++;
            psrc++;
        }//while
    }
    return dst;
}
void* memmove1(void* str1,const void* str2,size_t n){
    char* pStr1= (char*) str1;
    const char* pStr2=(const char*)str2;
    // 正序拷贝
    if  (pStr1  < pStr2) {
        for(size_t i=0;i!=n;++i){
            *(pStr1++)=*(pStr2++);
        }
    }
    // 倒序拷贝
    else{
        pStr1+=n-1;
        pStr2+=n-1;
        for(size_t i=0;i!=n;++i){
            *(pStr1--)=*(pStr2--);
        }
    }
    return pStr1;
}
int main(){
    char c1[] = "hello world";
    memcpy(c1+3, c1, 9);
    cout<<"memcpy result:"<<c1<<endl;
    char c2[] = "love you";
    memcpy(c1,c2,8);
    cout<<"memcpy result:"<<c1<<endl;
	   // 内存重叠
    char c11[] = "hello world";
    memmove(c11+3, c11, 8);
    cout<<"memmove result:"<<c11<<endl;
    // 内存不重叠
    char c22[] = "hello world";
    char c33[] = "love you";
    memmove(c22,c33,8);
    cout<<"memmove result:"<<c22<<endl;
}
/*
--------------------- 
作者：SunnyYoona 
来源：CSDN 
原文：https://blog.csdn.net/sunnyyoona/article/details/41877375?utm_source=copy 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/

