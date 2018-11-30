/**
   * @class gm_slab
   * @brief  类型cache，用于快速申请与释放内存空间
   */
  template <class T>
  class gm_slab
  {
    /**
     * @struct mem_unit
     * @brief  cache单元
     */
    struct mem_unit
    {
      void* ptr;          /**< 类型指针*/
      struct list_head element;  /**< 链表*/
    };
  public:
    explicit gm_slab(const int unit_max_num = 1024)
      : m_unit_max_num(unit_max_num)
    {
      create();
    }
    virtual ~gm_slab(void)
    {
      release();
    }

    inline T* alloc()
    {
      struct mem_unit* mem_un;
      if (!list_empty(&m_free_mem_objs)) {
        mem_un = container_of(m_free_mem_objs.next, struct mem_unit, element);
        list_del(&mem_un->element);
        list_add_tail(&mem_un->element ,&m_used_mem_objs);
        return (T*)(mem_un->ptr);
      }else{
        return new T;
      }
    }
    inline void free(T* pptr)
    {
      struct mem_unit* mem_un;
      if (is_in_slab(pptr)) {
        mem_un = container_of(m_used_mem_objs.next, struct mem_unit, element);
        list_del(&mem_un->element);
        list_add_tail(&mem_un->element ,&m_free_mem_objs);
        mem_un->ptr = pptr;
      }else{
        delete pptr;
      }
    }
  private:
    inline gm_errno create()//创建
    {
      INIT_LIST_HEAD(&m_free_mem_objs);//初始化free列表
      INIT_LIST_HEAD(&m_used_mem_objs);//初始化used列表

      m_mem_unit = new T[m_unit_max_num];//创建分配内存
      if (!m_mem_unit) {
        return gm_errno_nomem;
      }
      m_mem_uns = new struct mem_unit[m_unit_max_num];//
      if (!m_mem_uns) {
        delete[] m_mem_unit;
        m_mem_unit = NULL;
        return gm_errno_noform;
      }
      for (int i = 0; i < m_unit_max_num; i++) {
        m_mem_uns[i].ptr = (void*)(m_mem_unit+i);
        list_add_tail(&m_mem_uns[i].element, &m_free_mem_objs);
      }
      return gm_errno_ok;
    }
    void release()//释放
    {
      if (m_mem_unit) {
        delete[] m_mem_unit;
        m_mem_unit = NULL;
      }
      if (m_mem_uns) {
        delete[] m_mem_uns;
        m_mem_uns = NULL;
      }
    }
    inline bool is_in_slab(const T* ptr)//判断存在于缓冲区内
    {
      if (ptr < m_mem_unit || ptr > m_mem_unit + m_unit_max_num) {
        return false;
      }
      return true;
    }

  private:
    T* m_mem_unit;//缓冲区类型
    int m_unit_max_num;//缓冲区块数
    struct list_head m_free_mem_objs;//free
    struct list_head m_used_mem_objs;//used
    struct mem_unit* m_mem_uns;//
  };