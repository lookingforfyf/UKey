//
//  GetDeviceInfoFunc.cpp
//  Ukeytools
//
//  Created by 范云飞 on 2017/6/26.
//  Copyright © 2017年 nist. All rights reserved.
//

#include "GetDeviceInfoFunc.h"
#import "AppDelegate.h"
#import "LibrarySingleton.h"

NSArray * getDeviceInfo(){
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
         initLib();
    });

    BYTE dev[1000]= {0};
    DEVINFO dinfo;
    pFunction->SKF_GetDeviceInfo((PS_DEV_HANDLE)(&Handler),dev);
    memcpy(&dinfo, dev, 287);

    char * ukeySN = dinfo.SerialNumber;
    char * ukeyName = dinfo.Label;
    
    char32_t TotalSpace = dinfo.TotalSpace;
    char32_t FreeSpace = dinfo.FreeSpace;
    
    char32_t ukeySize = TotalSpace = FreeSpace;

    NSMutableArray  * array = [[NSMutableArray alloc]init];
    [array addObject:[NSString stringWithFormat:@"%s",ukeySN]];
    [array addObject:[NSString stringWithFormat:@"%s",ukeyName]];
    [array addObject:[NSString stringWithFormat:@"%d",ukeySize]];
    
    return (NSArray *)array;
}

