#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _parent
{
    int data_parent;
    void (*get_data)(void *);
}Parent;
typedef struct _child
{
    struct _parent parent;
    int data_child;
}Child;

void get_data_Parent (void *this)
{
    struct _parent * temp = (struct _parent *)this;
    printf("woshi parent : %d\n",temp->data_parent);
}
void get_data_Child(void *this)
{
    struct _child * temp = (struct _child *)this;
    temp->data_child = 30;
    printf("chengyuan hanshu child : %d\n",temp->data_child);
}

struct _parent * new_Parent(int data)
{
    struct _parent * temp = (struct _parent *)malloc(sizeof(struct _parent));
    temp->data_parent = data;
    temp->get_data = (void (*)(void *))get_data_Parent;
    return temp;
}

struct _child * new_child(int data)
{
    struct _child * temp = (struct _child *)malloc(sizeof(struct _child));
    temp->data_child = data;
    temp->parent.get_data = (void (*)(void *))get_data_Child;
    printf("gouzaohanshu child : %d\n",temp->data_child);
    return temp;
}


int main(int argc,char *argv[]){
    struct _parent * base = new_Parent(100);
    base->get_data((void *)base);

    base = (struct _parent *)new_child(80);
    base->get_data((void *)base);

    

    return 0;
}