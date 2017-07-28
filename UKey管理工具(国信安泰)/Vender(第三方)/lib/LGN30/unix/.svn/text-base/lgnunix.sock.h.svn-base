#ifndef __LGNUNIX_SOCK_H__
#define __LGNUNIX_SOCK_H__


namespace LGN
{
	class CSocket
	{
	public:
		static int Startup(BYTE btMajorVer=1, BYTE btMinorVer=0)
		{
			return 0;
		}
		static int Cleanup()
		{
			return 0;
		}

	public:
		CSocket() : m_hSocket(-1)
		{
		}
		explicit CSocket( int hSock ) : m_hSocket(hSock)
		{
		}

		virtual ~CSocket()
		{
			Close();
		}

		operator int() const
		{
			return( m_hSocket );
		}

	public:
		HRESULT Connect(LPCSTR pszHostIp, UINT32 nPort)
		{
			struct sockaddr_in sockAddr;				
			API::memset(&sockAddr,0,sizeof(sockAddr));
			sockAddr.sin_family = AF_INET; 
			sockAddr.sin_addr.s_addr = inet_addr(pszHostIp);
			sockAddr.sin_port = htons(nPort);

			if ((m_hSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				return API::HResultFromError(errno);;
			}
						
			if (connect(m_hSocket,(struct sockaddr *)&sockAddr,sizeof(sockAddr)) == -1)
			{
				return API::HResultFromError(errno);;
			}

			// 设置接收超时时间
			struct timeval timeout={45,0}; // 45S
			setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(timeout));
			
			return S_OK;
		}

		HRESULT Close()
		{
			if (m_hSocket != -1)
			{
				close(m_hSocket);
				m_hSocket = -1;
			}

			return S_OK;
		}

		HRESULT Send(LPCVOID pBuffer, UINT32 nBuffSize)
		{
			if (send(m_hSocket, (char*)pBuffer, nBuffSize, 0) == -1)
				return API::HResultFromError(errno);
			return S_OK;
		}

		HRESULT Receive(LPVOID pBuffer, UINT32 nBuffSize, UINT32 & nRecvLen)
		{
			if ((nRecvLen = recv(m_hSocket, (char*)pBuffer, nBuffSize, 0)) == -1)
				return API::HResultFromError(errno);
			return S_OK;
		}

	protected:
		int m_hSocket;
	};

}; // namespace LGN

#endif // __LGNUNIX_SOCK_H__
