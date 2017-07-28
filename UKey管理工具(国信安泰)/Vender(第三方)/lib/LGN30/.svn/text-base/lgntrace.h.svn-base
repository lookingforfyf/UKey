/*! \file lgntrace.h 
 *  \brief 跟踪输出调试信息的集类
 *
 *  被lgnbase.h包含，工程不用直接包含本文件
 *
 *  声明跟踪类别结构CTraceCategory，跟踪输出带错误所在的文件名和行号类CTraceFileAndLineInfo，跟踪输出类CTrace<br>
 *  一般不调用这些类，直接用其定义的一套跟踪宏，点击这里查看 \ref tracemacro_example_page
 *
 *  如果工程不要跟踪输出调试信息功能，定义以下宏：<br>
 *		\#define LGN_SUPPORT_TRACE 0<br>
 *
 *  修改默认的跟踪类别对象，在包含lgnbase.h（lgntrace.h）前，可以重新定义以下宏：<br>
 *		\#define LGN_TRACE_CATEGORY_DEF_FLAGS \<newValue\><br>
 *		\#define LGN_TRACE_CATEGORY_DEF_LEVEL \<newValue\><br>
 *		\#define LGN_TRACE_CATEGORY_DEF_NAME \<newValue\><br>
 *		\#define LGN_TRACE_CATEGORY_DEF_FILE \<newValue\><br>
 *  
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

#ifndef __LGNTRACE_H__
#define __LGNTRACE_H__

//#pragma once

#ifndef LGN_SUPPORT_TRACE
#define LGN_SUPPORT_TRACE 1 //!< 表示是否启用日志跟踪代码，默认为开启
#endif


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//! 跟踪类别的标志
	struct TraceFlag
	{
		//! 跟踪类别的标志Value
		enum _Value
		{
			CategoryNameA		= 0x00000001, //!< 表示pCategoryName为ANSI字符串，CategoryNameA和CategoryNameW只且选择一个
			CategoryNameW		= 0x00000002, //!< 表示pCategoryName为UNICODE字符串，CategoryNameA和CategoryNameW只且选择一个
			DateTime			= 0x00000004, //!< 输出包含当前时间
			FileLineInfo		= 0x00000008, //!< 输出包含出错文件名和行数
			ProcessIdTreadId	= 0x00000010, //!< 输出包含当前进程Id和线程Id
			OutputFileA			= 0x01000000, //!< 表示pOutputFileName为ANSI字符串，并把日志信息输出到该文件里
		};
	};

	//! 跟踪日志的级别
	struct TraceLevel
	{
		//! 跟踪类别的标志Value
		enum _Value // LogSeverity
		{
			Emergency = 0,		//!< system is unusable
			Alert = 1,			//!< action must be taken immediately
			Critical = 2,		//!< critical conditions
			Error = 3,			//!< error conditions
			Warning = 4,		//!< warning conditions
			Notice = 5,			//!< normal but significant condition
			Informational = 6,	//!< informational
			Debug = 7			//!< debug-level messages
		};
	};

	//! 跟踪类别结构
	/*!	来源 atltrace.h，改造<B>ATL::CTraceCategory</B>，具体参考MSDN
	*/
	struct CTraceCategory
	{
		UINT32 dwFlags; //!< 跟踪类别的标志值 LGN::TraceFlag
		UINT32 dwLevel; //!< 跟踪类别的级别值 LGN::TraceLevel
		LPCVOID pCategoryName; //!< 跟踪类别的名称，如果dwFlags & CategoryNameA表示ANSI字符串，如果dwFlags & CategoryNameW表示UNICODE字符串
		LPCVOID pOutputFileName; //!< 输出文件名，如果dwFlags & OutputFileA表示ANSI字符串，其他输出到调试器上

		//! 构造函数，直接调用Init
		CTraceCategory(UINT32 dwFlags, UINT32 dwLevel, LPCSTR pszCategoryName, LPCSTR pszOutputFileName = NULL)
		{
			Init(dwFlags | TraceFlag::CategoryNameA, dwLevel, pszCategoryName, pszOutputFileName);
		}
		//! 构造函数，直接调用Init
		CTraceCategory(UINT32 dwFlags, UINT32 dwLevel, LPCWSTR pszCategoryName, LPCSTR pszOutputFileName = NULL)
		{
			Init(dwFlags | TraceFlag::CategoryNameW, dwLevel, pszCategoryName, pszOutputFileName);
		}

		//! 设置结构的变量成员值
		void Init(UINT32 dwFlags, UINT32 dwLevel, LPCVOID pszCategoryName, LPCSTR pszOutputFileName)
		{
			this->dwFlags = dwFlags;
			this->dwLevel = dwLevel;
			this->pCategoryName = pszCategoryName;
			this->pOutputFileName = pszOutputFileName;
			if (pszOutputFileName != NULL)
				this->dwFlags |= (TraceFlag::OutputFileA | TraceFlag::DateTime);
		}
	};
	typedef CTraceCategory * PTRACECATEGORY; //!< 类型重定义 LGN::CTraceCategory指针类型
	typedef const CTraceCategory * PCTRACECATEGORY; //!< 类型重定义 LGN::CTraceCategory常量指针类型


////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if (LGN_SUPPORT_TRACE)

#ifndef LGN_TRACE_CATEGORY_DEF_FLAGS
# define LGN_TRACE_CATEGORY_DEF_FLAGS LGN::TraceFlag::FileLineInfo //!< 默认跟踪类别的标志值  \see LGN::TraceFlag
#endif
#ifndef LGN_TRACE_CATEGORY_DEF_LEVEL
# define LGN_TRACE_CATEGORY_DEF_LEVEL LGN::TraceLevel::Warning //!< 默认跟踪类别的级别值  \see LGN::TraceLevel
#endif
#ifndef LGN_TRACE_CATEGORY_DEF_NAME
# define LGN_TRACE_CATEGORY_DEF_NAME LGN_TEXT("TraceGeneral") //!< 默认跟踪类别的名称
#endif
#ifndef LGN_TRACE_CATEGORY_DEF_FILE
# define LGN_TRACE_CATEGORY_DEF_FILE NULL //!< 默认跟踪类别的输出文件名
#endif
#if (LGN_SUPPORT_SELECTANY)
	LGN_SELECTANY CTraceCategory lgnTraceGeneral(LGN_TRACE_CATEGORY_DEF_FLAGS, LGN_TRACE_CATEGORY_DEF_LEVEL, LGN_TRACE_CATEGORY_DEF_NAME, LGN_TRACE_CATEGORY_DEF_FILE);
#else
	//! 默认跟踪类别对象：LGN::CTraceCategory lgnTraceGeneral(LGN_TRACE_CATEGORY_DEF_FLAGS, LGN_TRACE_CATEGORY_DEF_LEVEL, LGN_TRACE_CATEGORY_DEF_NAME, LGN_TRACE_CATEGORY_DEF_FILE)
	extern LGN::CTraceCategory lgnTraceGeneral;
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 跟踪输出带错误所在的文件名和行号
	/*!	来源 atltrace.h，改造<B>ATL::CTraceFileAndLineInfo</B>，具体参考MSDN<br>
		该类不可继承，其对象不可复值操作<br>
		重载括号操作符，实现类对象的函数操作功能，点击这里查看 \ref tracefileandlineinfo_example_page
	*/
	class CTraceFileAndLineInfo
	{
	public:
		/*! \brief 唯一的构造函数，置成员变量值
			\param [in] pszFileName 文件名（ansi编码）
			\param [in] nLineNo 行号
		*/
		CTraceFileAndLineInfo(LPCSTR pszFileName, int nLineNo)
			: m_pszFileName(pszFileName), m_nLineNo(nLineNo)
		{}

	public:
		/*! \brief 重载括号操作符，printf格式输出调试信息
			\param [in] pCategory 跟踪类别
			\param [in] dwLevel 级别，其值小于等于pCategory->dwLevel才输出调试信息
			\param [in] pszFmt printf格式字符串
			\see TraceVA
		*/
		void LGN_CDECL operator()(PCTRACECATEGORY pCategory, UINT32 dwLevel, LPCSTR pszFmt, ...) const
		{
			if (dwLevel <= pCategory->dwLevel)
			{
				va_list argList;
				va_start(argList, pszFmt);
				TraceVA(pCategory, m_pszFileName, m_nLineNo, pszFmt, argList);
				va_end(argList);
			}
		}
#if (LGN_SUPPORT_UNICODE)
		//! 重载括号操作符，printf格式输出调试信息
		void LGN_CDECL operator()(PCTRACECATEGORY pCategory, UINT32 dwLevel, LPCWSTR pszFmt, ...) const
		{
			if (dwLevel <= pCategory->dwLevel)
			{
				va_list argList;
				va_start(argList, pszFmt);
				TraceVW(pCategory, m_pszFileName, m_nLineNo, pszFmt, argList);
				va_end(argList);
			}
		}
#endif // #if (LGN_SUPPORT_UNICODE)

		/*! \brief 重载括号操作符，用默认跟踪类别，printf格式输出调试信息
			\param [in] pszFmt printf格式字符串
			\see TraceVA
		*/
		void LGN_CDECL operator()(LPCSTR pszFmt, ...) const
		{
			va_list argList;
			va_start(argList, pszFmt);
			TraceVA(&lgnTraceGeneral, m_pszFileName, m_nLineNo, pszFmt, argList);
			va_end(argList);
		}
#if (LGN_SUPPORT_UNICODE)
		//! 重载括号操作符，printf格式输出调试信息
		void LGN_CDECL operator()(LPCWSTR pszFmt, ...) const
		{
			va_list argList; 
			va_start(argList, pszFmt);
			TraceVW(&lgnTraceGeneral, m_pszFileName, m_nLineNo, pszFmt, argList);
			va_end(argList);
		}
#endif // #if (LGN_SUPPORT_UNICODE)


		/*! \brief 输出跟踪类别的信息
			\param [in] pCategory 跟踪类别
		*/
		void TraceHeadA(PCTRACECATEGORY pCategory) const
		{
			TraceHeadA(pCategory, m_pszFileName, m_nLineNo);
		}
#if (LGN_SUPPORT_UNICODE)
		//! 输出跟踪类别的信息
		void TraceHeadW(PCTRACECATEGORY pCategory) const
		{
			TraceHeadW(pCategory, m_pszFileName, m_nLineNo);
		}
#endif

		/*! \brief 输出十六进制格式数据
			\param [in] pCategory 跟踪类别
			\param [in] pbData 待输出数据
			\param [in] nLen 数据\a pbData的长度
			\note 本函数不输出跟踪类别的信息，若要跟踪类别的信息，先调用TraceHeadA/TraceHeadW
		*/
		void TraceHex(PCTRACECATEGORY pCategory, LPCBYTE pbData, int nLen) const
		{
			CHAR szBuf[MAX_BUFF_SIZE];
			szBuf[0] = '\t';
			for (int i=0; i < nLen; )
			{		
				int nOffset=1;
				for (int j=0; (j<16) && (i<nLen) ; j++, i++)
				{
					nOffset += LGN::ChTraitsExA::FormatNP(szBuf+nOffset, MAX_BUFF_SIZE-nOffset, "%02X ", pbData[i]);
				}
				LGN::ChTraitsExA::StringCat(szBuf, LGN_BR);
				TraceOutputA(pCategory, szBuf);
			}
		}

	// static function
	public:
		/*! \brief 输出调试信息
			\param [in] pCategory 跟踪类别
			\param [in] pszFileName 文件名（ansi编码）
			\param [in] nLine 行号
			\param [in] pszFmt printf格式字符串
			\param [in] argList 参数列表
			\see TraceHeadA
			\see TraceFormatA
		*/
		static void TraceVA(PCTRACECATEGORY pCategory, LPCSTR pszFileName, int nLine, LPCSTR pszFmt, va_list argList)
		{
			TraceHeadA(pCategory, pszFileName, nLine);
			TraceFormatA(pCategory, pszFmt, argList);
			TraceOutputA(pCategory, LGN_BR);
		}
#if (LGN_SUPPORT_UNICODE)
		//! 输出调试信息
		static void TraceVW(PCTRACECATEGORY pCategory, LPCSTR pszFileName, int nLine, LPCWSTR pszFmt, va_list argList)
		{
			TraceHeadW(pCategory, pszFileName, nLine);
			TraceFormatW(pCategory, pszFmt, argList);
			TraceOutputW(pCategory, LGN_BR_W);
		}
#endif // #if (LGN_SUPPORT_UNICODE)

		enum { MAX_BUFF_SIZE = 1024};

		/*! \brief 根据标志值，输出跟踪类别的信息及文件名和行号
			\param [in] pCategory 跟踪类别
			\param [in] pszFileName 文件名（ansi编码）
			\param [in] nLine 行号
			\note 所有要输出的字符串长度不能超过1024-1
		*/
		static void TraceHeadA(PCTRACECATEGORY pCategory, LPCSTR pszFileName, int nLine)
		{
			CHAR szBuf[MAX_BUFF_SIZE];
			int nLen = 0;

			if (pCategory->dwFlags & TraceFlag::CategoryNameA)
			{
				nLen += LGN::ChTraitsExA::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, "[%s]", (LPCSTR)(pCategory->pCategoryName));
			}

			if (pCategory->dwFlags & TraceFlag::CategoryNameW)
			{
				nLen += LGN::ChTraitsExA::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, "[%S]", (LPCWSTR)(pCategory->pCategoryName));
			}

			if (pCategory->dwFlags & TraceFlag::DateTime)
			{
				SYSTEMTIME t;
				LGN::API::GetLocalTime(&t);
				nLen += LGN::ChTraitsExA::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, "[%02d,%02d:%02d:%02d.%03d]", t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
			}

			if ((pCategory->dwFlags & TraceFlag::FileLineInfo) && (pszFileName != NULL))
			{
				nLen += LGN::ChTraitsExA::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, "[%s(%d)]", pszFileName, nLine);
			}

			if (pCategory->dwFlags & TraceFlag::ProcessIdTreadId)
			{
				nLen += LGN::ChTraitsExA::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, "[PID:%d][TID:%d] ", LGN::API::GetCurrentProcessId(), LGN::API::GetCurrentThreadId());
			}
			TraceOutputA(pCategory, szBuf);
		}
#if (LGN_SUPPORT_UNICODE)
		//! 根据标志值，输出跟踪类别的信息及文件名和行号
		static void TraceHeadW(PCTRACECATEGORY pCategory, LPCSTR pszFileName, int nLine)
		{
			WCHAR szBuf[MAX_BUFF_SIZE];
			int nLen = 0;

			szBuf[0] = L'\0';
			if (pCategory->dwFlags & TraceFlag::CategoryNameA)
			{
				nLen += LGN::ChTraitsExW::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, L"[%S]", (LPCSTR)(pCategory->pCategoryName));
			}

			if (pCategory->dwFlags & TraceFlag::CategoryNameW)
			{
				nLen += LGN::ChTraitsExW::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, L"[%s]", (LPCWSTR)(pCategory->pCategoryName));
			}

			if (pCategory->dwFlags & TraceFlag::DateTime)
			{
				SYSTEMTIME t;
				LGN::API::GetLocalTime(&t);
				nLen += LGN::ChTraitsExW::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, L"[%02d,%02d:%02d:%02d.%03d]", t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
			}

			if ((pCategory->dwFlags & TraceFlag::FileLineInfo) && (pszFileName != NULL))
			{
				nLen += LGN::ChTraitsExW::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, L"[%S(%d)]", pszFileName, nLine);
			}

			if (pCategory->dwFlags & TraceFlag::ProcessIdTreadId)
			{
				nLen += LGN::ChTraitsExW::FormatNP(szBuf + nLen, MAX_BUFF_SIZE - nLen, L"[PID:%d][TID:%d] ", LGN::API::GetCurrentProcessId(), LGN::API::GetCurrentThreadId());
			}
			TraceOutputW(pCategory, szBuf);
		}
#endif // #if (LGN_SUPPORT_UNICODE)

		/*! \brief 根据参数列表printf格式字符串输出
			\param [in] pCategory 跟踪类别
			\param [in] pszFmt printf格式字符串
			\param [in] argList 参数列表
			\note 如果\a pszFmt长度大于1024，则直接输出\a pszFmt；其他格式化字符串长度不能超过1024-1
		*/
		static void TraceFormatA(PCTRACECATEGORY pCategory, LPCSTR pszFmt, va_list argList)
		{
			if (LGN::API::SafeStringLength(pszFmt) > MAX_BUFF_SIZE)
			{
				TraceOutputA(pCategory, pszFmt);
			}
			else
			{
				CHAR szBuf[MAX_BUFF_SIZE];
				szBuf[0] = '\0';
				LGN::ChTraitsExA::FormatN(szBuf, MAX_BUFF_SIZE, pszFmt, argList);
				TraceOutputA(pCategory, szBuf);
			}
		}
#if (LGN_SUPPORT_UNICODE)
		//! 根据参数列表printf格式字符串输出
		static void TraceFormatW(PCTRACECATEGORY pCategory, LPCWSTR pszFmt, va_list argList)
		{
			if (LGN::API::SafeStringLength(pszFmt) > MAX_BUFF_SIZE)
			{
				TraceOutputW(pCategory, pszFmt);
			}
			else
			{
				WCHAR szBuf[MAX_BUFF_SIZE];
  				szBuf[0] = L'\0';
				LGN::ChTraitsExW::FormatN(szBuf, MAX_BUFF_SIZE, pszFmt, argList);
				TraceOutputW(pCategory, szBuf);
			}
		}
#endif // #if (LGN_SUPPORT_UNICODE)

		/*! \brief 直接输出字符串到跟踪器上
			\param [in] pCategory 跟踪类别
			\param [in] pszMessage 待输出的字符串
		*/
		static void TraceOutputA(PCTRACECATEGORY pCategory, LPCSTR pszMessage)
		{
			if (pCategory->dwFlags & TraceFlag::OutputFileA)
			{
				LGN::FileTraitsTraceA::Append((LPCSTR)(pCategory->pOutputFileName), pszMessage, LGN::API::SafeStringLength(pszMessage));
			}
			else
			{
				LGN::ChTraitsExA::SOutputDebugString(pszMessage);
			}
		}
#if (LGN_SUPPORT_UNICODE)
		//! 直接输出字符串到跟踪器上
		static void TraceOutputW(PCTRACECATEGORY pCategory, LPCWSTR pszMessage)
		{
			if (pCategory->dwFlags & TraceFlag::OutputFileA)
			{
				CHAR szBuf[MAX_BUFF_SIZE];
				szBuf[0] = '\0';
				LGN::ChTraitsExA::FormatNP(szBuf, MAX_BUFF_SIZE, "%S", pszMessage);
				TraceOutputA(pCategory, szBuf);
			}
			else
			{
				LGN::ChTraitsExW::SOutputDebugString(pszMessage);
			}
		}
#endif // #if (LGN_SUPPORT_UNICODE)

	private:
		const char *const m_pszFileName; //!< 文件名（ansi编码）
		const int m_nLineNo; //!< 行号

		CTraceFileAndLineInfo(const CTraceFileAndLineInfo &other); //!< 本类不可继承
		CTraceFileAndLineInfo operator=(const CTraceFileAndLineInfo &other); //!< 本类对象不可复值操作
	};

/*! \page tracefileandlineinfo_example_page CTraceFileAndLineInfo例子
<pre>	%CTraceFileAndLineInfo类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// 修改默认跟踪类别的标志，输出增加：包含当前进程Id和线程Id，包含当前时间
	LGN_TRACE_CATEGORY_DEF->dwFlags |= LGN::TraceFlag::ProcessIdTreadId | LGN::TraceFlag::DateTime;

	// 十六进制输出
	LGN::CTraceFileAndLineInfo trace(__FILE__, __LINE__);
	trace(LGN_TRACE_CATEGORY_DEF, LGN::TraceLevel::Error , "hex data:");
	trace.TraceHex(LGN_TRACE_CATEGORY_DEF, (LPCBYTE)"1234678901234567890", 20);

	// printf格式输出
	LGN::CTraceFileAndLineInfo(__FILE__, __LINE__)(LGN_TRACE_CATEGORY_DEF, LGN::TraceLevel::Error, "errno = %x\n", -1);

	// 
	// 
	// 输出数据格式如下：
	// [TraceGeneral][\<Time\>][\<FileName\>(\<LineId\>)][PID:\<pid\>][TID:\<tid\>] hex data:	31 32 33 34 36 37 38 39 30 31 32 33 34 35 36 37 
	// 	38 39 30 00 
	// [TraceGeneral][\<Time\>][\<FileName\>(\<LineId\>)][PID:\<pid\>][TID:\<tid\>] errno = ffffffff
</pre>
*/


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 跟踪输出，调用CTraceFileAndLineInfo
	/*!	来源 atltrace.h，改造<B>ATL::CTrace</B>，具体参考MSDN<br>
		该类不可继承，其对象不可复值操作<br>
		重载括号操作符，实现类对象的函数操作功能，点击这里查看 \ref trace_example_page
	*/
	class CTrace
	{
	public:
		/*! \brief 唯一的构造函数，置成员变量值
			\param [in] pszFileName 文件名（ansi编码）
			\param [in] nLineNo 行号
			\param [in] pTraceCatecory 跟踪类别
		*/
		CTrace(LPCSTR pszFileName, int nLineNo, PCTRACECATEGORY pTraceCatecory = NULL)
			:m_trace(pszFileName, nLineNo)
		{
			m_pTraceCatecory = (pTraceCatecory != NULL)? pTraceCatecory : &lgnTraceGeneral;
			m_dwLevel = 0;
			m_dwErrorNo = 0;
			m_pszFuncNameA = NULL;
			m_pszFuncNameW = NULL;
		}

		//! \brief 虚构函数，如果待跟踪的函数名称（参考Entry）有效，则输出其函数调用结束的跟踪信息
		~CTrace()
		{
			if (m_pszFuncNameA != NULL)
			{
				if (m_dwErrorNo == 0)
					m_trace(m_pTraceCatecory, m_dwLevel, "<Function %s Leave> OK" LGN_BR LGN_BR, m_pszFuncNameA);
				else
					m_trace(m_pTraceCatecory, m_dwLevel, "<Function %s Leave> Error!!!=0x%0x(%d)" LGN_BR LGN_BR, m_pszFuncNameA, m_dwErrorNo, m_dwErrorNo);
			}
#if (LGN_SUPPORT_UNICODE)
			if (m_pszFuncNameW != NULL)
			{
				if (m_dwErrorNo == 0)
					m_trace(m_pTraceCatecory, m_dwLevel, L"<Function %s Leave> OK" LGN_BR_W LGN_BR_W, m_pszFuncNameW);
				else
					m_trace(m_pTraceCatecory, m_dwLevel, L"<Function %s Leave> Error!!!=0x%0x(%d)" LGN_BR_W LGN_BR_W, m_pszFuncNameW, m_dwErrorNo, m_dwErrorNo);
			}
#endif
		}

		/*! \brief 置成员变量值-错误号，在虚构函数内用到
			\param [in] dwErrorNo 错误号
		*/
		void SetErrorNo(UINT32 dwErrorNo)
		{
			m_dwErrorNo = dwErrorNo;
		}

		/*! \brief 输出待跟踪的函数调用开始的跟踪信息
			\param [in] dwLevel 级别，其值小于等于pCategory->dwLevel才输出调试信息
			\param [in] pszFuncName 待跟踪的函数名称，可以为NULL
		*/
		void Entry(UINT32 dwLevel, LPCSTR pszFuncName)
		{
			m_dwLevel = dwLevel;
			m_pszFuncNameA = pszFuncName;
			if (m_pszFuncNameA != NULL)
				m_trace(m_pTraceCatecory, m_dwLevel, "<Function %s Entry>" LGN_BR, m_pszFuncNameA);
		}
#if (LGN_SUPPORT_UNICODE)
		//! 输出待跟踪的函数调用开始的跟踪信息
		void Entry(UINT32 dwLevel, LPCWSTR pszFuncName)
		{
			m_dwLevel = dwLevel;
			m_pszFuncNameW = pszFuncName;
			if (m_pszFuncNameW != NULL)
				m_trace(m_pTraceCatecory, m_dwLevel, L"<Function %s Entry>" LGN_BR_W, m_pszFuncNameW);
		}
#endif

		/*! \brief 重载括号操作符，printf格式输出调试信息
			\param [in] pszFmt printf格式字符串
		*/
		void LGN_CDECL operator()(LPCSTR pszFmt, ...) const
		{
			if (m_dwLevel <= m_pTraceCatecory->dwLevel)
			{
				va_list argList;
				va_start(argList, pszFmt);
				m_trace.TraceHeadA(m_pTraceCatecory);
				m_trace.TraceFormatA(m_pTraceCatecory, pszFmt, argList);
				m_trace.TraceOutputA(m_pTraceCatecory, LGN_BR);
				va_end(argList);
			}
		}
		/*! \brief 重载括号操作符，printf格式输出调试信息
			\param [in] dwLevel 级别，其值小于等于跟踪类别的dwLevel才输出调试信息
			\param [in] pszFmt printf格式字符串
		*/
		void LGN_CDECL operator()(UINT32 dwLevel, LPCSTR pszFmt, ...) const
		{
			if (dwLevel <= m_pTraceCatecory->dwLevel)
			{
				va_list argList;
				va_start(argList, pszFmt);
				m_trace.TraceHeadA(m_pTraceCatecory);
				m_trace.TraceFormatA(m_pTraceCatecory, pszFmt, argList);
				m_trace.TraceOutputA(m_pTraceCatecory, LGN_BR);
				va_end(argList);
			}
		}
#if (LGN_SUPPORT_UNICODE)
		//! 重载括号操作符，printf格式输出调试信息
		void LGN_CDECL operator()(LPCWSTR pszFmt, ...) const
		{
			if (m_dwLevel <= m_pTraceCatecory->dwLevel)
			{
				va_list argList;
				va_start(argList, pszFmt);
				m_trace.TraceHeadW(m_pTraceCatecory);
				m_trace.TraceFormatW(m_pTraceCatecory, pszFmt, argList);
				m_trace.TraceOutputW(m_pTraceCatecory, LGN_BR_W);
				va_end(argList);
			}
		}
		//! 重载括号操作符，printf格式输出调试信息
		void LGN_CDECL operator()(UINT32 dwLevel, LPCWSTR pszFmt, ...) const
		{
			if (dwLevel <= m_pTraceCatecory->dwLevel)
			{
				va_list argList;
				va_start(argList, pszFmt);
				m_trace.TraceHeadW(m_pTraceCatecory);
				m_trace.TraceFormatW(m_pTraceCatecory, pszFmt, argList);
				m_trace.TraceOutputW(m_pTraceCatecory, LGN_BR_W);
				va_end(argList);
			}
		}
#endif // #if (LGN_SUPPORT_UNICODE)

		/*! \brief 输出消息头信息及十六进制格式数据
			\param [in] pszMsg 消息名称
			\param [in] pData 待输出数据
			\param [in] nLen 数据\a pbData的长度
		*/
		LGN_INLINE void WriteHex(LPCSTR pszMsg, LPCVOID pData, int nLen) const
		{
			WriteHex(m_dwLevel, pszMsg, pData, nLen);
		}
		/*! \brief 输出消息头信息及十六进制格式数据
			\param [in] dwLevel 级别，其值小于等于跟踪类别的dwLevel才输出调试信息
			\param [in] pszMsg 消息名称
			\param [in] pData 待输出数据
			\param [in] nLen 数据\a pbData的长度
		*/
		void WriteHex(UINT32 dwLevel, LPCSTR pszMsg, LPCVOID pData, int nLen) const
		{
			if (dwLevel <= m_pTraceCatecory->dwLevel)
			{
				m_trace(m_pTraceCatecory, dwLevel, "%s(0x%08X):%s", pszMsg, nLen, (nLen>0 && nLen<=16)?  "" : LGN_BR);
				m_trace.TraceHex(m_pTraceCatecory, (LPCBYTE)pData, nLen);
			}
		}
#if (LGN_SUPPORT_UNICODE)
		//! 输出消息头信息及十六进制格式数据
		LGN_INLINE void WriteHex(LPCWSTR pszMsg, LPCVOID pData, int nLen) const
		{
			WriteHex(m_dwLevel, pszMsg, pData, nLen);
		}
		//! 输出消息头信息及十六进制格式数据
		void WriteHex(UINT32 dwLevel, LPCWSTR pszMsg, LPCVOID pData, int nLen) const
		{
			if (dwLevel <= m_pTraceCatecory->dwLevel)
			{
				m_trace(m_pTraceCatecory, dwLevel, L"%s(0x%X):%s", pszMsg, nLen, (nLen>16)? LGN_BR_W : L"");
				m_trace.TraceHex(m_pTraceCatecory, (LPCBYTE)pData, nLen);
			}
		}
#endif // #if (LGN_SUPPORT_UNICODE)

	protected:
		CTraceFileAndLineInfo m_trace; //!< 类对象
		PCTRACECATEGORY m_pTraceCatecory; //!< 跟踪类别指针，这里只保存指针，调用者保证其对象不被释放
		UINT32 m_dwLevel; //!< 跟踪类别的级别
		UINT32 m_dwErrorNo; //!< 错误号
		LPCSTR m_pszFuncNameA; //!< 函数名称（ansi编码）
		LPCWSTR m_pszFuncNameW; //!< 函数名称（unicode编码）
	
	private:
		CTrace(const CTrace &other); //!< 本类不可继承
		CTrace operator=(const CTrace &other); //!< 本类对象不可复值操作
	};

/*! \page trace_example_page CTrace例子
<pre>	%CTrace类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	void func()
	{
		LGN::CTrace trace(NULL, 0);
		trace.Entry(LGN::TraceLevel::Error, "func");
		trace("---%s---", "test");
		trace.WriteHex("hex data", (LPCBYTE)"1234678901234567890", 20);
		trace.SetErrorNo(-1);
	}

	// ...
	func();

	// 
	// 
	// 输出数据格式如下：
	// [TraceGeneral]\<Function func Entry\>
	// [TraceGeneral]---test---
	// [TraceGeneral]hex data(0x00000014):
	//         31 32 33 34 36 37 38 39 30 31 32 33 34 35 36 37
	//         38 39 30 00
	// [TraceGeneral]\<Function func Leave\> Error!!!=0xffffffff(-1)
</pre>
*/


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LGN_TRACE_CATEGORY_DEF
#define LGN_TRACE_CATEGORY_DEF (&LGN::lgnTraceGeneral) //!< 默认的跟踪类别对象
#endif
#define LGNTRACE LGN::CTraceFileAndLineInfo(__FILE__, __LINE__) //!< 带当前文件名和行号的跟踪类对象
#define LGNTRACE_ENTRY(cate, level, func) LGN::CTrace _trace(NULL, 0, cate); _trace.Entry(level, func) //!< 带当前函数名称的跟踪类对象
#define LGNTRACE_MSG _trace //!< 用于输出字符串调试信息
#define LGNTRACE_HEX(msg, data, len) _trace.WriteHex(msg, data, len) //!< 用于输出十六进制的调试信息
#define LGNTRACE_HEX_LV(level, msg, data, len) _trace.WriteHex(level, msg, data, len) //!< 用于输出十六进制的调试信息，可指定级别
#define LGNTRACE_ERRORNO(err) _trace.SetErrorNo((UINT32)err); //!< 设置跟踪类对象的错误号

#else // !LGN_SUPPORT_TRACE
#define LGN_TRACE_CATEGORY_DEF 0
#define LGNTRACE LGN_NOOP
#define LGNTRACE_ENTRY(cate, level, func) LGN_NOOP
#define LGNTRACE_MSG LGN_NOOP
#define LGNTRACE_HEX(msg, data, len) LGN_NOOP
#define LGNTRACE_HEX_LV(level, msg, data, len) LGN_NOOP
#define LGNTRACE_ERRORNO(err) LGN_NOOP
#endif


/*! \page tracemacro_example_page 调试输出例子
<pre> 使用跟踪宏的例子</pre>
<pre>
\#include "lgnbase.h"

	LGN::CTraceCategory lgnTraceTest(LGN::TraceFlag::DateTime|LGN::TraceFlag::ProcessIdTreadId, LGN::TraceLevel::Notice, "TEST", NULL);

	void func()
	{
		LGNTRACE_ENTRY(&lgnTraceTest, LGN::TraceLevel::Notice, "func");
		LGNTRACE_HEX("hex data", (LPCBYTE)"1234678901234567890", 20);
		LGNTRACE_ERRORNO(-1);
	}

	// ...
	LGNTRACE(&lgnTraceTest, LGN::TraceLevel::Error , "lgnTraceTest messageid = %d\n", 1);
	func();
	LGNTRACE("defaultTrace messageid = %d\n", 2);

	// 
	// 
	// 输出数据格式如下：
	// [TEST][02,17:50:50.906][PID:1588][TID:2948] lgnTraceTest messageid = 1
	// [TEST][02,17:50:50.906][PID:1588][TID:2948] \<Function func Entry\>
	// [TEST][02,17:50:50.906][PID:1588][TID:2948] hex data(0x00000014):
	// 	31 32 33 34 36 37 38 39 30 31 32 33 34 35 36 37 
	// 	38 39 30 00 
	// [TEST][02,17:50:50.906][PID:1588][TID:2948] \<Function func Leave\> Error!!!=0xffffffff(-1)

	// [TraceGeneral][e:\\ljc\\card_n\\tcard\\tcard.cpp(94)]defaultTrace messageid = 2

</pre>
*/

}; // namespace LGN

#endif
