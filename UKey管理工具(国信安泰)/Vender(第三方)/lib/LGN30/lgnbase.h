/*! \file lgnbase.h 
 *  \brief 使用LGN必须包括本文件。
 *
 * 根据系统类型包含其基础文件，声明智能对象（自动释放）接口，智能指针模板类，句柄类，系统相关类的再次封装。
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */


#ifndef __LGN_BASE_H__
#define __LGN_BASE_H__

#include "lgndef.h"
#if (LGN_WINSYS)
#include "win\lgnwin.api.h"
#include "win\lgnwin.except.h"
#include "win\lgnwin.alloc.h"
#include "win\lgnwin.core.h"
#else
#include "unix/lgnunix.api.h"
#include "unix/lgnunix.except.h"
#include "unix/lgnunix.alloc.h"
#include "unix/lgnunix.core.h"
#endif
#include "lgntrace.h"
#include "lgnbuffer.h"
#include "lgnstr.h"
#include "lgnpath.h"
#include "lgnencoding.h"


namespace LGN //! 基本库全局命名空间
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 智能对象（自动释放）接口
	/*! 需要配合CSmartPtr\<T\>、CSmartObject\<Base\>一起使用，用类的虚构功能来自动释放对象。 
		-# 点击这里查看 \ref smartptr_smartobject_example_page
	*/
	LGN_INTERFACE ISmartObject
	{
	public:
#if (!LGN_WINSYS)
		//! 有些unix系统不支持interface，用struct替代必须有虚构函数
		virtual ~ISmartObject() {};
#endif
		//! 对象引用索引加一
		virtual ULONG AddRef() = 0;
		//! 对象引用索引减一，如果为0则删除该对象
		virtual ULONG Release() = 0;
	};


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 智能指针模板类
	/*! 对象T的类必须提供#LGN::ISmartObject接口。
		-# 点击这里查看 \ref smartptr_smartobject_example_page
	*/
	template < class T >
	class CSmartPtr
	{
	public:
		CSmartPtr() : m_pTVal(NULL)
		{
		}
		CSmartPtr(T* pTVal) : m_pTVal(pTVal)
		{
			if (pTVal != (T*)NULL)
				pTVal->AddRef();
		}
		CSmartPtr(CSmartPtr<T> & rPtr) : m_pTVal(rPtr.m_pTVal)
		{
			if (m_pTVal != (T*)NULL)
				m_pTVal->AddRef();
		}
		~CSmartPtr()
		{
			if (m_pTVal != (T*)NULL)
				m_pTVal->Release();
			m_pTVal = NULL;
		}

		operator T*() const
		{
			return m_pTVal;
		}
		T& operator*() const
		{
			LGNASSERT(m_pTVal!=NULL);
			return *m_pTVal;
		}
		//! The assert on operator& usually indicates a bug.  If this is really
		//! what is needed, however, take the address of the p member explicitly.
		T** operator&() throw()
		{
			LGNASSERT(m_pTVal==NULL);
			return &m_pTVal;
		}

		T * operator->()
		{
			return m_pTVal;
		}
		T const * operator->() const
		{
			return m_pTVal;
		}
		T &operator()()
		{
			return *m_pTVal;
		}
	     
		bool operator!() const throw()
		{
			return (m_pTVal == NULL);
		}

		CSmartPtr<T> &operator=(const CSmartPtr<T> & rPtr)
		{
			if (rPtr.m_pTVal != (T*)NULL)
				rPtr.m_pTVal->AddRef();
			if (m_pTVal != (T*)NULL)
				m_pTVal->Release();
			m_pTVal = rPtr.m_pTVal;
			return *this;
		}

		bool operator==(const CSmartPtr<T> & rPtr) const
		{
			return m_pTVal == rPtr.m_pTVal;
		}
		bool operator!=(const CSmartPtr<T> & rPtr) const
		{
			return m_pTVal != rPtr.m_pTVal;
		}

		bool operator==(const T * pTVal) const
		{
			return m_pTVal == pTVal;
		}
		bool operator!=(const T * pTVal) const
		{
			return m_pTVal != pTVal;
		}

		//! Attach to an existing interface (does not AddRef)
		void Attach(T* pTVal)
		{
			if (m_pTVal)
				m_pTVal->Release();
			m_pTVal = pTVal;
		}
		//! Detach the interface (does not Release)
		T* Detach() 
		{
			T* pTVal = m_pTVal;
			m_pTVal = NULL;
			return pTVal;
		}

	protected:
		//! 资源对象指针
		T * m_pTVal;
	};


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 智能对象模板类
	/*! 继承用户类Base，实现对象引用值加/减一接，并提供创建对象实例接口。
		-# 点击这里查看 \ref smartptr_smartobject_example_page
	*/
	template < class Base >
	class CSmartObject : public Base
	{
	public:
		//! 构造函数，设置对象引用为0
		CSmartObject() : m_nRefs(0)
		{
		}
		
		/*! \brief 实现对象引用值加一
			\return 加一后的值
		*/
		virtual ULONG AddRef()
		{
			return LGN::API::SafeIncrement(&m_nRefs);
		}
		/*! \brief 实现对象引用值减一
			\return 减一后的值
		*/
		virtual ULONG Release()
		{
			if ((LGN::API::SafeDecrement(&m_nRefs)) == 0)
				delete this;
			return m_nRefs;
		}

		//static CSmartObject<Base> * CreateInstance()
		//{
		//	CSmartObject<Base> * p = new CSmartObject<Base>();
		//	if (p)
		//		p->AddRef();
		//	return p;
		//}
		/*! 创建对象实例接口
			\param [out] pp 对象实例指针的指针。
			\return S_OK, S_FALSE
		*/
		static HRESULT CreateInstance( Base ** pp)
		{
			LGNASSERT(pp!=NULL);
			* pp = new CSmartObject<Base>();
			if (*pp)
			{
				(*pp)->AddRef();
				return S_OK;
			}
			return S_FALSE;
		}

	private:
		ULONG m_nRefs;
	};


/*! \page smartptr_smartobject_example_page CSmartPtr例子
<pre>	%ISmartObject接口、%CSmartPtr类和%CSmartObject类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// ...
	class A : public LGN::ISmartObject
	{
	public:
		void f() {}
	};

	LGN::CSmartPtr< A > a;
	LGN::CSmartObject< A >::CreateInstance(&a);
	a->f();
</pre>
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 系统内核对象句柄管理类
	/*! 主要用于基类，实现对系统内核对象句柄管理类，在虚构时关闭句柄。功能同<B>ATL::CHandle</B>类(atlbase.h)，具体参考MSDN。 */
	class CHandle
	{
	public:
		CHandle() throw() :
			m_h( NULL )
		{
		}
		CHandle( CHandle& h ) throw() :
			m_h( NULL )
		{
			Attach( h.Detach() );
		}
		explicit CHandle( HANDLE h ) throw() :
			m_h( h )
		{
		}
		~CHandle() throw()
		{
			if( m_h != NULL )
			{
				Close();
			}
		}

		CHandle& operator=( CHandle& h ) throw()
		{
			if( this != &h )
			{
				if( m_h != NULL )
				{
					Close();
				}
				Attach( h.Detach() );
			}

			return( *this );
		}

		operator HANDLE() const throw()
		{
			return( m_h );
		}

		//! Attach to an existing handle (takes ownership).
		void Attach( HANDLE h ) throw()
		{
			LGNASSERT( m_h == NULL );
			m_h = h;  // Take ownership
		}
		//! Detach the handle from the object (releases ownership).
		HANDLE Detach() throw()
		{
			HANDLE h = m_h;  // Release ownership
			m_h = NULL;
			return( h );
		}

		//! Close the handle.
		void Close() throw()
		{
			if( m_h != NULL )
			{
				LGN::API::CloseHandle( m_h );
				m_h = NULL;
			}
		}

	public:
		HANDLE m_h;
	};


}; // namespace LGN

// 需要用到CHandle
#include "lgnsync.h"


#endif // !__LGN_BASE_H__


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _LGN_IMPL
#if (LGN_WINSYS)
#include "win\lgnwin.base.inc" //!< 当工程不支持selectany或不用C运行库时，需要包含该文件（包含一次）。
#else
#include "unix/lgnunix.base.inc" //!< 由于g++不支持selectany，所以全局对象只能被定义一次，就是该文件只能被包含一次。
#endif
#endif


/*! \file lgnwin.base.inc
 *  \brief 全局对象定义和C运行库实现
 *
 * 当工程不支持selectany或不用C运行库时，必须在一个.cpp文件里包含本文件。
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

/*! \mainpage LGN参考手册
 *
 * \section intro_sec 介绍
 *
 * 目的：跨平台源码，不依赖第三方库，可编译出较小二进制执行代码。
 *
 * 环境：Window 2000以后系统，VC6-VC9、C++ Builder 4.0；linux下G++
 *
 * 功能：模仿ATL一些模板类，实现异常处理、日志处理、内存管理、字符串处理、编码转换。
 *
 * \section install_sec 安装
 *
 * \subsection step1 第一步：拷贝LGN30目录到自己的工程下，或在VC包含路径里增加LGN目录
 *  
 * \subsection step2 第二步：在stdafx.h里包含lgnbase.h
 *  
 * \subsection step3 第三步：当工程不支持selectany或不用C运行库时，需要在stdafx.cpp里先定义_LGN_IMPL宏后再次包含lgnbase.h
 *  
 * etc...
 */

/*! \page example_page 例子列表
 \subpage smartptr_smartobject_example_page 
 \subpage simplebuffer_example_page 
 \subpage buffer_example_page 
 \subpage encodingbase_example_page 
 \subpage encodingtraits_example_page 
 \subpage filewhole_example_page 
 \subpage stdiofile_example_page 
 \subpage sockwhole_example_page 
 \subpage stringex_example_page 
 \subpage tracefileandlineinfo_example_page 
 \subpage trace_example_page 
 \subpage tracemacro_example_page 
 \subpage win32heap_example_page 
 \subpage lgnbuffermgr_example_page 
 \subpage modulepath_example_page 
 \subpage loadmodule_example_page 
 \subpage lgnexception_example_page 
 \subpage file_example_page 
 \subpage temporaryfile_example_page 
 \subpage filemapping_example_page 
 \subpage socket_example_page
 \subpage thread_example_page 
*/
