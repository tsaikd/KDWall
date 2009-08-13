#ifndef _KDWALL_UTIL_H
#define _KDWALL_UTIL_H

#include "stable.h"

#ifdef WIN32
	#include <windows.h>
#endif//WIN32

class QUtil
{
public:
	static bool isDesktopVisible(int threshold = 5);

protected:
#ifdef WIN32
	static bool isUserVisiableAppWnd(HWND hWnd);
	static BOOL CALLBACK askMaxWindowSize(HWND hWnd, LPARAM lParam);
#endif//WIN32
};

#endif//_KDWALL_UTIL_H
