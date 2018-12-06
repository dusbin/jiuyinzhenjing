package main

import (
	"strings"
	"net/http"
	//"reflect"
	"fmt"
	"net"
	//"unsafe"
)
func hello(w http.ResponseWriter,r *http.Request){
	//fmt.Println(r.RemoteAddr)
	ip_str := r.RemoteAddr
	// addrs type []Addr
	// net.InterfaceAddrs net/interface.go 各平台实现不同 ，入口在net/interface_linux.go 120行左右
	addrs,err := net.InterfaceAddrs()
	if err != nil {
		panic(err)
	}
	kv := strings.Split(ip_str,":")
	ip_str = kv[0]
	fmt.Println(ip_str)
	// 将string的ip转换为IP类型的ip
	ip := net.ParseIP(ip_str)
	fmt.Println(ip)
	for _,address := range addrs {
		//fmt.Println(reflect.TypeOf(address))// *net.IPNet
		//fmt.Println(address)
		ipnet := address.(*net.IPNet)
		//fmt.Println(ipnet)
		// 参见net/ip.go
		// ipnet type net.IPNet struct{ IP IP Mask IPMask}
		// type IPMask []byte
		// type IP []byte
		// IP.To4 只接受type IP 类型
		if ipnet.IP.To4() != nil {
			fmt.Println("IP is :",ipnet.IP.String(),ipnet.Mask.String())
			//fmt.Println("IPs is :",ipnet.IP,ipnet.Mask)
			//fmt.Println("Mask is :",ipnet.IP.Mask(ipnet.Mask))
			//fmt.Println(reflect.TypeOf(ipnet.Mask))
			// ipnet 网段是否包含IP
			if ok := ipnet.Contains(ip/*ip IP*/);ok == true {
				fmt.Println("----------------------")
				//fmt.Println("find")
				fmt.Println("IP is :",ipnet.IP.String(),ipnet.Mask.String())
				//fmt.Println("IPs is :",ipnet.IP,ipnet.Mask)
				//fmt.Println("Mask is :",ipnet.IP.Mask(ipnet.Mask))
				//fmt.Println(reflect.TypeOf(ipnet.Mask))
				fmt.Println("----------------------")
			}
		}
		/*
		if ipnet.IP.To16() != nil {
			fmt.Println("16")
			fmt.Println("IP is :",ipnet.IP.String(),ipnet.Mask.String())
			fmt.Println("IPs is :",ipnet.IP,ipnet.Mask)
			fmt.Println("Mask is :",ipnet.IP.Mask(ipnet.Mask))
		}
		*/
		fmt.Println()
	}
}
func main(){
	http.HandleFunc("/",hello)
	http.ListenAndServe(":81",nil)
}