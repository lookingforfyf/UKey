//
//  Constants.cpp
//  Ukeytools
//
//  Created by 范云飞 on 2017/7/21.
//  Copyright © 2017年 nist. All rights reserved.
//

#include "Constants.h"


DWORD StrToHex(LPBYTE pbHexData, LPCSTR pszAscData)
{
    DWORD dwInLen = (DWORD)strlen(pszAscData);
    
    BYTE c, *ptr;
    DWORD i;
    
    ptr = pbHexData;
    for (i=0; i<dwInLen; i++ )
    {
        c = pszAscData[i];
        if (c>='0' && c<='9')
            c = c-'0';
        else // A-F,a-f
            c = (c&0x0F) + 9;
        if (i&0x01) // 1
        {
            *ptr += c;
            ptr ++;
        }
        else // 0
        {
            *ptr = c<<4;
        }
    }
    
    return dwInLen >> 1;
}

void X_PrintHex(LPCSTR pszMsg, LPBYTE pbData, DWORD dwLen)
{
    printf("%s", pszMsg);
    for (DWORD i=0; i<dwLen; i++)
        printf("%02X", pbData[i]);
    printf("\n");
}

void X_GetReaderList(ONKEY_FUNCTION_LIST_PTR pFuncList)
{
    char  szReaderList[1024];
    DWORD dwRet = pFuncList->SKF_EnumDev(szReaderList, 1024);
    
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
}

void X_OpenDevice(ONKEY_FUNCTION_LIST_PTR pFuncList,PS_DEV_HANDLE phDev)
{
    char str[32];
    char str1[32];
    memcpy(str,"test 1234-3456",14);
    unsigned long ul;
    unsigned int ui;
    memcpy(&ul,str,1);
    memcpy(&ui,str,1);
    ul=ntohl(ul);
    ui=ntohl(ui);
    sscanf(str,"%*[^ ] %[^-]",str1);
    DWORD dwRet;
    if (HDEV(phDev))
    {
        X_CloseDevice(pFuncList,phDev);
    }
    
    X_GetReaderList(pFuncList);
    
    char sCmdStr[32] = { '1'};
    char  szReaderList[1024];
    printf("SKF_EnumDev");
    pFuncList->SKF_EnumDev(szReaderList, 1024);
    
    char* p = szReaderList;
    for (; (*p) && (sCmdStr[0] > '1'); sCmdStr[0]--)
    {
        p += 15;
    }
    
    if (p)
    {       printf("SKF_ConnectDev");
        dwRet = pFuncList->SKF_ConnectDev(p,1, phDev);
        printf("Open Device, Name=%s, Handle=%8p, Ret=%08X\n", p, HDEV(phDev), dwRet);
    }
    //BYTE baData[10]="\x80\x50\x00\x00\x00\x00\x08\x00";
    //BYTE baReData[100]={0};
    //DWORD dwlen=100;
    //pFuncList->SKF_Transmit(phDev,baData,8,baReData,&dwlen);
    /*printf("SKF_Transmit %d",dwlen);
     pFuncList->SKF_GenRandom(phDev,baReData,8);
     printf("SKF_GenRandom %d",dwlen);*/
    
    /*PinStruct st_userpin;
     st_userpin.dwPinLen=6;
     memcpy(st_userpin.baPin, "\x38\x38\x38\x38\x38\x38", st_userpin.dwPinLen);
     pFuncList->SKF_VerifyPIN(phDev,1, st_userpin.baPin, st_userpin.dwPinLen);
     printf("SKF_VerifyPIN end");*/
    
}

void X_CloseDevice(ONKEY_FUNCTION_LIST_PTR pFuncList,PS_DEV_HANDLE phDev)
{
    if (HDEV(phDev) == NULL)
    {
        printf("Please Open Device!!");
    }
    else
    {
        HANDLE hDev = HDEV(phDev);
        DWORD dwRet = pFuncList->SKF_DisConnectDev(phDev, FALSE);
        printf("Close Device, Handle=%8p, Ret=%08X\n", hDev, dwRet);
    }
}



void X_UserLogin(ONKEY_FUNCTION_LIST_PTR pFuncList,PS_DEV_HANDLE phDev)
{
    if (HDEV(phDev) == NULL)
    {
        printf("Please Open Device!!");
    }
    else
    {
        char sInPin[256] ={0};
        bool bRight = false;
        int  RemNum = ONKEY_MAX_PIN_TIMES;
        PinStruct st_userpin;
        do{
            
            memset(sInPin, 0, 256);
            printf("Please enter password:");
            gets(sInPin);
            
            if((sInPin[0] == 'q' && sInPin[1] == 0x00) || (sInPin[0] == 'Q' && sInPin[1] == 0x00))
                break;
            
            st_userpin.dwPinLen = strlen(sInPin);
            if(st_userpin.dwPinLen < 6 || st_userpin.dwPinLen > 16)
            {
                printf("\nPlease enter a password 6-16!\n");
                continue;
            }
            
            memcpy(st_userpin.baPin, sInPin, st_userpin.dwPinLen);
            
            DWORD dwRet = pFuncList->SKF_VerifyPIN(phDev, ONKEY_TYPE_USER, st_userpin.baPin, st_userpin.dwPinLen);
            if(dwRet == 0)
            {
                bRight = true;
                printf("Verify Pin success, Ret=%08X\n", dwRet);
            }
            else
            {
                RemNum--;
                printf("Verify Pin error, Ret=%08X\n", dwRet);
            }
            
        }while(!bRight && RemNum>0);
        
    }
}
void X_SwitchMode(ONKEY_FUNCTION_LIST_PTR pFuncList,PS_DEV_HANDLE phDev)
{
    if (HDEV(phDev) == NULL)
    {
        printf("请先打开设备!!");
    }
    else
    {
        BYTE baData[10]="\x80\x50\x00\x00\x00\x00\x08\x00";
        BYTE baReData[100]={0};
        DWORD dwlen=100;
        pFuncList->SKF_Transmit(phDev,baData,8,baReData,&dwlen);
    }
    
}

void X_TestPairKey(ONKEY_FUNCTION_LIST_PTR pFuncList,PS_DEV_HANDLE phDev)
{
    //X_UserLogin(pFuncList,phDev);
    
    BYTE baDataBuff[1000], baSignature[256];
    DWORD dwRet, dwDataLen, dwSignatureLen = 256;
#if 0
    StrToHex(baDataBuff,       "3C3F786D6C2076657273696F6E3D22312E302220656E636F64696E673D225554462D3822203F3E0D0A3C523E0D0A093C483E0D0A09093C4D3E0D0A0909093C633E3C2F633E0D0A0909093C763E303C2F763E0D0A09093C2F4D3E0D0A093C2F483E0D0A093C503E2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A3C2F703E0D0A093C443E0D0A09093C4D3E0D0A0909093C633EE694B6E6ACBEE8B4A6E58FB7EFBC9A3C2F633E0D0A0909093C763E363232353638303234313030313837393039313C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE694B6E6ACBEE8B4A6E58FB7E688B7E5908DEFBC9A3C2F633E0D0A0909093C763EE794B0E4B8B9E4B8B93C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE8BDACE8B4A6E98791E9A29DEFBC9A3C2F633E0D0A0909093C763E312E30303C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE8BDACE8B4A6E5B881E7A78DEFBC9A3C2F633E0D0A0909093C763EE4BABAE6B091E5B8813C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE4BB98E6ACBEE8B4A6E58FB7EFBC9A3C2F633E0D0A0909093C763E393131313231303230303030303032343136343C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE4B880E7AB99E5BC8FE8BDACE8B4A63C2F633E0D0A0909093C763E3C2F763E0D0A09093C2F4D3E0D0A093C2F443E0D0A3C2F523E0D0A");
    //StrToHex(&baDataBuff[0xD0],"265061796565616464723DD5D0C9CCD2F8D0D0B1B1BEA9C7E5BBAAD4B0D6A7D0D0265472616E5375626D697454696D653D3230303630383138313430363334263030303030303030");
    //memcpy(baDataBuff, "\x41\x4F\xD6\x57\x49\xC1\xD8\x09\x14\x6D\x27\xF8\xB4\xE6\xB1\x57\x2F\xF3\x3B\x8D", 20);
    long unsigned int length =536;
    BYTE pbT[0x800 * 2];
    memset(pbT, 0x12, 256);
    //	X_ReadData("SignTest.txt", pbT, &length);
    dwRet = pFuncList->SKF_RSASignData(phDev, KERID, ONKEY_FLAG_HASH_IN_MSGSHA256, baDataBuff, length, baSignature, &dwSignatureLen);
    
#else
    StrToHex(baDataBuff, "3021300906052B0E03021A050004149720434590AE657A59494F9E3FE0F37C3AC5AFB2");
    dwDataLen = 35;
    dwRet = pFuncList->SKF_RSASignData(phDev, KERID, 0, baDataBuff, dwDataLen, baSignature, &dwSignatureLen);
#endif
    
    if (dwRet == 0)
        X_PrintHex(" Sign Result:", baSignature, dwSignatureLen);
    
}

void X_TestPairKey_Show(ONKEY_FUNCTION_LIST_PTR pFuncList,PS_DEV_HANDLE phDev)
{
    //X_UserLogin(pFuncList,phDev);
    
    BYTE baDataBuff[10000], baSignature[256];
    DWORD dwRet, dwDataLen, dwSignatureLen = 256;
#if 1
    StrToHex(baDataBuff,       "3C3F786D6C2076657273696F6E3D22312E302220656E636F64696E673D225554462D3822203F3E0D0A3C523E0D0A093C483E0D0A09093C4D3E0D0A0909093C633E3C2F633E0D0A0909093C763E303C2F763E0D0A09093C2F4D3E0D0A093C2F483E0D0A093C503E2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A3C2F703E0D0A093C443E0D0A09093C4D3E0D0A0909093C633EE694B6E6ACBEE8B4A6E58FB7EFBC9A3C2F633E0D0A0909093C763E363232353638303234313030313837393039313C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE694B6E6ACBEE8B4A6E58FB7E688B7E5908DEFBC9A3C2F633E0D0A0909093C763EE794B0E4B8B9E4B8B93C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE8BDACE8B4A6E98791E9A29DEFBC9A3C2F633E0D0A0909093C763E312E30303C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE8BDACE8B4A6E5B881E7A78DEFBC9A3C2F633E0D0A0909093C763EE4BABAE6B091E5B8813C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE4BB98E6ACBEE8B4A6E58FB7EFBC9A3C2F633E0D0A0909093C763E393131313231303230303030303032343136343C2F763E0D0A09093C2F4D3E0D0A09093C4D3E0D0A0909093C633EE4B880E7AB99E5BC8FE8BDACE8B4A63C2F633E0D0A0909093C763E3C2F763E0D0A09093C2F4D3E0D0A093C2F443E0D0A3C2F523E0D0A");
    //StrToHex(&baDataBuff[0xD0],"265061796565616464723DD5D0C9CCD2F8D0D0B1B1BEA9C7E5BBAAD4B0D6A7D0D0265472616E5375626D697454696D653D3230303630383138313430363334263030303030303030");
    //memcpy(baDataBuff, "\x41\x4F\xD6\x57\x49\xC1\xD8\x09\x14\x6D\x27\xF8\xB4\xE6\xB1\x57\x2F\xF3\x3B\x8D", 20);
    long unsigned int length =536;
    BYTE pbT[0x800 * 2];
    memset(pbT, 0x12, 256);
    //X_ReadData("SignTest.txt", pbT, &length);
    dwRet = pFuncList->SKF_RSASignData(phDev, KERID, ONKEY_FLAG_HASH_IN_MSGSHA256, baDataBuff, length, baSignature, &dwSignatureLen);
    
#else
    StrToHex(baDataBuff, "3021300906052B0E03021A050004149720434590AE657A59494F9E3FE0F37C3AC5AFB2");
    dwDataLen = 35;
    dwRet = pFuncList->SKF_RSASignData(phDev, KERID, 0, baDataBuff, dwDataLen, baSignature, &dwSignatureLen);
#endif
    
    if (dwRet == 0)
        X_PrintHex(" Sign Result:", baSignature, dwSignatureLen);
    
}

