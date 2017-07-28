
namespace LGN
{
#ifdef __LGNTRACE_H__
	// trace
#if (LGN_SUPPORT_TRACE)
	CTraceCategory lgnTraceGeneral(LGN_TRACE_CATEGORY_DEF_FLAGS, LGN_TRACE_CATEGORY_DEF_LEVEL, LGN_TRACE_CATEGORY_DEF_NAME, LGN_TRACE_CATEGORY_DEF_FILE);
#endif
#endif

#ifdef __LGNUNIX_ALLOC_H__
	// alloc
	CCRTHeap g_heapmgr;
	ILgnMemMgr * _lgnMemoryManager = &g_heapmgr;
	// buffer
	CLgnBufferMgr g_bufmgr(_lgnMemoryManager);
	ILgnBufferMgr * _lgnBufferManager = &g_bufmgr;
#endif
	
#ifdef __LGNUNIX_SYNC_H__
	static CCriticalSection g_csTheadSafe;

	void LgnTheadSafe_Enter()
	{
		g_csTheadSafe.Enter();
	}
	void LgnTheadSafe_Leave()
	{
		g_csTheadSafe.Leave();
	}
#endif
	
	void DoInitialization()
	{
	}
	
	void DoCleanup()
	{
	}
}; // namespace LGN

