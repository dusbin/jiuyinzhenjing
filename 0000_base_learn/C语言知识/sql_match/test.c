#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/**
 * search for regexp anywhere in text
 * success 1 failed 0
 */
int match(char *regexp,char *text){
	if(regexp[0] == '^')//以^开头,^只能作为开头字符才有特殊含义，在后面出现的无此含义，只做简单字符处理
		return matchhere(regexp+1,text);
	do{//不以^开始，
		if(1 == matchhere(regexp,text))
			return 1;
	}while(*text++!='\0');
	return 0;
}
/**
 * search for regexp at beginning of text
 * success 1 failed 0
 */
int matchhere(char *regexp,char *text){
	//printf("regexp = %s text = %s\n",regexp,text);
	if(regexp[0] == '\0')
		return 1;
	if(regexp[1] == '*')//匹配*，传入*的前一个字符，和*后的字符串
		return matchstar(regexp[0],regexp+2,text);
	if(regexp[0] == '$' && regexp[1] == '\0')//以$结束，判断text是否结束，结束即为匹配，未结束，不匹配
		return *text == '\0';
	//if(*text != '\0' && (regexp[0] == '.'||regexp[0]) == *text)//不结束，两个字符相等，递归匹配下一个字符
	//printf("text[0] = %c,regexp[0] = %c\n",text[0],regexp[0]);
	if(text[0] != '\0' && (regexp[0] == '.'||regexp[0] == text[0])){//如果当前字符相等或者是.匹配下一个
		//printf("比对下一个字符\n");
		return matchhere(regexp+1,text+1);
	}
	//printf("不符合\n");
	return 0;//不符合
}
/**
 * search for C*regexp at beginning of text
 * success 1 failed 0
 */
int matchstar(int c,char *regexp,char *text){
	do{
		if(matchhere(regexp,text))//匹配*后面的字符和text后面的字符，
			return 1;
	}while(*text != '\0' && (*text++ == c||c=='.'));
	return 0;
}
int main(){
	//test ^
	printf("test ^\n");
	printf("1%d ^abc abc\n",match("^abc","abc"));
	printf("1%d ^abc abcd\n",match("^abc","abcd"));
	printf("0%d ^abc bcd\n",match("^abc","bcd"));
	
	//test $
	printf("test $\n");
	printf("1%d ^abc$ abc\n",match("^abc$","abc"));
	printf("1%d txt$ test.txt\n",match("txt$","test.txt"));
	printf("0%d txt$ test.tst\n",match("txt$","test.tst"));
	
	//test *
	printf("test *\n");
	printf("1%d t*a a\n",match("t*a","a"));
	printf("1%d t*a ta\n",match("t*a","ta"));
	printf("1%d t*a tta\n",match("t*a","tta"));
	printf("0%d t*a ttt\n",match("t*a","ttt"));
	
	//test .
	printf("test .\n");
	printf("0%d t..a tta\n",match("t..a","tta"));
	printf("1%d t..a ttaa\n",match("t..a","ttaa"));
	
	//test all
	
	printf("test all\n");
	printf("1%d ^td*b.abc$ tbeabc\n",match("^td*b.abc$","tbeabc"));
	printf("1%d ^td*b.abc$ tdbeabc\n",match("^td*b.abc$","tdbeabc"));
	return 0;
}


 