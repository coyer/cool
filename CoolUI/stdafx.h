// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
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
