#include "comm_slab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
uxsino_slab g_slab;
void print(mem_unit mem[]){
    int i = 0;
    for(i = 0; i<32;i++){
        printf("%0.64s\n",mem[i].p);
    }
    printf("--------------------------------\n");
}
void print_point(mem_unit mem[]){
    printf("print by point\n");
    mem_unit* temp;
    temp = &mem[0];
    if(NULL == temp)
        return;
    int i = 0;
    do{
        i++;
        printf("%d %0.64s\n",i,(*temp).p);
        temp = temp->next;
    }while(NULL != temp);
    printf("--------------------------------\n");
}
void print_list(struct _tag_mem_unit* list){
    printf("--------print list--------------\n");
    if(NULL == list){
        printf("list is empty\n");
        printf("--------------------------------\n");
        return;
    }
    struct _tag_mem_unit* temp = list;
    int i=1;
    do{
        printf("%d %0.64s\n",i,(*temp).p);
        temp = temp->next;
        i++;
    }while(NULL != temp);
    printf("--------------------------------\n");
};
char* genRandomString(int length)  
{  
    int flag, i;  
    char* string;  
    srand((unsigned) time(NULL ));  
    if ((string = (char*) malloc(length)) == NULL )  
    {    
        return NULL ;  
    }  
  
    for (i = 0; i < length - 1; i++)  
    {  
        flag = rand() % 3;  
        switch (flag)  
        {  
            case 0:  
                string[i] = 'A' + rand() % 26;  
                break;  
            case 1:  
                string[i] = 'a' + rand() % 26;  
                break;  
            case 2:  
                string[i] = '0' + rand() % 10;  
                break;  
            default:  
                string[i] = 'x';  
                break;  
        }  
    }  
    string[length - 1] = '\0';  
    return string;  
}  

void test_001();
void test_002();
void test_003();
void test_004();
int main(){
    printf("test begin...\n");
    printf("--------------------------------\n");
    g_slab.ops = &g_uxsino_slab_ops;
    g_slab.ops->create_slab(&g_slab,32,1024);
    printf("create_slab_end\n");
    test_001();
    //test_002();
    //test_003();
    //test_004();
    g_slab.ops->release_slab(&g_slab);
    /*pint_list after free */
    printf("after release_slab\n");
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);
    printf("create_slab_end 2\n");
    g_slab.ops->create_slab(&g_slab,32,1024);
    //test_001();
    test_002();
    //test_003();
    //test_004();
    g_slab.ops->release_slab(&g_slab);
    /*pint_list after free */
    printf("after release_slab\n");
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);
    printf("create_slab_end 3\n");
    g_slab.ops->create_slab(&g_slab,32,1024);
    //test_001();
    //test_002();
    test_003();
    //test_004();
    g_slab.ops->release_slab(&g_slab);
    /*pint_list after free */
    printf("after release_slab\n");
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);
    printf("create_slab_end 4\n");
    g_slab.ops->create_slab(&g_slab,32,1024);
    //test_001();
    //test_002();
    //test_003();
    test_004();
    g_slab.ops->release_slab(&g_slab);
    printf("test end...\n");
    return 0;
}
void test_001(){
    printf("===========TEST_001===========\n");
    printf("g_slab.m_mem_uns\n");
    print(g_slab.m_mem_uns);
    print_point(g_slab.m_mem_uns);
    print_list(g_slab.m_free_mem_objs);
    print_list(g_slab.m_used_mem_objs);
    char* mem_slab = g_slab.ops->alloc_slab(&g_slab,1024);
    strcpy(mem_slab,"2017_10_17_10_20");
    print_point(g_slab.m_mem_uns);
    printf("g_slab.m_free_mem_objs\n");
    print_list(g_slab.m_free_mem_objs);
    printf("g_slab.m_used_mem_objs\n");
    print_list(g_slab.m_used_mem_objs);
    printf("%0.32s\n",mem_slab);
    char* mem_slab1 = g_slab.ops->alloc_slab(&g_slab,1024);
    strcpy(mem_slab1,"2017_10_17_10_29");
    print_point(g_slab.m_mem_uns);
    printf("g_slab.m_free_mem_objs\n");
    print_list(g_slab.m_free_mem_objs);
    printf("g_slab.m_used_mem_objs\n");
    print_list(g_slab.m_used_mem_objs);
    print(g_slab.m_mem_uns);
    printf("free_slab\n");
    g_slab.ops->free_slab(&g_slab,mem_slab);
    mem_slab = NULL;
    g_slab.ops->free_slab(&g_slab,mem_slab1);
    g_slab.ops->free_slab(&g_slab,mem_slab);
    print(g_slab.m_mem_uns);
    char * dzb = malloc(32);
    strcpy(dzb,"duzhengbin");
    printf("before free_slab %s\n",dzb);
    g_slab.ops->free_slab(&g_slab,dzb);
    if(dzb == NULL){
        printf("dzb==NULL\n");
    }
    printf("after free_slab %s\n",dzb);
    dzb = NULL;
    printf("after dzb = NULL %s\n",dzb);

}
/*分配全部空间和释放全部空间*/
void test_002(){
    printf("===========TEST_002===========\n");
    char * array[33];/**< array[32] is malloc by self*/
    int i = 0;
    char * temp;
    for(i = 0;i < 33;i++){
        array[i] = g_slab.ops->alloc_slab(&g_slab,32);
        strcpy(array[i],"uxsino xi'an duzhengbin");
        temp = genRandomString(i+1);
        strcat(array[i],temp);
        //printf("%s\n",temp);
    }
    printf("after alloc_slab\n");
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);
    printf("array[32] = %s\n",array[32]);
    for(i = 0;i <33;i++){
        g_slab.ops->free_slab(&g_slab,array[i]);
    }
    printf("after free_slab\n");
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);

}

void test_003(){
    printf("===========TEST_003===========\n");
    struct dus{
        char name[33];
        char tel[33];
        int id;
        
    };
    int i = 0;
    struct dus * in[32];
    for (i = 0 ;i < 32;i++){
        in[i] = (struct dus*)g_slab.ops->alloc_slab(&g_slab,1024);
        if(in[i] == NULL)
            break;
        in[i]->id = i;
        strcpy(in[i]->name,genRandomString(i+2));
        strcpy(in[i]->tel,genRandomString(i+2));
    }
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);
    for(i = 0;i<32;i++){
        if(in[i] == NULL)break;
        printf("[i = %d]id = [%d] name = [%s] tel = [%s]\n",i,in[i]->id,in[i]->name,in[i]->tel);
    }
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);
}
/*分配比slab块内存大的数据结构*/
void test_004(){
    printf("===========TEST_004===========\n");
    struct dus{
        char name[33];
        char tel[33];
        int id;
        
    };
    /*big than g_slab.m_unit_size*/
    struct dus_big{
        char name[1024];
        char tel[1024];
        int id;
        
    };
    struct dus * du = (struct dus*)g_slab.ops->alloc_slab(&g_slab,sizeof(struct dus));
    struct dus * du_big = (struct dus_big*)g_slab.ops->alloc_slab(&g_slab,sizeof(struct dus_big));
    du->id = 0;
    strcpy(du->name,genRandomString(7));
    strcpy(du->tel,genRandomString(7));
    du_big->id = 1;
    strcpy(du_big->name,genRandomString(14));
    strcpy(du_big->tel,genRandomString(14));
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);
    printf("id = [%d] name = [%s] tel = [%s]\n",du->id,du->name,du->tel);
    printf("id = [%d] name = [%s] tel = [%s]\n",du_big->id,du_big->name,du_big->tel);
    g_slab.ops->free_slab(&g_slab,du);
    g_slab.ops->free_slab(&g_slab,du_big);
    print_list(g_slab.m_used_mem_objs);
    print_list(g_slab.m_free_mem_objs);    
    
}

