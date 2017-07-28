//
//  ChangeName.m
//  Ukeytools
//
//  Created by 范云飞 on 2017/7/3.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "ChangeName.h"
#import "ChangeNameFunc.h"
#import "Constants.h"

//Tool
#import "NSString+conversion.h"
#import "HexStringtoByte.h"

@implementation ChangeName
- (instancetype)initChangeName:(NSString *)name{
    self = [super init];
    if (self) {
        NSString * OCString = [NSString stringWithFormat:@"%@",[NSString convertStringToHexStr:name]];
        
        const char *cString = [OCString cStringUsingEncoding:NSUTF8StringEncoding];
        
        char * charStr;
        charStr = new char[strlen(cString)+1];
        strcpy(charStr, cString);
        
        unsigned char temp[100] = {0};
        hexstringtobyte(charStr, temp);
        for (int i = 0; i < 100; i++) {
            printf("%d", temp[i]);
        }
        printf("\n");
        
        ChangeNameFunc(temp);

    }
    return self;
}
@end
