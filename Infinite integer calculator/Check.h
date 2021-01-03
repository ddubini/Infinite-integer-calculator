#ifndef _CHECK_H
#define _CHECK_H
#include <iostream>
#include <map>
#include <deque>
using namespace std;

class Check {
private:
	map <char, int> atri;
	deque<pair<char, int>> stack; //LR paser���� ����ϴ� stack�̴�.
	int curser;
	int nflg; //nflg�� 0�� �� �ݺ��ϰ� nflg�� 1�̸� ���� ���̸� nflg�� 2�̸� �߸��� ���̴�.
	string str;
	pair<char, int> temp; // pop�� ���� ��� ��Ƶ� �����̴�.

public:
	Check(string);
	int giveCheck();
	void func(char inst[4]);
};
#endif