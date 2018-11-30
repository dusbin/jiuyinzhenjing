package main
import(
	//"./version"
	"fmt"
	"runtime"
)
var (
	BuildTime 		= "0"
	CommitID 		= "XXXXX"
)
func main(){
	//version.Version()
	fmt.Println("Hello World")
	fmt.Printf("Build time:\t%s\n",BuildTime)
	fmt.Printf("Commit ID:\t%s\n",CommitID)
	fmt.Printf("Go Version:\t%s\n",runtime.Version())
	fmt.Printf("Go OS/Arch:\t%s/%s\n",runtime.GOOS,runtime.GOARCH)

}