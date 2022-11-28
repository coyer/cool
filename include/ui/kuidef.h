#pragma once

#ifndef KUI_API

#ifdef _USRDLL

	#ifdef COOLUI_EXPORTS
		#define KUI_API __declspec(dllexport)
	#else
		#define KUI_API __declspec(dllimport)
	#endif

#else
    #define KUI_API    
#endif

#endif

class KSkin;
class KView;

#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
