package referee

import (
	. "../person"
)

var Candidates = make(map[uint]*Candidate)

func JoinCandidate(candidates ...*Candidate) {
	for _, value := range candidates {
		Candidates[value.Id] = value
	}
}
func Statistical() Candidate {
	var max uint = 0   //保存最大票数
	var index uint = 0 //保存最大票数的候选人的id
	for key, value := range Candidates {
		if max < value.Voters { //如果有票数大于max的，进行重新赋值
			max = value.Voters
			index = key //保存最大票数的id
		}
	}
	return *Candidates[index]
}
