# import . _ 别名的用法
## import . "fmt"
* 可以直接使用Printf函数而不需要带包名
## import _ "fmt"
* 只使用fmt包的init函数，不能调用其他函数
## import myfmt "fmt"
* 可以用myfmt来使用fmt的函数