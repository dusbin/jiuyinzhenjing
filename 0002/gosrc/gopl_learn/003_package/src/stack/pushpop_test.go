package stack
import "testing"
func TestPushPop(t *testing.T){
	c:= new(Stack)
	c.Push(5)
	if c.Pop() != 5 {
		t.Log("Pop doestn't give 5")
		t.Fail()
	}
}
func TestPushPop_1(t *testing.T){
	c:= new(Stack)
	c.Push(5)
	if c.Pop() != 5 {
		t.Log("Pop doestn't give 5")
		t.Fail()
	}
	if c.Pop() != 0 {
		t.Log("Pop doestn't give 5")
		t.Fail()
	}
}