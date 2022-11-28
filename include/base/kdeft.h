#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kdeft.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : some template function.

*******************************************************************************************/

template <class T>
void K_Swap(T& x, T& y)
{
	T t;
	t = x;
	x = y;
	y = t;
};

template <class T>
void K_Swap(T* pX, T* pY, int len)
{
	T t;
	for(int i=0; i<len; i++)
	{
		t = pX[i];
		pX[i] = pY[i];
		pY[i] = t;
	}
};

template <class T>
T K_Abs(T& x)
{
	return x < 0 ? 0 - x : x;
};

template <class T>
T K_Max(const T& a, const T& b)
{
	return a > b ? a : b;
};

template <class T>
T K_Min(const T& a, const T& b)
{
	return a < b ? a : b;
};

template <class T>
T K_Max3(const T& a, const T& b, const T& c)
{
	return K_Max(K_Max(a, b), c);
};

template <class T>
T K_Min3(const T& a, const T& b, const T& c)
{
	return K_Min(K_Min(a, b), c);
};
