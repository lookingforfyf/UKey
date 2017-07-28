//
//  CustomView.h
//  UKey管理工具(国信安泰)
//
//  Created by 范云飞 on 2017/7/13.
//  Copyright © 2017年 nist. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
@interface CustomView : NSTableRowView
@property (copy, nonatomic) NSString * content;
@property (strong, nonatomic) NSTextField * text;
- (instancetype)initWithFrame:(NSRect)frameRect;
@end

