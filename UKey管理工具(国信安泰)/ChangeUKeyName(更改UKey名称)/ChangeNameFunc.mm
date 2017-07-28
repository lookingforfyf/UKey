//
//  ChangeNameFunc.cpp
//  Ukeytools
//
//  Created by 范云飞 on 2017/7/3.
//  Copyright © 2017年 nist. All rights reserved.
//

#include "ChangeNameFunc.h"
#import "AppDelegate.h"

DWORD ChangeNameFunc(LPBYTE name){

    __block DWORD dword;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        dword = pFunction->SKF_SetLabel((PS_DEV_HANDLE)(&Handler),name,sizeof(name));
        dword = 0;
        dispatch_async(dispatch_get_main_queue(), ^{
            if (dword == 0) {
                
                [[NSApplication sharedApplication]stopModal];
            }
        });
    });
    
    return dword;
}

