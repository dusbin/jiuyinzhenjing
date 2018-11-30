#ifndef __LIST_H__
#define __LIST_H__
#ifndef NULL
#define NULL ((void*)0)
#endif
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) (                     \
	(type *)( (char *)(ptr) - offsetof(type,member) ))
struct list_head{struct list_head *next,prev;};
/*初始化链表，有一个元素，自身构成一个环*/
/*static inline*/ void INIT_LIST_HEAD(struct list_head *list){
	list->next = list;
	list->pre  = list;
}
/*在prev和next中间插入new，必须保证prev的下一个指针为next，所以一般不对外开放*/
/*static inline*/ void __list_add(struct list_head *new,struct list_head *prev,struct list_head *next){
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
/*头插法插入数据，head也可以是要插入位置的前一个节点*/
/*static inline*/ void list_add(struct list_head *new,struct list_head *head){
	__list_add(new,head,head->next);
}
/*尾插法插入数据，head也可以是要插入位置的后一个节点*/
/*static inline*/ void list_add_tail(struct list_head *new,struct list_head *head){
	__list_add(new,head->prev,head);
}
/*删除节点辅助函数，不对外提供*/
/*static inline*/ void __list_del(struct list_head *prev,struct list_head * next){
	next->prev = prev;
	prev->next = next;
}
/*删除节点*/
/*static inline*/ list_del(struct list_head *entry){
	__list_del(entry->prev,entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}
/*将节点list移到head节点后面*/
/*static inline*/ void list_move(struct list_head *list,struct list_head *head){
	__list_del(list->prev,list->next);
	list_add(list,head);
}
/*将节点list移到head节点的前面*/
/*static inline*/ void list_move_tail(struct list_head *list,struct list_head *head){
	__list_del(list->prev,list->next);
	list_add_tail(list,head);
}
/*是最后一个节点*/
/*static inline*/ int list_is_last(const struct list_head *list,const struct list_head *head){
	return list->next == head;
}
/*是第一个节点*/
/*static inline*/ int list_is_head(const struct list_head *list,const struct list_head *head){
	return list->prev == head;
}
/*只有一个元素*/
/*static inline*/ int list_is_single(const struct list_head *list,const struct list_head *head){
	return list->prev == head && list->next == head/*&&list!=head*/;
}
/*列表为空*/
/*static inline*/ int list_empty(const struct list_head *head){
	return head->next == head;
}
/*安全的列表为空*/
/*static inline*/ int list_empty_careful(const struct list_head *head){
	struct list_head *next = head->next;
	return (next == head)&&(head->prev == next);
}
/*用新节点替换旧节点*/
/*static inline*/ void list_replace(struct list_head *old,struct list_head *new){
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

/*遍历*/
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)
/*安全遍历*/		
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)
/*将链list插入到prev和next之间，*/
/*static inline*/ void __list_splice(const struct list_head *list,struct list_head *prev,struct list_head *next){
	struct list_head *first = list->next;//被合并链的头
	struct list_head *last = list->prev;//被合并链的尾
	first->prev = prev;
	prev->next = first;//将头节点接在head的头部
	last->next = next;
	next->prev = last;//将尾接在head链的第一个节点前面
}
/*将list链插入到head链的头部*/
/*static inline*/ void list_splice(const struct list_head *list,struct list_head *head){
	if(!list_empty(list))
		__list_splice(list,head,head->next);
}
/*将list链插入到head链的尾部*/
/*static inline*/ void list_splice_tail(const struct list_head *list,struct list_head *head){
	if(!list_empty(list))
		__list_splice(list,head->prev,head);
}
/*将list链插入到head链的头部然后初始化list链*/
/*static inline*/ void list_splice_init(const struct list_head *list,struct list_head *head){
	if(!list_empty(list)){
		__list_splice(list,head,head->next);
		INIT_LIST_HEAD(list);
	}
}
/*将list链插入到head链的尾部然后初始化list链*/
/*static inline*/ void list_splice_tail_init(const struct list_head *list,struct list_head *head){
	if(!list_empty(list)){
		__list_splice(list,head->prev,head);
		INIT_LIST_HEAD(list);
	}
}
#endif

