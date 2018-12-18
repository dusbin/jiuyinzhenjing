package person

type Voter struct {
	id        uint
	name      string     // 投票者姓名
	weight    uint       // 权重
	support   *Candidate // 支持者
	voted     bool
	entruster *Voter
}

var voterId uint = 1

/*
	创建选民，
	参数 _name(string)选民姓名
	返回值，Voter经过初始化的选民
*/
func CreateVoter(_name string) Voter {
	var tempVoter Voter
	tempVoter.id = voterId
	tempVoter.name = _name
	tempVoter.weight = 1
	tempVoter.support = nil
	tempVoter.voted = false
	tempVoter.entruster = nil
	voterId += 1
	return tempVoter
}

/*
	投票：
	参数：candidate(*Candidate):候选人的地址
	绑定：voter(*Voter)：选民调用者的地址
	返回值：成功或者失败
*/
func (voter *Voter) Vote(candidate *Candidate) bool {
	// 如果已经进行过投票，那么投票失败，返回false
	if voter.weight == 0 && voter.voted == true {
		return false
	}
	voter.support = candidate
	candidate.Voters += voter.weight
	voter.weight = 0
	voter.voted = true
	candidate.supporter[voter.id] = voter
	return true
}

/*
	委托人投票
*/
func (voter *Voter) Entrust(other *Voter) bool {
	for v := other.entruster; v != nil; v = v.entruster {
		if *v == *voter {
			return false
		}
	}
	voter.entruster = other
	if other.voted == true {
		voter.support = other.support
		voter.Vote(voter.support)
	} else {
		other.weight += voter.weight
		voter.weight = 0
		voter.voted = true
	}
	return true
}
