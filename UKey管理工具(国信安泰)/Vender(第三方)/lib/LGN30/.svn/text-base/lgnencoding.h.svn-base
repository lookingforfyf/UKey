/*! \file lgnencoding.h 
*  \brief 编码转换类
*
 *  被lgnbase.h包含，工程不用直接包含本文件
*
*  声明编码基础静态类EncodingBase，编码转换静态模板类EncodingTraits
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGN_ENCODING_H__
#define __LGN_ENCODING_H__


#if (LGN_WINSYS)
#include "win\lgnwin.encoding.h"
#else
#include "unix/lgnunix.encoding.h"
#endif

#ifndef __LGN_BASE_H__
#error lgnencoding.h requires include lgnbase.h
#endif


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 编码基础静态类
	/*! 编码类型定义，编码类型的提取及编码之间的转换。 
	 * -# 点击这里查看 \ref encodingbase_example_page	
	*/
	class EncodingBase : public EncodingBaseOS
	{
	public:
		//! 编码类型
		enum EncodingType {
			eUnknown = -1,		//!< 未知编码类型
			eDefault = 0,		//!< 系统默认的非UNICODE字符集编码
			eUtf8,				//!< UNICODE编码，以UTF-8存储
			eUtf16BigEndian,	//!< UNICODE编码，以UTF-16存储（低地址存放最高有效字节）
			eUtf16LittleEndian, //!< UNICODE编码，以UTF-16存储（低地址存放最低有效字节） // Default on Windows
			eUtf8Plain			//!< UNICODE编码，以UTF-8存储（没有编码前缀，所有数据符合UTF-8编码）
		};

		// static function
	public:
		/*! \brief 根据二进制数据的前几个字节获取其编码类型
			\param [in] pbBuff 二进制数据存放的指针
			\param [in] dwLength  二进制数据\a pbBuff 的长度
			\param [out] dwOffset  编码类型占有的字节数
			\return 编码类型EncodingType；如果没有符合的编码前缀，则返回eDefault
			\note 该函数不判断编码类型eUtf8Plain，可以单独执行IsUTF8_16函数来判断
		*/
		static EncodingType GetEncoding(LPCBYTE pbBuff, UINT32 dwLength, UINT32 & dwOffset)
		{
			if (pbBuff[0] == 0xFF && pbBuff[1] == 0xFE)
			{
				dwOffset = 2;
				return eUtf16LittleEndian;
			}
			else if (pbBuff[0] == 0xFE && pbBuff[1] == 0xFF)
			{
				dwOffset = 2;
				return eUtf16BigEndian;
			}
			else if (pbBuff[0] == 0xEF && pbBuff[1] == 0xBB && pbBuff[2] == 0xBF)
			{
				dwOffset = 3;
				return eUtf8;
			}
			//else if (IsUTF8_16((LPBYTE)buff.GetCBuffer(), buff.GetLength()))
			//{
			//	dwOffset = 0;
			//	return eUtf8Plain;
			//}
			else
			{
				dwOffset = 0;
				return eDefault;
			}
		}

		/*! \brief 获取编码类型，调用GetEncoding(pbBuff, dwLength, dwOffset)
			\param [in] buff 二进制数据存放的对象
			\param [out] dwOffset  编码类型占有的字节数
			\return 编码类型EncodingType
		*/
		static EncodingType GetEncoding(const CByteBuffer & buff, UINT32 & dwOffset)
		{
			return GetEncoding(buff.GetCBuffer(), buff.GetLength(), dwOffset);
		}

		/*! \brief 获取编码类型，调用GetEncoding(pbBuff, dwLength, dwOffset)
			\param [in,out] buff 二进制数据存放的对象，输出时删除编码前缀
			\return 编码类型EncodingType
		*/
		static EncodingType GetEncoding(CByteBuffer & buff)
		{
			UINT32 dwOffset = 0;
			EncodingType eEncoding = GetEncoding(buff.GetCBuffer(), buff.GetLength(), dwOffset);
			if (dwOffset > 0)
				buff.Delete(0, dwOffset);
			return eEncoding;
		}

		/*! \brief 获取编码前缀
			\param [in] eEncoding 编码类型
			\return 编码前缀；如果没有编码前缀，则返回一个空
		*/
		static CByteBuffer GetEncodingFeature(EncodingType eEncoding)
		{
			CByteBuffer buffFeature;
			if (eEncoding == eUtf16LittleEndian)
				buffFeature.Append((LPBYTE)"\xFF\xFE", 2);
			else if (eEncoding == eUtf16BigEndian)
				buffFeature.Append((LPBYTE)"\xFE\xFF", 2);
			else if (eEncoding == eUtf8)
				buffFeature.Append((LPBYTE)"\xEF\xBB\xBF", 3);

			return buffFeature;
		}

		/*! \brief 判断一个二进制串是否为UFT-8编码
			\param [in] pbBuffer 存放二进制串的指针
			\param [in] nLength 二进制串\a pbBuffer 的长度
			\return true 或 flase
		*/
		static bool IsUTF8_16(LPBYTE pbBuffer, int nLength)
		{
			bool rv = true;
			bool bASCII7only = true;
			LPBYTE sx = pbBuffer;
			LPBYTE endx = pbBuffer + nLength;

			while (sx<endx)
			{
				if (!*sx)
				{											// For detection, we'll say that NUL means not UTF8
					bASCII7only = false;
					rv = false;
					break;
				} 
				else if (*sx < 0x80)
				{			// 0nnnnnnn If the byte's first hex code begins with 0-7, it is an ASCII character.
					sx ++;
				} 
				else if (*sx < (0x80 + 0x40)) 
				{											  // 10nnnnnn 8 through B cannot be first hex codes
					bASCII7only = false;
					rv = false;
					break;
				} 
				else if (*sx < (0x80 + 0x40 + 0x20))
				{					  // 110xxxvv 10nnnnnn  If it begins with C or D, it is an 11 bit character
					bASCII7only = false;
					if (sx >= endx-1) 
						break;
					if (!(*sx & 0x1F) || (sx[1]&(0x80+0x40)) != 0x80) {
						rv = false; 
						break;
					}
					sx += 2;
				} 
				else if (*sx < (0x80 + 0x40 + 0x20 + 0x10))
				{								// 1110qqqq 10xxxxvv 10nnnnnn If it begins with E, it is 16 bit
					bASCII7only = false;
					if (sx >= endx-2) 
						break;
					if (!(*sx & 0xF) || (sx[1]&(0x80+0x40)) != 0x80 || (sx[2]&(0x80+0x40)) != 0x80) {
						rv = false; 
						break;
					}
					sx += 3;
				} 
				else 
				{													  // more than 16 bits are not allowed here
					bASCII7only = 0;
					rv=0;
					break;
				}
			}
			return (bASCII7only? false : rv);
		}
	}; // static class CharsetBase

/*! \page encodingbase_example_page EncodingBase例子
<pre> 读取文件的所有数据，并转换为当前编码的字符串</pre>
<pre>
\#include "lgnbase.h"

	//...
	LGN::CByteBuffer fileData;
	if (LGN::File::GetAllData(TEXT("1.txt"), fileData))
	{
		LGN::EncodingBase::EncodingType eEncoding = LGN::EncodingBase::GetEncoding(fileData);
\#ifdef UNICODE
		LGN::CStrBuffer strFileText = LGN::EncodingBase::Bytes2WideChars(fileData, eEncoding);
\#else
		LGN::CStrBuffer strFileText = LGN::EncodingBase::Bytes2AnsiChars(fileData, eEncoding);
\#endif
	}
</pre>
*/



////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 编码转换静态模板类
	/*! 继承于EncodingBase，主要实现系统默认非UNICODE编码的字符串与指定编码类型的数据进行转换。 
	 * -# 点击这里查看 \ref encodingtraits_example_page	
	*/
	template< typename BaseType = CHAR >
	class EncodingTraits : public EncodingBase
	{
	public:

		/*! \brief 把系统默认非UNICODE编码的字符串转换为指定编码的数据
			\param [in] buffSrc 系统默认非UNICODE编码的字符串对象
			\param [in] eEncoding 返回数据的编码类型
			\return 指定编码的数据
		*/
		static CByteBuffer GetBytes(const CAnsiBuffer & buffSrc, EncodingType eEncoding)
		{
			CByteBuffer buffRet( *((CByteBuffer *)&buffSrc) );
			switch (eEncoding)
			{
			case eUtf8:
				EncodingBase::CodeVonvert(CP_ACP, CP_UTF8, buffRet);
				break;
			case eUtf16BigEndian:
				EncodingBase::CodeVonvert(CP_ACP, CP_UFT16BE, buffRet);
				break;
			case eUtf16LittleEndian:
				EncodingBase::CodeVonvert(CP_ACP, CP_UFT16LE, buffRet);
				break;

			default:
				break;
			}
			return buffRet;
		}


		/*! \brief 把指定编码的数据转换为系统默认非UNICODE编码的字符串
			\param [in] buffSrc 指定编码的数据
			\param [in] eEncoding 数据\a buffSrc 的编码类型
			\return 系统默认非UNICODE编码的字符串对象
		*/
		static CAnsiBuffer GetString(const CByteBuffer & buffSrc, EncodingType eEncoding)
		{
			CByteBuffer buffRet(buffSrc);			
			switch (eEncoding)
			{
			case eUtf8:
				CodeVonvert(CP_UTF8, CP_ACP, buffRet);
				break;
			case eUtf16BigEndian:
				CodeVonvert(CP_UFT16BE, CP_ACP, buffRet);
				break;
			case eUtf16LittleEndian:
				CodeVonvert(CP_UFT16LE, CP_ACP, buffRet);
				break;

			default:
				break;
			}
			return CAnsiBuffer( *((CAnsiBuffer *)&buffRet) );;
		}

		/*! \brief 把带编码前缀的数据转换为系统默认非UNICODE编码的字符串
			\param [in] buffSrc 带编码前缀的数据
			\return 系统默认非UNICODE编码的字符串对象
		*/
		static CAnsiBuffer GetString(const CByteBuffer & buffSrc)
		{
			CByteBuffer buffTmp = buffSrc;
			EncodingType eEncoding = EncodingBase::GetEncoding(buffTmp);
			return GetString(buffTmp, eEncoding);
		}
	};

	//! 编码转换静态模板类
	/*! 继承于EncodingBase，主要实现UNICODE编码的字符串与指定编码类型的数据进行转换。 
	 * -# 点击这里查看 \ref encodingtraits_example_page	
	*/
	template<>
	class EncodingTraits< WCHAR > : public EncodingBase
	{
	public:
		/*! \brief 把UNICODE编码的字符串转换为指定编码的数据
			\param [in] buffSrc UNICODE编码的字符串对象
			\param [in] eEncoding 返回数据的编码类型
			\return 指定编码的数据
		*/
		static CByteBuffer GetBytes(const CWideBuffer & buffSrc, EncodingType eEncoding)
		{
			CByteBuffer buffRet((LPBYTE)buffSrc.GetCBuffer(), buffSrc.GetLength() << 1, buffSrc.GetManager() );
			switch (eEncoding)
			{
			case eUtf8:
				CodeVonvert(CP_UFT16LE, CP_UTF8, buffRet);
				break;
			case eUtf16BigEndian:
				CodeVonvert(CP_UFT16LE, CP_UFT16BE, buffRet);
				break;
			case eUtf16LittleEndian:
				//CodeVonvert(CP_UFT16LE, CP_UFT16LE, buffRet);
				break;

			default:
				CodeVonvert(CP_UFT16LE, CP_ACP, buffRet);
				break;
			}
			return buffRet;
		}

		/*! \brief 把指定编码的数据转换为UNICODE编码的字符串
			\param [in] buffSrc 指定编码的数据
			\param [in] eEncoding 数据\a buffSrc 的编码类型
			\return UNICODE编码的字符串对象
		*/
		static CWideBuffer GetString(const CByteBuffer & buffSrc, EncodingType eEncoding)
		{
			CByteBuffer buffRet( buffSrc );			
			switch (eEncoding)
			{
			case eUtf8:
				CodeVonvert(CP_UTF8, CP_UFT16LE, buffRet);
				break;
			case eUtf16BigEndian:
				CodeVonvert(CP_UFT16BE, CP_UFT16LE, buffRet);
				break;
			case eUtf16LittleEndian:
				//CodeVonvert(CP_UFT16LE, CP_UFT16LE, buffRet);
				break;

			default:
				CodeVonvert(CP_ACP, CP_UFT16LE, buffRet);
				break;
			}
			return CWideBuffer((LPCWSTR)buffRet.GetCBuffer(), buffRet.GetLength() >> 1, buffSrc.GetManager() );;
		}

		/*! \brief 把带编码前缀的数据转换为UNICODE编码的字符串
			\param [in] buffSrc 带编码前缀的数据
			\return UNICODE编码的字符串对象
		*/
		static CWideBuffer GetString(const CByteBuffer & buffSrc)
		{
			CByteBuffer buffTmp = buffSrc;
			EncodingType eEncoding = EncodingBase::GetEncoding(buffTmp);
			return GetString(buffTmp, eEncoding);
		}
	};

	typedef EncodingTraits< CHAR > EncodingA;	//!< 类型重定义 LGN::EncodingTraits
	typedef EncodingTraits< WCHAR > EncodingW;	//!< 类型重定义 LGN::EncodingTraits
	typedef EncodingTraits< TCHAR > Encoding;	//!< 类型重定义 LGN::EncodingTraits

/*! \page encodingtraits_example_page EncodingTraits例子
<pre> 读取文件的所有数据，并转换为当前编码的字符串</pre>
<pre>
\#include "lgnbase.h"

	//...
	LGN::CByteBuffer fileData;
	if (LGN::File::GetAllData(TEXT("1.txt"), fileData))
	{
		LGN::CString strText = LGN::Encoding::GetString(fileData);
	}
</pre>
*/

}; // namespace LGN

#endif // #ifndef __LGN_ENCODING_H__
