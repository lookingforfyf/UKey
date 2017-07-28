//
//  UKeyInfoCtr.m
//  Ukeytools
//
//  Created by wuyangfan on 17/5/27.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "UKeyInfoCtr.h"
#import "GetDeviceInfo.h"//设备信息
#import "HextoString.h"

@interface UKeyInfoCtr ()
@property (weak) IBOutlet NSTextField *ukeyName;
@property (weak) IBOutlet NSTextField *ukeySN;
@property (weak) IBOutlet NSTextField *ukeySize;
@property (weak) IBOutlet NSTextField *ukeyPinCount;

@property (weak) IBOutlet NSTextField *labTryCount;
@property (weak) IBOutlet NSTextField *labSize;
@property (weak) IBOutlet NSTextField *labSN;
@property (weak) IBOutlet NSTextField *labName;
@property (weak) IBOutlet NSBox *box;


@end

@implementation UKeyInfoCtr

- (void)windowDidLoad {
    [super windowDidLoad];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(doCloseWindow) name:NSWindowWillCloseNotification object:nil];
#pragma mark ------------------- 获取设备的SN和LABEL--------------------
    
    GetDeviceInfo * deviceInfo = [[[GetDeviceInfo alloc]init]initDeviceInfo];
    if (deviceInfo.array.count==0) {
        //        self.ukeySN.stringValue = @"1234567890";
        //        self.ukeyName.stringValue = @"nist2000";
        //        self.ukeySize.stringValue = @"12345k";
        //        self.ukeyPinCount.stringValue = @"6次";
    }
    else{
        NSArray * UkeyInfo = [deviceInfo.array copy];
        self.ukeySN.stringValue = [UkeyInfo objectAtIndex:0];
        self.ukeyName.stringValue =  [UkeyInfo objectAtIndex:1];
        self.ukeySize.stringValue = [NSString stringWithFormat:@"%@k",[UkeyInfo objectAtIndex:2]];
        self.ukeyPinCount.stringValue = @"6次";
        NSLog(@"***********%@*********",deviceInfo.array);
    }
}

-(void)setLanguage:(BOOL)isCN{
    if(isCN){
        self.box.title = @"UKey信息";
        self.labName.stringValue = @"UKey名称:";
        self.labSN.stringValue = @"UKey序列号:";
        self.labSize.stringValue = @"UKey容量:";
        self.labTryCount.stringValue = @"UKey密码重试次数:";
        self.window.title = @"UKey信息";
        
    }else{
        self.box.title = @"UKey Infomation";
        self.labName.stringValue = @"UKey Name:";
        self.labSN.stringValue = @"Serial Number:";
        self.labSize.stringValue = @"Capacity:";
        self.labTryCount.stringValue = @"The Chance Left To Try:";
        self.window.title = @"UKey Infomation";
    }
}

-(void)doCloseWindow{
    [[NSApplication sharedApplication] stopModal];
}


@end
