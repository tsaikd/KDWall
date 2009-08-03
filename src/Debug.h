/**
 * @file Debug.h
 * @brief General debug functions and MACROs
 *
 * Support for library QT4 \n
 * Support for IDE msvc2005 msvc2008
 *
 * @author tsaikd@gmail.com
 * @version 1.0.0.2
 * @date 2009/08/03
 **/

#ifndef _GENERAL_DEBUG_H
#define _GENERAL_DEBUG_H

#if defined(_DEBUG) && !defined(DEBUG)
	#define DEBUG
#endif//defined(_DEBUG) && !defined(DEBUG)

#if !defined(__PRETTY_FUNCTION__) && defined(__FUNCTION__)
	#define __PRETTY_FUNCTION__ __FUNCTION__
#endif//!defined(__PRETTY_FUNCTION__) && defined(__FUNCTION__)

#ifdef DEBUG
	#define DEBUG_BUFSIZE 8192
	extern char g_debugBuf[DEBUG_BUFSIZE];

	#ifdef WIN32
		#include <windows.h>
		#include <crtdbg.h>
		#define TRACEFUNCLINE() { sprintf_s(g_debugBuf, DEBUG_BUFSIZE, "%s():%d: ", __PRETTY_FUNCTION__, __LINE__); OutputDebugStringA(g_debugBuf); }
		#define TRACEL() { TRACEFUNCLINE(); OutputDebugStringA("\n"); }
		#define TRACEN() { OutputDebugStringA("\n"); }
		#define TRACED(var) { TRACE2("%-60s (%3d)", #var, (int)var); }

		#define TRACE0(msg) { TRACEFUNCLINE(); OutputDebugStringA(msg"\n"); }
		#define TRACE1(fmt, arg1) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1); OutputDebugStringA(g_debugBuf); }
		#define TRACE2(fmt, arg1, arg2) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1, arg2); OutputDebugStringA(g_debugBuf); }
		#define TRACE3(fmt, arg1, arg2, arg3) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1, arg2, arg3); OutputDebugStringA(g_debugBuf); }
		#define TRACE4(fmt, arg1, arg2, arg3, arg4) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1, arg2, arg3, arg4); OutputDebugStringA(g_debugBuf); }
		#define TRACE5(fmt, arg1, arg2, arg3, arg4, arg5) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1, arg2, arg3, arg4, arg5); OutputDebugStringA(g_debugBuf); }

		#define ASSERT(expr) ((!!(expr)) || \
                (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, _CRT_WIDE(#expr))) || \
                (_CrtDbgBreak(), 0))
		#define VERIFY(expr) ASSERT(expr)
	#endif //WIN32

	#include <stdio.h>
	extern FILE* g_fpDebugLog;

	#define DOPEN(filename) { DCLOSE(); g_fpDebugLog = fopen(filename, "wb"); }
	#define DCLOSE() { if (g_fpDebugLog) { fclose(g_fpDebugLog); g_fpDebugLog = NULL; } }
	#define DLOGFUNCLINE() { if (g_fpDebugLog) { fprintf(g_fpDebugLog, "%s():%d: ", __PRETTY_FUNCTION__, __LINE__); fflush(g_fpDebugLog); } }
	#define DLOGL(msg) { if (g_fpDebugLog) { DLOGFUNCLINE(); fprintf(g_fpDebugLog, "\n"); fflush(g_fpDebugLog); } }
	#define DLOGN(msg) { if (g_fpDebugLog) { fprintf(g_fpDebugLog, "\n"); fflush(g_fpDebugLog); } }
	#define DLOGD(var) { if (g_fpDebugLog) { DLOG2("%-60s (%3d)", #var, (int)var); } }
	#define DNOTIMP() { DLOG0("not implement"); TRACE0("not implement"); ASSERT(0 && "not implement"); }
	#define DNOHERE() { DLOG0("should not go here"); TRACE0("should not go here"); ASSERT(0 && "should not go here"); }

	#define DLOG0(msg) { if (g_fpDebugLog) { DLOGFUNCLINE(); fprintf(g_fpDebugLog, msg"\n"); fflush(g_fpDebugLog); } }
	#define DLOG1(fmt, arg1) { if (g_fpDebugLog) { DLOGFUNCLINE(); fprintf(g_fpDebugLog, fmt"\n", arg1); fflush(g_fpDebugLog); } }
	#define DLOG2(fmt, arg1, arg2) { if (g_fpDebugLog) { DLOGFUNCLINE(); fprintf(g_fpDebugLog, fmt"\n", arg1, arg2); fflush(g_fpDebugLog); } }
	#define DLOG3(fmt, arg1, arg2, arg3) { if (g_fpDebugLog) { DLOGFUNCLINE(); fprintf(g_fpDebugLog, fmt"\n", arg1, arg2, arg3); fflush(g_fpDebugLog); } }
	#define DLOG4(fmt, arg1, arg2, arg3, arg4) { if (g_fpDebugLog) { DLOGFUNCLINE(); fprintf(g_fpDebugLog, fmt"\n", arg1, arg2, arg3, arg4); fflush(g_fpDebugLog); } }
	#define DLOG5(fmt, arg1, arg2, arg3, arg4, arg5) { if (g_fpDebugLog) { DLOGFUNCLINE(); fprintf(g_fpDebugLog, fmt"\n", arg1, arg2, arg3, arg4, arg5); fflush(g_fpDebugLog); } }
#endif//DEBUG

#ifdef QT_VERSION
	#include <QtCore/QDebug>
	#ifndef QT_NO_DEBUG_OUTPUT
		#define QTRACE() (qDebug().nospace() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ":").space()
	#else//QT_NO_DEBUG_OUTPUT
		#define QTRACE qDebug
	#endif//QT_NO_DEBUG_OUTPUT
	#ifndef WIN32
		#define TRACEL() QTRACE()
		#define TRACEN() qDebug()

		#define TRACE0(msg) (QTRACE() << msg)
		#define TRACE1(fmt, arg1) (QTRACE() << qDebug(fmr, arg1))
		#define TRACE2(fmt, arg1, arg2) (QTRACE() << qDebug(fmr, arg1, arg2))
		#define TRACE3(fmt, arg1, arg2, arg3) (QTRACE() << qDebug(fmr, arg1, arg2, arg3))
		#define TRACE4(fmt, arg1, arg2, arg3, arg4) (QTRACE() << qDebug(fmr, arg1, arg2, arg3, arg4))
		#define TRACE5(fmt, arg1, arg2, arg3, arg4, arg5) (QTRACE() << qDebug(fmr, arg1, arg2, arg3, arg4, arg5))
	#endif//WIN32
#endif//QT_VERSION

#ifndef QTRACE
	#define QTRACE()
#endif//QTRACE

#ifndef TRACEFUNCLINE
	#define TRACEFUNCLINE()
#endif//TRACEFUNCLINE
#ifndef TRACEL
	#define TRACEL()
#endif//TRACEL
#ifndef TRACEN
	#define TRACEN()
#endif//TRACEN
#ifndef TRACED
	#define TRACED(var)
#endif//TRACED

#ifndef TRACE0
	#define TRACE0(msg)
#endif//TRACE0
#ifndef TRACE1
	#define TRACE1(fmt, arg1)
#endif//TRACE1
#ifndef TRACE2
	#define TRACE2(fmt, arg1, arg2)
#endif//TRACE2
#ifndef TRACE3
	#define TRACE3(fmt, arg1, arg2, arg3)
#endif//TRACE3
#ifndef TRACE4
	#define TRACE4(fmt, arg1, arg2, arg3, arg4)()
#endif//TRACE4
#ifndef TRACE5
	#define TRACE5(fmt, arg1, arg2, arg3, arg4, arg5)
#endif//TRACE5

#ifdef DEBUG
	#ifndef ASSERT
		#define ASSERT(expr) ((expr) || (exit(1), 0))
	#endif//ASSERT
	#ifndef VERIFY
		#define VERIFY(expr) ASSERT(expr)
	#endif//VERIFY
#else//DEBUG
	#ifndef ASSERT
		#define ASSERT(expr)
	#endif//ASSERT
	#ifndef VERIFY
		#define VERIFY(expr) (expr)
	#endif//VERIFY
#endif//DEBUG

#ifndef DOPEN
	#define DOPEN(filename)
#endif//DOPEN
#ifndef DCLOSE
	#define DCLOSE()
#endif//DCLOSE
#ifndef DLOGFUNCLINE
	#define DLOGFUNCLINE()
#endif//DLOGFUNCLINE
#ifndef DLOGL
	#define DLOGL(msg)
#endif//DLOGL
#ifndef DLOGN
	#define DLOGN(msg)
#endif//DLOGN
#ifndef DLOGD
	#define DLOGD(var)
#endif//DLOGD
#ifndef DNOTIMP
	#define DNOTIMP()
#endif//DNOTIMP
#ifndef DNOHERE
	#define DNOHERE()
#endif//DNOHERE

#ifndef DLOG0
	#define DLOG0(msg)
#endif//DLOG0
#ifndef DLOG1
	#define DLOG1(fmt, arg1)
#endif//DLOG1
#ifndef DLOG2
	#define DLOG2(fmt, arg1, arg2)
#endif//DLOG2
#ifndef DLOG3
	#define DLOG3(fmt, arg1, arg2, arg3)
#endif//DLOG3
#ifndef DLOG4
	#define DLOG4(fmt, arg1, arg2, arg3, arg4)
#endif//DLOG4

// the folloing is powerful definition
#ifndef NULL
	#define NULL 0
#endif//NULL

#ifndef SAFE_FREE
	#define SAFE_FREE(x) if (x) { free(x); (x) = NULL; }
#endif//SAFE_FREE

#ifndef SAFE_NEW
	#define SAFE_NEW(var, type) if ((var) == NULL) { (var) = new (type); }
#endif//SAFE_NEW

#ifndef SAFE_DELETE
	#define SAFE_DELETE(x) if (x) { delete (x); (x) = NULL; }
#endif//SAFE_DELETE

#ifndef SAFE_NEWARR
	#define SAFE_NEWARR(var, type, num) if ((var) == NULL) { (var) = new (type)[(num)]; }
#endif//SAFE_NEWARR

#ifndef SAFE_DELETEARR
	#define SAFE_DELETEARR(x) if (x) { delete [] (x); (x) = NULL; }
#endif//SAFE_DELETEARR

#ifndef ZERO_ARR
	#define ZERO_ARR(var) memset(var, 0, sizeof(var));
#endif//ZERO_ARR

#ifndef DECCV
	#define DECCV(type, var)			type& var = m_##var					// declare current class variable
#endif//DECCV
#ifndef DECCP
	#define DECCP(type, var)			type& var = *m_##var				// declare current class variable pointer
#endif//DECCP
#ifndef DECOV
	#define DECOV(type, base, var)		type& var = (base).m_##var			// declare other class variable
#endif//DECOV
#ifndef DECOP
	#define DECOP(type, base, var)		type& var = *(base).m_##var			// declare other class variable pointer
#endif//DECOP
#ifndef CDECCV
	#define CDECCV(type, var)			const type& var = m_##var			// declare current class const variable
#endif//CDECCV
#ifndef CDECCP
	#define CDECCP(type, var)			const type& var = *m_##var			// declare current class const variable pointer
#endif//CDECCP
#ifndef CDECOV
	#define CDECOV(type, base, var)		const type& var = (base).m_##var	// declare other class const variable
#endif//CDECOV
#ifndef CDECOP
	#define CDECOP(type, base, var)		const type& var = *(base).m_##var	// declare other class const variable pointer
#endif//CDECOP
#ifndef DECRV
	#define DECRV(type, var, member)	type& var = (member)				// declare renamed variable pointer
#endif//DECRV
#ifndef DECRP
	#define DECRP(type, var, member)	type& var = *(member)				// declare renamed variable pointer
#endif//DECRP
#ifndef DEWRV
	// declare and write renamed variable with new value
	#define DEWRV(type, var, member, val) \
		member = (val); \
		type& var = member
#endif//DEWRV
#ifndef DEWRP
	// declare and write renamed variable pointer with new value
	#define DEWRP(type, var, member, val) \
		member = (val); \
		type& var = *member
#endif//DEWRP

#endif//_GENERAL_DEBUG_H
