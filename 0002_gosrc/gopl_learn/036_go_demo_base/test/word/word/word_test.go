package word

import (
	"testing"
)

/*
	是回文字符串
*/
func TestPalindrome(t *testing.T) {
	if !IsPalindrome("detartrated") {
		t.Error(`IsPalindrome("detartrated") = false`)
	}
	if !IsPalindrome("kayak") {
		t.Error(`IsPalindrome("kayak") = false`)
	}
}

type Data struct {
	s string
	b bool
}

var tests = []Data{
	{"kayak", true},
	{"hello", false},
	//{"world", true},
	{"a", true},
	{"", true},
	//{"123", true},
}

func TestPalindromeList(t *testing.T) {
	for _, d := range tests {
		if d.b != IsPalindrome(d.s) { // 测试出现问题
			t.Error("you give[", d.b, "]string:[", d.s, "]should is[", IsPalindrome(d.s), "]")
		}
	}
}

/*
	不是回文字符串
*/
func TestNonPalindrome(t *testing.T) {
	if IsPalindrome("palindrome") {
		t.Error(`IsPalindrome("palindrome") = true`)
	}
}
