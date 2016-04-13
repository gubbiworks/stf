/*
 * TestSTF.cpp
 *
 *  Created on: 14-Apr-2016
 *      Author: gubbiworks
 */


#include "SimpleTestFramework.hpp"


using namespace std;


int add(int a, int b)
{
	return a + b;
}

int sub(int a, int b)
{
	return a - b;
}

int mul(int a, int b)
{
	return a * b;
}


bool test1()
{
	if (3 == add(1, 2))
		return true;
	return false;
}

bool test1_V2( const char *)
{
	return test1();
}

bool testadd2(const char *params)
{
	int ret = 1 + 2;
	return true;
}

bool test2()
{
	if (-1 == sub(2, 3))
		return true;
	return false;
}
bool test2_V2( const char *)
{
	return test2();
}

bool test3()
{
	if (6 == mul(2, 3))
		return true;
	return false;
}

bool test4()
{
	if (7 == mul(2, 3))
		return true;
	return false;
}

void testSTF()
{

	RunTest(Basic Mathematics)
	{
		stftest("Case 1: Addition - should return 3", ( true == test1() ) );
		stftest("Case 2: Subtraction - should return -2", ( true == test2() ) );
		stftest("Case 3: Multiplication - should return 3030", ( true == test3() ) );
		stftest("Case 4: Multiplication(Error) - checking for return 3031", ( true == test4() ) );

	}

		stftest_summary();
}


int main() {

	testSTF();

	return 0;
}
