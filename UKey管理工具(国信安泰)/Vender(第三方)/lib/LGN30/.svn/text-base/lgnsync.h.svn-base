/*! \file lgnsync.h 
*  \brief 同步机制集类
*
*  被lgnbase.h包含，工程不用直接包含本文件
*
*  根据系统类型保护对应系统的同步机制集类，并声明线程临界区锁定类CCriticalSectionLock
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGN_SYNC_H__
#define __LGN_SYNC_H__

#if (LGN_WINSYS)
#include "win\lgnwin.sync.h"
#else
#include "unix/lgnunix.sync.h"
#endif



namespace LGN
{
	//! 为了兼容ATL而重定义类型
	typedef CCriticalSection CAutoCriticalSection;

	//! 线程临界区锁定类
	class CCriticalSectionLock
	{
	public:
		//! 构造函数，进入临界区
		CCriticalSectionLock( CAutoCriticalSection& cs) : m_cs(cs)
		{
			m_cs.Enter();
		}
		//! 虚造函数，离开临界区
		~CCriticalSectionLock()
		{
			m_cs.Leave();
		}

	// Implementation
	private:
		CAutoCriticalSection& m_cs;

	// Private to avoid accidental use
		CCriticalSectionLock( const CCriticalSectionLock& ) throw();
		CCriticalSectionLock& operator=( const CCriticalSectionLock& ) throw();
	};


}; // namespace LGN

#endif // __LGN_SYNC_H__
