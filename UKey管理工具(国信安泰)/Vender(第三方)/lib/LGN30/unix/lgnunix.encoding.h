#ifndef __LGNUNIX_ENCODING_H__
#define __LGNUNIX_ENCODING_H__


#define CP_ACP		"CP936"
#define CP_UTF8		"UTF-8"
#define CP_UFT16LE	"UTF-16LE"
#define CP_UFT16BE	"UTF-16BE"

namespace LGN
{

	class EncodingBaseOS
	{
	public:
		static int CodeVonvert(LPCSTR pszFromCharset, LPCSTR pszToCharset, CByteBuffer & buffData)
		{
#ifdef _ICONV_H			
			iconv_t cd;

			cd = iconv_open(pszToCharset, pszFromCharset);
			if (cd == 0)
			{
				return -1;
			}
			CByteBuffer buffIn(buffData);
			size_t nSLen = buffIn.GetLength();
			size_t nDLen = nSLen * 2;
			size_t nLeft = nDLen;
			char *pSrc = (char*)buffIn.GetBuffer();
			char *pDest = (char*)buffData.GetBuffer(nDLen);
			/*size_t nResult = */iconv(cd, &pSrc, &nSLen, &pDest, &nLeft);
			buffData.ReleaseBufferSetLength(nDLen - nLeft);
			iconv_close(cd);	

			return nDLen - nLeft;
#else
			return -1;
#endif
		}
	};
};

#endif
