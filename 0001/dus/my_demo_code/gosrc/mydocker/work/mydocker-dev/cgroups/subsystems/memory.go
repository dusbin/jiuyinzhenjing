package subsystems

import(
	"fmt"
	"io/ioutil"
	"os"
	"path"
	"strconv"
)
//memory subsystem的实现
type MemorySubSystem struct {
}
//设置cgroup对应的cgroup的内存资源限制
func (s *MemorySubSystem) Set(cgroupPath string, res *ResourceConfig) error {
	/*
	 *GetCgroupPath的作用是获取当前subsystem在虚拟文件系统中的路径，
	 */
	if subsysCgroupPath, err := GetCgroupPath(s.Name(), cgroupPath, true); err == nil {
		if res.MemoryLimit != "" {
			/*
			 *设置这个cgroup的内存限制，即将限制写入到cgroup对应目录的memory.limit_in_bytes文件中
			 */
			if err := ioutil.WriteFile(path.Join(subsysCgroupPath, "memory.limit_in_bytes"), []byte(res.MemoryLimit), 0644); err != nil {
				return fmt.Errorf("set cgroup memory fail %v", err)
			}
		}
		return nil
	} else {
		return err
	}

}
//删除cgrouppath对应的cgroup
func (s *MemorySubSystem) Remove(cgroupPath string) error {
	if subsysCgroupPath, err := GetCgroupPath(s.Name(), cgroupPath, false); err == nil {
		//删除cgroup便是删除对应的cgrouppath的目录
		return os.RemoveAll(subsysCgroupPath)
	} else {
		return err
	}
}

//将一个进程加入到cgrouppath对应的cgroup中
func (s *MemorySubSystem) Apply(cgroupPath string, pid int) error {
	if subsysCgroupPath, err := GetCgroupPath(s.Name(), cgroupPath, false); err == nil {
		//把进程的PID写到cgroup的虚拟文件系统对应目录下的task文件中
		if err := ioutil.WriteFile(path.Join(subsysCgroupPath, "tasks"),  []byte(strconv.Itoa(pid)), 0644); err != nil {
			return fmt.Errorf("set cgroup proc fail %v", err)
		}
		return nil
	} else {
		return fmt.Errorf("get cgroup %s error: %v", cgroupPath, err)
	}
}

//返回cgroup的名字
func (s *MemorySubSystem) Name() string {
	return "memory"
}