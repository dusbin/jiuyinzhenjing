package person

type Candidate struct {
	Id        uint            //候选人Id
	supporter map[uint]*Voter //支持者
	Voters    uint            // 支持人数
}

// 候选人Id生成器，每创建一个增加一个
var canId uint = 1

/*
	创建候选人
	返回值：Candidate：经过初始化的候选人
*/
func CreateCan() Candidate {
	var tempCan Candidate
	tempCan.supporter = make(map[uint]*Voter)
	tempCan.Voters = 0
	tempCan.Id = canId
	canId += 1
	return tempCan
}

/*
	查看支持者的信息
	参数sId(uInt:(支持者ID))
	绑定 candidate（*Candidate）调用候选人地址
	返回值：支持者
*/
func (candidate *Candidate) PrintSupporter(sId uint) Voter {
	return *candidate.supporter[sId]
}
