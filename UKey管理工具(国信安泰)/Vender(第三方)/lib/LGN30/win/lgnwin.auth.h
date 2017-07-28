/*! \file lgnwin.auth.h 
*  \brief WIN系统的安全认证集类
*
*  工程要包含本文件，依赖文件：lgnwin.ver.h
*
*  声明安全属性类CSecurityAttributes，用于创建低级别安全描述对象
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGNWIN_AUTH_H__
#define __LGNWIN_AUTH_H__

#include <Sddl.h>
//#include "lgnwin.ver.h"


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 安全属性类
	/*! 继承于系统结构SECURITY_ATTRIBUTES，创建低级别安全描述对象，主要用内核对象的创建 */
	class CSecurityAttributes : public SECURITY_ATTRIBUTES
	{
	public:
		//! 构造函数，不对基类结构初始化
		CSecurityAttributes()
		{
			m_pTempSD = NULL;
		}
		//! 构造函数，创建低级别安全描述对象
		/*! \param[in] bInheritHandle 内核对象句柄是否可继承
			\see 调用BuildLowIntegrity
		*/
		CSecurityAttributes(bool bInheritHandle)
		{
			BuildLowIntegrity(bInheritHandle);
		}
		//! 虚造函数，释放已分配的资源
		~CSecurityAttributes()
		{
			if (m_pTempSD != NULL)
				::LocalFree(m_pTempSD);
		}

		//! 创建低级别安全描述对象
		/*! \param[in] bInheritHandle 内核对象句柄是否可继承
			\note 如果是vista后的系统，内部调用ConvertStringSecurityDescriptorToSecurityDescriptor
		*/
		bool BuildLowIntegrity(bool bInheritHandle)
		{
			nLength					= sizeof(SECURITY_ATTRIBUTES);
			bInheritHandle			= bInheritHandle;
			lpSecurityDescriptor	= &m_pSecDesc;

			::InitializeSecurityDescriptor(lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
			if (!::SetSecurityDescriptorDacl(lpSecurityDescriptor, TRUE, 0, FALSE))
			{
				LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Error, TEXT("BuildLowIntegrity.SetSecurityDescriptorDacl error=%x!!!\n"), ::GetLastError() );
				return false;
			}

			if (g_osVersionInfo.IsVistaLater())
			{
				LPCTSTR LOW_INTEGRITY_SDDL_SACL = TEXT("S:(ML;;NW;;;LW)"); 
				// (_WIN32_WINNT >= 0x0500)
				if (::ConvertStringSecurityDescriptorToSecurityDescriptor(LOW_INTEGRITY_SDDL_SACL, SDDL_REVISION_1, &m_pTempSD, NULL ))
				{
					PACL pSacl = NULL;                  // not allocated
					BOOL fSaclPresent = FALSE;
					BOOL fSaclDefaulted = FALSE;
					if (::GetSecurityDescriptorSacl(m_pTempSD, &fSaclPresent, &pSacl, &fSaclDefaulted))
					{
						if (::SetSecurityDescriptorSacl(lpSecurityDescriptor, TRUE, pSacl, FALSE))
						{
							return true;
						}
					}
				}
				LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Error, TEXT("BuildLowIntegrity(vista) error=%x!!!\n"), ::GetLastError() );
				return false;
			}
			return true;
		}

	private:
		CHAR m_pSecDesc[SECURITY_DESCRIPTOR_MIN_LENGTH];
		PSECURITY_DESCRIPTOR m_pTempSD;

	private:
		CSecurityAttributes(const CSecurityAttributes& other); //! 本类不可继承
		CSecurityAttributes& operator=(const CSecurityAttributes& other); //!< 本类对象不可复值操作
	};

#if (LGN_SUPPORT_SELECTANY)
	LGN_SELECTANY CSecurityAttributes g_saLowIntegrity(false);
#else
	extern CSecurityAttributes g_saLowIntegrity; //!< 声明全局低级别安全描述对象
#endif

}; // namespace LGN

#endif // #ifndef __LGNWIN_AUTH_H__
