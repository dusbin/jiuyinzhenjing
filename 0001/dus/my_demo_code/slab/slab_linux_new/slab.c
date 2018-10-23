#include "comm_slab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct dus{
	char name[33];
	char tel[33];
	int id;
};
/**************************************************************    
* 函数名称：test_001()
* 功能概要：slab_create(),slab_release()的测试
* 功能详细：slab_create(),slab_release()的测试
* 输入参数：无
* 输出参数：无
* 返回值：  void
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/
void test_001(){
    printf("===============test_001 begin========================\n");
	uxsino_slab* slab_dzb = slab_create();
	slab_release(slab_dzb);
	slab_dzb = NULL;
    printf("===============test_001 end========================\n");

}
/**************************************************************    
* 函数名称：test_002()
* 功能概要：slab_malloc(),slab_free()的测试
* 功能详细：slab_malloc(),slab_free()的测试
* 输入参数：无
* 输出参数：无
* 返回值：  void
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/
void test_002(){
    printf("===============test_002 begin========================\n");
    printf("slab_create\n");
    uxsino_slab* slab_dzb = slab_create();
    struct dus * in[9];
    int i = 0;
	for(i = 0;i < 9;i++){
        in[i] = (struct dus*)slab_dzb->ops->slab_malloc(slab_dzb,sizeof(struct dus));
        if(in[i] == NULL) continue;
        in[i]->id = i;
        strcpy(in[i]->name,"uxsino");
        strcpy(in[i]->tel,"15500270029");
	}
    printf("after slab_malloc\n");
	for(i = 0;i < 9;i++){
        if(in[i] == NULL)continue;
        printf("[i = %d]id = [%d] name = [%s] tel = [%s]\n",i,in[i]->id,in[i]->name,in[i]->tel);
    }
    for(i = 0;i < 9;i++){
        slab_dzb->ops->slab_free(slab_dzb,in[i]);
        in[i]=NULL;
    }
    printf("after slab_free\n");
    for(i = 0;i < 9;i++){
        if(in[i] == NULL)continue;
        printf("[i = %d]id = [%d] name = [%s] tel = [%s]\n",i,in[i]->id,in[i]->name,in[i]->tel);
    }
	slab_release(slab_dzb);
	slab_dzb = NULL;
    printf("===============test_002 end========================\n");
}
/**************************************************************    
* 函数名称：test_003()
* 功能概要：slab_malloc(),slab_free()重复使用，slab_malloc()比slab_create()时内存区域大的测试
* 功能详细：slab_malloc(),slab_free()重复使用，slab_malloc()比slab_create()时内存区域大的测试
* 输入参数：无
* 输出参数：无
* 返回值：  void
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/
void test_003(){
    printf("===============test_003 begin========================\n");
    printf("slab_create\n");
    uxsino_slab* slab_dzb = slab_create();
    struct dus * in[9];
    int i = 0;
	for(i = 0;i < 9;i++){
        in[i] = (struct dus*)slab_dzb->ops->slab_malloc(slab_dzb,sizeof(struct dus));
        if(in[i] == NULL) continue;
        printf("in[%d] = [%s]\n",i,in[i]);
        in[i]->id = i;
        strcpy(in[i]->name,"uxsino");
        strcpy(in[i]->tel,"15500270029");
	}
    printf("after slab_malloc\n");
	for(i = 0;i < 9;i++){
        if(in[i] == NULL)continue;
        printf("[i = %d]id = [%d] name = [%s] tel = [%s]\n",i,in[i]->id,in[i]->name,in[i]->tel);
    }
    printf("slab_free in[4]\n");
    slab_dzb->ops->slab_free(slab_dzb,in[4]);
    in[4]=NULL;
    printf("after slab_free in[4]\n");
    for(i = 0;i < 9;i++){
        if(in[i] == NULL)continue;
        printf("[i = %d]id = [%d] name = [%s] tel = [%s]\n",i,in[i]->id,in[i]->name,in[i]->tel);
    } 
    printf("slab_malloc\n");
    struct dus* again = (struct dus*)slab_dzb->ops->slab_malloc(slab_dzb,sizeof(struct dus));
    printf("again = [%s]\n",again);
    again->id = 1;
    strcpy(again->name,"dzb");
    strcpy(again->tel,"15500210022");
    printf("id = [%d] name = [%s] tel = [%s]\n",again->id,again->name,again->tel);
    printf("slab_free in[4]\n");
    slab_dzb->ops->slab_free(slab_dzb,again);
    again=NULL;
    printf("slab_malloc sizof bigger than struct dus\n");
    struct dus* bigger = (struct dus*)slab_dzb->ops->slab_malloc(slab_dzb,sizeof(struct dus)+1);
    printf("bigger = [%s]\n",bigger);
    if(bigger == NULL){
        printf("bigger == NULL\n");
    }else{
        printf("bigger != NULL\n");
    }
	slab_release(slab_dzb);
	slab_dzb = NULL;
    printf("===============test_003 end========================\n");
}
void test_004(){
    printf("===============test_004 begin========================\n");
    printf("slab_create\n");
    uxsino_slab* slab_dzb = slab_create();
    printf("slab_malloc\n");
    print(slab_dzb);
    char* again = (char*)slab_dzb->ops->slab_malloc(slab_dzb,7);
    print(slab_dzb);
    strcpy(again,"dzb");    
    printf("again = [%s]\n",again);
    again = slab_dzb->ops->slab_realloc(slab_dzb,again,34);
    print(slab_dzb);
    strcpy(again,"duzhengbin"); 
    printf("again = [%s]\n",again);
    print(slab_dzb);

	slab_release(slab_dzb);
	slab_dzb = NULL;
    printf("===============test_004 end========================\n");

}
/**************************************************************    
* 函数名称：main()
* 功能概要：测试主入口
* 功能详细：测试主入口
* 输入参数：无
* 输出参数：无
* 返回值：  int
* 创建日期：2017/10/23    
* 创建作者：duzhengbin
* **************************************************************/
int main(){
    printf("begin test\n");
	//test_001();
	//test_002();
    //test_003();
    test_004();
	return 0;
}

