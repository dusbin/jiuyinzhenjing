package main
import(
	"fmt"
	//"os"
	fsrepo "./fsrepo"
)
func main(){
    var err error
	var ipfsPath string;
	//目前还不知道在哪儿配置的
	ipfsPath,err = fsrepo.BestKnownPath()
	if err != nil {
		fmt.Println("[",ipfsPath,"]")
	}
	fmt.Println("end")
}