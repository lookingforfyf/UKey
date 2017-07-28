/*! \file lgnwin.alloc.h 
 *  \brief WIN系统的内存分配及管理
 *
 *  被lgnbase.h包含，工程不用直接包含本文件
 *
 *  声明内存分配接口ILgnMemMgr，缓存管理接口ILgnBufferMgr；及堆内存分配类CWin32Heap，缓存管理类CLgnBufferMgr
 *  
 *  <ul>
 *  	<li> ILgnMemMgr：当包含atlmem.h则指向ATL::IAtlMemMgr
 *		<li> CWin32Heap：当包含atlmem.h则指向ATL::CWin32Heap
 *  	<li> ILgnBufferMgr：当包含atlsimpstr.h则指向ATL::IAtlStringMgr
 *  	<li> CLgnBufferMgr：当包含atlstr.h则指向ATL::CAtlStringMgr
 *  </ul>
 *
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

#ifndef __LGNWIN_ALLOC_H__
#define __LGNWIN_ALLOC_H__

//#pragma once


/////////////////////////////////////////////////////////////////////////////
// Win32 libraries


#ifndef __ATLMEM_H__
namespace ATL //! Active Template Library
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 模板函数，向高位对齐。如AtlAlignUp(10, 8) = 16
	/*! 来源 atlmem.h，同<B>ATL::AtlAlignUp</B>，具体参考MSDN
		\param[in] n 由模板指定类型的数
		\param[in] nAlign 对齐值
		\return 向高位对齐后的值
	*/
	template< typename N >
		inline N WINAPI AtlAlignUp( N n, ULONG nAlign ) throw()
	{
		return( N( (n+(nAlign-1))&~(N( nAlign )-1) ) );
	}

	//! 模板函数，向低位对齐。如AtlAlignDown(10, 8) = 8
	/*! 来源 atlmem.h，同<B>ATL::AtlAlignDown</B>，具体参考MSDN
		\param[in] n 由模板指定类型的数
		\param[in] nAlign 对齐值
		\return 向低位对齐后的值
	*/
	template< typename N >
	inline N WINAPI AtlAlignDown( N n, ULONG nAlign ) throw()
	{
		return( N( n&~(N( nAlign )-1) ) );
	}


////////////////////////////////////////////////////////////////////////////////////////////////////
// __interface IAtlMemMgr

#if (_MSC_VER >= 1300)
	__interface __declspec(uuid("654F7EF5-CFDF-4df9-A450-6C6A13C622C0")) IAtlMemMgr
	{
	public:
		void* Allocate( size_t nBytes ) throw();
		void Free( void* p ) throw();
		void* Reallocate( void* p, size_t nBytes ) throw();
		size_t GetSize( void* p ) throw();
	};

#else
#if (_MSC_VER >= 1000)
	interface __declspec(uuid("654F7EF5-CFDF-4df9-A450-6C6A13C622C0")) IAtlMemMgr
#else
	//! 内存分配接口
	/*! 来源 atlmem.h，同<B>ATL::IAtlMemMgr</B>，具体参考MSDN
	*/
	struct IAtlMemMgr
#endif
	{
	public:
		//! allocate a block of memory
		virtual void* Allocate( size_t nBytes ) throw() = 0;
		//! free a block of memory
		virtual void Free( void* p ) throw() = 0;
		//! reallocate a block of memory
		virtual void* Reallocate( void* p, size_t nBytes ) throw() = 0;
		//! retrieve the size of an allocated memory block
		virtual size_t GetSize( void* p ) throw() = 0;
	};
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
// class CWin32Heap

	//! 堆内存分配类
	/*! 来源 atlmem.h，同<B>ATL::CWin32Heap</B>，具体参考MSDN
	 * -# 点击这里查看 \ref win32heap_example_page	
	*/
	class CWin32Heap :
		public IAtlMemMgr
	{
	public:
		//! The constructor
		/*! \param[in] hHeap An existing heap object
		*/
		CWin32Heap(HANDLE hHeap) throw() :
		  m_hHeap( hHeap )
		{
		}
		//! Allocates a block of memory from the heap object.
		virtual void* Allocate( size_t nBytes ) throw()
		{
			return( ::HeapAlloc( m_hHeap, 0, nBytes ) );
		}
		//! Frees memory previously allocated from the heap
		virtual void Free( void* p ) throw()
		{
			if( p != NULL )
				::HeapFree( m_hHeap, 0, p );
		}
		//! Reallocates a block of memory from the heap object
		virtual void* Reallocate( void* p, size_t nBytes ) throw()
		{
			return (p == NULL)?  Allocate( nBytes ) : ::HeapReAlloc( m_hHeap, 0, p, nBytes );
		}
		//! Returns the size of a memory block allocated from the heap object
		virtual size_t GetSize( void* p ) throw()
		{
			return( ::HeapSize( m_hHeap, 0, p ) );
		}

	public:
		HANDLE m_hHeap;
	};

}; // namespace ATL
#endif // __ATLMEM_H__


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////
//

	//! 宏定义 ATL::AtlAlignUp
#define LgnAlignUp ATL::AtlAlignUp 
	//! 宏定义 ATL::AtlAlignDown
#define LgnAlignDown ATL::AtlAlignDown 

	//! 类型重定义 ATL::IAtlMemMgr
	typedef ATL::IAtlMemMgr ILgnMemMgr;
	//! 类型重定义 ATL::CWin32Heap
	typedef ATL::CWin32Heap CWin32Heap;

#if (LGN_SUPPORT_SELECTANY)
	LGN_SELECTANY CWin32Heap g_heapmgr(::GetProcessHeap()); //!< 定义和声明全局堆管理对象
	LGN_SELECTANY ILgnMemMgr * _lgnMemoryManager = &g_heapmgr; //!< 定义和声明全局堆管理对象的指针
#else
	extern LGN::CWin32Heap g_heapmgr; //!< 声明全局堆管理对象
	extern LGN::ILgnMemMgr * _lgnMemoryManager; //!< 声明全局堆管理对象的指针
#endif

/*! \page win32heap_example_page CWin32Heap例子
<pre>	%CWin32Heap类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// ...
	LGN::CWin32Heap MyHeap(GetProcessHeap());
	LPBYTE pbBuff = (LPBYTE)MyHeap.Allocate(10);
	MyHeap.Free(pbBuff);

	// or
	LPBYTE pbBuff2 = (LPBYTE)LGN::_lgnMemoryManager->Allocate(10);
	LGN::_lgnMemoryManager->Free(pbBuff2);
</pre>
*/

}; // namespace LGN


// buffer alloc
namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////
//

#ifdef __ATLSIMPSTR_H__
# define ILgnBufferMgr ATL::IAtlStringMgr
# define CBufferData ATL::CStringData
# define CNilBufferData ATL::CNilStringData
# define pBufferMgr pStringMgr
# define GetNilBuffer GetNilString
#else

	struct CBufferData;

#if (_MSC_VER >= 1300)
	__interface ILgnBufferMgr
	{
	public:
		CBufferData* Allocate( int nAllocLength, int nBaseTypeSize ) throw();
		void Free( CBufferData* pData ) throw();
		CBufferData* Reallocate( CBufferData* pData, int nAllocLength, int nBaseTypeSize ) throw();
		CBufferData* GetNilBuffer() throw();
		ILgnBufferMgr* Clone() throw();
	};
#else
	//! 缓存管理接口
	/*! 来源 atlsimpstr.h，同<B>ATL::IAtlStringMgr</B>，具体参考MSDN
	*/
	interface ILgnBufferMgr
	{
	public:
		//! Allocate a new CBufferData
		virtual CBufferData* Allocate( int nAllocLength, int nBaseTypeSize ) throw() = 0;
		//! Free an existing CBufferData
		virtual void Free( CBufferData* pData ) throw() = 0;
		//! Change the size of an existing CBufferData
		virtual CBufferData* Reallocate( CBufferData* pData, int nAllocLength, int nBaseTypeSize ) throw() = 0;
		//! Get the CBufferData for a Nil buffer
		virtual CBufferData* GetNilBuffer() throw() = 0;
		//!  return a pointer to a new buffer manager for use with another instance of CBufferT
		virtual ILgnBufferMgr* Clone() throw() = 0;
	};
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
// struct CBufferData

	//! 缓存对象的数据结构
	/*! 来源 atlsimpstr.h，同<B>ATL::CStringData</B>，具体参考MSDN
	 * -# 点击这里查看 \ref lgnbuffermgr_example_page	
	*/
	struct CBufferData
	{
		ILgnBufferMgr* pBufferMgr;  //!< Buffer manager for this CBufferData
		int nDataLength;  //!< Length of currently used data in TYPEs
		int nAllocLength;  //!< Length of allocated data in TYPEs
		long nRefs;     //!< Reference count: negative == locked
		// TYPE data[nAllocLength+1]  // A CBufferData is always followed in memory by the actual array of character data

		void* data() throw()
		{
			return (this+1);
		}

		void AddRef() throw()
		{
			LGNASSERT(nRefs > 0);
			::InterlockedIncrement(&nRefs);
		}
		bool IsLocked() const throw()
		{
			return nRefs < 0;
		}
		bool IsShared() const throw()
		{
			return( nRefs > 1 ); 
		}
		void Lock() throw()
		{
			LGNASSERT( nRefs <= 1 );
			nRefs--;  // Locked buffers can't be shared, so no interlocked operation necessary
			if( nRefs == 0 )
			{
				nRefs = -1;
			}
		}
		void Release() throw()
		{
			LGNASSERT( nRefs != 0 );

			if( ::InterlockedDecrement( &nRefs ) <= 0 )
			{
				pBufferMgr->Free( this );
			}
		}
		void Unlock() throw()
		{
			LGNASSERT( IsLocked() );

			if(IsLocked())
			{
				nRefs++;  // Locked buffers can't be shared, so no interlocked operation necessary
				if( nRefs == 0 )
				{
					nRefs = 1;
				}
			}
		}
	};


////////////////////////////////////////////////////////////////////////////////////////////////////
// class CNilBufferData

	//! 缓存对象的<B>空</B>数据结构
	/*! 来源 atlsimpstr.h，同<B>ATL::CNilStringData</B>，具体参考MSDN
	*/
	class CNilBufferData :
		public CBufferData
	{
	public:
		CNilBufferData() throw()
		{
			pBufferMgr = NULL;
			nRefs = 2;  // Never gets freed by ILgnBufferMgr
			nDataLength = 0;
			nAllocLength = 0;
			awNil[0] = 0;
			awNil[1] = 0;
		}

		void SetManager( ILgnBufferMgr* pMgr ) throw()
		{
			LGNASSERT( pBufferMgr == NULL );
			pBufferMgr = pMgr;
		}

	public:
		WCHAR awNil[2];
	};
#endif // #ifndef __ATLSIMPSTR_H__


////////////////////////////////////////////////////////////////////////////////////////////////////
// class CLgnBufferMgr

#ifdef __ATLSTR_H__

	LGN_SELECTANY ILgnBufferMgr * _lgnBufferManager = &(ATL::g_strmgr);

#else
	//! 缓存管理类
	/*! 来源 atlstr.h，同<B>ATL::CAtlStringMgr</B>，具体参考MSDN
	 * -# 点击这里查看 \ref lgnbuffermgr_example_page	
	*/
	class CLgnBufferMgr :
		public ILgnBufferMgr
	{
	public:
		CLgnBufferMgr( ILgnMemMgr* pMemMgr = NULL ) throw() :
		  m_pMemMgr( pMemMgr )
		  {
			  m_nil.SetManager( this );
		  }
		  virtual ~CLgnBufferMgr() throw()
		  {
		  }

		  void SetMemoryManager( ILgnMemMgr* pMemMgr ) throw()
		  {
			  LGNASSERT( m_pMemMgr == NULL );
			  m_pMemMgr = pMemMgr;
		  }

		  // ILgnBufferMgr
	public:
		virtual CBufferData* Allocate( int nBaseTypes, int nBaseTypeSize ) throw()
		{
			nBaseTypes = LgnAlignUp( nBaseTypes + 1, 8 );  // Prevent excessive reallocation.  The heap will usually round up anyway.

			size_t nDataBytes = nBaseTypes * nBaseTypeSize;
			size_t nTotalSize = sizeof( CBufferData ) + nDataBytes;
			CBufferData* pData = static_cast< CBufferData* >( m_pMemMgr->Allocate( nTotalSize ) );
			if( pData == NULL )
			{
				return( NULL );
			}
			pData->pBufferMgr = this;
			pData->nRefs = 1;
			pData->nAllocLength = nBaseTypes - 1;
			pData->nDataLength = 0;

			return( pData );
		}
		virtual void Free( CBufferData* pData ) throw()
		{
			LGNASSERT( pData->pBufferMgr == this );
			m_pMemMgr->Free( pData );
		}
		virtual CBufferData* Reallocate( CBufferData* pData, int nBaseTypes, int nBaseTypeSize ) throw()
		{
			LGNASSERT( pData->pBufferMgr == this );
			nBaseTypes = LgnAlignUp( nBaseTypes+1, 8 );  // Prevent excessive reallocation.  The heap will usually round up anyway.

			ULONG nDataBytes = nBaseTypes * nBaseTypeSize;
			ULONG nTotalSize = sizeof( CBufferData ) + nDataBytes;
			CBufferData* pNewData = static_cast< CBufferData* >( m_pMemMgr->Reallocate( pData, nTotalSize ) );
			if( pNewData == NULL )
			{
				return NULL;
			}
			pNewData->nAllocLength = nBaseTypes - 1;

			return pNewData;
		}
		virtual CBufferData* GetNilBuffer() throw()
		{
			m_nil.AddRef();
			return &m_nil;
		}
		virtual ILgnBufferMgr* Clone() throw()
		{
			return this;
		}

	protected:
		ILgnMemMgr* m_pMemMgr;
		CNilBufferData m_nil;
	};

#if (LGN_SUPPORT_SELECTANY)
	LGN_SELECTANY CLgnBufferMgr g_bufmgr(_lgnMemoryManager); //!< 定义和声明全局缓存管理类对象
	LGN_SELECTANY ILgnBufferMgr * _lgnBufferManager = &g_bufmgr; //!< 定义和声明全局缓存管理类对象的指针
#else
	extern CLgnBufferMgr g_bufmgr; //!< 声明全局缓存管理类对象
	extern ILgnBufferMgr * _lgnBufferManager; //!< 声明全局缓存管理类对象的指针
#endif

#endif // #ifdef __ATLSTR_H__

/*! \page lgnbuffermgr_example_page CLgnBufferMgr例子
<pre>	%CLgnBufferMgr类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// ...
	LGN::CLgnBufferMgr MyBuff(LGN::_lgnMemoryManager);
	LGN::CBufferData  * data = MyBuff.Allocate(1, 10);
	MyBuff.Free(data);

	// or
	LGN::CBufferData  * data2 = LGN::_lgnBufferManager->Allocate(1, 10);
	LGN::_lgnBufferManager->Free(data2);
</pre>
*/

}; // namespace LGN

#endif 
