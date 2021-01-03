#include "StringStack.h"
#include "Check.h"
#include <regex>

#include <algorithm>
#include <string.h>
#include <stdlib.h>

#include <deque>
#include <map>
#include <regex>

StringStack::StringStack(string str) {
	string check_str="";//확인에 사용하는 check_str 생성

    // 공백 제거
    int nBlank = count(str.begin(), str.end(), ' ');
    remove(str.begin(), str.end(), ' ');
    str = str.substr(0, str.length() - nBlank);
    str = modify(str);
    regex pattern("(\\%|\\/|\\+|\\*|-|\\(|\\)|\\*|\\d+)");
    smatch m;
    while (regex_search(str, m, pattern)) {
        stack.push_back(m[0]);
        str = m.suffix();
    }
}

string StringStack::modify(string str) {
    // "2*(-1)"나 "-1+2"과 같이 -앞에 수가 없을 경우 에러가 발생한다.
    //"("뒤에 0과 시작이 -나 숫자로 할 경우 제일 앞에 0을 붙여주는 함수이다.
    // 이것을 통해 2*(-1) 은 2*(0-1) , -1+2는 0-1+2가 된다.
    if (str.at(0) == '-')
        str.insert(0, 1, '0');
	else if (str.at(0) == '+')
		str.insert(0, 1, '0');
    int pos = 0;
    while (1) {
        if (str.find("(-", pos) == string::npos)
            break;
		else {
			pos = str.find("(-", pos) + 1;
			str.insert(pos, 1, '0');
		}
    }
	pos = 0;
	while (1) {
		if (str.find("(+", pos) == string::npos)
			break;
		else {
			pos = str.find("(+", pos) + 1;
			str.insert(pos, 1, '0');
		}
	}
    return str;
}

string StringStack::pop() {
    string result = stack.front();
    stack.pop_front();
    return result;
}

inf_int StringStack::calculate() {  //계산하는 함수 infix를 postfix로 바꾼뒤 eval 함수를 씀
	makeCheck_str();
	if (Check(check_str).giveCheck() == 1) {
		infix_to_postfix();
		return eval_postfix();
	}
	else
		return "잘못된 수식입니다.";
}

inf_int StringStack::eval_postfix() {      //후위연산자 계산하는 함수
    int i = 0;

    string str;
    deque <inf_int> s; //피연산자

    while (!stack.empty()) {
        str = pop();
        if (str != "+" && str != "-" && str != "*" && str != "/" && str != "%") {
            inf_int temp(str.c_str());
            s.push_back(temp);
        }
        else {
            inf_int op2 = s.back();
            s.pop_back();
            inf_int op1 = s.back();
            s.pop_back();

            if (str == "+")
                s.push_back(op1 + op2);
            else if (str == "*")
                s.push_back(op1 * op2);
            else if (str == "-")
                s.push_back(op1 - op2);
            else if (str == "/")
                s.push_back(op1 / op2);
            else if (str == "%")
                s.push_back(op1 % op2);
        }
        i++;
    }
    return s.back();
}
void StringStack::makeCheck_str() {
	for (int i = 0; i < stack.size(); i++) {
		string str = stack.at(i);
		if (str == "+" || str == "*" || str == "-" || str == "/" || str == "%" || str == "(" || str == ")")
			check_str = check_str + str;
		else
			check_str = check_str + "a"; //숫자일 경우 a 투입
	}
		check_str = check_str + "$";
	
}


void StringStack::infix_to_postfix() {      //중위연산을 후위연산으로 바꾸는 함수
    deque <string> postfix; // postfix 저장
    deque <string> math; //+-*()같은 거 담는 곳
    string str;
	
    map<string, int> prec = { {"+", 2},{"-", 2},{"*", 3},{"/", 3}, {"%", 3}, {"(", 0},{")", 0} };
    do {
        str = pop(); //stack에서 pop시킨 값

        if (str == "+" || str == "*" || str == "-" || str == "/" || str == "%") { //연산자일 경우
            while (!math.empty() && (prec[str] <= prec[math.back()])) { //formula가 비어있고 투입된 연산자보다 우선순위가 낮을 경우
                postfix.push_back(math.back()); //연산자를 뱉어내 postfix에 투입
                math.pop_back();
            }
            math.push_back(str); //연산자 투입
        }
        else if (str == "(") {
            math.push_back(str);
        }
        else if (str == ")") {
            while (math.back() != "(") { //(이 나올 때까지 모두 pop해서 postfix에 투입
                postfix.push_back(math.back());
                math.pop_back();
            }
            math.pop_back();  // 남아 있는 "(" 제거
        }
		else // 피연산자
			postfix.push_back(str);

    } while (!stack.empty()); //stack가 빌 때까지 반복


    while (!math.empty()) {
        postfix.push_back(math.back());
        math.pop_back();
    }

    stack = postfix; //stack를 postfix로 바꾸어줌
}