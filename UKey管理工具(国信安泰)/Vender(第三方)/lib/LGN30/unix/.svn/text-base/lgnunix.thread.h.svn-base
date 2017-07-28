#ifndef __LGNUNIX_THREAD_H__
#define __LGNUNIX_THREAD_H__


#ifndef __LGN_BASE_H__
#error lgnunix.thread.h requires include lgnbase.h
#endif


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	class CThread : public CHandle
	{
	public:
		CThread()
		{
			m_threadId = (pthread_t)-1;
		}
		CThread( CThread & th ) : CHandle( th )
		{
			m_threadId = th.m_threadId;
		}
		explicit CThread( HANDLE hThread ) : CHandle( hThread )
		{
			m_threadId = (pthread_t)-1;
		}

		HRESULT Start(int nPriority = 0)
		{
			if (m_h != NULL)
			{
				return API::HResultFromError( 2 );
			}

			int ret = pthread_create(&m_threadId, NULL, _ThreadProc, this);
			if (ret != 0)
			{
				return API::HResultFromError( ret );
			}

			m_h = &m_threadId;
			return S_OK;
		}

		HRESULT Terminate(UINT32 unExitCode = 0)
		{
			pthread_cancel(m_threadId);
			//pthread_kill(m_threadId, SIGKILL); // 把进程也杀了
			//pthread_exit((void*)unExitCode);
			m_threadId = (pthread_t)-1;
			m_h = NULL;
			return S_OK;
		}

		HRESULT Wait(UINT32 unTimeout = INFINITE)
		{
			unTimeout;
			
			int ret = pthread_join(m_threadId, NULL);
			if (ret != 0)
			{
				return API::HResultFromError( ret );
			}

			return S_OK;
		}

	protected:
		virtual UINT32 OnRun() = 0;

		static void * _ThreadProc(void * lpThreadParameter)
		{
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); //允许退出线程   
      pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //设置立即取消  			
      
			UINT32 unRet = ((CThread*)lpThreadParameter)->OnRun();
			return (void *)unRet;
		}
		
		pthread_t m_threadId;
	};

	
}; // namespace LGN

#endif // __LGNUNIX_THREAD_H__
