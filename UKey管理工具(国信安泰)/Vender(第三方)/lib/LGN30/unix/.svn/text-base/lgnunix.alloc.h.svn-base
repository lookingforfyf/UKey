#ifndef __LGNUNIX_ALLOC_H__
#define __LGNUNIX_ALLOC_H__


namespace LGN
{
	template< typename N >
		inline N LgnAlignUp( N n, ULONG nAlign ) throw()
	{
		return( N( (n+(nAlign-1))&~(N( nAlign )-1) ) );
	}

	template< typename N >
		inline N LgnAlignDown( N n, ULONG nAlign ) throw()
	{
		return( N( n&~(N( nAlign )-1) ) );
	}

	class ILgnMemMgr
	{
	public:
		virtual ~ILgnMemMgr() {};
		virtual void* Allocate( size_t nBytes ) throw() = 0;
		virtual void Free( void* p ) throw() = 0;
		virtual void* Reallocate( void* p, size_t nBytes ) throw() = 0;
		virtual size_t GetSize( void* p ) throw() = 0;
	};

	class CCRTHeap :
		public ILgnMemMgr
	{
	public:
		CCRTHeap()
		{
			m_nAllocSize = 0;
		}

		virtual void* Allocate( size_t nBytes ) throw()
		{
			m_nAllocSize = nBytes;
			return( malloc( nBytes ) );
		}
		virtual void Free( void* p ) throw()
		{
			m_nAllocSize = 0;
			free( p );
		}
		virtual void* Reallocate( void* p, size_t nBytes ) throw()
		{
			m_nAllocSize = nBytes;
			return( realloc( p, nBytes ) );
		}
		virtual size_t GetSize( void* p ) throw()
		{
			return( m_nAllocSize );
		}

	protected:
		size_t m_nAllocSize;
	};

	extern LGN::ILgnMemMgr * _lgnMemoryManager;

	//
	class CTempStrBuff
	{
	public:
		CTempStrBuff(LPCSTR pszStr, int nLength = -1)
		{
			m_pszStr = NULL;
			if ((nLength < 0) && (pszStr != NULL))
			{
				nLength = LGN::API::SafeStringLength(pszStr);
			}
			
			if (nLength >= 0)
			{
				m_pszStr = (LPSTR)_lgnMemoryManager->Allocate(nLength+1);
				if (pszStr != NULL)
				{
					LGN::API::memcpy(m_pszStr,pszStr,nLength);
					m_pszStr[nLength] = 0;
				}
				else				
				{
					LGN::API::memset(m_pszStr,0,nLength+1);
				}
			}
		}
		~CTempStrBuff()
		{
			_lgnMemoryManager->Free(m_pszStr);
		}

		operator LPSTR()
		{
			return( m_pszStr );
		}		
		operator LPCSTR()
		{
			return( m_pszStr );
		}
		
	protected:
		LPSTR m_pszStr;
		
	private:
		CTempStrBuff( const CTempStrBuff& );
		CTempStrBuff& operator=( const CTempStrBuff& );
	};

}; // namespace LGN

namespace LGN
{

	struct CBufferData;

	class ILgnBufferMgr
	{
	public:
		virtual ~ILgnBufferMgr() {};
		// Allocate a new CBufferData
		virtual CBufferData* Allocate( int nAllocLength, int nBaseTypeSize ) throw() = 0;
		// Free an existing CBufferData
		virtual void Free( CBufferData* pData ) throw() = 0;
		// Change the size of an existing CBufferData
		virtual CBufferData* Reallocate( CBufferData* pData, int nAllocLength, int nBaseTypeSize ) throw() = 0;
		// Get the CBufferData for a Nil buffer
		virtual CBufferData* GetNilBuffer() throw() = 0;
		virtual ILgnBufferMgr* Clone() throw() = 0;
	};


	struct CBufferData
	{
		ILgnBufferMgr* pBufferMgr;  // Buffer manager for this CBufferData
		int nDataLength;  // Length of currently used data in TYPEs
		int nAllocLength;  // Length of allocated data in TYPEs
		long nRefs;     // Reference count: negative == locked
		// TYPE data[nAllocLength+1]  // A CBufferData is always followed in memory by the actual array of character data

		void* data() throw()
		{
			return (this+1);
		}

		void AddRef() throw()
		{
			LGNASSERT(nRefs > 0);
			nRefs ++ ;	// ::InterlockedIncrement(&nRefs);
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

			if( (--nRefs) /*::InterlockedDecrement( &nRefs )*/ <= 0 )
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

	// Ä¬ÈÏ·ÖÅäÆ÷
	extern ILgnBufferMgr * _lgnBufferManager;

}; // namespace LGN

#endif  // #ifndef __LGNUNIX_ALLOC_H__
