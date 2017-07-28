//
//  ChangePIN.h
//  Ukeytools
//
//  Created by 范云飞 on 2017/6/29.
//  Copyright © 2017年 nist. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ChangePIN : NSObject
@property (copy, nonatomic) NSString * OldPIN;
@property (copy, nonatomic) NSString * NewPIN;
- (instancetype)initChangePinOldpin:(NSString *)oldPin
                             Newpin:(NSString *)newPin;
@end
