/*! \file lgnwin.encoding.h
*  \brief WIN系统字符集编码转换
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  提供字符集编码转换，依赖操作系统
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/


#ifndef __LGNWIN_ENCODING_H__
#define __LGNWIN_ENCODING_H__


#define CP_UFT16LE 0x80000000
#define CP_UFT16BE 0x80000001

namespace LGN
{

	class EncodingBaseOS
	{
	public:
		static int CodeVonvert(UINT nFromCodePage, UINT nToCodePage, CByteBuffer & buffData)
		{
			if (nFromCodePage == nToCodePage)
			{
			}				
			else if ((nFromCodePage == CP_UFT16LE && nToCodePage == CP_UFT16BE) || (nFromCodePage == CP_UFT16BE && nToCodePage == CP_UFT16LE))
			{
				CByteBuffer::ReverseBaseTypes(buffData.GetBuffer(), buffData.GetLength());
			}		
			else if (nFromCodePage == CP_UFT16LE)
			{
				CByteBuffer buffRet;
				int nLen = ::WideCharToMultiByte(nToCodePage, 0, (LPCWSTR)buffData.GetCBuffer(), buffData.GetLength() >> 1, NULL, 0, NULL, NULL);
				::WideCharToMultiByte(nToCodePage, 0, (LPCWSTR)buffData.GetCBuffer(), buffData.GetLength() >> 1, (LPSTR)buffRet.GetBufferSetLength(nLen), nLen, NULL, NULL);
				buffData = buffRet;
			}
			else if (nFromCodePage == CP_UFT16BE)
			{
				CByteBuffer::ReverseBaseTypes(buffData.GetBuffer(), buffData.GetLength());
				return CodeVonvert(CP_UFT16LE, nToCodePage, buffData);
			}
			else
			{
				CByteBuffer buffRet;
				int nLen = ::MultiByteToWideChar(nFromCodePage, 0, (LPCSTR)buffData.GetCBuffer(), buffData.GetLength(), NULL, 0);
				::MultiByteToWideChar(nFromCodePage, 0, (LPCSTR)buffData.GetCBuffer(), buffData.GetLength(), (LPWSTR)buffRet.GetBufferSetLength(nLen<<1), nLen);

				buffData = buffRet;
				if (nToCodePage != CP_UFT16LE)
				{
					return CodeVonvert(CP_UFT16LE, nToCodePage, buffData);
				}
			}
			return buffData.GetLength();
		}
	};
};

#endif
