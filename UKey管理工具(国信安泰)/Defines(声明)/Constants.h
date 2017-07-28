//
//  Constants.hpp
//  Ukeytools
//
//  Created by 范云飞 on 2017/7/21.
//  Copyright © 2017年 nist. All rights reserved.
//

#ifndef Constants_hpp
#define Constants_hpp

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <dlfcn.h>
#include "lgnincludes.h"
#include "OnKeyPreDef.h"
#include "OnKeyDevice.h"
#include "pkcs11t_ext.h"

#define ONKEY_MAX_PIN_TIMES    6
#define HDEV(phDev) (((PONKEYCOMMPARAM)(phDev->baCommParam))->hDev2)
#define	KERID 1

void X_GetReaderList(ONKEY_FUNCTION_LIST_PTR pFuncList);
DWORD StrToHex(LPBYTE pbHexData, LPCSTR pszAscData);
void X_PrintHex(LPCSTR pszMsg, LPBYTE pbData, DWORD dwLen);
void X_GetReaderList(ONKEY_FUNCTION_LIST_PTR pFuncList);
void X_OpenDevice(ONKEY_FUNCTION_LIST_PTR pFuncList, PS_DEV_HANDLE phDev);
void X_CloseDevice(ONKEY_FUNCTION_LIST_PTR pFuncList,PS_DEV_HANDLE phDev);

typedef struct _ONKEYCOMMPARAM
{
    HANDLE hDev;
    HANDLE hDev2;
    //#ifdef _COMM_CRYPT
    DWORD dwSessionActive: 1;//0表示没有建立过通道，1 表示成功建立通道，
    DWORD dwSessionEncrypt: 1;	//是否为加密通道
    DWORD dwEncKeyID: 8;	//key id
    BYTE baSessionKey[8];
    HANDLE hDev3;//for ccid reader
    //#endif
}ONKEYCOMMPARAM, *PONKEYCOMMPARAM;

typedef struct _PinStruct
{
    LPCSTR pszDefPin;
    BYTE baPin[16];
    DWORD dwPinLen;
}PinStruct;

#endif /* Constants_hpp */
