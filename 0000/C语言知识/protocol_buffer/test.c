#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "game.pb-c.h"
 static void proto_store_string(
    char **addr,
    const char * str,
    int max_str_len)
{
    *addr = (char *)malloc(max_str_len);
    strncpy(*addr, str, max_str_len);
    (*addr)[max_str_len - 1] = '\0';    
}

int protobuf_game_pack(
    unsigned char * out_buff,
    int buff_len)
{
    /** 1.Assembly and packaging data */
#define STR_LEN 20

    /**
     * 1.初始化方式有两种，一种是定义变量时，如下边的TEAM_INFO__INIT;
     *   另一种是变量已经定义好了，则需要使用函数初始化，如personal_info__init()。
     * 2.每一个结构都必须初始化之后才能使用，所有的初始化宏或函数都在相应的.pc-c.h中。
     * 3.如果结构体成员是指向结构体的指针，则需要为其动态分配空间。原因是栈上的空间有可能不足，
     *   堆上分配空间更安全。如下边的teaminfo.leader。
     */
    TeamInfo teaminfo = TEAM_INFO__INIT;
    teaminfo.leader = (PersonalInfo *)malloc(sizeof(PersonalInfo));
    teaminfo.aaron = (PersonalInfo *)malloc(sizeof(PersonalInfo));
    personal_info__init(teaminfo.leader);
    personal_info__init(teaminfo.aaron);

    /**
     * 1.对于string成员也需要动态分配空间，如teaminfo.leader->name
     * 2.对于数组成员，常常是二重指针，也需要分配空间，如teaminfo.leader->armor；
     *   在结构体成员中也多出了一个n_armor成员，该成员需要填入数组的长度，方便打包时确认。
     * 3.对于整型成员，如teaminfo.leader->money，发现它也多出了一个has_money成员，因为
     *   在game.proto中我们对money定义的属性是optional。如果has_money=0，表示money成员没有数据；
     *   反之，表示money成员带有数据。
     * 4.对于string和结构体等成员，不管他们的类型是否是optional的，都不需要增加多余的成员。原因是这些成员
     *   在结构体中是由指针表示的，可以直接检查该指针是否为空来判断该成员是否有数据。例如teaminfo.aaron。
     */
    /** Assemble leader */
    proto_store_string(&teaminfo.leader->name, "leader", STR_LEN);
    teaminfo.leader->year = 20;
    proto_store_string(&teaminfo.leader->weapon, "Lightning sword", STR_LEN);
    teaminfo.leader->n_armor = 3;
    teaminfo.leader->armor =
        (char **)malloc(sizeof(char *) * teaminfo.leader->n_armor);
    proto_store_string(&teaminfo.leader->armor[0], "Lightning armor", STR_LEN);
    proto_store_string(&teaminfo.leader->armor[1], "Lightning pants", STR_LEN);
    proto_store_string(&teaminfo.leader->armor[2], "Lightning boots", STR_LEN);
    teaminfo.leader->has_money = 1;
    teaminfo.leader->money = 1000;

    /** Assemble aaron */
    proto_store_string(&teaminfo.aaron->name, "aaron", STR_LEN);
    teaminfo.aaron->year = 18;
    proto_store_string(&teaminfo.aaron->weapon, "Fire sword", STR_LEN);
    teaminfo.aaron->n_armor = 3;
    teaminfo.aaron->armor =
        (char **)malloc(sizeof(char *) * teaminfo.aaron->n_armor);
    proto_store_string(&teaminfo.aaron->armor[0], "Fire armor", STR_LEN);
    proto_store_string(&teaminfo.aaron->armor[1], "Fire pants", STR_LEN);
    proto_store_string(&teaminfo.aaron->armor[2], "Fire boots", STR_LEN);
    teaminfo.aaron->has_money = 0;

    /**
     * 1.结构体填充完数据后，我们需要将数据编码打包并复制到缓冲上。
     * 2.在数据进行打包之前需要先判断打包后数据的长度是否比缓冲区大，
     *   这时可以调用结构体对应的判断数据长度的函数，如team_info__get_packed_size()。
     * 3.确认打包后长度确实比可使用的缓冲区小时，则可以直接调用结构体的打包函数将数据打包到
     *   缓冲区上，如team_info__pack()。
     */
    /** packaging data */
    int len = team_info__get_packed_size(&teaminfo);

    if (len > buff_len) {
        printf("not enough memory..\n");
        len = 0;
    } else {
        if (len != team_info__pack(&teaminfo, out_buff)) {
            len = 0;
        }
    }

    /**
     * 1.释放动态分配的空间。
     */
    /** free */
    int i = 0;
    for(; i < teaminfo.leader->n_armor; i++) {
        free(teaminfo.leader->armor[i]);
    }
    free(teaminfo.leader->armor);
    free(teaminfo.leader->weapon);
    free(teaminfo.leader->name);
    free(teaminfo.leader);


    i = 0;
    for(; i < teaminfo.aaron->n_armor; i++) {
        free(teaminfo.aaron->armor[i]);
    }
    free(teaminfo.aaron->armor);
    free(teaminfo.aaron->weapon);
    free(teaminfo.aaron->name);
    free(teaminfo.aaron);

    return len;
}

int protobuf_game_parse(
    unsigned char * in_buff,
    int buff_len)
{
    /**
     * 调用解包函数，得到结构体指针
     */
    /** parse data */
    TeamInfo * teaminfo = team_info__unpack(NULL, buff_len, in_buff);

    /**
     * 获取数据，只要成员是指针都必须判断该指针是否为空，optional的其它类型也需要进行判断
     */
    if (teaminfo->leader) {
        PersonalInfo *leader = teaminfo->leader;
        printf("leader->name [%s]\n",leader->name);
        printf("leader->year [%d]\n",leader->year);
        printf("leader->weapon [%s]\n",leader->weapon);

        int i = 0;
        for (; i < leader->n_armor; i++) { //数组需要判断长度
            printf("leader->armor[%d] [%s]\n", i, leader->armor[i]);
        }

        if (leader->has_money) { //判断是否有money
            printf("leader->money [%d]\n",leader->money);        
        }
    }

    printf("\n");

    if (teaminfo->aaron) {
        PersonalInfo *aaron = teaminfo->aaron;
        printf("aaron->name [%s]\n",aaron->name);
        printf("aaron->year [%d]\n",aaron->year);
        printf("aaron->weapon [%s]\n",aaron->weapon);

        int i = 0;
        for (; i < aaron->n_armor; i++) {
            printf("aaron->armor[%d] [%s]\n", i, aaron->armor[i]);
        }

        if (aaron->has_money) {
            printf("leader->money [%d]\n",aaron->money);        
        }
    }

    /** free memory */
    team_info__free_unpacked(teaminfo, NULL);
    return 0;    
}
void proto_test_demo(void){
	#define BUFF_LEN 1024
    unsigned char buff[BUFF_LEN] = {0};
    int len = 0;

    /* 打包数据 */
    len = protobuf_game_pack(buff, BUFF_LEN);

    /* 解包数据 */
    protobuf_game_parse(buff, len);
}
int main(){
	proto_test_demo();
	return 0;
}


 