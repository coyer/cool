// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "../include/base/kinc.h"


#ifdef _DEBUG
	#pragma comment(lib, "../Debug/CoolBase.lib")
#else
	#pragma comment(lib, "../Release/CoolBase.lib")
#endif