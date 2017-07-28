/*! \file lgnwin.ver.h 
*  \brief WIN系统的版本类
*
*  工程要包含本文件
*
*  声明操作系统版本类COSVersionInfoEx
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGNWIN_VER_H__
#define __LGNWIN_VER_H__

namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 操作系统版本类
	/*! 继承于系统结构OSVERSIONINFOEX，获取当前操作系统的版本信息 */
	class COSVersionInfoEx : public OSVERSIONINFOEX
	{
	public:
		//! 构造函数，获取当前操作系统的版本信息
		COSVersionInfoEx()
		{
			dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			wServicePackMajor = 0; // 补丁号
			::GetVersionEx((OSVERSIONINFO*)this);
		}

		//! 是Windows 2000 系统
		bool IsWin2K()
		{
			return (dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion == 5 && dwMinorVersion == 0);
		}
		//! 是Windows XP 系统
		bool IsWinXP()
		{
			return	(dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion == 5 && dwMinorVersion == 1);
		}
		//! 是Windows 2003 系统
		bool IsWin2003()
		{
			return (dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion == 5 && dwMinorVersion == 2);
		}		
		//! 是Windows Vista 系统
		bool IsVista()
		{
			return (dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion == 6 && dwMinorVersion == 1);
		}

		//! 是Windows 7 系统
		bool IsWin7()
		{
			return (dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion == 6 && dwMinorVersion == 2);
		}


		//! 是Windows 2000 及以后系统
		bool IsWin2KLater()
		{
			return (dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion >= 5);
		}
		//! 是Windows XP 及以后系统
		bool IsWinXPLater()
		{
			return (dwPlatformId == VER_PLATFORM_WIN32_NT) && ((dwMajorVersion > 5) || (dwMajorVersion == 5 && dwMinorVersion >= 1));
		}
		//! 是Windows Vista 及以后系统
		bool IsVistaLater()
		{
			return (dwPlatformId == VER_PLATFORM_WIN32_NT) && (dwMajorVersion >= 6);
		}
		//! 是Windows 7 及以后系统
		bool IsWin7Later()
		{
			return (dwPlatformId == VER_PLATFORM_WIN32_NT) && ((dwMajorVersion > 6) || (dwMajorVersion == 6 && dwMinorVersion >= 2));
		}

	private:
		COSVersionInfoEx(const COSVersionInfoEx& other); //! 本类不可继承
		COSVersionInfoEx& operator=(const COSVersionInfoEx& other); //!< 本类对象不可复值操作
	};

#if (LGN_SUPPORT_SELECTANY)
	LGN_SELECTANY COSVersionInfoEx g_osVersionInfo;
#else
	extern COSVersionInfoEx g_osVersionInfo;  //!< 声明全局操作系统版本对象
#endif

}; // namespace LGN

#endif // __LGNWIN_VER_H__
