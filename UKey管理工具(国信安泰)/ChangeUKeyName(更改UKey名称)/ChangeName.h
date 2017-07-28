//
//  ChangeName.h
//  Ukeytools
//
//  Created by 范云飞 on 2017/7/3.
//  Copyright © 2017年 nist. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ChangeName : NSObject
@property (copy, nonatomic) NSString * name;
- (instancetype)initChangeName:(NSString *)name;
@end
