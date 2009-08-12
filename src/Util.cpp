#include "Util.h"

/**
 * @param threshold space less than (threshold) pixel will return true
 **/
bool QUtil::isDesktopVisible(int threshold/* = 5*/)
{
#ifdef WIN32
	RECT rcTmp;
	QSize sizeWin;
	QSize sizeMax;
	HWND hDesktop = GetDesktopWindow();

	EnumWindows(askMaxWindowSize, (LPARAM)&sizeWin);

	GetWindowRect(hDesktop, &rcTmp);
	sizeMax.setWidth(min(rcTmp.right-rcTmp.left, GetSystemMetrics(SM_CXMAXIMIZED)) - threshold);
	sizeMax.setHeight(min(rcTmp.bottom-rcTmp.top, GetSystemMetrics(SM_CYMAXIMIZED)) - threshold);

	if ((sizeWin.width() >= sizeMax.width()) && (sizeWin.height() >= sizeMax.height()))
		return false;

	return true;
#endif//WIN32

	return true;
}

#ifdef WIN32
bool QUtil::isUserVisiableAppWnd(HWND hWnd)
{
	if (!IsWindowVisible(hWnd))
		return false;
	if (hWnd == GetDesktopWindow())
		return false;
	if (hWnd == FindWindowA("Shell_TrayWnd", NULL))
		return false;
	if (hWnd == FindWindowA("Progman", NULL))
		return false;

	return true;
}
BOOL QUtil::askMaxWindowSize(HWND hWnd, LPARAM lParam)
{
	while (IsWindow(hWnd) && IsWindow(GetParent(hWnd)))
		hWnd = GetParent(hWnd);

	if (!isUserVisiableAppWnd(hWnd))
		return TRUE;

	RECT rcTmp;
	QSize& sizeWin = *((QSize*)lParam);

	GetWindowRect(hWnd, &rcTmp);
	int rcWidth = rcTmp.right - rcTmp.left;
	int rcHeight = rcTmp.bottom - rcTmp.top;

	if (sizeWin.width() < rcWidth)
		sizeWin.setWidth(rcWidth);
	if (sizeWin.height() < rcHeight)
		sizeWin.setHeight(rcHeight);

	return TRUE;
}
#endif//WIN32
