package library
import(
	"errors"
	"fmt"
)
//定义音乐结构体
type MusicEntry struct {
	Id string //音乐唯一id
	Name string //音乐名
	Artist string //艺术家名
	Source string //音乐位置
	Type string //音乐类型
}
type MusicManager struct{
	musics []MusicEntry
}
func NewMusicManager() *MusicManager{
	return &MusicManager{make([]MusicEntry,0)}
}
//获取歌曲的歌名长度
func (m *MusicManager) Len() int{
	return len(m.musics)
}
//通过下标获取音乐
func (m *MusicManager) Get(index int)(music *MusicEntry,err error){
	if index < 0 ||index >= len(m.musics){
		return nil,errors.New("Index out of range.")
	}
	return &m.musics[index],err
}
//通过对比名字查看要找的歌曲是否存在
func (m *MusicManager) Find(name string) *MusicEntry{
	if len(m.musics) == 0{
		return nil
	}
	for _,m:=range m.musics {
		if m.Name == name{
			return &m
		}
	}
	return nil
}
//添加歌曲
func (m *MusicManager) Add(music *MusicEntry){
	m.musics = append(m.musics,*music)
}
//通过下标删除歌曲
func (m *MusicManager)Remove(index int) *MusicEntry{
	if index < 0 ||index > len(m.musics){
		return nil
	}
	removedMusic := m.musics[index]
	if index < len(m.musics) - 1{
		if len(m.musics) != 1 {
			m.musics = append(m.musics[:index -1],m.musics[index +1:]...)
		}
	}
	return &removedMusic
}
//通过歌曲名称删除
func (m *MusicManager)RemoveByName(name string) *MusicEntry{
	removedMusic:=m.Find(name)
	if removedMusic == nil{
		fmt.Println("Want to delete the song does not exist")
		return nil
	}
	return removedMusic
}