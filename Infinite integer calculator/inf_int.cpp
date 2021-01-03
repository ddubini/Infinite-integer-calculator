#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <cstring>
#include <cstdlib>

/*
Originally written by
컴퓨터공학부
정진경
Revised by
러시아어문학과
이현규
*/

inf_int::inf_int()
{
	this->digits=new char[2];	// dynamically allocate

	this->digits[0]='0';		// default 값 0 설정
	this->digits[1]='\0';
	this->length=1;
	this->thesign=true;
}

// 문자열이 아닌 int형 숫자로 초기화
inf_int::inf_int(int n)
{	
	char buf[100];

	if(n<0)
    {		// 음수 처리
		this->thesign=false;
		n=-n;
	}
    else
    {
		this->thesign=true;
	}

	int i=0;
	while(n>0)
    {			// 숫자를 문자열로 변환하는 과정
		buf[i]=n%10+'0';

		n/=10;
		i++;
	}

	if (i == 0) {	// 숫자의 절댓값이 0일 경우
		new (this) inf_int();	// 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨. 
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
		//앞에 0 지우기
		*this = this->EraseZero();
	}
}

inf_int::inf_int(const char* str)
{
	// to be filled 
	// 부호 처리 
    int lastIdx = strlen(str) - 1;

    if(str[0] == '-')   // when negative
    {
        this->thesign = false;
        this->length = strlen(str)-1;
    }
	else				// when positive
    {
        this->thesign = true;
        this->length = strlen(str);
    }

    this->digits = new char[this->length + 1];

    for(unsigned int i = 0; i < this->length; i++)
    {
        this->digits[i] = str[lastIdx--];
    }
	this->digits[this->length] = '\0';

	*this = this->EraseZero();
	// "100"이 들어왔다면 내부 표현에 맞게 "001"로 변환
	// ex) "-1053" -> thesign=false, digits="3501", len=4
}

inf_int::inf_int(const inf_int& a)
{
	this->digits=new char[a.length+1];

	strcpy(this->digits, a.digits);
	this->length=a.length;
	this->thesign=a.thesign;
}

inf_int::~inf_int()
{
	delete[] this->digits;		// 메모리 할당 해제
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if(this->digits) // 이미 문자열이 있을 경우 제거.
	{
		delete[] this->digits;			
	}
	this->digits=new char[a.length+1];

	strcpy(this->digits, a.digits);
	this->length=a.length;
	this->thesign=a.thesign;

	return *this; 
}

bool operator==(const inf_int& a, const inf_int& b)
{
    // we assume 0 is always positive.
    if ( (strcmp(a.digits , b.digits)==0) && a.thesign==b.thesign )	// 부호가 같고, 절댓값이 일치해야함.
        return true;
    return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	// to be filled
	// 절대값 비교
    bool absAbigger;
    if(a.length > b.length) //a의 길이가 b보다 클 때 
    {
        absAbigger = true;
    }
    else if (a.length < b.length) //a의 길이가 b보다 작을 때
    {
        absAbigger = false;
    }
    else    //a의 길이와 b의 길이가 같을 때
    {
        char* numberA = new char[a.length + 1];
        char* numberB = new char[b.length + 1];
        //반전된 문자열 다시 뒤집어서 저장 
        for(unsigned int i = 0; i < a.length; i++)
        {
            numberA[i] = a.digits[a.length-1-i];
            numberB[i] = b.digits[b.length-1-i];
        }

        if(strcmp(numberA, numberB) > 0)
            absAbigger = true;
        else
            absAbigger = false;
        
        delete[] numberA;
        delete[] numberB;
    }
    

	// 둘 다 양수일 경우 절댓값 비교한 것을 그대로 return
    if(a.thesign == true && b.thesign == true)
    {
        return absAbigger;
    }
	// 둘 다 음수일 경우 절댓값 비교의 것을 반전하여 return
    else if(a.thesign == false && b.thesign == false)
    {
        return !absAbigger;
    }
	// 부호가 다를 경우, a가 양수일 경우 b는 음수, a가 음수일 경우 b는 양수이기에 a의 부호진리값을 반환하면 됨
    else
    {
        return a.thesign;
    }
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if(operator>(a, b) || operator==(a, b)) {
		return false;
	}else{
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if(a.thesign==b.thesign){	// 이항의 부호가 같을 경우 + 연산자로 연산
		for(i=0; i<a.length; i++){
			c.Add(a.digits[i], i+1);
		}	
		for(i=0; i<b.length; i++){
			c.Add(b.digits[i], i+1);
		}

		c.thesign=a.thesign;	

		return c;
	}else{	// 이항의 부호가 다를 경우 - 연산자로 연산
		c=b;
		c.thesign=a.thesign;

		return a-c;
	}
}


inf_int operator-(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a == b)
	{
		return c;
	}

	if (a.thesign != b.thesign)	// 이항의 부호가 다를 경우 +연산자로 연산
	{
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else						// 이항의 부호가 같을 경우 -연산자로 연산
	{
		//둘 다 양수일 경우
		if (a.thesign == true)
		{
			if (a > b)
			{
				c = a;
				for (i = 0; i < b.length; i++)
					c.Subtract(b.digits[i], i + 1);
			}
			else
			{
				c = b;
				for (i = 0; i < a.length; i++)
					c.Subtract(a.digits[i], i + 1);
				c.thesign = false;
			}
		}
		else
		{
			if (a > b)
			{
				c = b;
				for (i = 0; i < a.length; i++)
					c.Subtract(a.digits[i], i + 1);
				c.thesign = true;
			}
			else
			{
				c = a;
				for (i = 0; i < b.length; i++)
					c.Subtract(b.digits[i], i + 1);
			}
		}
		c = c.EraseZero();

		return c;
	}
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
    // to be filled
	inf_int c(0);
	c.thesign = true;
	inf_int sum(0);
	unsigned int i = 0;
	int num;

	if (a == c || b == c)
		return c;

	while (i < b.length)
	{
		for (num = 0; num < b.digits[i] - '0'; num++)
		{
			c = c + a;
		}
		//shift 함수
		sum = sum + c.ShiftLeft(i);
		c = 0;
		i++;
	}


	// 부호가 같을 경우 양수
	if (a.thesign == b.thesign)
		sum.thesign = true;
	// 부호가 다를 경우 음수
	else
		sum.thesign = false;

	return sum;
}

inf_int operator/(const inf_int& a, const inf_int& b)
{
	inf_int zero;
	inf_int one("1");
	inf_int quotient;	// 0

	// when divider is 0
	if (b == zero)
	{
		cout << "Error: Divide by Zero" << endl;
		return zero;
	}

	// copy constructor for inf_int a and b
	// except make them all positive to compare absolute values
	inf_int a_copy(a);
	a_copy.thesign = true;
	inf_int b_copy(b);
	b_copy.thesign = true;

	// when absolute of a is smaller than absolute of b
	if (a_copy < b_copy)
		return zero;

	//shift (increment by 10) b_copy as much as needed to make the same length of a_copy
	unsigned int len_a = a_copy.length;
	unsigned int len_b = b_copy.length;
	unsigned int len_dif = len_a - len_b;
	unsigned int len_copy = len_dif;
	inf_int b_dec = b_copy.ShiftLeft(len_dif);	// multiplication of b_copy by 10 power

	while (0 <= len_dif && len_dif <= len_copy)
	{
		while (a_copy > b_dec || a_copy == b_dec)
		{
			a_copy = a_copy - b_dec;
			quotient = quotient + one;
		}
		b_dec = b_dec.ShiftRight(1);
		quotient = quotient.ShiftLeft(1);
		len_dif--;
	}
	quotient = quotient.ShiftRight(1);

	if (a.thesign == b.thesign)
		quotient.thesign = true;
	else
		quotient.thesign = false;

	quotient = quotient.EraseZero();

	return quotient;
}

inf_int operator%(const inf_int& a, const inf_int& b)
{
	inf_int quotient = a / b;
	inf_int remainder = a - b * quotient;

	return remainder;
}

ostream& operator<<(ostream& out, const inf_int& a)
{
	int i;

	if(a.thesign==false){
		out<<'-';
	}
	for(i=a.length-1; i>=0; i--){
		out<<a.digits[i];
	}
	return out;
}

void inf_int::Add(const char num, const unsigned int index)	// a의 index 자리수에 n을 더한다. 0<=n<=9, ex) a가 391일때, a.Add(2, 2)의 결과는 411
{
	if(this->length<index){
		this->digits=(char*)realloc(this->digits, index+1);

		if(this->digits==NULL){		// 할당 실패 예외처리
			cout<<"Memory reallocation failed, the program will terminate."<<endl;

			exit(0);
		}

		this->length=index;					// 길이 지정
		this->digits[this->length]='\0';	// 널문자 삽입
	}

	if(this->digits[index-1]<'0'){	// 연산 전에 '0'보다 작은 아스키값인 경우 0으로 채움. 쓰여지지 않았던 새로운 자리수일 경우 발생
		this->digits[index-1]='0';
	}

	this->digits[index-1]+=num-'0';	// 값 연산


	if(this->digits[index-1]>'9'){	// 자리올림이 발생할 경우
		this->digits[index-1]-=10;	// 현재 자릿수에서 (아스키값) 10을 빼고
		Add('1', index+1);			// 윗자리에 1을 더한다
	}
}

void inf_int::Subtract(const char num, const unsigned int index)	//a의 index 자리수에 num을 뺀다. 0<=n<=9, ex) a가 391일때, a.Subtract(2, 1)의 결과는 389
{
	if (this->digits[index - 1] < num)
	{
		this->digits[index - 1] += (10 - num + '0');
		Subtract('1', index + 1);	// 윗자리에 1을 뺀다
	}
	else
	{
		this->digits[index - 1] -= (num -'0');
	}
}

inf_int inf_int::ShiftLeft(const unsigned int dec)	//숫자를 dec자리수만큼 10을 곱한다
{
	inf_int c;
	unsigned int i;
	c.thesign = this->thesign;
	c.length = this->length + dec;

	c.digits = new char[c.length + 1];

	for (i = 0; i < c.length; i++)
	{
		if (i < dec)
			c.digits[i] = '0';
		else
			c.digits[i] = this->digits[i - dec];
	}
	c.digits[c.length] = '\0';

	return c;
}

inf_int inf_int::ShiftRight(const unsigned int dec) //숫자를 dec자리수만큼 10을 나눈다
{
	inf_int c;
	unsigned int i;
	c.thesign = this->thesign;
	c.length = this->length - dec;

	c.digits = new char[c.length + 1];

	for (i = 0; i < c.length; i++)
	{
		c.digits[i] = this->digits[i + dec];
	}
	c.digits[c.length] = '\0';

	return c;
}

inf_int inf_int::EraseZero(void)
{
	unsigned int lastIdx = this->length - 1;

	while (this->digits[lastIdx] == '0')
		lastIdx--;

	if (lastIdx == -1)	// ex) 0000000 => 0
		lastIdx = 0;

	char* buf = new char[lastIdx + 2];

	for (unsigned int i = 0; i < lastIdx + 1; i++)
	{
		buf[i] = this->digits[i];
	}
	buf[lastIdx + 1] = '\0';

	inf_int c;
	c.digits = new char[lastIdx + 2];
	strcpy(c.digits, buf);
	c.length = lastIdx + 1;
	c.thesign = this->thesign;

	return c;
}
