//
//  ChangePINFunc.cpp
//  Ukeytools
//
//  Created by 范云飞 on 2017/6/29.
//  Copyright © 2017年 nist. All rights reserved.
//

#include "ChangePINFunc.h"
#import "AppDelegate.h"
#import "LibrarySingleton.h"

DWORD ChangePinFunc(LPBYTE OldPIN,LPBYTE NewPIN){
    
    __block DWORD dword;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        dword = pFunction->SKF_ChangePin((PS_DEV_HANDLE)(&Handler), ONKEY_TYPE_USER, OldPIN,sizeof(OldPIN), NewPIN,sizeof(NewPIN));
        dispatch_async(dispatch_get_main_queue(), ^{
            if (dword == 0) {
                [[NSApplication sharedApplication]stopModal];
            }
        });
    });

    return dword;
    
//    BYTE baData[10]="\x80\xf3\x00\x00\x00\x00\x00\x00";
//    BYTE baReData[100]={0};
//    DWORD dwlen;
//    pFunction->SKF_Transmit((PS_DEV_HANDLE)(&Handler),baData,8,baReData,&dwlen);
//    X_PrintHex("返回信息", baReData, 4);
//    printf("_______________%ld_______________",sizeof(baReData));
//    
//    for (int i =0; i<2; i++) {
//        
//        BYTE baData[10]="\x80\xf3\x00\x00\x00\x00\x00\x00";
//        BYTE baReData[4]={0};
//        DWORD dwlen;
//        pFunction->SKF_Transmit((PS_DEV_HANDLE)(&Handler),baData,8,baReData,&dwlen);
//        
//        X_PrintHex("返回信息", baReData, 4);
//        printf("_______________%ld_______________",sizeof(baReData));
//        
//        unsigned short ret_status = *( unsigned short*)baReData;
//        printf("\n RET_STATUS %x",ret_status);
//        if(ret_status == 0x6666) return ;
//        if(ret_status == 0x7777) return ;
//        if(ret_status == 0x9999) return ;
//        
//    }

}
