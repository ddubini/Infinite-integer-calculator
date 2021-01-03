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
	string check_str="";//Ȯ�ο� ����ϴ� check_str ����

    // ���� ����
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
    // "2*(-1)"�� "-1+2"�� ���� -�տ� ���� ���� ��� ������ �߻��Ѵ�.
    //"("�ڿ� 0�� ������ -�� ���ڷ� �� ��� ���� �տ� 0�� �ٿ��ִ� �Լ��̴�.
    // �̰��� ���� 2*(-1) �� 2*(0-1) , -1+2�� 0-1+2�� �ȴ�.
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

inf_int StringStack::calculate() {  //����ϴ� �Լ� infix�� postfix�� �ٲ۵� eval �Լ��� ��
	makeCheck_str();
	if (Check(check_str).giveCheck() == 1) {
		infix_to_postfix();
		return eval_postfix();
	}
	else
		return "�߸��� �����Դϴ�.";
}

inf_int StringStack::eval_postfix() {      //���������� ����ϴ� �Լ�
    int i = 0;

    string str;
    deque <inf_int> s; //�ǿ�����

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
			check_str = check_str + "a"; //������ ��� a ����
	}
		check_str = check_str + "$";
	
}


void StringStack::infix_to_postfix() {      //���������� ������������ �ٲٴ� �Լ�
    deque <string> postfix; // postfix ����
    deque <string> math; //+-*()���� �� ��� ��
    string str;
	
    map<string, int> prec = { {"+", 2},{"-", 2},{"*", 3},{"/", 3}, {"%", 3}, {"(", 0},{")", 0} };
    do {
        str = pop(); //stack���� pop��Ų ��

        if (str == "+" || str == "*" || str == "-" || str == "/" || str == "%") { //�������� ���
            while (!math.empty() && (prec[str] <= prec[math.back()])) { //formula�� ����ְ� ���Ե� �����ں��� �켱������ ���� ���
                postfix.push_back(math.back()); //�����ڸ� �� postfix�� ����
                math.pop_back();
            }
            math.push_back(str); //������ ����
        }
        else if (str == "(") {
            math.push_back(str);
        }
        else if (str == ")") {
            while (math.back() != "(") { //(�� ���� ������ ��� pop�ؼ� postfix�� ����
                postfix.push_back(math.back());
                math.pop_back();
            }
            math.pop_back();  // ���� �ִ� "(" ����
        }
		else // �ǿ�����
			postfix.push_back(str);

    } while (!stack.empty()); //stack�� �� ������ �ݺ�


    while (!math.empty()) {
        postfix.push_back(math.back());
        math.pop_back();
    }

    stack = postfix; //stack�� postfix�� �ٲپ���
}