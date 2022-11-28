#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : coredef.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : 插件式系统，基本的数据定义文件。

*******************************************************************************************/

//整个系统的版本，插件版本需要与主版本匹配，不匹配就不加载
#define KAPP_VERSION	100


//扩展接口的类型
#define FT_UNKNOWN		0
#define FT_EXTMGR		1



//平台提供的路径类型，可以去获取平台的各种路径
#define K_PATH_EXE		0		//exe程序所在的路径
#define K_PATH_REG		1		//注册表路径
#define K_PATH_LOG		2		//日志路径
#define K_PATH_CONFIG	3		//配置文件路径
#define K_PATH_PLUGIN	4		//插件路径
#define K_PATH_COMMON	5		//公共的路径
#define K_PATH_IMAGE	6		//图像路径
#define K_PATH_APPDATA	7		//所有用户的路径
#define K_PATH_COMMON_APPDATA	8	//当前登录用户的路径

#define K_PATH_MAX		9

//this plugin plat need my base common library.
#include "../base/kinc.h"


#define	FN_EXTMGR	L"core_extmgr"



