#ifndef _STRINGSTACK_H
#define _STRINGSTACK_H
#include "inf_int.h"
#include <deque>

class StringStack {
private:
    deque <string> stack; // deque¸¦ »ç¿ë
	string check_str;
public:
    StringStack(string);
    string modify(string str);
    string pop();
	void makeCheck_str();
	inf_int calculate();
    inf_int eval_postfix();
    void infix_to_postfix();
};
#endif