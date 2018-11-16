package myswitch
import (
	"testing"
)
func Test_Switch_1(t *testing.T){
	strinput := "1"
	input    := []byte(strinput)
	flag,output   := MySwitch(input)
	if flag != 0{
		t.Log("Test_switch_1 is error")
		t.Fail()
	}
	t.Log(output)
}
func Test_Switch_2(t *testing.T){
	strinput := "2"
	input    := []byte(strinput)
	flag,output   := MySwitch(input)
	if flag != 0{
		t.Log("Test_switch_2 is error")
		t.Fail()
	}
	t.Log(output)
}
func Test_Switch_3(t *testing.T){
	strinput := "3"
	input    := []byte(strinput)
	flag,output   := MySwitch(input)
	if flag != 1{
		t.Log("Test_switch_3 is error")
		t.Fail()
	}
	t.Log(output)
}