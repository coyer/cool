// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>
#include <tchar.h>

#define _CRTDBG_MAP_ALLOC
#include<stdlib.h> 
#include<crtdbg.h> 


#pragma comment(lib, "gdiplus.lib")


#include "../include/base/kinc.h"

#include "../include/ui/kuiinc.h"
#include "KSkinDefault.h"



#ifdef _DEBUG
	#pragma comment(lib, "../Debug/coolbase.lib")
#else
	#pragma comment(lib, "../Release/coolbase.lib")
#endif
