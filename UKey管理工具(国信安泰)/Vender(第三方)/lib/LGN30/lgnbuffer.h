/*! \file lgnbuffer.h 
 *  \brief 缓冲区分配管理模板类
 *
 *  被lgnbase.h包含，工程不用直接包含本文件
 *
 *  声明简单缓冲区分配管理模板类CSimpleBufferT，缓冲区分配管理模板类CBufferT；<br>
 *  可以对任何数据类型（包括结构）的数组方式分配缓冲区，一个对象的多实例共用一块缓冲区，当任何一个实例要更改时，则重新分配缓冲区。
 *  
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

#ifndef __LGNBUFFER_H__
#define __LGNBUFFER_H__


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 简单缓冲区分配管理模板类
	/*!	来源 atlsimpstr.h，改造<B>ATL::CSimpleStringT</B>，没有函数说明的，请参考MSDN<br>
	 *  继承类或调用着必须传入内存分配接口对象指针（<B>LGN::ILgnBufferMgr *</B>）；不同于<B>ATL::CSimpleStringT</B>，本模板类可以指定任何类型（包括结构）的数据，点击这里查看 \ref simplebuffer_example_page
	*/
	template< typename BaseType>
	class CSimpleBufferT
	{
	public:
		//! 明确类型的构造函数
		explicit CSimpleBufferT( ILgnBufferMgr* pBufferMgr) throw()
		{
			LGNASSERT( pBufferMgr != NULL );

			CBufferData* pData = pBufferMgr->GetNilBuffer();
			AttachData( pData );
		}

		CSimpleBufferT( const CSimpleBufferT& bufSrc )
		{
			CBufferData* pSrcData = bufSrc.GetData();
			CBufferData* pNewData = CloneData( pSrcData );
			AttachData( pNewData );
		}

		CSimpleBufferT( const BaseType* pSrc, int nLength, ILgnBufferMgr* pBufferMgr )
		{
			LGNASSERT( pBufferMgr != NULL );

			if(pSrc == NULL && nLength != 0)
				LGN::LgnThrow(E_INVALIDARG);

			CBufferData* pData = pBufferMgr->Allocate( nLength, sizeof( BaseType ) );
			if( pData == NULL )
			{
				ThrowMemoryException();
			}
			AttachData( pData );
			SetLength( nLength );
			CopyBaseTypes( m_pData, pSrc, nLength );
		}

		~CSimpleBufferT() throw()
		{
			CBufferData* pData = GetData();
			pData->Release();
		}

		CSimpleBufferT& operator=( const CSimpleBufferT& bufSrc )
		{
			CBufferData* pSrcData = bufSrc.GetData();
			CBufferData* pOldData = GetData();
			if( pSrcData != pOldData)
			{
				if( pOldData->IsLocked() || pSrcData->pBufferMgr != pOldData->pBufferMgr )
				{
					SetCBuffer( bufSrc.GetCBuffer(), bufSrc.GetLength() );
				}
				else
				{
					CBufferData* pNewData = CloneData( pSrcData );
					pOldData->Release();
					AttachData( pNewData );
				}
			}

			return( *this );
		}

		CSimpleBufferT& operator+=( const CSimpleBufferT& bufSrc )
		{
			Append( bufSrc );

			return( *this );
		}

		CSimpleBufferT& operator+=( BaseType bt )
		{
			AppendBaseType( bt );

			return( *this );
		}

		BaseType operator[]( int iIndex ) const
		{
			return GetAt(iIndex);
		}

		operator const BaseType *() const throw()
		{
			return( m_pData );
		}

		//! 添加指定类型的多单元数据（单元复制方式）
		template< typename _BaseTypeIn >
		void AppendT(const _BaseTypeIn * pSrc, int nLength)
		{
			UINT nOldLength = GetLength();
			UINT nNewLength = nOldLength+nLength;
			BaseType * pBuffer = GetBufferSetLength( nNewLength );
			pBuffer += nOldLength;
			for (int i=0; i<nLength; i++)
				pBuffer[i] = (BaseType)pSrc[i];
		}

		void Append( const BaseType * pSrc, int nLength )
		{
			// See comment in SetString() about why we do this
			UINT_PTR nOffset = (UINT_PTR)(pSrc-GetCBuffer());

			UINT nOldLength = GetLength();
			UINT nNewLength = nOldLength+nLength;
			BaseType * pBuffer = GetBuffer( nNewLength );
			if( nOffset <= nOldLength )
			{
				pSrc = pBuffer+nOffset;
				// No need to call CopyBaseTypesOverlapped, since the destination is
				// beyond the end of the original buffer
			}
			CopyBaseTypes( pBuffer+nOldLength, pSrc, nLength );
			ReleaseBufferSetLength( nNewLength );
		}

		//! 增加一个单元数据，参考<B>ATL::CSimpleStringT::AppendChar</B>
		void AppendBaseType( BaseType bt )
		{
			int nOldLength = GetLength();
			int nNewLength = nOldLength+1;
			BaseType * pBuffer = GetBuffer( nNewLength );
			pBuffer[nOldLength] = bt;
			ReleaseBufferSetLength( nNewLength );
		}
		void Append( const CSimpleBufferT& strSrc )
		{
			Append( strSrc.GetCBuffer(), strSrc.GetLength() );
		}
		void Empty() throw()
		{
			CBufferData* pOldData = GetData();
			ILgnBufferMgr* pBufferMgr = pOldData->pBufferMgr;
			if( pOldData->nDataLength == 0 )
			{
				return;
			}

			if( pOldData->IsLocked() )
			{
				// Don't reallocate a locked buffer that's shrinking
				SetLength( 0 );
			}
			else
			{
				pOldData->Release();
				CBufferData* pNewData = pBufferMgr->GetNilBuffer();
				AttachData( pNewData );
			}
		}
		void FreeExtra()
		{
			CBufferData* pOldData = GetData();
			int nLength = pOldData->nDataLength;
			ILgnBufferMgr* pBufferMgr = pOldData->pBufferMgr;
			if( pOldData->nAllocLength == nLength )
			{
				return;
			}

			if( !pOldData->IsLocked() )  // Don't reallocate a locked buffer that's shrinking
			{
				CBufferData* pNewData = pBufferMgr->Allocate( nLength, sizeof( BaseType ) );
				if( pNewData == NULL )
				{
					SetLength( nLength );
					return;
				}
				CopyBaseTypes( (BaseType *)( pNewData->data() ), (BaseType *)( pOldData->data() ), nLength );
				pOldData->Release();
				AttachData( pNewData );
				SetLength( nLength );
			}
		}

		int GetAllocLength() const throw()
		{
			return( GetData()->nAllocLength );
		}
		BaseType GetAt( int iIndex ) const
		{
			LGNASSERT( (iIndex >= 0) && (iIndex <= GetLength()) );  // Indexing the '\0' is OK
			if( (iIndex < 0) || (iIndex > GetLength()) )
				LGN::LgnThrow(E_INVALIDARG);		

			return( m_pData[iIndex] );
		}
		BaseType * GetBuffer()
		{
			CBufferData* pData = GetData();
			if( pData->IsShared() )
			{
				Fork( pData->nDataLength );
			}

			return( m_pData );
		}
		//! 增加一个参数dwFlags，其他参考<B>ATL::CSimpleStringT::GetBufferSetLength</B>
		/*! \param[in] nMinBufferLength 要分配最小单元长度
		    \param[in] dwFlags =0 标志位，其值：GMEM_ZEROINIT-对新分配的空间清零操作*/
		BaseType * GetBuffer( int nMinBufferLength, UINT32 dwFlags = 0 )
		{
			BaseType * pBuffer = PrepareWrite( nMinBufferLength );
			if (dwFlags & GMEM_ZEROINIT)
			{
				int nLength = GetData()->nDataLength;
				if (GetData()->nAllocLength > nLength)
					LGN::API::memset(pBuffer+nLength, 0, (GetData()->nAllocLength-nLength) * sizeof( BaseType ));
			}
			return( pBuffer );
		}
		//! 增加一个参数dwFlags，其他参考<B>ATL::CSimpleStringT::GetBufferSetLength</B>
		/*! \param[in] nLength 要分配单元长度
		    \param[in] dwFlags = 0 标志位，其值：GMEM_ZEROINIT-对新分配的空间清零操作*/
		BaseType * GetBufferSetLength( int nLength, UINT32 dwFlags = 0 )
		{
			BaseType * pBuffer = GetBuffer( nLength, dwFlags );
			SetLength( nLength );

			return( pBuffer );
		}
		int GetLength() const throw()
		{
			return( GetData()->nDataLength );
		}
		ILgnBufferMgr* GetManager() const throw()
		{
			return( GetData()->pBufferMgr->Clone() );
		}

		//! 获取缓冲区的常量指针，参考<B>ATL::CSimpleStringT::GetString</B>
		const BaseType * GetCBuffer() const throw()
		{
			return( m_pData );
		}
		bool IsEmpty() const throw()
		{
			return( GetLength() == 0 );
		}
		BaseType * LockBuffer()
		{
			CBufferData* pData = GetData();
			if( pData->IsShared() )
			{
				Fork( pData->nDataLength );
				pData = GetData();  // Do it again, because the fork might have changed it
			}
			pData->Lock();

			return( m_pData );
		}
		void UnlockBuffer() throw()
		{
			CBufferData* pData = GetData();
			pData->Unlock();
		}
		void Preallocate( int nLength )
		{
			PrepareWrite( nLength );
		}
		//! 增加一个参数dwFlags，其他参考<B>ATL::CSimpleStringT::ReleaseBufferSetLength</B>
		/*! \param[in] nNewLength 指定单元长度，不能大于已分配的单元长度
		    \param[in] dwFlags = 0 标志位，其值：GMEM_ZEROINIT-对新分配的空间清零操作*/
		void ReleaseBufferSetLength( int nNewLength, UINT32 dwFlags = 0)
		{
			LGNASSERT( nNewLength >= 0 );
			int nOldLength = GetData()->nDataLength;
			SetLength( nNewLength );

			if ((dwFlags & GMEM_ZEROINIT) && (nNewLength > nOldLength))
				LGN::API::memset(m_pData+nOldLength, 0, (nNewLength - nOldLength) * sizeof( BaseType ));
		}
		void Truncate( int nNewLength )
		{
			LGNASSERT( nNewLength <= GetLength() );
			GetBuffer( nNewLength );
			ReleaseBufferSetLength( nNewLength );
		}
		void SetAt( int iIndex, BaseType bt )
		{
			LGNASSERT( (iIndex >= 0) && (iIndex < GetLength()) );

			if( (iIndex < 0) || (iIndex >= GetLength()) )
				LGN::LgnThrow(E_INVALIDARG);		

			int nLength = GetLength();
			BaseType * pBuffer = GetBuffer();
			pBuffer[iIndex] = bt;
			ReleaseBufferSetLength( nLength );

		}
		void SetManager( ILgnBufferMgr* pBufferMgr )
		{
			LGNASSERT( IsEmpty() );

			CBufferData* pData = GetData();
			pData->Release();
			pData = pBufferMgr->GetNilBuffer();
			AttachData( pData );
		}
		//! 设置缓冲区的内容，参考<B>ATL::CSimpleStringT::SetString</B>
		void SetCBuffer( const BaseType * pSrc, int nLength )
		{
			if( nLength == 0 )
			{
				Empty();
			}
			else
			{
				// It is possible that pSrc points to a location inside of our 
				// buffer.  GetBuffer() might change m_pData if (1) the buffer 
				// is shared or (2) the buffer is too small to hold the new 
				// string.  We detect this aliasing, and modify pSrc to point
				// into the newly allocated buffer instead.

				if(pSrc == NULL)
					LGN::LgnThrow(E_INVALIDARG);			

				UINT nOldLength = GetLength();
				UINT_PTR nOffset = (UINT_PTR)(pSrc-GetCBuffer());
				// If 0 <= nOffset <= nOldLength, then pSrc points into our 
				// buffer

				BaseType * pBuffer = GetBuffer( nLength );
				if( nOffset <= nOldLength )
				{
					CopyBaseTypesOverlapped( pBuffer, pBuffer+nOffset, nLength );
				}
				else
				{
					CopyBaseTypes( pBuffer, pSrc, nLength );
				}
				ReleaseBufferSetLength( nLength );
			}
		}

		void Attach( BaseType * pData, int nNewLength = -1 ) throw()
		{
			GetData()->Release();
			if (nNewLength >= 0)
				GetData()->nDataLength = nNewLength;
			m_pData = pData;
		}

		BaseType * Detach() throw()
		{
			BaseType * pBuffer = GetBuffer();
			GetData()->AddRef();
			return pBuffer;
		}

	public:
		friend CSimpleBufferT operator+(const CSimpleBufferT& str1, const CSimpleBufferT& str2 )
		{
			CSimpleBufferT s( str1.GetManager() );

			Concatenate( s, str1, str1.GetLength(), str2, str2.GetLength() );

			return( s );
		}

		//! 按单元复制，参考<B>ATL::CSimpleStringT::CopyChars</B>
		static void LGN_CDECL CopyBaseTypes( BaseType* pDest, const BaseType* pSrc, int nCount ) throw()
		{
			LGN::API::memcpy(pDest, pSrc, nCount*sizeof( BaseType ));
		}
		//! 按单元移动，参考<B>ATL::CSimpleStringT::CopyCharsOverlapped</B>
		static void LGN_CDECL CopyBaseTypesOverlapped( BaseType* pDest, const BaseType* pSrc, int nCount ) throw()
		{
			LGN::API::memmove(pDest, pSrc, nCount*sizeof( BaseType ));
		}
		//! 按单元查找
		static int LGN_CDECL FindBaseType(const BaseType * pBuffer, int nLength, BaseType bt)
		{
			for (int i=0; i<nLength; i++)
			{
				if (pBuffer[i] == bt)
					return i;
			}
			return -1;
		}

		//! 按单元反转
		static void LGN_CDECL ReverseBaseTypes(BaseType * pBuffer, int nLength)
		{
			BaseType * pEnd = pBuffer + nLength;
			for (pEnd--; pBuffer < pEnd; pBuffer++,pEnd--)
			{
				BaseType t = *pBuffer;
				*pBuffer = *pEnd;
				*pEnd = t;
			}
		}

	protected:
		static void LGN_CDECL Concatenate( CSimpleBufferT& strResult, const BaseType* psz1, int nLength1, const BaseType* psz2, int nLength2 )
		{
			int nNewLength = nLength1+nLength2;
			BaseType * pBuffer = strResult.GetBuffer( nNewLength );
			CopyBaseTypes( pBuffer, psz1, nLength1 );
			CopyBaseTypes( pBuffer+nLength1, psz2, nLength2 );
			strResult.ReleaseBufferSetLength( nNewLength );
		}
		LGN_NOINLINE static void LGN_CDECL ThrowMemoryException()
		{
			LGN::LgnThrow( E_OUTOFMEMORY );
		}

		// Implementation
	private:
		void AttachData( CBufferData* pData ) throw()
		{
			m_pData = static_cast< BaseType * >( pData->data() );
		}
		LGN_NOINLINE void Fork( int nLength )
		{
			CBufferData* pOldData = GetData();
			int nOldLength = pOldData->nDataLength;
			CBufferData* pNewData = pOldData->pBufferMgr->Clone()->Allocate( nLength, sizeof( BaseType ) );
			if( pNewData == NULL )
			{
				ThrowMemoryException();
			}
			int nCountToCopy = ((nOldLength < nLength) ? nOldLength : nLength)+1;  // Copy Last
			CopyBaseTypes( (BaseType*)( pNewData->data() ), (BaseType*)( pOldData->data() ), nCountToCopy );
			pNewData->nDataLength = nOldLength;
			pOldData->Release();
			AttachData( pNewData );
		}
		CBufferData* GetData() const throw()
		{
			return( reinterpret_cast< CBufferData* >( m_pData )-1 );
		}
		BaseType * PrepareWrite( int nLength )
		{
			CBufferData* pOldData = GetData();
			int nShared = 1-pOldData->nRefs;  // nShared < 0 means true, >= 0 means false
			int nTooShort = pOldData->nAllocLength-nLength;  // nTooShort < 0 means true, >= 0 means false
			if( (nShared|nTooShort) < 0 )  // If either sign bit is set (i.e. either is less than zero), we need to copy data
			{
				PrepareWrite2( nLength );
			}

			return( m_pData );
		}
		LGN_NOINLINE void PrepareWrite2( int nLength )
		{
			CBufferData* pOldData = GetData();
			if( pOldData->nDataLength > nLength )
			{
				nLength = pOldData->nDataLength;
			}
			if( pOldData->IsShared() )
			{
				Fork( nLength );
			}
			else if( pOldData->nAllocLength < nLength )
			{
				// Grow exponentially, until we hit 1K.
				int nNewLength = pOldData->nAllocLength;
				if( nNewLength > 1024 )
				{
					nNewLength += 1024;
				}
				else
				{
					nNewLength *= 2;
				}
				if( nNewLength < nLength )
				{
					nNewLength = nLength;
				}
				Reallocate( nNewLength );
			}
		}
		LGN_NOINLINE void Reallocate( int nLength )
		{
			CBufferData* pOldData = GetData();
			LGNASSERT( pOldData->nAllocLength < nLength );
			ILgnBufferMgr* pBufferMgr = pOldData->pBufferMgr;
			CBufferData* pNewData = pBufferMgr->Reallocate( pOldData, nLength, sizeof( BaseType ) );
			if( pNewData == NULL )
			{
				ThrowMemoryException();
			}
			AttachData( pNewData );
		}

		void SetLength( int nLength )
		{
			LGNASSERT( nLength >= 0 );
			LGNASSERT( nLength <= GetData()->nAllocLength );

			if( nLength < 0 || nLength > GetData()->nAllocLength)
				LGN::LgnThrow(E_INVALIDARG);

			GetData()->nDataLength = nLength;
			//m_pData[nLength] = 0;
			LGN::API::memset(m_pData + nLength, 0, sizeof( BaseType ));
		}

		static CBufferData* LGN_CDECL CloneData( CBufferData* pData )
		{
			CBufferData* pNewData = NULL;

			ILgnBufferMgr* pNewBufferMgr = pData->pBufferMgr->Clone();
			if( !pData->IsLocked() && (pNewBufferMgr == pData->pBufferMgr) )
			{
				pNewData = pData;
				pNewData->AddRef();
			}
			else
			{
				pNewData = pNewBufferMgr->Allocate( pData->nDataLength, sizeof( BaseType ) );
				if( pNewData == NULL )
				{
					ThrowMemoryException();
				}
				pNewData->nDataLength = pData->nDataLength;
				CopyBaseTypes( (BaseType*)( pNewData->data() ), (BaseType*)( pData->data() ), pData->nDataLength+1 );  // Copy Last
			}

			return( pNewData );
		}

	private:
		BaseType * m_pData;
	};

/*! \page simplebuffer_example_page CSimpleBufferT例子
<pre>	%CSimpleBufferT模板类的使用例子</pre>
<pre>
\#include "lgnbase.h"

struct ST_XY
{
	int x;
	int y;
};
	// ...
	// 构造函数：分配缓冲区，并填充
	LGN::CSimpleBufferT<CHAR> buffA("123", 3, LGN::_lgnBufferManager);

	// 调用GetBufferSetLength函数来分配缓冲区，返回其类型的数组指针
	LGN::CSimpleBufferT<DWORD> buffB(LGN::_lgnBufferManager);
	LPDWORD pdwB = buffB.GetBufferSetLength(4);
	pdwB[0] = 1;
	pdwB[2] = 3;

	// 调用GetBuffer函数来分配缓冲区，返回其类型（结构）的数组指针；调用ReleaseBufferSetLength留下指定个数单元数据
	LGN::CSimpleBufferT<ST_XY> buffC(LGN::_lgnBufferManager);
	ST_XY * pxy = buffC.GetBuffer(10);
	pxy[0].x = 1; pxy[0].y = 2;
	buffC.ReleaseBufferSetLength(1);

	// 新建个实例buffD指向同一对象（buffC）
	LGN::CSimpleBufferT<ST_XY> buffD = buffC;
	// 重新分配缓冲区，实例buffD与buffC无关
	buffD.GetBufferSetLength(2);</pre>
*/


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 缓冲区分配管理模板类
	/*!	来源 cstringt.h，改造<B>ATL::CStringT</B>，没有函数说明的，请参考MSDN<br>
	 *  提供更多缓冲区操作函数，用全局的内存管理对象_lgnBufferManager来管理内存；不同于<B>ATL::CStringT</B>，本模板类可以指定任何类型（包括结构）的数据，点击这里查看 \ref buffer_example_page
	*/
	template< typename BaseType >
	class CBufferT : public CSimpleBufferT< BaseType >
	{
	public:
		typedef CSimpleBufferT< BaseType > CThisSimpleBuffer;

		static ILgnBufferMgr * GetDefaultManager()
		{
			return _lgnBufferManager;
		}

	public:
		CBufferT()
			: CThisSimpleBuffer(_lgnBufferManager)
		{
		}
		CBufferT( const CBufferT& bufSrc )
			: CThisSimpleBuffer(bufSrc)
		{
		}
		explicit CBufferT( const CThisSimpleBuffer& bufSrc )
			: CThisSimpleBuffer(bufSrc)
		{
		}
		CBufferT( const BaseType* pSrc, int nLength )
			: CThisSimpleBuffer(pSrc, nLength, _lgnBufferManager)
		{
		}

		explicit CBufferT( ILgnBufferMgr* pBufferMgr)
			: CThisSimpleBuffer( pBufferMgr )
		{
		}
		CBufferT( const BaseType* pSrc, int nLength, ILgnBufferMgr* pBufferMgr)
			: CThisSimpleBuffer( pSrc, nLength, pBufferMgr )
		{
		}


		CBufferT& operator=( const CBufferT& bufSrc )
		{
			CThisSimpleBuffer:: operator=(bufSrc);
			return (*this);
		}

		CBufferT& operator+=( const CBufferT& bufSrc )
		{
			CThisSimpleBuffer::Append( bufSrc );

			return( *this );
		}

		CBufferT& operator+=( BaseType bt )
		{
			CThisSimpleBuffer::AppendBaseType( bt );

			return( *this );
		}

		bool operator==(const CBufferT& bufSrc) const
		{
			return (Compare(bufSrc, bufSrc.GetLength()) == 0);
		}
		bool operator!=(const CBufferT& bufSrc) const
		{
			return (Compare(bufSrc, bufSrc.GetLength()) != 0);
		}
		bool operator>(const CBufferT& bufSrc) const
		{
			return (Compare(bufSrc, bufSrc.GetLength()) > 0);
		}
		bool operator>=(const CBufferT& bufSrc) const
		{
			return (Compare(bufSrc, bufSrc.GetLength()) >= 0);
		}
		bool operator<(const CBufferT& bufSrc) const
		{
			return (Compare(bufSrc, bufSrc.GetLength()) < 0);
		}
		bool operator<=(const CBufferT& bufSrc) const
		{
			return (Compare(bufSrc, bufSrc.GetLength()) <= 0);
		}


		void Append( const BaseType * pSrc, int nLength )
		{
			CThisSimpleBuffer::Append( pSrc, nLength );
		}

		void Append( const CBufferT& strSrc )
		{
			CThisSimpleBuffer::Append( strSrc.GetCBuffer(), strSrc.GetLength() );
		}

		void Append( BaseType bt, int nLength = 1 )
		{
			if (nLength > 0)
			{
				int nOldLength = CThisSimpleBuffer::GetLength();
				int nNewLength = nOldLength+nLength;
				BaseType * pBuffer = CThisSimpleBuffer::GetBuffer( nNewLength );
				for (int i=nOldLength; i<nNewLength; i++)
					pBuffer[i] = bt;
				CThisSimpleBuffer::ReleaseBufferSetLength( nNewLength );
			}
		}

		//! 把本缓冲区的数据按单元复制出一个新缓冲区
		template< typename _BaseTypeOut >
		CBufferT< _BaseTypeOut > ToBufferT(_BaseTypeOut * pOutBuff=NULL)
		{
			if (sizeof(BaseType) == sizeof(_BaseTypeOut))
			{
				return *((CBufferT< _BaseTypeOut > *)this);
			}
			else
			{
				CBufferT< _BaseTypeOut > buffRet;
#if (LGN_HI_TEMPLATE)
				buffRet.AppendT< BaseType >(CThisSimpleBuffer::GetCBuffer(), CThisSimpleBuffer::GetLength());
#else
				buffRet.AppendT(CThisSimpleBuffer::GetCBuffer(), CThisSimpleBuffer::GetLength());
#endif
				return buffRet;
			}
		}

		// Comparison
		int Compare( const CBufferT& bufSrc ) const throw()
		{
			return Compare(bufSrc.GetCBuffer(), bufSrc.GetLength());
		}
		int Compare( const BaseType* pSrc, int nLength) const throw()
		{
			LGNASSERT( nLength >= 0);
			int nMinLength = min(nLength, CThisSimpleBuffer::GetLength());
			int nRet = LGN::API::memcmp(CThisSimpleBuffer::GetCBuffer(), pSrc, sizeof(BaseType) * nMinLength);
			if (nRet == 0)
			{
				if ( CThisSimpleBuffer::GetLength() > nMinLength)
					nRet = 1;
				else if ( nLength > nMinLength)
					nRet = -1;
			}
			return nRet;
		}

		// Advanced manipulation

		// Delete 'nCount' BaseTypes, starting at index 'iIndex'
		int Delete( int iIndex, int nCount = 1 )
		{
			if( iIndex < 0 )
				iIndex = 0;

			if( nCount < 0 )
				nCount = 0;

			int nLength = CThisSimpleBuffer::GetLength();
			if( (nCount+iIndex) > nLength )
			{
				nCount = nLength-iIndex;
			}
			if( nCount > 0 )
			{
				int nNewLength = nLength-nCount;
				int nBTsToCopy = nLength-(iIndex+nCount)+1;
				BaseType * pBuffer = CThisSimpleBuffer::GetBuffer();
				LGN::API::memmove( pBuffer+iIndex, pBuffer+iIndex+nCount, nBTsToCopy*sizeof( BaseType ) );
				CThisSimpleBuffer::ReleaseBufferSetLength( nNewLength );
			}

			return( CThisSimpleBuffer::GetLength() );
		}

		// Insert BaseType 'bt' before index 'iIndex'
		int Insert( int iIndex, BaseType bt )
		{
			if( iIndex < 0 )
				iIndex = 0;

			if( iIndex > CThisSimpleBuffer::GetLength() )
			{
				iIndex = CThisSimpleBuffer::GetLength();
			}
			int nNewLength = CThisSimpleBuffer::GetLength()+1;

			BaseType * pBuffer = CThisSimpleBuffer::GetBuffer( nNewLength );

			// move existing bytes down
			LGN::API::memmove( pBuffer+iIndex+1, pBuffer+iIndex, (nNewLength-iIndex)*sizeof( BaseType ) );
			pBuffer[iIndex] = bt;

			CThisSimpleBuffer::ReleaseBufferSetLength( nNewLength );

			return( nNewLength );
		}

		// Insert string 'psz' before index 'iIndex'
		int Insert( int iIndex, const BaseType * pSrc, int nLength )
		{
			if( iIndex < 0 )
				iIndex = 0;

			if( iIndex > CThisSimpleBuffer::GetLength() )
			{
				iIndex = CThisSimpleBuffer::GetLength();
			}

			// nInsertLength and nNewLength are in BaseTypes
			int nInsertLength = nLength;
			int nNewLength = CThisSimpleBuffer::GetLength();
			if( nInsertLength > 0 )
			{
				nNewLength += nInsertLength;

				BaseType * pBuffer = CThisSimpleBuffer::GetBuffer( nNewLength );
				// move existing bytes down
				LGN::API::memmove( pBuffer+iIndex+nInsertLength,
					pBuffer+iIndex, (nNewLength-iIndex-nInsertLength+1)*sizeof( BaseType ) );
				LGN::API::memcpy( pBuffer+iIndex, pSrc, nInsertLength*sizeof( BaseType ) );
				CThisSimpleBuffer::ReleaseBufferSetLength( nNewLength );
			}

			return( nNewLength );
		}

		int Insert( int iIndex, const CBufferT& bufSrc )
		{
			return Insert(iIndex, bufSrc.GetCBuffer(), bufSrc.GetLength());
		}

		// Replace all occurrences of BaseType 'btOld' with BaseType 'btNew'
		int Replace( BaseType btOld, BaseType btNew )
		{
			int nCount = 0;

			// short-circuit the nop case
			if( btOld != btNew )
			{
				BaseType * pBuffer = CThisSimpleBuffer::GetBuffer();
				int nLength = CThisSimpleBuffer::GetLength();
				for(int iIndex = 0; iIndex < nLength; iIndex ++)
				{
					// replace instances of the specified BaseType only
					if( pBuffer[iIndex] == btOld )
					{
						pBuffer[iIndex] = btNew;
						nCount++;
					}
				}
			}

			return( nCount );
		}

		// Replace all occurrences of buffer 'pSource' with buffer 'pReplacement'
		int Replace( const BaseType * pSource, int nSourceLen, const BaseType * pReplacement, int nReplacementLen )
		{
			// loop once to figure out the size of the result buffer
			int nCount = 0;
			{
				int iStart = 0, iFind;
				while( (iFind = Find( pSource, nSourceLen, iStart ) ) != -1)
				{
					nCount++;
					iStart = iFind+nSourceLen;
				}
			}

			if (nCount > 0)
			{
				CBufferT oldBuff( *this );
				int nOldLength = oldBuff.GetLength();
				int nNewLength = nOldLength+(nReplacementLen-nSourceLen)*nCount;
				BaseType * pStart = CThisSimpleBuffer::GetBufferSetLength( nNewLength );
				int iStart = 0, iFind;
				while( (iFind = oldBuff.Find( pSource, nSourceLen, iStart ) ) != -1 )
				{					
					if (iFind > iStart)
					{
						LGN::API::memcpy( pStart, oldBuff.GetCBuffer()+iStart, (iFind-iStart)*sizeof( BaseType ) );
						pStart += (iFind-iStart);
					}
					LGN::API::memcpy( pStart, pReplacement, nReplacementLen*sizeof( BaseType ) );
					pStart += nReplacementLen;

					iStart = iFind+nSourceLen;
				}
				if (iStart < nOldLength)
				{
					LGN::API::memcpy( pStart, oldBuff.GetCBuffer()+iStart, (nOldLength-iStart)*sizeof( BaseType ) );
				}
			}

			return( nCount );
		}

		int Replace( const CBufferT& buffSrc, const CBufferT& buffRep )
		{
			return Replace(buffSrc.GetCBuffer(), buffSrc.GetLength(), buffRep.GetCBuffer(), buffRep.GetLength());
		}

		// Remove all occurrences of BaseType 'btRemove'
		int Remove( BaseType btRemove )
		{
			int nLength = CThisSimpleBuffer::GetLength();
			BaseType * pBuffer = CThisSimpleBuffer::GetBuffer( nLength );

			BaseType * pSource = pBuffer;
			BaseType * pDest = pBuffer;
			BaseType * pEnd = pBuffer+nLength;

			while( pSource < pEnd )
			{
				BaseType * pNewSource = pSource + 1;
				//if( *pSource != btRemove )
				if(LGN::API::memcmp(pSource,&btRemove,sizeof(btRemove)) != 0)
				{
					BaseType * pNewDest = pDest+(pNewSource-pSource);
					while( pDest != pNewDest )
					{
						*pDest = *pSource;
						pSource++;
						pDest++;
					}
				}
				pSource = pNewSource;
			}
			int nCount = int( pSource-pDest );
			CThisSimpleBuffer::ReleaseBufferSetLength( nLength-nCount );

			return( nCount );
		}


		// find routines

		// Find the first occurrence of BaseType 'bt', starting at index 'iStart'
		int Find( BaseType bt, int iStart = 0 ) const throw()
		{
			// nLength is in BaseTypes
			int nLength = CThisSimpleBuffer::GetLength();
			if( iStart < 0 || iStart >= nLength)
			{
				return( -1 );
			}

			const BaseType * pBuffer = CThisSimpleBuffer::GetCBuffer();
			for (int i=iStart; i<nLength; i++)
			{
				if (pBuffer[i] == bt)
					return i;
			}
			return -1;
		}

		// look for a specific sub-string

		// Find the first occurrence of buffer 'pSub', starting at index 'iStart'
		int Find( const BaseType * pSub, int nSubLen, int iStart = 0 ) const throw()
		{
			if(pSub == NULL || nSubLen < 1)
			{
				return( -1 );
			}
			// nLength is in BaseTypes
			int nLength = CThisSimpleBuffer::GetLength();
			if( iStart < 0 || iStart > nLength )
			{
				return( -1 );
			}

			const BaseType * pBuffer = CThisSimpleBuffer::GetCBuffer();
			for (int i=iStart; i<=(nLength-nSubLen); i++)
			{
				if (pBuffer[i] == pSub[0])
				{
					if (LGN::API::memcmp(pBuffer+i, pSub, nSubLen * sizeof(BaseType)) == 0)
						return i;
				}
			}

			// return -1 for not found, distance from beginning otherwise
			return( -1 );
		}

		int Find( const CBufferT& buffSub, int iStart = 0 ) const throw()
		{
			return Find(buffSub.GetCBuffer(), buffSub.GetLength(), iStart);
		}

		// Find the first occurrence of any of the BaseTypes in string 'pszCharSet'
		int FindOneOf( const BaseType * pszSet, int nSetLen, int iStart = 0 ) const throw()
		{
			if (iStart < 0)
			{
				return -1;
			}

			for (int i=iStart; i<CThisSimpleBuffer::GetLength(); i++)
			{
				if (CThisSimpleBuffer::FindBaseType(pszSet, nSetLen, CThisSimpleBuffer::GetAt(i)) != -1)
					return i;
			}
			return -1;
		}
		int FindOneOf( const CBufferT& buffSet, int iStart = 0 ) const throw()
		{
			return FindOneOf(buffSet.GetCBuffer(), buffSet.GetLength(), iStart);
		}

		// Find the last occurrence of BaseType 'bt'
		int ReverseFind( BaseType bt ) const throw()
		{
			const BaseType * pBuffer = CThisSimpleBuffer::GetCBuffer();
			for (int i=CThisSimpleBuffer::GetLength()-1; i>=0; i--)
			{
				if (pBuffer[i] == bt)
					return i;
			}

			return -1;
		}

		// Return the subbuffer starting at index 'iFirst'
		CBufferT Mid( int iFirst ) const
		{
			return( Mid( iFirst, CThisSimpleBuffer::GetLength()-iFirst ) );
		}

		// Return the subbuffer starting at index 'iFirst', with length 'nCount'
		CBufferT Mid( int iFirst, int nCount ) const
		{
			if (iFirst < 0)
				iFirst = 0;
			if (nCount < 0)
				nCount = 0;

			if( (iFirst+nCount) > CThisSimpleBuffer::GetLength() )
			{
				nCount = CThisSimpleBuffer::GetLength()-iFirst;
			}
			if( iFirst > CThisSimpleBuffer::GetLength() )
			{
				nCount = 0;
			}

			LGNASSERT( (nCount == 0) || ((iFirst+nCount) <= CThisSimpleBuffer::GetLength()) );

			// optimize case of returning entire string
			if( (iFirst == 0) && ((iFirst+nCount) == CThisSimpleBuffer::GetLength()) )
			{
				return( *this );
			}

			return( CBufferT( CThisSimpleBuffer::GetCBuffer()+iFirst, nCount, CThisSimpleBuffer::GetManager() ) );
		}

		// Return the subbuffer consisting of the rightmost 'nCount' BaseTypes
		CBufferT Right( int nCount ) const
		{
			if (nCount < 0)
				nCount = 0;

			int nLength = CThisSimpleBuffer::GetLength();
			if( nCount >= nLength )
			{
				return( *this );
			}

			return( CBufferT( CThisSimpleBuffer::GetCBuffer()+nLength-nCount, nCount, CThisSimpleBuffer::GetManager() ) );
		}

		// Return the subbuffer consisting of the leftmost 'nCount' BaseTypes
		CBufferT Left( int nCount ) const
		{
			if (nCount < 0)
				nCount = 0;

			int nLength = CThisSimpleBuffer::GetLength();
			if( nCount >= nLength )
			{
				return( *this );
			}

			return( CBufferT( CThisSimpleBuffer::GetCBuffer(), nCount, CThisSimpleBuffer::GetManager() ) );
		}

	public:
		friend CBufferT operator+(const CBufferT& str1,const CBufferT& str2 )
		{
			CBufferT s(str1.GetManager() );

			CThisSimpleBuffer::Concatenate( s, str1, str1.GetLength(), str2, str2.GetLength() );

			return( s );
		}

	};

	typedef CBufferT<BYTE>  CByteBuffer;
	typedef CBufferT<CHAR>  CAnsiBuffer;
	typedef CBufferT<WCHAR> CWideBuffer;
#ifdef UNICODE
	typedef CWideBuffer  CStrBuffer; //!< 在定义UNICODE下
#else
	typedef CAnsiBuffer  CStrBuffer; //!< 在没有定义UNICODE下
#endif

/*! \page buffer_example_page CBufferT例子
<pre>	%CBufferT模板类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// ...
	LGN::CByteBuffer buffA((LPBYTE)"123", 3);
	LGN::CBufferT<BYTE> buffB = buffA.Left(2);
</pre>
*/

}; // namespace LGN

#endif
