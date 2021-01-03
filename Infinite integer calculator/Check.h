#ifndef _CHECK_H
#define _CHECK_H
#include <iostream>
#include <map>
#include <deque>
using namespace std;

class Check {
private:
	map <char, int> atri;
	deque<pair<char, int>> stack; //LR paser에서 사용하는 stack이다.
	int curser;
	int nflg; //nflg가 0일 때 반복하고 nflg가 1이면 옳은 식이며 nflg가 2이면 잘못된 식이다.
	string str;
	pair<char, int> temp; // pop한 값을 잠시 담아둘 공간이다.

public:
	Check(string);
	int giveCheck();
	void func(char inst[4]);
};
#endif