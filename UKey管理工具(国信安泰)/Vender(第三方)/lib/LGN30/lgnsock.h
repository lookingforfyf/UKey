/*! \file lgnsock.h 
*  \brief SOCKET通讯集类
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  根据系统类型保护对应系统的SOCKET通讯集类，并声SOCKET整体性操作静态模板类SockWhole
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGN_SOCK_H__
#define __LGN_SOCK_H__

#if (LGN_WINSYS)
#include "win\lgnwin.sock.h"
#else
#include "unix/lgnunix.sock.h"
#endif

#ifndef __LGN_BASE_H__
#error lgnwin.gsmem.h requires include lgnbase.h
#endif


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! SOCKET整体性操作静态模板类
	/*! 主要实现SOCKET通讯的发送/接收报文操作，不考虑数据编码，以二进制方式读写。
	 * -# 点击这里查看 \ref sockwhole_example_page	
	*/
	class SockWhole
	{
	public:
		//! 发送报文，先发送2字节的长度，再发其长度的数据
		static HRESULT SendPacket(CSocket & sock, const CByteBuffer & buffSend)
		{
			// send 2 bytes
			BYTE abDataLen[2] = {(BYTE)(buffSend.GetLength() >> 8), (BYTE)(buffSend.GetLength())};
			HRESULT hr = sock.Send(abDataLen, 2);
			if (FAILED(hr))
				return hr;
			// send data
			return sock.Send(buffSend.GetCBuffer(), buffSend.GetLength());
		}

		//! 接收报文，先收2字节的长度，再收其长度的数据
		static HRESULT ReceivePacket(CSocket & sock, CByteBuffer & buffRecv)
		{
			BYTE abDataLen[2];
			UINT32 nRecvLen = 2;

			// recv 2 bytes
			HRESULT hr = sock.Receive(abDataLen, 2, nRecvLen);
			if (FAILED(hr))
				return hr;
			if (nRecvLen == 1)
			{
				if (FAILED(hr = sock.Receive(abDataLen+1, 1, nRecvLen)))
					return hr;
			}

			// recv data
			UINT32 nDataLen = (abDataLen[0] << 8) + abDataLen[1];
			LPBYTE pbBuffer = buffRecv.GetBufferSetLength(nDataLen);
			while(nDataLen > 0)
			{
				if (FAILED(hr = sock.Receive(pbBuffer, nDataLen, nRecvLen)))
					return hr;
				nDataLen -= nRecvLen;
				pbBuffer += nRecvLen;
			}
			return S_OK;
		}

		/*! \brief TCP客户端连接，发送和接收报文
			\param [in] pszHostIp 主机IP
			\param [in] nPort 端口
			\param [in] buffSend 待发送的数据
			\param [out] buffRecv 接收到的数据
			\return HRESULT
			\see SendPacket
			\see ReceivePacket
		*/
		static HRESULT SendAndReceive(LPCSTR pszHostIp, UINT32 nPort, const CByteBuffer & buffSend, CByteBuffer & buffRecv)
		{
			CSocket sock;
			HRESULT hr = sock.Connect(pszHostIp, nPort);

			if (SUCCEEDED(hr))
			{
				if (SUCCEEDED(hr = SendPacket(sock, buffSend)))
				{
					hr = ReceivePacket(sock, buffRecv);
				}
			}

			sock.Close();
			return hr;
		}

	};

	typedef SockWhole Sock; //!< 类型重定义 LGN::SockWhole

}; // end namespace LGN

/*! \page sockwhole_example_page SockWhole例子
<pre> TCP客户端连接，发送和接收报文</pre>
<pre>
\#include "lgnbase.h"
\#include "lgnsock.h"

	//...
	LGN::CByteBuffer sendData((LPBYTE)"\x00\x01\x00\x01\x00\x00\x00\x00\x11\x22\x33\x44\x55", 12);
	LGN::CByteBuffer recvData;
	HRESULT hr = LGN::Sock::SendAndReceive("127.0.0.1", 30003, sendData, recvData);
	if (SUCCEEDED(hr))
	{
		LGN::CString strText;
		strText.FromHexs(recvData);
	}
</pre>
*/

#endif // #ifndef __LGN_SOCK_H__
