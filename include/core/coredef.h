#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : coredef.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : ���ʽϵͳ�����������ݶ����ļ���

*******************************************************************************************/

//����ϵͳ�İ汾������汾��Ҫ�����汾ƥ�䣬��ƥ��Ͳ�����
#define KAPP_VERSION	100


//��չ�ӿڵ�����
#define FT_UNKNOWN		0
#define FT_EXTMGR		1



//ƽ̨�ṩ��·�����ͣ�����ȥ��ȡƽ̨�ĸ���·��
#define K_PATH_EXE		0		//exe�������ڵ�·��
#define K_PATH_REG		1		//ע���·��
#define K_PATH_LOG		2		//��־·��
#define K_PATH_CONFIG	3		//�����ļ�·��
#define K_PATH_PLUGIN	4		//���·��
#define K_PATH_COMMON	5		//������·��
#define K_PATH_IMAGE	6		//ͼ��·��
#define K_PATH_APPDATA	7		//�����û���·��
#define K_PATH_COMMON_APPDATA	8	//��ǰ��¼�û���·��

#define K_PATH_MAX		9

//this plugin plat need my base common library.
#include "../base/kinc.h"


#define	FN_EXTMGR	L"core_extmgr"



