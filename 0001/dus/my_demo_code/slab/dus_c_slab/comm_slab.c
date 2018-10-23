#include<comm_slab.h>
int create_slab(struct uxsino_slab* slab,int slab_max_num,int slab_size){
	INIT_LIST_HEAD(&slab->m_free_mem_objs);//初始化free列表
    INIT_LIST_HEAD(&slab->m_used_mem_objs);//初始化used列表
	//slab.m_used_mem_objs = NULL;
	slab->m_mem_unit = malloc((slab_max_num*slab_size)); 
	if(!slab->m_mem_unit){
		return ERROR_MALLOC_FAILED;
	}
	slab->m_mem_uns = slab->m_mem_unit;
	for(int i = 0;i <slab_max_num;i++){
		slab.m_mem_uns->ptr = slab->m_mem_unit;
		slab.m_mem_uns->element = (void*)(slab->m_mem_unit+slab_size);
		slab.m_free_mem_objs = slab.m_free_mem_objs.next;
	}
	
}
int release_slab(struct uxsino_slab* slab ){
	
}
bool is_in_slab(const void* ptr){//判断存在于缓冲区内
    if (ptr < m_mem_unit || ptr > m_mem_unit + m_unit_max_num * m_unit_size) {
		return false;
    }
    return true;
}
void* alloc(){
    struct mem_unit* mem_un;
    if (!list_empty(&m_free_mem_objs)) {
        mem_un = container_of(m_free_mem_objs.next, struct mem_unit, element);
        list_del(&mem_un->element);
        list_add_tail(&mem_un->element ,&m_used_mem_objs);
        return (void*)(mem_un->ptr);
    }else{
		//return new T;
        return NULL;
    }
}
void free(void* pptr){
	struct mem_unit* mem_un;
    if (is_in_slab(pptr)) {
        mem_un = container_of(m_used_mem_objs.next, struct mem_unit, element);
        list_del(&mem_un->element);
        list_add_tail(&mem_un->element ,&m_free_mem_objs);
        mem_un->ptr = pptr;
    }else{
        free(pptr);
    }	
}

	