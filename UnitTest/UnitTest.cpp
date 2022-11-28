// UnitTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TestString.h"
#include "TestXml.h"

int _tmain(int argc, _TCHAR* argv[])
{
	printf("\nStart unit testing.\n");

	printf("\n1 Test stringa.\n");
	{
		CTestString test1;

		int ret = test1.Test1();
		if (ret != 0)
		{
			printf("Error to test stringa. error = %d", ret);
		}
		else
		{
			printf("Test  1  OK.");
		}
	}

	printf("\n2 Test stringw.\n");
	{
		CTestString test1;

		int ret = test1.Test2();
		if (ret != 0)
		{
			printf("Error to test stringw. error = %d", ret);
		}
		else
		{
			printf("Test  2  OK.");
		}
	}

	printf("\n3 Test string helper.\n");
	{
		CTestString test1;

		int ret = test1.Test3();
		if (ret != 0)
		{
			printf("Error to test string helper. error = %d", ret);
		}
		else
		{
			printf("Test  3  OK.");
		}
	}

	printf("\n4 Test string helper.\n");
	{
		CTestString test1;

		int ret = test1.Test4();
		if (ret != 0)
		{
			printf("Error to test string array. error = %d", ret);
		}
		else
		{
			printf("\nTest  4 OK.");
		}
	}

	printf("\n5 Test xml.\n");
	{
		CTestXml test1;
		int ret = test1.Test1();
		if (ret != 0)
		{
			printf("Error to test xml. error = %d", ret);
		}
		else
		{
			printf("Test  5  OK.");
		}		
	}

	printf("\nAll test end.\n");
	getchar();
	return 0;
}

