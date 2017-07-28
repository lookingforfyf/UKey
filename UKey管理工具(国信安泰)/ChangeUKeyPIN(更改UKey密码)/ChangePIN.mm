//
//  ChangePIN.m
//  Ukeytools
//
//  Created by 范云飞 on 2017/6/29.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "ChangePIN.h"
#import "ChangePINFunc.h"
#import "HexStringtoByte.h"
#import "Constants.h"


//Tool
#import "NSString+conversion.h"

@implementation ChangePIN

- (instancetype)initChangePinOldpin:(NSString *)oldPin Newpin:(NSString *)newPin{
    self = [super init];
    if (self) {

        NSString * OCString1 = [NSString stringWithFormat:@"%@",[NSString convertStringToHexStr:oldPin]];
        
        NSString * OCString2 = [NSString stringWithFormat:@"%@",[NSString convertStringToHexStr:newPin]];
        
        const char *cString1 = [OCString1 cStringUsingEncoding:NSUTF8StringEncoding];
        const char *cString2 = [OCString2 cStringUsingEncoding:NSUTF8StringEncoding];
        
        char * charStr1;
        char * charStr2;
        charStr1 = new char[strlen(cString1)+1];
        strcpy(charStr1, cString1);
        
        charStr2 = new char[strlen(cString2)+1];
        strcpy(charStr2, cString2);

        unsigned char temp1[100] = {0};
        unsigned char temp2[100] = {0};

        hexstringtobyte(charStr1, temp1);
        for (int i = 0; i < 100; i++) {
            printf("%d", temp1[i]);
        }
        printf("\n");
        
        hexstringtobyte(charStr2, temp2);
        for (int i = 0; i < 100; i++) {
            printf("%d", temp2[i]);
        }
        printf("\n");

        ChangePinFunc(temp1,temp2);
    }
    return self;
}

@end
