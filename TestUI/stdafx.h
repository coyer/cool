// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#define _CRTDBG_MAP_ALLOC 

#include<stdlib.h> 

#include<crtdbg.h> 


// C ����ʱͷ�ļ�
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "../include/ui/kuiinc.h"
#include "Resource.h"

#ifdef _DEBUG
	#pragma comment(lib, "../Debug/coolbase.lib")
	#pragma comment(lib, "../Debug/coolui.lib")
#else
	#pragma comment(lib, "../Release/coolbase.lib")
	#pragma comment(lib, "../Release/coolui.lib")

#endif