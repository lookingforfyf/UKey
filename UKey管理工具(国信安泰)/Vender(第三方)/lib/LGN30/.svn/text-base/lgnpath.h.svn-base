/*! \file lgnpath.h 
*  \brief 目录（路径）处理类
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  路径处理模板类CPathT
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGN_PATH_H__
#define __LGN_PATH_H__


#ifndef MAX_PATH
#define MAX_PATH 260
#endif


namespace LGN
{

////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPathT

	//! 模块路径操作的模板类
	/*! 根据模块句柄获取其路径，路径合并。 
		-# 点击这里查看 \ref path_example_page
	*/
	template< typename _BaseType>
	class CPathT
	{
	public:
		typedef ChTraitsEx< _BaseType > ThisTraits;
	public:
		/*! \brief 构造函数，设置路径为空
		*/
		CPathT()
		{
			m_szPath[0] = 0;
		}

		/*! \brief 构造函数，设置路径						
			\param[in] pszPath 路径
		*/
		CPathT(const _BaseType * pszPath)
		{
			ThisTraits::StringCopy(m_szPath, pszPath);
		}

		/*! \brief 构造函数，获取模块路径与\a pszSubPath合并						
			\param[in] hModule 模块句柄
			\param[in] pszSubPath 子路径，允许为空			
		*/
		CPathT(HANDLE hModule, const _BaseType * pszSubPath)
		{
			m_szPath[0] = 0;
			ThisTraits::SGetModuleFileName(hModule, m_szPath, MAX_PATH);
			ThisTraits::SPathRemoveFileSpec(m_szPath);
			if (pszSubPath != NULL)
				ThisTraits::SPathCombine(m_szPath, m_szPath, pszSubPath);
		}

		const _BaseType * Combine(const _BaseType * pszSubPath)
		{
			ThisTraits::SPathCombine(m_szPath, m_szPath, pszSubPath);
			return m_szPath;
		}

		const _BaseType * RemoveFileSpec()
		{
			ThisTraits::SPathRemoveFileSpec(m_szPath);
			return m_szPath;
		}

		/*! \brief 操作符类型					
			\return 获取路径 
		*/
		operator const _BaseType *()
		{
			return m_szPath;
		}

	protected:
		//! 存放路径
		_BaseType m_szPath[MAX_PATH];
	};


	typedef CPathT< CHAR > CPathA; //!< 类型定义 LGN::CPathT< CHAR >
	typedef CPathT< WCHAR > CPathW; //!< 类型定义 LGN::CPathT< WCHAR >
	typedef CPathT< TCHAR > CPath; //!< 类型定义 LGN::CPathT< TCHAR >

/*! \page path_example_page CPathT例子
<pre>	%CPathT类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// ...
	LGN::CPath modPath1(NULL, "Ins.log");
	LPCTSTR pszPath1 = modPath1;

	LGN::CPath modPath2(::GetModuleHandle(TEXT("user32.dll")), "my.exe");
	LPCTSTR pszPath2 = modPath2;
</pre>
*/

};

#endif // #ifndef __LGN_PATH_H__
