/*! \file lgnwin.sock.h 
*  \brief WIN系统的SOCKET通讯类
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  声明SOCKET通讯类CSocket
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGNWIN_SOCK_H__
#define __LGNWIN_SOCK_H__

#ifndef __LGN_BASE_H__
#error lgnwin.gsmem.h requires include lgnbase.h
#endif

#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")


namespace LGN
{
	//! SOCKET通讯类
	/*! 内部调用系统API实现SOCKET通讯通讯功能，目前只实现了TCP客户端通讯部分
		-# 点击这里查看 \ref socket_example_page
	*/
	class CSocket
	{
	public:
		//! 启动SOCKET使用，具体参考WSAStartup
		static int Startup(BYTE btMajorVer=1, BYTE btMinorVer=0)
		{
			WSADATA wsaData;
			return ::WSAStartup(MAKEWORD(btMajorVer,btMinorVer), &wsaData);
		}
		//! 终止SOCKET使用，具体参考WSACleanup
		static int Cleanup()
		{
			return ::WSACleanup();
		}

	public:
		//! 构造函数，初始成员变量
		CSocket() : m_hSocket(INVALID_SOCKET)
		{
		}
		//! 明确类型的构造函数，初始成员变量
		explicit CSocket( SOCKET hSock ) : m_hSocket(hSock)
		{
		}

		//! 虚造函数，如果SOCKET有效则关闭
		virtual ~CSocket()
		{
			Close();
		}

		//! 返回SOCKET句柄
		operator SOCKET() const
		{
			return( m_hSocket );
		}

	public:
		/*! \brief 连接主机
			创建SOCKET并连接主机成功后，设置通讯的接收超时时间(45S)
			\param[in] pszHostIp 主机的IP地址，ANSI字符串
			\param[in] nPort 端口
			\return 错误号
		*/
		HRESULT Connect(LPCSTR pszHostIp, UINT32 nPort)
		{
			m_hSocket = ::socket(PF_INET, SOCK_STREAM, 0);
			if (m_hSocket == INVALID_SOCKET)
			{
				return API::HResultFromError(::WSAGetLastError());
			}

			// socket addr
			SOCKADDR_IN sockAddr;
			API::memset(&sockAddr,0,sizeof(sockAddr));
			sockAddr.sin_family = AF_INET;
			sockAddr.sin_addr.s_addr = ::inet_addr(pszHostIp);
			sockAddr.sin_port = htons(nPort);
			// connects to peer
			if (::connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) != 0)
			{		
				m_hSocket = INVALID_SOCKET;
				return API::HResultFromError(::WSAGetLastError());
			}

			// 设置接收超时时间
			UINT32 nTimeOut = 45000; // 45S
			::setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nTimeOut, sizeof(nTimeOut));
			
			return S_OK;
		}

		/*! \brief 关闭已连接的SOCKET
			\return 错误号
		*/
		HRESULT Close()
		{
			if (m_hSocket != INVALID_SOCKET)
			{
				::closesocket(m_hSocket);
				m_hSocket = INVALID_SOCKET;
			}

			return S_OK;
		}

		/*! \brief 往SOCKET发送数据
			\param[in] pBuffer 待发送数据的缓冲区指针
			\param[in] nBuffSize 待发送数据\a pBuffer的长度
			\return 错误号
		*/
		HRESULT Send(LPCVOID pBuffer, UINT32 nBuffSize)
		{
			if (::send(m_hSocket, (char*)pBuffer, nBuffSize, 0) == SOCKET_ERROR)
				return API::HResultFromError(::WSAGetLastError());
			return S_OK;
		}

		/*! \brief 从SOCKET接收数据
			\param[in] pBuffer 待接收数据的缓冲区指针
			\param[in] nBuffSize 想接收数据的长度，不大于缓冲区\a pBuffer的分配空间长度
			\param[in] nRecvLen 接收到的数据长度
			\return 错误号
		*/
		HRESULT Receive(LPVOID pBuffer, UINT32 nBuffSize, UINT32 & nRecvLen)
		{
			if ((nRecvLen = ::recv(m_hSocket, (char*)pBuffer, nBuffSize, 0)) == SOCKET_ERROR)
				return API::HResultFromError(::WSAGetLastError());
			return S_OK;
		}

	protected:
		SOCKET m_hSocket; //!< SOCKET句柄

	private:
		CSocket(const CSocket& other); //! 本类不可继承
		CSocket& operator=(const CSocket& other); //!< 本类对象不可复值操作
	};


/*! \page socket_example_page CSocketT例子
<pre>	%CSocketT类的使用例子</pre>
<pre>
\#include "lgnbase.h"
\#include "lgnsock.h"

	// ...
	// 在使用SOCKET前必须执行以下一行代码
	LGN::CSocket::Startup();
	
	LGN::CSocket socket;
	socket.Connect("127.0.0.1", 30003);
	socket.Send("123", 3);

	//
	LGN::CSocket::Cleanup();
</pre>
*/

}; // namespace LGN

#endif // __LGNWIN_SOCK_H__
