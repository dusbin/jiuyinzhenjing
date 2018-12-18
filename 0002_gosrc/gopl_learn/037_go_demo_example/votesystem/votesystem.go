package main

import (
	"./person"
	"./referee"
	"fmt"
)

func main() {
	//初始化三个候选人
	var can1 = person.CreateCan()
	var can2 = person.CreateCan()
	var can3 = person.CreateCan()
	//将三个候选人添加到候选人容器中
	referee.JoinCandidate(&can1, &can2, &can3)

	//初始化五个选民
	var v1 = person.CreateVoter("张三")
	var v2 = person.CreateVoter("李四")
	var v3 = person.CreateVoter("王五")
	var v4 = person.CreateVoter("赵六")
	var v5 = person.CreateVoter("陈七")

	(v1).Vote(&can1) //张三选候选人1
	(v2).Vote(&can3) //李四选候选人3

	(&v3).Entrust(&v5) //王五委托陈七进行投票
	(&v4).Entrust(&v2) //赵六委托李四进行投票
	(&v5).Entrust(&v1) //陈七委托张三进行投票

	//统计最终胜利者
	fmt.Println(referee.Statistical())

}
