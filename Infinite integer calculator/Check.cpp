#include "Check.h"
#include <iostream>
#include <string>
#include <deque>
#include <map>
using namespace std;

char table[17][12][4] = {
{"0","0","0","0","0","s3","s4","0","0","0","1","2"},
{"s5","s6","s7","s8","s9","0","0","0","acc","0","0","0"},
{"r6","r6","r6","r6","r6","0","0","r6","r6","0","0","0"},
{"r7","r7","r7","r7","r7","0","0","r7","r7","0","0","0"},
{"0","0","0","0","0","s3","s4","0","0","0","10","2"},
{"0","0","0","0","0","s3","s4","0","0","0","0","11"},
{"0","0","0","0","0","s3","s4","0","0","0","0","12"},
{"0","0","0","0","0","s3","s4","0","0","0","0","13"},
{"0","0","0","0","0","s3","s4","0","0","0","0","14"},
{"0","0","0","0","0","s3","s4","0","0","0","0","15"},
{"s5","s6","s7","s8","s9","0","0","s16","0","0","0","0"},
{"r1","r1","r1","r1","r1","0","0","r1","r1","0","0","0"},
{"r2","r2","r2","r2","r2","0","0","r2","r2","0","0","0"},
{"r3","r3","r3","r3","r3","0","0","r3","r3","0","0","0"},
{"r4","r4","r4","r4","r4","0","0","r4","r4","0","0","0"},
{"r5","r5","r5","r5","r5","0","0","r5","r5","0","0","0"},
{"r8","r8","r8","r8","r8","0","0","r8","r8","0","0","0"}
};

Check::Check(string str_input) {
		curser=0;
		nflg=0;
		atri = { {'+',0},{'-',1},{'*',2},{'/',3},{'%',4},{'a',5},{'(',6},{')',7},{'$',8},{'S',9} ,{'E',10} ,{'F',11} };

		str = str_input;
		stack.push_back(make_pair(0, 0)); //처음 0을 넣어줌
		while (!nflg) {
			temp = stack.back(); // top의 값을 가져온다.
			func(table[temp.second][atri[str[curser]]]);
		}
	}
int Check::giveCheck() {
		return nflg;
	}
void Check::func(char inst[4]) {

	switch (inst[0])
	{
	case 's':
	{
		temp.first = str[curser++];
		temp.second = atoi(&inst[1]);
		stack.push_back(temp);
		break;
	}
	case 'r':
	{
		int num = atoi(&inst[1]);
		if (num > 0 && num < 6) { //r1,r2,r3,r4,r5의 경우 //
			stack.pop_back(); //뒤에 3개의 stack 제거
			stack.pop_back();
			stack.pop_back();
			temp = stack.back();
			temp.first = 'E';
			temp.second = atoi(table[temp.second][atri['E']]);
			stack.push_back(temp);
		}
		else if (num == 6) { //E -> F
			stack.pop_back();
			temp = stack.back();
			temp.first = 'E';
			temp.second = atoi(table[temp.second][atri['E']]);
			stack.push_back(temp);
		}
		else if (num == 7) { //F -> a
			stack.pop_back();
			temp = stack.back();
			temp.first = 'F';
			temp.second = atoi(table[temp.second][atri['F']]);
			stack.push_back(temp);
		}
		else if (num == 8) { //F -> ( E )
			stack.pop_back(); //뒤에 3개의 stack 제거
			stack.pop_back();
			stack.pop_back();
			temp = stack.back();
			temp.first = 'F';
			temp.second = atoi(table[temp.second][atri['F']]);
			stack.push_back(temp);
		}
		break;
	}
	case 'a':
	{
		nflg = 1;
		break;
	}
	case '0':
	{
		nflg = 2;
		break;
	}
	default:
		cout << "Error?";
		break;
	}
}
