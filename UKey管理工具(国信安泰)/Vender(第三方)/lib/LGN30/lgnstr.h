/*! \file lgnstr.h 
 *  \brief 字符串处理模板类
 *
 *  被lgnbase.h包含，工程不用直接包含本文件
 *
 *  声明简单字符串处理模板类CSimpleStringT，字符串处理模板类CStringT，扩展字符串处理模板类CStringExT；<br>
 *  注意CString是模板类CStringExT类型重定义。代码启用如下：
 *  <ul>
 *  	<li> 如果没有包含atlsimpstr.h，则实现静态字符串类CStaticString和简单字符串处理模板类CSimpleStringT
 *		<li> 如果没有包含atlstr.h，则实现字符串处理模板类CStringT
 *  	<li> 扩展字符串处理模板类CStringExT继承于CStringT
 *  </ul>
 *  
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

#ifndef __LGNSTR_H__
#define __LGNSTR_H__


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __ATLSIMPSTR_H__
#define LGN_SIMPLESTRING_NS ATL
#else
#define LGN_SIMPLESTRING_NS LGN
	//! 静态字符串类
	/*!	来源 atlsimpstr.h，同<B>ATL::CStaticString</B>，请参考MSDN<br>*/
	template< typename BaseType, const int t_nSize >
	class CStaticString
	{
	public:
		CStaticString( const BaseType* psz ) : m_psz( psz )
		{
		}

		operator const BaseType*() const
		{
			return m_psz;
		}

		static int LGN_CDECL GetLength() 
		{
			return (t_nSize/sizeof( BaseType ))-1;
		}

	private:
		const BaseType* m_psz;

	private:
		CStaticString( const CStaticString& str ) throw();
		CStaticString& operator=( const CStaticString& str ) throw();
	};


	//! 简单字符串处理模板类
	/*!	来源 atlsimpstr.h，同<B>ATL::CSimpleStringT</B>，请参考MSDN<br>*/
	template< typename BaseType>
	class CSimpleStringT : public CSimpleBufferT< BaseType >
	{
	public:
		typedef typename ChTraitsBase< BaseType >::XCHAR XCHAR;
		typedef typename ChTraitsBase< BaseType >::PXSTR PXSTR;
		typedef typename ChTraitsBase< BaseType >::PCXSTR PCXSTR;
		typedef typename ChTraitsBase< BaseType >::YCHAR YCHAR;
		typedef typename ChTraitsBase< BaseType >::PYSTR PYSTR;
		typedef typename ChTraitsBase< BaseType >::PCYSTR PCYSTR;

		typedef CSimpleBufferT< BaseType > CThisSimpleBuffer;

	public:
		explicit CSimpleStringT( ILgnBufferMgr* pStringMgr ) throw()
			: CThisSimpleBuffer(pStringMgr)
		{
		}

		CSimpleStringT( const CSimpleStringT& strSrc )
			: CThisSimpleBuffer(strSrc)
		{
		}

		CSimpleStringT( PCXSTR pszSrc, ILgnBufferMgr* pStringMgr )
			: CThisSimpleBuffer(pszSrc, pStringMgr)
		{
		}
		CSimpleStringT( PCXSTR pchSrc, int nLength, ILgnBufferMgr* pStringMgr )
			: CThisSimpleBuffer(pchSrc, nLength, pStringMgr)
		{
		}
		~CSimpleStringT() throw()
		{
		}

		CSimpleStringT& operator=( const CSimpleStringT& strSrc )
		{
			CThisSimpleBuffer::operator=(strSrc);
			return (*this);
		}

		CSimpleStringT& operator=( PCXSTR pszSrc )
		{
			SetString( pszSrc );

			return( *this );
		}

		CSimpleStringT& operator+=( const CSimpleStringT& strSrc )
		{
			Append( strSrc );

			return( *this );
		}

		CSimpleStringT& operator+=( PCXSTR pszSrc )
		{
			Append( pszSrc );

			return( *this );
		}
		template< int t_nSize >
			CSimpleStringT& operator+=( const CStaticString< XCHAR, t_nSize >& strSrc )
		{
			Append( strSrc.m_psz, strSrc.GetLength() );

			return( *this );
		}
		CSimpleStringT& operator+=( CHAR ch )
		{
			AppendChar( XCHAR( ch ) );

			return( *this );
		}
		CSimpleStringT& operator+=( WCHAR ch )
		{
			AppendChar( XCHAR( ch ) );

			return( *this );
		}

		XCHAR operator[]( int iChar ) const
		{
			return CThisSimpleBuffer::GetAt(iChar);;
		}

		operator PCXSTR() const throw()
		{
			return( CThisSimpleBuffer::GetCBuffer() );
		}

		void Append( PCXSTR pszSrc )
		{
			Append( pszSrc, StringLength( pszSrc ) );
		}
		void Append( PCXSTR pszSrc, int nLength )
		{
			CThisSimpleBuffer::Append(pszSrc, nLength);
		}
		void AppendChar( XCHAR ch )
		{
			CThisSimpleBuffer::AppendBaseType(ch);
		}
		void Append( const CSimpleStringT& strSrc )
		{
			Append( strSrc.GetString(), strSrc.GetLength() );
		}

		PXSTR GetBuffer()
		{
			return CThisSimpleBuffer::GetBuffer();
		}
		PXSTR GetBuffer( int nMinBufferLength )
		{
			return CThisSimpleBuffer::GetBuffer(nMinBufferLength);
		}
		PXSTR GetBufferSetLength( int nLength )
		{
			return CThisSimpleBuffer::GetBufferSetLength(nLength);
		}

		PCXSTR GetString() const throw()
		{
			return( CThisSimpleBuffer::GetCBuffer() );
		}
		void ReleaseBuffer( int nNewLength = -1 )
		{
			if( nNewLength == -1 )
			{
				nNewLength = StringLength(  CThisSimpleBuffer::GetCBuffer() );
			}
			CThisSimpleBuffer::ReleaseBufferSetLength(nNewLength);
		}
		void ReleaseBufferSetLength( int nNewLength )
		{
			CThisSimpleBuffer::ReleaseBufferSetLength(nNewLength);
		}
		void SetString( PCXSTR pszSrc )
		{
			SetString( pszSrc, StringLength( pszSrc ) );
		}
		void SetString( PCXSTR pszSrc, int nLength )
		{
			CThisSimpleBuffer::SetCBuffer(pszSrc, nLength);
		}

	public:
		friend CSimpleStringT operator+(const CSimpleStringT& str1,const CSimpleStringT& str2 )
		{
			CSimpleStringT s( str1.GetManager() );

			Concatenate( s, str1, str1.GetLength(), str2, str2.GetLength() );

			return( s );
		}

		friend CSimpleStringT operator+(const CSimpleStringT& str1,PCXSTR psz2 )
		{
			CSimpleStringT s( str1.GetManager() );

			Concatenate( s, str1, str1.GetLength(), psz2, StringLength( psz2 ) );

			return( s );
		}

		friend CSimpleStringT operator+(PCXSTR psz1,const CSimpleStringT& str2 )
		{
			CSimpleStringT s( str2.GetManager() );

			Concatenate( s, psz1, StringLength( psz1 ), str2, str2.GetLength() );

			return( s );
		}

		static void LGN_CDECL CopyChars( PXSTR pchDest, PCXSTR pchSrc, int nChars ) throw()
		{
			LGN::API::memcpy( pchDest, pchSrc, nChars*sizeof( XCHAR ) );
		}
		static void LGN_CDECL CopyCharsOverlapped( PXSTR pchDest, PCXSTR pchSrc, int nChars ) throw()
		{
			LGN::API::memmove( pchDest, pchSrc, nChars*sizeof( XCHAR ) );
		}
		static int LGN_CDECL StringLength( PCXSTR psz ) throw()
		{
			return LGN::API::SafeStringLength(psz);
		}

		// Implementation
	private:

	};
#endif // #ifdef __ATLSIMPSTR_H__


////////////////////////////////////////////////////////////////////////////////////////////////
// CString
#ifdef __CSTRINGT_H__
#define LGN_CSTRING_NS ATL
#else
#define LGN_CSTRING_NS LGN

	//! 字符串处理模板类
	/*!	来源 atlstr.h，同<B>ATL::CStringT</B>，请参考MSDN<br>
		不支持VARIANT类型及OEM字符的转换功能
	*/
	template< typename BaseType, class StringTraits >
	class CStringT : public LGN_SIMPLESTRING_NS::CSimpleStringT< BaseType >
	{
	public:
		typedef LGN_SIMPLESTRING_NS::CSimpleStringT< BaseType > CThisSimpleString;
		typedef StringTraits StrTraits;
#if (_MSC_VER < 1000) // (!LGN_WINSYS)
		typedef typename ChTraitsBase< BaseType >::XCHAR XCHAR;
		typedef typename ChTraitsBase< BaseType >::PXSTR PXSTR;
		typedef typename ChTraitsBase< BaseType >::PCXSTR PCXSTR;
		typedef typename ChTraitsBase< BaseType >::PCYSTR PCYSTR;
#endif
			
	public:
		CStringT() throw() : CThisSimpleString( StringTraits::GetDefaultManager() )
		{
		}

		explicit CStringT( ILgnBufferMgr* pStringMgr ) throw() : CThisSimpleString( pStringMgr )
		{ 
		}

		//CStringT( const VARIANT& varSrc );
		//CStringT( const VARIANT& varSrc, ILgnBufferMgr* pStringMgr );

		static void LGN_CDECL Construct( CStringT* pString )
		{
			new( pString ) CStringT;
		}

		// Copy constructor
		CStringT( const CStringT& strSrc ) : CThisSimpleString( strSrc )
		{
		}

		// Construct from CSimpleStringT
		operator CThisSimpleString&()
		{
			return *this;
		}
		CStringT( const CThisSimpleString& strSrc ) : CThisSimpleString( strSrc )
		{
		}

		CStringT( PCXSTR pszSrc ) : CThisSimpleString( StringTraits::GetDefaultManager() )
		{
			// nDestLength is in XCHARs
			*this = pszSrc;
		}

		CStringT( PCXSTR pszSrc, ILgnBufferMgr* pStringMgr ) : CThisSimpleString( pStringMgr )
		{
			// nDestLength is in XCHARs
			*this = pszSrc;
		}

		explicit CStringT( PCYSTR pszSrc ) :  CThisSimpleString( StringTraits::GetDefaultManager() )
		{
			*this = pszSrc;
		}

		explicit CStringT( PCYSTR pszSrc, ILgnBufferMgr* pStringMgr ) : CThisSimpleString( pStringMgr )
		{
			*this = pszSrc;
		}


		explicit CStringT( CHAR ch, int nLength = 1 ) : CThisSimpleString( StringTraits::GetDefaultManager() )
		{
			LGNASSERT( nLength >= 0 );
			if( nLength > 0 )
			{
				PXSTR pszBuffer = CThisSimpleString::GetBuffer( nLength );
				StringTraits::FloodCharacters( XCHAR( ch ), nLength, pszBuffer );
				CThisSimpleString::ReleaseBufferSetLength( nLength );
			}
		}

		explicit CStringT( WCHAR ch, int nLength = 1 ) : CThisSimpleString( StringTraits::GetDefaultManager() )
		{
			LGNASSERT( nLength >= 0 );
			if( nLength > 0 )
			{
				PXSTR pszBuffer = CThisSimpleString::GetBuffer( nLength );
				StringTraits::FloodCharacters( XCHAR( ch ), nLength, pszBuffer );
				CThisSimpleString::ReleaseBufferSetLength( nLength );
			}
		}

		CStringT( PCXSTR pch, int nLength ) : CThisSimpleString( pch, nLength, StringTraits::GetDefaultManager() )
		{
		}

		CStringT( PCXSTR pch, int nLength, ILgnBufferMgr* pStringMgr ) :  CThisSimpleString( pch, nLength, pStringMgr )
		{
		}

		explicit CStringT( PCYSTR pch, int nLength ) : CThisSimpleString( StringTraits::GetDefaultManager() )
		{
			LGNASSERT( nLength >= 0 );
			if( nLength > 0 )
			{
				LGNASSERT( LgnIsValidAddress( pch, nLength*sizeof( YCHAR ), FALSE ) );
				if(pch == NULL)
					LgnThrow(E_INVALIDARG);

				int nDestLength = StringTraits::GetBaseTypeLength( pch, nLength );
				PXSTR pszBuffer = CThisSimpleString::GetBuffer( nDestLength );
				StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pch, nLength );
				CThisSimpleString::ReleaseBufferSetLength( nDestLength );
			}
		}

		explicit CStringT( PCYSTR pch, int nLength, ILgnBufferMgr* pStringMgr ) : CThisSimpleString( pStringMgr )
		{
			LGNASSERT( nLength >= 0 );
			if( nLength > 0 )
			{
				LGNASSERT( LgnIsValidAddress( pch, nLength*sizeof( YCHAR ), FALSE ) );
				if(pch == NULL)
					LgnThrow(E_INVALIDARG);

				int nDestLength = StringTraits::GetBaseTypeLength( pch, nLength );
				PXSTR pszBuffer = CThisSimpleString::GetBuffer( nDestLength );
				StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pch, nLength );
				CThisSimpleString::ReleaseBufferSetLength( nDestLength );
			}
		}

		// Destructor
		~CStringT() throw()
		{
		}

		// Assignment operators
		CStringT& operator=( const CStringT& strSrc )
		{
			CThisSimpleString::operator=( strSrc );

			return( *this );
		}

		CStringT& operator=( const CThisSimpleString& strSrc )
		{
			CThisSimpleString::operator=( strSrc );

			return( *this );
		}

		CStringT& operator=( PCXSTR pszSrc )
		{
			CThisSimpleString::operator=( pszSrc );

			return( *this );
		}

		CStringT& operator=( PCYSTR pszSrc )
		{
			// nDestLength is in XCHARs
			int nDestLength = (pszSrc != NULL) ? StringTraits::GetBaseTypeLength( pszSrc ) : 0;
			if( nDestLength > 0 )
			{
				PXSTR pszBuffer = CThisSimpleString::GetBuffer( nDestLength );
				StringTraits::ConvertToBaseType( pszBuffer, nDestLength, pszSrc );
				CThisSimpleString::ReleaseBufferSetLength( nDestLength );
			}
			else
			{
				CThisSimpleString::Empty();
			}

			return( *this );
		}

		CStringT& operator=( CHAR ch )
		{
			char ach[2] = { ch, 0 };

			return( operator=( ach ) );
		}

		CStringT& operator=( WCHAR ch )
		{
			wchar_t ach[2] = { ch, 0 };

			return( operator=( ach ) );
		}

		//CStringT& operator=( const VARIANT& var );

		CStringT& operator+=( const CStringT& str )
		{
			CThisSimpleString::Append( str );
			//CThisSimpleString::operator+=( str );

			return( *this );
		}

		CStringT& operator+=( const CThisSimpleString& str )
		{
			CThisSimpleString::Append( str );
			//CThisSimpleString::operator+=( str );

			return( *this );
		}

		CStringT& operator+=( PCXSTR pszSrc )
		{
			CThisSimpleString::Append( pszSrc );
			//CThisSimpleString::operator+=( pszSrc );

			return( *this );
		}
		template< int t_nSize >
			CStringT& operator+=( const LGN_SIMPLESTRING_NS::CStaticString< XCHAR, t_nSize >& strSrc )
		{
			CThisSimpleString::Append( strSrc );
			//CThisSimpleString::operator+=( strSrc );

			return( *this );
		}
		CStringT& operator+=( PCYSTR pszSrc )
		{
			CStringT str( pszSrc, CThisSimpleString::GetManager() );

			return( operator+=( str ) );
		}

		CStringT& operator+=( CHAR ch )
		{
			CThisSimpleString::AppendChar( (XCHAR)ch );
			//CThisSimpleString::operator+=( ch );

			return( *this );
		}

		CStringT& operator+=( WCHAR ch )
		{
			CThisSimpleString::AppendChar( (XCHAR)ch );
			//CThisSimpleString::operator+=( ch );

			return( *this );
		}

		//CStringT& operator+=( const VARIANT& var );

		// Comparison

		int Compare( PCXSTR psz ) const throw()
		{
			LGNASSERT( LgnIsValidString( psz ) );
			return( StringTraits::StringCompare( CThisSimpleString::GetString(), psz ) );
		}

		int CompareNoCase( PCXSTR psz ) const throw()
		{
			LGNASSERT( LgnIsValidString( psz ) );
			return( StringTraits::StringCompareIgnore( CThisSimpleString::GetString(), psz ) );
		}

		int Collate( PCXSTR psz ) const throw()
		{
			LGNASSERT( LgnIsValidString( psz ) );
			return( StringTraits::StringCollate( CThisSimpleString::GetString(), psz ) );
		}

		int CollateNoCase( PCXSTR psz ) const throw()
		{
			LGNASSERT( LgnIsValidString( psz ) );
			return( StringTraits::StringCollateIgnore( CThisSimpleString::GetString(), psz ) );
		}

		// Advanced manipulation

		// Delete 'nCount' characters, starting at index 'iIndex'
		int Delete( int iIndex, int nCount = 1 )
		{
			if( iIndex < 0 )
				iIndex = 0;

			if( nCount < 0 )
				nCount = 0;

			int nLength = CThisSimpleString::GetLength();
			if( (nCount+iIndex) > nLength )
			{
				nCount = nLength-iIndex;
			}
			if( nCount > 0 )
			{
				int nNewLength = nLength-nCount;
				int nXCHARsToCopy = nLength-(iIndex+nCount)+1;
				PXSTR pszBuffer = CThisSimpleString::GetBuffer();
				LGN::API::memmove( pszBuffer+iIndex, pszBuffer+iIndex+nCount, nXCHARsToCopy*sizeof( XCHAR ) );
				CThisSimpleString::ReleaseBufferSetLength( nNewLength );
			}

			return( CThisSimpleString::GetLength() );
		}

		// Insert character 'ch' before index 'iIndex'
		int Insert( int iIndex, XCHAR ch )
		{
			if( iIndex < 0 )
				iIndex = 0;

			if( iIndex > CThisSimpleString::GetLength() )
			{
				iIndex = CThisSimpleString::GetLength();
			}
			int nNewLength = CThisSimpleString::GetLength()+1;

			PXSTR pszBuffer = CThisSimpleString::GetBuffer( nNewLength );

			// move existing bytes down
			LGN::API::memmove( pszBuffer+iIndex+1, pszBuffer+iIndex, (nNewLength-iIndex)*sizeof( XCHAR ) );
			pszBuffer[iIndex] = ch;

			CThisSimpleString::ReleaseBufferSetLength( nNewLength );

			return( nNewLength );
		}

		// Insert string 'psz' before index 'iIndex'
		int Insert( int iIndex, PCXSTR psz )
		{
			if( iIndex < 0 )
				iIndex = 0;

			if( iIndex > CThisSimpleString::GetLength() )
			{
				iIndex = CThisSimpleString::GetLength();
			}

			// nInsertLength and nNewLength are in XCHARs
			int nInsertLength = StringTraits::SafeStringLen( psz );
			int nNewLength = CThisSimpleString::GetLength();
			if( nInsertLength > 0 )
			{
				nNewLength += nInsertLength;

				PXSTR pszBuffer = CThisSimpleString::GetBuffer( nNewLength );
				// move existing bytes down
				LGN::API::memmove( pszBuffer+iIndex+nInsertLength,
					pszBuffer+iIndex, (nNewLength-iIndex-nInsertLength+1)*sizeof( XCHAR ) );
				LGN::API::memcpy( pszBuffer+iIndex, psz, nInsertLength*sizeof( XCHAR ) );
				CThisSimpleString::ReleaseBufferSetLength( nNewLength );
			}

			return( nNewLength );
		}

		// Replace all occurrences of character 'chOld' with character 'chNew'
		int Replace( XCHAR chOld, XCHAR chNew )
		{
			int nCount = 0;

			// short-circuit the nop case
			if( chOld != chNew )
			{
				// otherwise modify each character that matches in the string
				bool bCopied = false;
				PXSTR pszBuffer = const_cast< PXSTR >( CThisSimpleString::GetString() );  // We don't actually write to pszBuffer until we've called GetBuffer().

				int nLength = CThisSimpleString::GetLength();
				int iChar = 0;
				while( iChar < nLength )
				{
					// replace instances of the specified character only
					if( pszBuffer[iChar] == chOld )
					{
						if( !bCopied )
						{
							bCopied = true;
							pszBuffer = CThisSimpleString::GetBuffer( nLength );
						}
						pszBuffer[iChar] = chNew;
						nCount++;
					}
					iChar = int( StringTraits::CharNext( pszBuffer+iChar )-pszBuffer );
				}
				if( bCopied )
				{
					CThisSimpleString::ReleaseBufferSetLength( nLength );
				}
			}

			return( nCount );
		}

		// Replace all occurrences of string 'pszOld' with string 'pszNew'
		int Replace( PCXSTR pszOld, PCXSTR pszNew )
		{
			// can't have empty or NULL lpszOld

			// nSourceLen is in XCHARs
			int nSourceLen = StringTraits::SafeStringLen( pszOld );
			if( nSourceLen == 0 )
				return( 0 );
			// nReplacementLen is in XCHARs
			int nReplacementLen = StringTraits::SafeStringLen( pszNew );

			// loop once to figure out the size of the result string
			int nCount = 0;
			{
				PCXSTR pszStart = CThisSimpleString::GetString();
				PCXSTR pszEnd = pszStart+CThisSimpleString::GetLength();
				while( pszStart < pszEnd )
				{
					PCXSTR pszTarget;
					while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL)
					{
						nCount++;
						pszStart = pszTarget+nSourceLen;
					}
					pszStart += StringTraits::SafeStringLen( pszStart )+1;
				}
			}

			// if any changes were made, make them
			if( nCount > 0 )
			{
				// if the buffer is too small, just
				//   allocate a new buffer (slow but sure)
				int nOldLength = CThisSimpleString::GetLength();
				int nNewLength = nOldLength+(nReplacementLen-nSourceLen)*nCount;

				PXSTR pszBuffer = CThisSimpleString::GetBuffer( max( nNewLength, nOldLength ) );

				PXSTR pszStart = pszBuffer;
				PXSTR pszEnd = pszStart+nOldLength;

				// loop again to actually do the work
				while( pszStart < pszEnd )
				{
					PXSTR pszTarget;
					while( (pszTarget = StringTraits::StringFindString( pszStart, pszOld ) ) != NULL )
					{
						int nBalance = nOldLength-int(pszTarget-pszBuffer+nSourceLen);
						LGN::API::memmove( pszTarget+nReplacementLen, pszTarget+nSourceLen, nBalance*sizeof( XCHAR ) );

						LGN::API::memcpy( pszTarget, pszNew, nReplacementLen*sizeof( XCHAR ) );
						pszStart = pszTarget+nReplacementLen;
						pszTarget[nReplacementLen+nBalance] = 0;
						nOldLength += (nReplacementLen-nSourceLen);
					}
					pszStart += StringTraits::SafeStringLen( pszStart )+1;
				}
				LGNASSERT( pszBuffer[nNewLength] == 0 );
				CThisSimpleString::ReleaseBufferSetLength( nNewLength );
			}

			return( nCount );
		}

		// Remove all occurrences of character 'chRemove'
		int Remove( XCHAR chRemove )
		{
			int nLength = CThisSimpleString::GetLength();
			PXSTR pszBuffer = CThisSimpleString::GetBuffer( nLength );

			PXSTR pszSource = pszBuffer;
			PXSTR pszDest = pszBuffer;
			PXSTR pszEnd = pszBuffer+nLength;

			while( pszSource < pszEnd )
			{
				PXSTR pszNewSource = StringTraits::CharNext( pszSource );
				if( *pszSource != chRemove )
				{
					// Copy the source to the destination.  Remember to copy all bytes of an MBCS character
					PXSTR pszNewDest = pszDest+(pszNewSource-pszSource);
					while( pszDest != pszNewDest )
					{
						*pszDest = *pszSource;
						pszSource++;
						pszDest++;
					}
				}
				pszSource = pszNewSource;
			}
			*pszDest = 0;
			int nCount = int( pszSource-pszDest );
			CThisSimpleString::ReleaseBufferSetLength( nLength-nCount );

			return( nCount );
		}

		CStringT Tokenize( PCXSTR pszTokens, int& iStart ) const
		{
			LGNASSERT( iStart >= 0 );

			if(iStart < 0)
				LgnThrow(E_INVALIDARG);			

			if( pszTokens == NULL )
			{
				return( *this );
			}

			PCXSTR pszPlace = CThisSimpleString::GetString()+iStart;
			PCXSTR pszEnd = CThisSimpleString::GetString()+CThisSimpleString::GetLength();
			if( pszPlace < pszEnd )
			{
				int nIncluding = StringTraits::StringSpanIncluding( pszPlace,
					pszTokens );

				if( (pszPlace+nIncluding) < pszEnd )
				{
					pszPlace += nIncluding;
					int nExcluding = StringTraits::StringSpanExcluding( pszPlace, pszTokens );

					int iFrom = iStart+nIncluding;
					int nUntil = nExcluding;
					iStart = iFrom+nUntil+1;

					return( Mid( iFrom, nUntil ) );
				}
			}

			// return empty string, done tokenizing
			iStart = -1;

#if (_MSC_VER >= 1000)
			return( CStringT( CThisSimpleString::GetManager() ) );
#else
			CStringT strResult( CThisSimpleString::GetManager() );
			return strResult;
#endif
		}

		// find routines

		// Find the first occurrence of character 'ch', starting at index 'iStart'
		int Find( XCHAR ch, int iStart = 0 ) const throw()
		{
			// iStart is in XCHARs
			LGNASSERT( iStart >= 0 );

			// nLength is in XCHARs
			int nLength = CThisSimpleString::GetLength();
			if( iStart < 0 || iStart >= nLength)
			{
				return( -1 );
			}

			// find first single character
			PCXSTR psz = StringTraits::StringFindChar( CThisSimpleString::GetString()+iStart, ch );

			// return -1 if not found and index otherwise
			return( (psz == NULL) ? -1 : int( psz-CThisSimpleString::GetString() ) );
		}

		// look for a specific sub-string

		// Find the first occurrence of string 'pszSub', starting at index 'iStart'
		int Find( PCXSTR pszSub, int iStart = 0 ) const throw()
		{
			// iStart is in XCHARs
			LGNASSERT( iStart >= 0 );
			LGNASSERT( LgnIsValidString( pszSub ) );

			if(pszSub == NULL)
			{
				return( -1 );
			}
			// nLength is in XCHARs
			int nLength = CThisSimpleString::GetLength();
			if( iStart < 0 || iStart > nLength )
			{
				return( -1 );
			}

			// find first matching substring
			PCXSTR psz = StringTraits::StringFindString( CThisSimpleString::GetString()+iStart, pszSub );

			// return -1 for not found, distance from beginning otherwise
			return( (psz == NULL) ? -1 : int( psz-CThisSimpleString::GetString() ) );
		}

		// Find the first occurrence of any of the characters in string 'pszCharSet'
		int FindOneOf( PCXSTR pszCharSet ) const throw()
		{
			LGNASSERT( LgnIsValidString( pszCharSet ) );
			PCXSTR psz = StringTraits::StringScanSet( CThisSimpleString::GetString(), pszCharSet );
			return( (psz == NULL) ? -1 : int( psz-CThisSimpleString::GetString() ) );
		}

		// Find the last occurrence of character 'ch'
		int ReverseFind( XCHAR ch ) const throw()
		{
			// find last single character
			PCXSTR psz = StringTraits::StringFindCharRev( CThisSimpleString::GetString(), ch );

			// return -1 if not found, distance from beginning otherwise
			return( (psz == NULL) ? -1 : int( psz-CThisSimpleString::GetString() ) );
		}

		// manipulation

		// Convert the string to uppercase
		CStringT& MakeUpper()
		{
			int nLength = CThisSimpleString::GetLength();
			PXSTR pszBuffer = CThisSimpleString::GetBuffer( nLength );
			StringTraits::StringUppercase( pszBuffer );
			CThisSimpleString::ReleaseBufferSetLength( nLength );

			return( *this );
		}

		// Convert the string to lowercase
		CStringT& MakeLower()
		{
			int nLength = CThisSimpleString::GetLength();
			PXSTR pszBuffer = CThisSimpleString::GetBuffer( nLength );
			StringTraits::StringLowercase( pszBuffer );
			CThisSimpleString::ReleaseBufferSetLength( nLength );

			return( *this );
		}

		// Reverse the string
		CStringT& MakeReverse()
		{
			int nLength = CThisSimpleString::GetLength();
			PXSTR pszBuffer = CThisSimpleString::GetBuffer( nLength );
			StringTraits::StringReverse( pszBuffer );
			CThisSimpleString::ReleaseBufferSetLength( nLength );

			return( *this );
		}

		// trimming

		// Remove all trailing whitespace
		CStringT& TrimRight()
		{
			// find beginning of trailing spaces by starting
			// at beginning (DBCS aware)

			PCXSTR psz = CThisSimpleString::GetString();
			PCXSTR pszLast = NULL;

			while( *psz != 0 )
			{
				if( StringTraits::IsSpace( *psz ) )
				{
					if( pszLast == NULL )
						pszLast = psz;
				}
				else
				{
					pszLast = NULL;
				}
				psz = StringTraits::CharNext( psz );
			}

			if( pszLast != NULL )
			{
				// truncate at trailing space start
				int iLast = int( pszLast-CThisSimpleString::GetString() );

				CThisSimpleString::Truncate( iLast );
			}

			return( *this );
		}

		// Remove all leading whitespace
		CStringT& TrimLeft()
		{
			// find first non-space character

			PCXSTR psz = CThisSimpleString::GetString();

			while( StringTraits::IsSpace( *psz ) )
			{
				psz = StringTraits::CharNext( psz );
			}

			if( psz != CThisSimpleString::GetString() )
			{
				// fix up data and length
				int iFirst = int( psz-CThisSimpleString::GetString() );
				PXSTR pszBuffer = CThisSimpleString::GetBuffer( CThisSimpleString::GetLength() );
				psz = pszBuffer+iFirst;
				int nDataLength = CThisSimpleString::GetLength()-iFirst;
				LGN::API::memmove( pszBuffer, psz, (nDataLength+1)*sizeof( XCHAR ) );
				CThisSimpleString::ReleaseBufferSetLength( nDataLength );
			}

			return( *this );
		}

		// Remove all leading and trailing whitespace
		CStringT& Trim()
		{
			return( TrimRight().TrimLeft() );
		}

		// Remove all leading and trailing occurrences of character 'chTarget'
		CStringT& Trim( XCHAR chTarget )
		{
			return( TrimRight( chTarget ).TrimLeft( chTarget ) );
		}

		// Remove all leading and trailing occurrences of any of the characters in the string 'pszTargets'
		CStringT& Trim( PCXSTR pszTargets )
		{
			return( TrimRight( pszTargets ).TrimLeft( pszTargets ) );
		}

		// trimming anything (either side)

		// Remove all trailing occurrences of character 'chTarget'
		CStringT& TrimRight( XCHAR chTarget )
		{
			// find beginning of trailing matches
			// by starting at beginning (DBCS aware)

			PCXSTR psz = CThisSimpleString::GetString();
			PCXSTR pszLast = NULL;

			while( *psz != 0 )
			{
				if( *psz == chTarget )
				{
					if( pszLast == NULL )
					{
						pszLast = psz;
					}
				}
				else
				{
					pszLast = NULL;
				}
				psz = StringTraits::CharNext( psz );
			}

			if( pszLast != NULL )
			{
				// truncate at left-most matching character  
				int iLast = int( pszLast-CThisSimpleString::GetString() );
				CThisSimpleString::Truncate( iLast );
			}

			return( *this );
		}

		// Remove all trailing occurrences of any of the characters in string 'pszTargets'
		CStringT& TrimRight( PCXSTR pszTargets )
		{
			// if we're not trimming anything, we're not doing any work
			if( (pszTargets == NULL) || (*pszTargets == 0) )
			{
				return( *this );
			}

			// find beginning of trailing matches
			// by starting at beginning (DBCS aware)

			PCXSTR psz = CThisSimpleString::GetString();
			PCXSTR pszLast = NULL;

			while( *psz != 0 )
			{
				if( StringTraits::StringFindChar( pszTargets, *psz ) != NULL )
				{
					if( pszLast == NULL )
					{
						pszLast = psz;
					}
				}
				else
				{
					pszLast = NULL;
				}
				psz = StringTraits::CharNext( psz );
			}

			if( pszLast != NULL )
			{
				// truncate at left-most matching character  
				int iLast = int( pszLast-CThisSimpleString::GetString() );
				CThisSimpleString::Truncate( iLast );
			}

			return( *this );
		}

		// Remove all leading occurrences of character 'chTarget'
		CStringT& TrimLeft( XCHAR chTarget )
		{
			// find first non-matching character
			PCXSTR psz = CThisSimpleString::GetString();

			while( chTarget == *psz )
			{
				psz = StringTraits::CharNext( psz );
			}

			if( psz != CThisSimpleString::GetString() )
			{
				// fix up data and length
				int iFirst = int( psz-CThisSimpleString::GetString() );
				PXSTR pszBuffer = CThisSimpleString::GetBuffer( CThisSimpleString::GetLength() );
				psz = pszBuffer+iFirst;
				int nDataLength = CThisSimpleString::GetLength()-iFirst;
				LGN::API::memmove( pszBuffer, psz, (nDataLength+1)*sizeof( XCHAR ) );
				CThisSimpleString::ReleaseBufferSetLength( nDataLength );
			}

			return( *this );
		}

		// Remove all leading occurrences of any of the characters in string 'pszTargets'
		CStringT& TrimLeft( PCXSTR pszTargets )
		{
			// if we're not trimming anything, we're not doing any work
			if( (pszTargets == NULL) || (*pszTargets == 0) )
			{
				return( *this );
			}

			PCXSTR psz = CThisSimpleString::GetString();
			while( (*psz != 0) && (StringTraits::StringFindChar( pszTargets, *psz ) != NULL) )
			{
				psz = StringTraits::CharNext( psz );
			}

			if( psz != CThisSimpleString::GetString() )
			{
				// fix up data and length
				int iFirst = int( psz-CThisSimpleString::GetString() );
				PXSTR pszBuffer = CThisSimpleString::GetBuffer( CThisSimpleString::GetLength() );
				psz = pszBuffer+iFirst;
				int nDataLength = CThisSimpleString::GetLength()-iFirst;
				LGN::API::memmove( pszBuffer, psz, (nDataLength+1)*sizeof( XCHAR ) );
				CThisSimpleString::ReleaseBufferSetLength( nDataLength );
			}

			return( *this );
		}

		//__if_exists( StringTraits::ConvertToOem )
		//{
		//	// Convert the string to the OEM character set
		//	void AnsiToOem()
		//	{
		//		int nLength = GetLength();
		//		PXSTR pszBuffer = GetBuffer( nLength );
		//		StringTraits::ConvertToOem( pszBuffer );
		//		ReleaseBufferSetLength( nLength );
		//	}
		//}

		//__if_exists( StringTraits::ConvertToAnsi )
		//{
		//	// Convert the string to the ANSI character set
		//	void OemToAnsi()
		//	{
		//		int nLength = GetLength();
		//		PXSTR pszBuffer = GetBuffer( nLength );
		//		StringTraits::ConvertToAnsi( pszBuffer );
		//		ReleaseBufferSetLength( nLength );
		//	}
		//}

		// Very simple sub-string extraction

		// Return the substring starting at index 'iFirst'
		CStringT Mid( int iFirst ) const
		{
			return( Mid( iFirst, CThisSimpleString::GetLength()-iFirst ) );
		}

		// Return the substring starting at index 'iFirst', with length 'nCount'
		CStringT Mid( int iFirst, int nCount ) const
		{
			// nCount is in XCHARs

			// out-of-bounds requests return sensible things
			if (iFirst < 0)
				iFirst = 0;
			if (nCount < 0)
				nCount = 0;

			if( (iFirst+nCount) > CThisSimpleString::GetLength() )
			{
				nCount = CThisSimpleString::GetLength()-iFirst;
			}
			if( iFirst > CThisSimpleString::GetLength() )
			{
				nCount = 0;
			}

			LGNASSERT( (nCount == 0) || ((iFirst+nCount) <= GetLength()) );

			// optimize case of returning entire string
			if( (iFirst == 0) && ((iFirst+nCount) == CThisSimpleString::GetLength()) )
			{
				return( *this );
			}

#if (_MSC_VER >= 1000)
			return( CStringT( CThisSimpleString::GetString()+iFirst, nCount, CThisSimpleString::GetManager() ) );
#else
			CStringT strResult( CThisSimpleString::GetString()+iFirst, nCount, CThisSimpleString::GetManager() );
			return strResult;
#endif
		}

		// Return the substring consisting of the rightmost 'nCount' characters
		CStringT Right( int nCount ) const
		{
			// nCount is in XCHARs
			if (nCount < 0)
				nCount = 0;

			int nLength = CThisSimpleString::GetLength();
			if( nCount >= nLength )
			{
				return( *this );
			}

#if (_MSC_VER >= 1000)
			return( CStringT( CThisSimpleString::GetString()+nLength-nCount, nCount, CThisSimpleString::GetManager() ) );
#else
			CStringT strResult( CThisSimpleString::GetString()+nLength-nCount, nCount, CThisSimpleString::GetManager() );
			return strResult;
#endif
		}

		// Return the substring consisting of the leftmost 'nCount' characters
		CStringT Left( int nCount ) const
		{
			// nCount is in XCHARs
			if (nCount < 0)
				nCount = 0;

			int nLength = CThisSimpleString::GetLength();
			if( nCount >= nLength )
			{
				return( *this );
			}

#if (_MSC_VER >= 1000)
			return( CStringT( CThisSimpleString::GetString(), nCount, CThisSimpleString::GetManager() ) );
#else
			CStringT strResult( CThisSimpleString::GetString(), nCount, CThisSimpleString::GetManager() );
			return strResult;
#endif
		}

		// Return the substring consisting of the leftmost characters in the set 'pszCharSet'
		CStringT SpanIncluding( PCXSTR pszCharSet ) const
		{
			LGNASSERT( LgnIsValidString( pszCharSet ) );
			if(pszCharSet == NULL)
				LgnThrow(E_INVALIDARG);

			return( Left( StringTraits::StringSpanIncluding( CThisSimpleString::GetString(), pszCharSet ) ) );
		}

		// Return the substring consisting of the leftmost characters not in the set 'pszCharSet'
		CStringT SpanExcluding( PCXSTR pszCharSet ) const
		{
			LGNASSERT( LgnIsValidString( pszCharSet ) );
			if(pszCharSet == NULL)
				LgnThrow(E_INVALIDARG);

			return( Left( StringTraits::StringSpanExcluding( CThisSimpleString::GetString(), pszCharSet ) ) );
		}

		// Format data using format string 'pszFormat'
		void LGN_CDECL Format( PCXSTR pszFormat, ... )
		{
			LGNASSERT( LgnIsValidString( pszFormat ) );

			va_list argList;
			va_start( argList, pszFormat );
			FormatV( pszFormat, argList );
			va_end( argList );
		}

		// Append formatted data using format string 'pszFormat'
		void LGN_CDECL AppendFormat( PCXSTR pszFormat, ... )
		{
			LGNASSERT( LgnIsValidString( pszFormat ) );

			va_list argList;
			va_start( argList, pszFormat );

			AppendFormatV( pszFormat, argList );

			va_end( argList );
		}

		void AppendFormatV( PCXSTR pszFormat, va_list args )
		{
			LGNASSERT( LgnIsValidString( pszFormat ) );

			int nCurrentLength = CThisSimpleString::GetLength();
			int nAppendLength = StringTraits::GetFormattedLength( pszFormat, args );
			PXSTR pszBuffer = CThisSimpleString::GetBuffer( nCurrentLength+nAppendLength );
			StringTraits::Format( pszBuffer+nCurrentLength, pszFormat, args );
			CThisSimpleString::ReleaseBufferSetLength( nCurrentLength+nAppendLength );
		}

		void FormatV( PCXSTR pszFormat, va_list args )
		{
			LGNASSERT( LgnIsValidString( pszFormat ) );
			if(pszFormat == NULL)
				LgnThrow(E_INVALIDARG);

			int nLength = StringTraits::GetFormattedLength( pszFormat, args );
			PXSTR pszBuffer = CThisSimpleString::GetBuffer( nLength );
			StringTraits::Format( pszBuffer, pszFormat, args );
			CThisSimpleString::ReleaseBufferSetLength( nLength );
		}


		// OLE BSTR support
#if (LGN_WINSYS)
		// Allocate a BSTR containing a copy of the string
		BSTR AllocSysString() const
		{
			BSTR bstrResult = StringTraits::AllocSysString( GetString(), 
				GetLength() );
			if( bstrResult == NULL )
			{
				ThrowMemoryException();
			}

			return( bstrResult );
		}

		BSTR SetSysString( BSTR* pbstr ) const
		{
			LGNASSERT( LgnIsValidAddress( pbstr, sizeof( BSTR ) ) );

			if( !StringTraits::ReAllocSysString( GetString(), pbstr,
				GetLength() ) )
			{
				ThrowMemoryException();
			}

			LGNASSERT( *pbstr != NULL );
			return( *pbstr );
		}

		// Set the string to the value of environment variable 'pszVar'
		BOOL GetEnvironmentVariable( PCXSTR pszVar )
		{
			ULONG nLength = StringTraits::GetEnvironmentVariable( pszVar, NULL, 0 );
			BOOL bRetVal = FALSE;

			if( nLength == 0 )
			{
				Empty();
			}
			else
			{
				PXSTR pszBuffer = GetBuffer( nLength );
				StringTraits::GetEnvironmentVariable( pszVar, pszBuffer, nLength );
				ReleaseBuffer();
				bRetVal = TRUE;
			}

			return( bRetVal );
		}
#endif // #if (LGN_WINSYS)

		friend CStringT operator+( const CStringT& str1, const CStringT& str2 )
		{
			CStringT strResult( str1.GetManager() );

			Concatenate( strResult, str1, str1.GetLength(), str2, str2.GetLength() );

			return( strResult );
		}

		friend CStringT operator+( const CStringT& str1, PCXSTR psz2 )
		{
			CStringT strResult( str1.GetManager() );

			Concatenate( strResult, str1, str1.GetLength(), psz2, StringLength( psz2 ) );

			return( strResult );
		}

		friend CStringT operator+( PCXSTR psz1, const CStringT& str2 )
		{
			CStringT strResult( str2.GetManager() );

			Concatenate( strResult, psz1, StringLength( psz1 ), str2, str2.GetLength() );

			return( strResult );
		}

		friend CStringT operator+( const CStringT& str1, WCHAR ch2 )
		{
			CStringT strResult( str1.GetManager() );
			XCHAR chTemp = XCHAR( ch2 );

			Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );

			return( strResult );
		}

		friend CStringT operator+( const CStringT& str1, CHAR ch2 )
		{
			CStringT strResult( str1.GetManager() );
			XCHAR chTemp = XCHAR( ch2 );

			Concatenate( strResult, str1, str1.GetLength(), &chTemp, 1 );

			return( strResult );
		}

		friend CStringT operator+( WCHAR ch1, const CStringT& str2 )
		{
			CStringT strResult( str2.GetManager() );
			XCHAR chTemp = XCHAR( ch1 );

			Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );

			return( strResult );
		}

		friend CStringT operator+( CHAR ch1, const CStringT& str2 )
		{
			CStringT strResult( str2.GetManager() );
			XCHAR chTemp = XCHAR( ch1 );

			Concatenate( strResult, &chTemp, 1, str2, str2.GetLength() );

			return( strResult );
		}

		friend bool operator==( const CStringT& str1, const CStringT& str2 ) throw()
		{
			return( str1.Compare( str2 ) == 0 );
		}

		friend bool operator==(const CStringT& str1, PCXSTR psz2 ) throw()
		{
			return( str1.Compare( psz2 ) == 0 );
		}

		friend bool operator==(PCXSTR psz1, const CStringT& str2 ) throw()
		{
			return( str2.Compare( psz1 ) == 0 );
		}

		friend bool operator==(const CStringT& str1, PCYSTR psz2 ) throw()
		{
			CStringT str2( psz2, str1.GetManager() );

			return( str1 == str2 );
		}

		friend bool operator==(PCYSTR psz1, const CStringT& str2 ) throw()
		{
			CStringT str1( psz1, str2.GetManager() );

			return( str1 == str2 );
		}

		friend bool operator!=(const CStringT& str1, const CStringT& str2 ) throw()
		{
			return( str1.Compare( str2 ) != 0 );
		}

		friend bool operator!=(const CStringT& str1, PCXSTR psz2 ) throw()
		{
			return( str1.Compare( psz2 ) != 0 );
		}

		friend bool operator!=(PCXSTR psz1, const CStringT& str2 ) throw()
		{
			return( str2.Compare( psz1 ) != 0 );
		}

		friend bool operator!=(const CStringT& str1, PCYSTR psz2 ) throw()
		{
			CStringT str2( psz2, str1.GetManager() );

			return( str1 != str2 );
		}

		friend bool operator!=(PCYSTR psz1, const CStringT& str2 ) throw()
		{
			CStringT str1( psz1, str2.GetManager() );

			return( str1 != str2 );
		}

		friend bool operator<( const CStringT& str1, const CStringT& str2 ) throw()
		{
			return( str1.Compare( str2 ) < 0 );
		}

		friend bool operator<( const CStringT& str1, PCXSTR psz2 ) throw()
		{
			return( str1.Compare( psz2 ) < 0 );
		}

		friend bool operator<( PCXSTR psz1, const CStringT& str2 ) throw()
		{
			return( str2.Compare( psz1 ) > 0 );
		}

		friend bool operator>( const CStringT& str1, const CStringT& str2 ) throw()
		{
			return( str1.Compare( str2 ) > 0 );
		}

		friend bool operator>( const CStringT& str1, PCXSTR psz2 ) throw()
		{
			return( str1.Compare( psz2 ) > 0 );
		}

		friend bool operator>( PCXSTR psz1, const CStringT& str2 ) throw()
		{
			return( str2.Compare( psz1 ) < 0 );
		}

		friend bool operator<=( const CStringT& str1, const CStringT& str2 ) throw()
		{
			return( str1.Compare( str2 ) <= 0 );
		}

		friend bool operator<=( const CStringT& str1, PCXSTR psz2 ) throw()
		{
			return( str1.Compare( psz2 ) <= 0 );
		}

		friend bool operator<=( PCXSTR psz1, const CStringT& str2 ) throw()
		{
			return( str2.Compare( psz1 ) >= 0 );
		}

		friend bool operator>=( const CStringT& str1, const CStringT& str2 ) throw()
		{
			return( str1.Compare( str2 ) >= 0 );
		}

		friend bool operator>=( const CStringT& str1, PCXSTR psz2 ) throw()
		{
			return( str1.Compare( psz2 ) >= 0 );
		}

		friend bool operator>=( PCXSTR psz1, const CStringT& str2 ) throw()
		{
			return( str2.Compare( psz1 ) <= 0 );
		}

		friend bool operator==( XCHAR ch1, const CStringT& str2 ) throw()
		{
			return( (str2.GetLength() == 1) && (str2[0] == ch1) );
		}

		friend bool operator==( const CStringT& str1, XCHAR ch2 ) throw()
		{
			return( (str1.GetLength() == 1) && (str1[0] == ch2) );
		}

		friend bool operator!=( XCHAR ch1, const CStringT& str2 ) throw()
		{
			return( (str2.GetLength() != 1) || (str2[0] != ch1) );
		}

		friend bool operator!=( const CStringT& str1, XCHAR ch2 ) throw()
		{
			return( (str1.GetLength() != 1) || (str1[0] != ch2) );
		}

	};
#endif // #ifdef __CSTRINGT_H__


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 扩展字符串处理模板类
	/*!	主要增加以下功能：字符串与数字之间的转换，字符串与十六进制串之间的转换，本类对象与缓冲区对象之间的转换，点击这里查看 \ref stringex_example_page
	*/
	template< typename BaseType, class StringTraits >
	class CStringExT : public LGN_CSTRING_NS::CStringT< BaseType, StringTraits >
	{
	public:
		typedef LGN_CSTRING_NS::CStringT< BaseType, StringTraits > CThisString;
		typedef LGN_SIMPLESTRING_NS::CSimpleStringT< BaseType > CThisSimpleString;
		//typedef CThisString::CThisSimpleString CThisSimpleString;
		typedef StringTraits StrTraits;
#if (_MSC_VER < 1000) // (!LGN_WINSYS)
		typedef typename ChTraitsBase< BaseType >::XCHAR XCHAR;
		typedef typename ChTraitsBase< BaseType >::PCXSTR PCXSTR;
		typedef typename ChTraitsBase< BaseType >::PCYSTR PCYSTR;
#endif

	public:
		CStringExT() throw() : CThisString()
		{
		}

		// Copy constructor
		CStringExT( const CStringExT& strSrc ) : CThisString( strSrc )
		{
		}

		CStringExT( const CThisString& strSrc ) : CThisString( strSrc )
		{
		}

		CStringExT( const CThisSimpleString& strSrc ) : CThisString( strSrc )
		{
		}

		//! 明确类型的构造，将缓冲区对象转换为本类对象
		explicit CStringExT( const CBufferT< BaseType > & bufSrc ) : CThisString()
		{
			CThisSimpleString::operator= (*((const CThisSimpleString *)(const CSimpleBufferT< BaseType > *)&bufSrc));
		}

		CStringExT( PCXSTR pszSrc ) //: CThisString(pszSrc)
		{
			// nDestLength is in XCHARs
			*this = pszSrc;
		}

		explicit CStringExT( PCYSTR pszSrc ) //: CThisString(pszSrc)
		{
			*this = pszSrc;
		}


		explicit CStringExT( CHAR ch, int nLength = 1 ) : CThisString( ch, nLength )
		{
		}

		explicit CStringExT( WCHAR ch, int nLength = 1 ) : CThisString( ch, nLength )
		{
		}

		CStringExT( PCXSTR pch, int nLength ) : CThisString( pch, nLength )
		{
		}

		CStringExT( PCYSTR pch, int nLength ) : CThisString( pch, nLength )
		{
		}

		// Destructor
		~CStringExT() throw()
		{
		}

		//! 将本类对象转换为缓冲区对象
		operator CBufferT< BaseType >()
		{
			return CBufferT< BaseType > (*((CSimpleBufferT< BaseType > *)(CThisSimpleString *)this));
		}

		// Assignment operators
		CStringExT& operator=( const CStringExT& strSrc )
		{
			CThisSimpleString::operator=( strSrc );

			return( *this );
		}

		CStringExT& operator=( const CThisString& strSrc )
		{
			CThisSimpleString::operator=( strSrc );

			return( *this );
		}

		CStringExT& operator=( const CThisSimpleString& strSrc )
		{
			CThisSimpleString::operator=( strSrc );

			return( *this );
		}

		//! 将缓冲区对象转换为本类对象
		CStringExT& operator=( const CBufferT< BaseType > & bufSrc )
		{
			CThisSimpleString::operator= (*((const CThisSimpleString *)(const CSimpleBufferT< BaseType > *)&bufSrc));

			return (*this);
		}

		CStringExT& operator=( PCXSTR pszSrc )
		{
			CThisSimpleString::operator=( pszSrc );

			return( *this );
		}

		CStringExT& operator=( PCYSTR pszSrc )
		{
			CThisString::operator=( pszSrc );

			return( *this );
		}

		CStringExT& operator=( CHAR ch )
		{
			char ach[2] = { ch, 0 };

			return( operator=( ach ) );
		}

		CStringExT& operator=( WCHAR ch )
		{
			wchar_t ach[2] = { ch, 0 };

			return( operator=( ach ) );
		}


		//// 以下操作符可以不重载
		//CStringExT& operator+=( const CThisSimpleString& str )
		//{
		//	CThisSimpleString::operator+=( str );

		//	return( *this );
		//}

		//CStringExT& operator+=( PCXSTR pszSrc )
		//{
		//	CThisSimpleString::operator+=( pszSrc );

		//	return( *this );
		//}
		//template< int t_nSize >
		//	CStringExT& operator+=( const LGN_SIMPLESTRING_NS::CStaticString< XCHAR, t_nSize >& strSrc )
		//{
		//	CThisSimpleString::operator+=( strSrc );

		//	return( *this );
		//}
		//CStringExT& operator+=( PCYSTR pszSrc )
		//{
		//	CThisString str( pszSrc, GetManager() );

		//	return( operator+=( str ) );
		//}

		//CStringExT& operator+=( CHAR ch )
		//{
		//	CThisSimpleString::operator+=( ch );

		//	return( *this );
		//}

		//CStringExT& operator+=( WCHAR ch )
		//{
		//	CThisSimpleString::operator+=( ch );

		//	return( *this );
		//}

		CStringExT Tokenize( PCXSTR pszTokens, int& iStart ) const
		{
			return CThisString::Tokenize(pszTokens, iStart);
		}

		CStringExT& MakeUpper()
		{
			CThisString::MakeUpper();
			return( *this );
		}
		CStringExT& MakeLower()
		{
			CThisString::MakeLower();
			return( *this );
		}

		CStringExT& MakeReverse()
		{
			CThisString::MakeReverse();
			return( *this );
		}

		CStringExT& TrimRight()
		{
			CThisString::TrimRight();
			return( *this );
		}
		CStringExT& TrimLeft()
		{
			CThisString::TrimLeft();
			return( *this );
		}
		CStringExT& Trim()
		{
			CThisString::Trim();
			return( *this );
		}
		CStringExT& Trim( XCHAR chTarget )
		{
			CThisString::Trim(chTarget);
			return( *this );
		}
		CStringExT& Trim( PCXSTR pszTargets )
		{
			CThisString::Trim(pszTargets);
			return( *this );
		}
		CStringExT& TrimRight( XCHAR chTarget )
		{
			CThisString::TrimRight(chTarget);
			return( *this );
		}
		CStringExT& TrimRight( PCXSTR pszTargets )
		{
			CThisString::TrimRight(pszTargets);
			return( *this );
		}
		CStringExT& TrimLeft( XCHAR chTarget )
		{
			CThisString::TrimLeft(chTarget);
			return( *this );
		}
		CStringExT& TrimLeft( PCXSTR pszTargets )
		{
			CThisString::TrimLeft(pszTargets);
			return( *this );
		}

		CStringExT Mid( int iFirst ) const
		{
			return CThisString::Mid(iFirst);
		}
		CStringExT Mid( int iFirst, int nCount ) const
		{
			return CThisString::Mid(iFirst, nCount);
		}
		CStringExT Right( int nCount ) const
		{
			return CThisString::Right(nCount);
		}
		CStringExT Left( int nCount ) const
		{
			return CThisString::Left(nCount);
		}

		CStringExT SpanIncluding( PCXSTR pszCharSet ) const
		{
			return CThisString::SpanIncluding(pszCharSet);
		}
		CStringExT SpanExcluding( PCXSTR pszCharSet ) const
		{
			return CThisString::SpanExcluding(pszCharSet);
		}

		// data convert

		//! 模板函数，返回字符串的整数值
		/*! \param[in] base = 10 进制，当小于0时自识别
		    \param[out] pnResult = NULL 输出的整数值，用于模板匹配
			\return 整数值，可以为负值 */
		template< typename _IntType >
		_IntType ToIntT(int base = 10, _IntType * pnResult=NULL) const
		{
			PCXSTR pszString = CThisString::GetString();
			if (base < 0)
			{
				base = 10;
				if ((pszString[0] == '0') && ((pszString[1] == 'x')  || (pszString[1] == 'X')))
				{
					base = 16;
					pszString += 2;
				}
			}
#if (LGN_HI_TEMPLATE)
			return StrTraits::StringToIntT< _IntType >(pszString, base, pnResult);
#else				
			return StrTraits::StringToIntT(pszString, base, pnResult);
#endif			
		}
		//! 返回字符串的整数值
		/*! \param[in] base = 10 进制，当小于0时自识别
			\return 整数值，可以为负值
			\see ToIntT */
		int ToInt(int base = 10) const
		{
#if (LGN_HI_TEMPLATE)
			return ToIntT< int >(base);
#else				
			return ToIntT(base, (int *)NULL);
#endif			
		}

		//! 模板函数，把整数转换为字符串
		/*! \param[in] n 整数值，可以为负值
			\param[in] length = 0 字符串存储长度，不足前补0
			\param[in] base = 10 进制[2-36]
			\return 本对象引用 */
		template< typename _IntType >
		CStringExT& FromIntT(_IntType n, int length = 0, int base = 10)
		{
#if (LGN_HI_TEMPLATE)
			int nNewLength = StrTraits::StringFromIntT< _IntType >(GetBuffer(130), n, length, base);
#else				
			int nNewLength = StrTraits::StringFromIntT(CThisString::GetBuffer(130), n, length, base);
#endif				
			CThisSimpleString::ReleaseBuffer(nNewLength);

			return ( *this );
		}
		//! 把整数转换为字符串
		/*! \param[in] n 整数值，可以为负值
			\param[in] length = 0 字符串存储长度，不足前补0
			\param[in] base = 10 进制[2-36]
			\return 本对象引用
			\see FromIntT */
		CStringExT& FromInt(int n, int length = 0, int base = 10)
		{
#if (LGN_HI_TEMPLATE)
			return FromIntT< int >(n, length, base);
#else
			return FromIntT(n, length, base);
#endif			
		}

		//! 把其字符串转换为十六进制串
		/*! \param[in] chRepace = -1 当字符不是有效的十六进制数，如果有替换字符则继续，否则抛异常
			\return 存放十六进制串的缓冲区对象 */
		CByteBuffer ToHexs(int chRepace = -1) const throw()
		{
			int nLength = CThisSimpleString::GetLength();
			CByteBuffer buffRet(CThisSimpleString::GetManager());
			if (StrTraits::StringToHexs(buffRet.GetBufferSetLength((nLength + 1) >> 1), CThisSimpleString::GetString(), nLength, chRepace) < 0)
				LgnThrow(E_LGN_INVALID_DATA);

			return buffRet;
		}

		//! 把十六进制串转换为字符串，替换本字符串对象
		/*! \param[in] buffHexs 十六进制串的缓冲区对象
			\return 本对象引用 */
		CStringExT& FromHexs(const CByteBuffer & buffHexs)
		{
			return FromHexs(buffHexs.GetCBuffer(), buffHexs.GetLength());
		}
		//! 把十六进制串转换为字符串，替换本字符串对象
		/*! \param[in] pbBuffer 十六进制串的指针
			\param[in] nLength 十六进制串\a pbBuffer的长度
			\return 本对象引用 */
		CStringExT& FromHexs(const BYTE * pbBuffer, int nLength)
		{
			StrTraits::StringFromHexs(CThisSimpleString::GetBufferSetLength( nLength << 1 ), pbBuffer, nLength);

			return ( *this );
		}
		//! 把十六进制串转换为字符串，添加到本字符串对象后
		/*! \param[in] pbBuffer 十六进制串的指针
			\param[in] nLength 十六进制串\a pbBuffer的长度
			\return 本对象引用 */
		CStringExT& AppendFromHexs(const BYTE * pbBuffer, int nLength)
		{
			int nOldLength = CThisSimpleString::GetLength();
			StrTraits::StringFromHexs(CThisSimpleString::GetBufferSetLength( nOldLength + (nLength << 1) ) + nOldLength, pbBuffer, nLength);

			return ( *this );
		}

	};

/*! \page stringex_example_page CStringExT例子
<pre>	%CStringExT模板类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// ...
	// 字符串对象与缓冲区对象之间的转换
	LGN::CStrBuffer str1("1234", 4);
	LGN::CString str2(str1);
	LGN::CStrBuffer str3 = str2;
	str2 = str1;

	// 字符串与数字之间的转换
	int n1 = str2.ToInt(); // n1 = 1234
	short n2 = str2.ToIntT<short>(16); // n2 = 0x1234
	str2.FromInt(1234); // out = 001234 
	str2.FromInt(0x1234, 6, 16); // out = 001234 

	// 字符串与十六进制串之间的转换
	LGN::CByteBuffer buff1 = str2.ToHexs(); // out = \\x00\\x12\\x34
	str2.FromHexs((LPBYTE)"1234", 4); // out = 31323334
	str2.AppendFromHexs((LPBYTE)"\x56\x78", 2); // out = 313233345678

	// 直接调用静态模板类成员函数 LGN::CString::StrTraits == LGN::StrTraitLGN\<TCHAR\>
	int n3 = LGN::CString::StrTraits::StringToIntT(TEXT("1234"), 16, (int *)NULL); // n3 = 0x1234
</pre>
*/


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 字符串处理静态模板类
	/*!	继承于LGN::ChTraitsEx，主要增加以下功能：字符串与数字之间的转换，字符串与十六进制串之间的转换，尽量用<B>CStringExT<_BaseType>::StrTraits</B>来调用本类功能
	*/
	template< typename _BaseType = CHAR, class StringIterator = ChTraitsEx< _BaseType > >
	class StrTraitLGN : public StringIterator
	{
	public:
#if (LGN_WINSYS)
		//! 获取资源句柄（WIN）
		static HINSTANCE FindStringResourceInstance(UINT nID) throw()
		{
			return( LgnFindStringResourceInstance( nID ) );
		}
#endif

		//! 返回默认内存管理对象的指针
		static ILgnBufferMgr * GetDefaultManager() throw()
		{
			return _lgnBufferManager;
		}

		//! 返回一个字符的进制数，不在36进制内返回-1
		static int CharToInt(int ch)
		{
			if (ch >= '0' && ch <= '9')
				return ch - '0';
			else if (ch >= 'A' && ch <= 'Z')
				return ch - 'A' + 10;
			else if (ch >= 'a' && ch <= 'z')
				return ch - 'a' + 10;
			else
				return -1;
		}
		//! 返回进制数的字符，不在36内返回0
		static int CharFromInt(int ch)
		{
			if (ch >=0 && ch <= 9)
				return ch + '0';
			else if (ch >= 10 && ch<=36)
				return ch - 10 + 'A';
			else
				return 0;
		}

		//! 模板函数，返回字符串的整数值
		/*! \param[in] pszSrc 字符串的指针
			\param[in] base = 10 进制，当小于0时自识别
		    \param[out] pnResult = NULL 输出的整数值，用于模板匹配
			\return 整数值，可以为负值
			\note  为了兼容VC6，unix下的模板函数调用；必须在有其对应的参数
		*/
		template< typename _IntType >
		static _IntType StringToIntT(const _BaseType * pszSrc, int base = 10, _IntType * pnResult = NULL)
		{
			LGNASSERT(base>1 && base<=36);

			bool bNegative =  (*pszSrc == '-')? (pszSrc ++, true) : (false);
			_IntType n = 0;
			for (; ;)
			{
				int offset = CharToInt(*(pszSrc++));
				if (offset < 0 || !(offset <  base))
					break;
				n = n * base + offset;
			}

			if (pnResult != NULL)
				*pnResult = (bNegative)? (0-n) : (n);
			return (bNegative)? (0-n) : (n);
		}
		//! 模板函数，把整数转换为字符串
		/*! \param[out] pszDest 输出字符串的缓冲区
			\param[in] n 整数值，可以为负值
			\param[in] length 字符串存储长度，不足前补0
			\param[in] base = 10 进制[2-36]
			\return 字符串\a pszDest的长度 */
		template< typename _IntType >
		static int StringFromIntT(_BaseType * pszDest, _IntType n, int length, int base = 10)
		{
			LGNASSERT(length < 130 && base>1 && base<=36);
			if (n < 0)
			{
				*(pszDest++) = '-';
				n = 0-n;
			}
			_BaseType * pszStart = pszDest;
			do{
				*(pszDest++) = (_BaseType)CharFromInt( static_cast<int>(n % base) );
				n = n / base;
			}while(n);
			
			if (length <= 0)
				length = (int)(pszDest - pszStart);
			else
			{
				for (; pszDest < (pszStart + length); )
					*(pszDest++) = '0';
			}

			// StringReverse
			for (_BaseType * p= pszStart + length - 1, *q=pszStart; q < p; q++,p--)
			{
				_BaseType t = *q;
				*q = *p;
				*p = t;
			}
			return ( length );
		}

		//! 把其字符串转换为十六进制串
		/*! \param[out] pbBytes 十六进制串的指针
			\param[in] pszSrc 字符串的指针
			\param[in] nLength 字符串\a pszSrc的长度
			\param[in] chRepace = -1 当字符不是有效的十六进制数，如果有替换字符则继续，否则退出
			\return 输出十六进制串\a pbBytes的长度，-1表示转换失败 */
		static int StringToHexs(LPBYTE pbBytes, const _BaseType * pszSrc, int nLength, int chRepace=-1)
		{
			if (pbBytes == NULL)
				return ((nLength + 1) >> 1);

			for (int i=0; i<nLength; i++)
			{
				int c = CharToInt(pszSrc[i]);
				if (c < 0 || c > 16)
				{
					if (chRepace == -1)
					{
						return -1;
						//break;
					}
					c = chRepace; // LgnThrow( API::HResultFromError(ERROR_INVALID_DATA) );
				}
				if (i & 1)
				{
					*pbBytes += (BYTE)c;
					pbBytes ++;
				}
				else
				{
					*pbBytes = (BYTE)(c << 4);
				}
			}
			return ((nLength + 1) >> 1);
		}
		//! 把十六进制串转换为字符串
		/*! \param[in] pszDest 字符串的指针
			\param[in] pbBuffer 十六进制串的指针
			\param[in] nLength 十六进制串\a pbBuffer的长度
			\return 输出字符串\a pszDest的长度 */
		static int StringFromHexs(_BaseType * pszDest, const BYTE * pbBuffer, int nLength)
		{
			if (pszDest == NULL)
				return nLength << 1;

			for (int i=0; i<(nLength<<1); i++)
			{
				*(pszDest++) = (_BaseType)CharFromInt( (i&1)? ((pbBuffer[i>>1]) & 0x0F) : ((pbBuffer[i>>1]) >> 4) );
			}
			return ( nLength<<1 );
		}
	};


	typedef LGN_CSTRING_NS::CStringT< WCHAR, StrTraitLGN< WCHAR > > CLgnStringW;
	typedef LGN_CSTRING_NS::CStringT< CHAR, StrTraitLGN< CHAR > > CLgnStringA;
	typedef LGN_CSTRING_NS::CStringT< TCHAR, StrTraitLGN< TCHAR > > CLgnString;

	typedef CStringExT< WCHAR, StrTraitLGN< WCHAR > > CStringW;
	typedef CStringExT< CHAR, StrTraitLGN< CHAR > > CStringA;
	typedef CStringExT< TCHAR, StrTraitLGN< TCHAR > > CString;

}; // namespace LGN

//! 把CStringW对象转成CSimpleStringW对象
#define CString2CSimpleStringW(str) ((LGN::CStringW::CThisSimpleString)(str))
//! 把CStringA对象转成CSimpleStringA对象
#define CString2CSimpleStringA(str) ((LGN::CStringA::CThisSimpleString)(str))
/*! 把CString对象转成CSimpleString对象，只有用于ATL、MFC的CString与LGN的CString转换
    LGN::CString str; CString str2(CString2CSimpleString(str));
	CString str; LGN::CString str2(CString2CSimpleString(str));
*/
#define CString2CSimpleString(str) ((LGN::CString::CThisSimpleString)(str))

#endif
