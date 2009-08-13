/**
 * @file Debug.h
 * @brief General debug functions and MACROs
 *
 * Support for library QT4 \n
 * Support for IDE msvc2005 msvc2008
 *
 * @author tsaikd@gmail.com
 * @version 1.0.0.5
 * @date 2009/08/12
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
		#define TRACES(msg) OutputDebugStringA(msg)
		#define TRACEN() TRACES("\n")
		#define TRACEFUNCLINE() { sprintf_s(g_debugBuf, DEBUG_BUFSIZE, "%s():%d: ", __PRETTY_FUNCTION__, __LINE__); TRACES(g_debugBuf); }
		#define TRACEL() { TRACEFUNCLINE(); TRACEN(); }
		#define TRACE1S(fmt, arg1) { sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt, arg1); TRACES(g_debugBuf); }

		#define TRACE0(msg) { TRACEFUNCLINE(); TRACES(msg"\n"); }
		#define TRACE1(fmt, arg1) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1); TRACES(g_debugBuf); }
		#define TRACE2(fmt, arg1, arg2) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1, arg2); TRACES(g_debugBuf); }
		#define TRACE3(fmt, arg1, arg2, arg3) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1, arg2, arg3); TRACES(g_debugBuf); }
		#define TRACE4(fmt, arg1, arg2, arg3, arg4) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1, arg2, arg3, arg4); TRACES(g_debugBuf); }
		#define TRACE5(fmt, arg1, arg2, arg3, arg4, arg5) { TRACEFUNCLINE(); sprintf_s(g_debugBuf, DEBUG_BUFSIZE, fmt"\n", arg1, arg2, arg3, arg4, arg5); TRACES(g_debugBuf); }

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
		#define QTRACE() ((qDebug().nospace() << __PRETTY_FUNCTION__ << ":" << __LINE__ << ":").space())
	#else//QT_NO_DEBUG_OUTPUT
		#define QTRACE qDebug
	#endif//QT_NO_DEBUG_OUTPUT
	#ifndef WIN32
		#define TRACES(msg) qDebug(msg)
		#define TRACEN() qDebug()
		#define TRACEL() QTRACE()
		#define TRACE1S(fmt, arg1) qDebug(fmt, arg1)

		#define TRACE0(msg) (QTRACE() << msg)
		#define TRACE1(fmt, arg1) (QTRACE() << qDebug(fmr, arg1))
		#define TRACE2(fmt, arg1, arg2) (QTRACE() << qDebug(fmr, arg1, arg2))
		#define TRACE3(fmt, arg1, arg2, arg3) (QTRACE() << qDebug(fmr, arg1, arg2, arg3))
		#define TRACE4(fmt, arg1, arg2, arg3, arg4) (QTRACE() << qDebug(fmr, arg1, arg2, arg3, arg4))
		#define TRACE5(fmt, arg1, arg2, arg3, arg4, arg5) (QTRACE() << qDebug(fmr, arg1, arg2, arg3, arg4, arg5))

		#define TRACED(var) TRACE2("%-60s (%3d)", #var, (int)var)
	#endif//WIN32
#endif//QT_VERSION

#ifdef TRACE2
	#define TRACED(var) TRACE2("%-60s (%3d)", #var, (int)var)
#endif//TRACE2

#if defined(TRACES) && defined(TRACEN) && defined(TRACE1S)
	/**
	 * @brief Trace all elements in array
	 * @note if var_num_per_line < 0 will force insert a newline before data
	 * @code
	 *   int iarr[100];
	 *   // ... fill integer array iarr
	 *   TRACEARR(int, iarr, "%d", 100, 10);
	 * @endcode
	 **/
	#define TRACEARR(sym_type, arr, sym_fmt, sym_num, var_num_per_line) { \
			int _lnum = 0; \
			int _vnpl = ((var_num_per_line) > 0) ? (var_num_per_line) : -(var_num_per_line); \
			TRACES(#arr":"); \
			if ((sym_num) > (var_num_per_line)) { \
				TRACEN(); \
			} else { \
				TRACES("\t"); \
			} \
			for (int _i=0 ; _i<(sym_num) ; _i++) { \
				if (_lnum++ >= _vnpl) { \
					TRACEN(); \
					_lnum = 1; \
				} \
				TRACE1S(" "sym_fmt, (sym_type)(arr[_i])); \
			} \
			TRACEN(); \
		}
#endif//defined(TRACES) && defined(TRACEN) && defined(TRACE1S)

#if defined(TRACE0) && defined(TRACE1S) && defined(TRACEN)
	/**
	 * @brief Trace sub elements in 2D-array
	 * @note MxN == YxX == array[y][x]
	 **/
	#define TRACEARR_S2D(sym_type, arr, sym_fmt, start_y, start_x, height, width) { \
			int _sy = (start_y); \
			int _sx = (start_x); \
			int _sh = _sy + (height); \
			int _sw = _sx + (width); \
			TRACE0(#arr":"); \
			for (int _j=_sy ; _j<_sh ; _j++) { \
				for (int _i=_sx ; _i<_sw ; _i++) { \
					TRACE1S(" "sym_fmt, arr[_j][_i]); \
				} \
				TRACEN(); \
			} \
		}
	/**
	 * @brief Trace all elements in 2D-array
	 * @see TRACEARR_S2D
	 **/
	#define TRACEARR_2D(sym_type, arr, sym_fmt, m, n) TRACEARR_S2D(sym_type, arr, sym_fmt, 0, 0, m, n)
#endif//defined(TRACE0) && defined(TRACE1S) && defined(TRACEN)

#ifndef QTRACE
	#define QTRACE()
#endif//QTRACE

#ifndef TRACES
	#define TRACES(msg)
#endif//TRACES
#ifndef TRACEN
	#define TRACEN()
#endif//TRACEN
#ifndef TRACEFUNCLINE
	#define TRACEFUNCLINE()
#endif//TRACEFUNCLINE
#ifndef TRACEL
	#define TRACEL()
#endif//TRACEL
#ifndef TRACE1S
	#define TRACE1S(fmt, arg1)
#endif//TRACE1S

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

#ifndef TRACED
	#define TRACED(var)
#endif//TRACED
#ifndef TRACEARR
	#define TRACEARR(sym_type, arr, sym_fmt, sym_num, var_num_per_line)
#endif//TRACEARR
#ifndef TRACEARR_S2D
	#define TRACEARR_S2D(sym_type, arr, sym_fmt, start_y, start_x, height, width)
#endif//TRACEARR_S2D
#ifndef TRACEARR_2D
	#define TRACEARR_2D(sym_type, arr, sym_fmt, m, n)
#endif//TRACEARR_2D

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
#ifndef DECRV
	#define DECRV(type, var, member)	type& var = (member)				// declare renamed variable
#endif//DECRV
#ifndef DECRP
	#define DECRP(type, var, member)	type& var = *(member)				// declare renamed variable pointer
#endif//DECRP
#ifndef CDCCV
	#define CDCCV(type, var)			const type& var = m_##var			// declare current class const variable
#endif//CDCCV
#ifndef CDCCP
	#define CDCCP(type, var)			const type& var = *m_##var			// declare current class const variable pointer
#endif//CDCCP
#ifndef CDCOV
	#define CDCOV(type, base, var)		const type& var = (base).m_##var	// declare other class const variable
#endif//CDCOV
#ifndef CDCOP
	#define CDCOP(type, base, var)		const type& var = *(base).m_##var	// declare other class const variable pointer
#endif//CDCOP
#ifndef CDCRV
	#define CDCRV(type, var, member)	const type& var = (member)			// declare renamed variable
#endif//CDCRV
#ifndef CDCRP
	#define CDCRP(type, var, member)	const type& var = *(member)			// declare renamed variable pointer
#endif//CDCRP
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
#ifndef DEWRA_2D
	/**
	 * @see TRACEARR_S2D
	 **/
	#define DEWRA_2D(type, var, m, n, member) { \
			type* _p; \
			int _m = (m); \
			int _n = (n); \
			int size = _m*sizeof(type*) + _m*_n*sizeof(type); \
			member = (type**)malloc(size); \
			memset(member, 0, size); \
			_p = (type*)(member+_m); \
			for (int _i=0 ; _i<_m ; _i++) { \
				member[_i] = _p; \
				_p+=_n; \
			} \
		} \
		type**& var = member
#endif//DEWRA_2D

#endif//_GENERAL_DEBUG_H
