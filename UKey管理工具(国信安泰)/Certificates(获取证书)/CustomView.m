//
//  CustomView.m
//  Ukeytools
//
//  Created by 范云飞 on 2017/7/13.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "CustomView.h"

@implementation CustomView

- (instancetype)initWithFrame:(NSRect)frameRect{
    self = [super initWithFrame:frameRect];
    if (self) {
        [self setUI];
    }
    return self;
}

- (void)setUI{
    self.text = [[NSTextField alloc]initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
    self.text.bordered = NO;
    self.text.font = [NSFont systemFontOfSize:10];
    self.text.editable = NO;
    self.text.backgroundColor = [NSColor clearColor];
    [self addSubview:self.text];
    
}

- (void)setContent:(NSString *)content{
    self.text.stringValue = content;
}


@end
