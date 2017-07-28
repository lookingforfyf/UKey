//
//  GetDeviceInfo.m
//  Ukeytools
//
//  Created by 范云飞 on 2017/6/26.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "GetDeviceInfo.h"
#import "GetDeviceInfoFunc.h"
#import "Constants.h"


@implementation GetDeviceInfo

- (instancetype)initDeviceInfo{
    self = [super init];
    if (self) {
        self.array = getDeviceInfo();
    }
    return self;
}
@end
