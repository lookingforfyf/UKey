//
//  LibrarySingleton.m
//  Ukeytools
//
//  Created by 范云飞 on 2017/7/21.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "LibrarySingleton.h"

 S_DEV_HANDLE Handler;
 ONKEY_FUNCTION_LIST_PTR pFunction = NULL;

void initLib(){
    void *hLibrary=NULL;//操作库对象
    void *pTemp=NULL;
    
    fnOnKey_GetFunctionList OnKey_GetFunctionList = NULL;
    hLibrary=dlopen ("nxy_dev_nist.dylib", 1);
    pTemp = dlsym (hLibrary,"OnKey_GetFunctionList");
    memcpy(&OnKey_GetFunctionList,&pTemp,sizeof(void *));
    ONKEY_FUNCTION_LIST_PTR pFuncList = NULL;
    
    DWORD dwRet = OnKey_GetFunctionList(&pFuncList);
    printf("<INFO>OnKey_GetFunctionList, Ver=%08X, Ret=%08X\n", (dwRet==0)?pFuncList->version : 0, dwRet);
    
    S_DEV_CONFIG devConfig;
    S_DEV_HANDLE devHandle;
    memset(&devConfig, 0, sizeof(devConfig));
    memset(&devHandle, 0, sizeof(devHandle));
    devConfig.dwSize = sizeof(devConfig);
    devConfig.sCOSInfo.m_wLen = sizeof(devConfig.sCOSInfo);
    devConfig.sAppInfoR.m_wLen = sizeof(devConfig.sAppInfoR);
    devConfig.sAppInfoRW.m_wLen = sizeof(devConfig.sAppInfoRW);
    //需要设置safe和修改用户pin码的标志
    devConfig.sAppInfoR.AppFormatSafeMode=1;
    devConfig.sAppInfoR.SupportKeyBoardInputPin=1;
    //end
    devHandle.pDevConfig = &devConfig;
    X_GetReaderList(pFuncList);
    
    char  szReaderList[1024];
    dwRet = pFuncList->SKF_EnumDev(szReaderList, 1024);
    
    printf("Reader List:\n");
    if (dwRet == 0)
    {
        char* p = szReaderList;
        for (int k=0; *p; k++)
        {
            printf("  (%d) Name=%s\n", k+1, p);
            p += strlen(p)+1;
        }
    }
    
    if (HDEV((PS_DEV_HANDLE)(&devHandle)))
    {
        X_CloseDevice(pFuncList,(PS_DEV_HANDLE)(&devHandle));
    }
    
    char sCmdStr[32] = { '1'};
    printf("SKF_EnumDev");
    pFuncList->SKF_EnumDev(szReaderList, 1024);
    
    char* p = szReaderList;
    for (; (*p) && (sCmdStr[0] > '1'); sCmdStr[0]--)
    {
        p += 15;
    }
    
    if (p)
    {       printf("SKF_ConnectDev");
        dwRet = pFuncList->SKF_ConnectDev(p,1, (PS_DEV_HANDLE)(&devHandle));
        printf("Open Device, Name=%s, Handle=%8p, Ret=%08X\n", p, HDEV((PS_DEV_HANDLE)(&devHandle)), dwRet);
    }
    
    Handler = devHandle;
    pFunction = pFuncList;

}



