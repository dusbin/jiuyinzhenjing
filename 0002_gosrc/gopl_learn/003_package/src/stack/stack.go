package stack
type Stack struct{
	i int
	data [10]int
}
func (s *Stack)Push(k int){
	s.data[s.i] = k;
	s.i++
}
func (s *Stack)Pop()(ret int){
	if s.i == 0 {
		return
	}
	s.i--
	ret = s.data[s.i]
	s.data[s.i] = 0
	return
}