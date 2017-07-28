//
//  ChangePinCtr.m
//  Ukeytools
//
//  Created by wuyangfan on 17/5/26.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "ChangePinCtr.h"

#import "ChangePIN.h"//修改UKey密码

@interface ChangePinCtr ()<NSTextFieldDelegate>
{
    BOOL isOpenKeyboard;
    BOOL isUpper;
    BOOL isCN;
}
@property (weak) IBOutlet NSButton *btnSoftKeyboard;
@property (weak) IBOutlet NSButton *btnOk;
@property (weak) IBOutlet NSButton *btnCancel;
@property (weak) IBOutlet NSButton *btnSoftBack;
@property (weak) IBOutlet NSButton *btnSoftClear;
@property (weak) IBOutlet NSButton *btnSoftLock;
@property (weak) IBOutlet NSButton *btnSoftOk;
@property (weak) IBOutlet NSButton *btnSoftExit;


@property (weak) IBOutlet NSTextField *labNew11;
@property (weak) IBOutlet NSTextField *labNew;
@property (weak) IBOutlet NSTextField *labOld;
@property (weak) IBOutlet NSSecureTextField *tfNewPin2;
@property (weak) IBOutlet NSSecureTextField *tfNewPin;
@property (weak) IBOutlet NSSecureTextField *tfOldPin;
@property (weak) IBOutlet NSView *buttonViews;

@property (copy, nonatomic) NSString * password;
@end



@implementation ChangePinCtr

#pragma mark - Life

- (void)windowDidLoad {
    [super windowDidLoad];
    
    isOpenKeyboard = YES;
    [self openKeyBoard:nil];
    isUpper = NO;
    
    //添加事件
    [self addClickAction];
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(doCloseWindow) name:NSWindowWillCloseNotification object:nil];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    [self initData];
}

#pragma mark - init

- (void)initData{
    self.tfOldPin.delegate = self;
    self.tfNewPin.delegate = self;
    self.tfNewPin2.delegate = self;
    self.password = @"123456789";
}


-(void)setLan:(BOOL)cn{
    isCN = cn;
    if(isCN){
        self.labOld.stringValue = @"原UKey密码";
        self.labNew.stringValue = @"新UKey密码";
        self.labNew11.stringValue = @"再次输入";
        self.btnSoftKeyboard.title = @"软键盘";
        self.btnOk.title = @"确定";
        self.btnCancel.title = @"取消";
        self.btnSoftBack.title =@"退格";
        self.btnSoftLock.title = isUpper?@"小写":@"大写";
        self.btnSoftOk.title =@"确定";
        self.btnSoftExit.title = @"退出";
        self.btnSoftClear.title = @"清除";
        self.window.title = @"更改UKey密码";
        
    }else{
        self.labOld.stringValue = @"Old Password";
        self.labNew.stringValue = @"New Password";
        self.labNew11.stringValue = @"Enter Again";
        self.btnSoftKeyboard.title = @"Soft\nKeyboard";
        self.btnOk.title = @"OK";
        self.btnCancel.title = @"Cancel";
        self.btnSoftBack.title =@"Back";
        self.btnSoftLock.title = isUpper?@"Lowerc":@"Capital";
        self.btnSoftOk.title =@"OK";
        self.btnSoftExit.title = @"Quit";
        self.btnSoftClear.title = @"Clear";
        self.window.title = @"Change UKey Passowrd";
    }
}

- (IBAction)closeWindow:(id)sender {
    
    [self.window close];
    
    //[[NSApplication sharedApplication]stopModal];
}
- (IBAction)openKeyBoard:(id)sender {
    if(isOpenKeyboard){
        //表示已经打开了，关闭
        CGSize size = CGSizeMake(400, 345);
        NSRect rect = self.window.frame;
        rect.size =size;
        [self.window setFrame:rect display:NO animate:NO];
    }else{
        //已经关闭了，打开
        CGSize size = CGSizeMake(694, 565);
        
        NSRect rect = self.window.frame;
        rect.size =size;
        [self.window setFrame:rect display:NO animate:NO];
        [self randomDigitAndLetter];
    }
    isOpenKeyboard = !isOpenKeyboard;
}

/**每次展开软键盘，改变数字和字母的顺序*/
-(void)randomDigitAndLetter{
    //大小写维持不变
    
    NSString *digit = @"0123456789";
    
    NSMutableArray *arrDigit = [NSMutableArray array];
    for(int i=0;i<digit.length;i++){
        [arrDigit addObject:[NSString stringWithFormat:@"%c",[digit characterAtIndex:i]]];
    }
    
    NSButton *btn = nil;
    for(NSView *view in _buttonViews.subviews){
        if([view isKindOfClass:[NSButton class]]){
            btn = (NSButton*)view;
            if([view.identifier isEqualToString:@"digit"]){
                int r = (int)arc4random_uniform((int)arrDigit.count);
                btn.title = arrDigit[r];
                [arrDigit removeObjectAtIndex:r];
            }
        }
    }
    [self changeUp];
}

-(void)changeUp{
    NSString *letter = @"abcdefghijklmnopqrstuvwlyz";
    NSMutableArray *arrLetter = [NSMutableArray array];
    for(int i=0;i<letter.length;i++){
        
        [arrLetter addObject:[NSString stringWithFormat:@"%c",[letter characterAtIndex:i]]];
    }
    
    NSButton *btn = nil;
    for(NSView *view in _buttonViews.subviews){
        if([view isKindOfClass:[NSButton class]]){
            btn = (NSButton*)view;
            if([view.identifier isEqualToString:@"letter"]){
                int r = (int)arc4random_uniform((int)arrLetter.count);
                if(isUpper){
                    btn.title = [((NSString*)arrLetter[r]) uppercaseString] ;
                }else{
                    btn.title = arrLetter[r];
                }
                
                [arrLetter removeObjectAtIndex:r];
            }
        }
    }
}

-(void)addClickAction{
    for(NSView *view in _buttonViews.subviews){
        if([view isKindOfClass:[NSButton class]]){
            [((NSButton*)view) setAction:@selector(keyboardButtonClick:)];
        }
    }
}

-(void)keyboardButtonClick:(id)sender{
    NSButton *btn = (NSButton*) sender;
    NSTextField *tf = [self getFocusView];
    if(btn.identifier && btn.identifier.length>0){
        
        if([@"other" isEqualToString:btn.identifier] || [@"digit" isEqualToString:btn.identifier]
           || [@"letter" isEqualToString:btn.identifier]){
            if(tf){
                NSString *value = [NSString stringWithFormat:@"%@%@",tf.stringValue,btn.title];
                tf.stringValue = value;
            }
            return;
        }
        
        if([@"back" isEqualToString:btn.identifier]){
            if(tf && tf.stringValue && tf.stringValue.length>0){
                tf.stringValue = [tf.stringValue substringToIndex:tf.stringValue.length-1];
            }
            return;
        }
        if([@"clear" isEqualToString:btn.identifier]){
            if(tf){
                tf.stringValue = @"";
            }
            return;
        }
        if([@"up" isEqualToString:btn.identifier]){
            isUpper = !isUpper;
            [self changeUp];
            if(isUpper){
                btn.title = isCN?@"小写":@"Lowerc";
                //btn.title = @"小写";
            }else{
                //                btn.title = @"大写";
                btn.title = isCN?@"大写":@"Capital";
            }
            return;
        }if([@"ok" isEqualToString:btn.identifier]){
            
        }if([@"exit" isEqualToString:btn.identifier]){
            
        }
    }
}


- (IBAction)sureBtn:(NSButton *)sender {
    if (self.tfOldPin.stringValue.length !=0) {
        if ([self.tfNewPin.stringValue isEqualToString:self.tfNewPin2.stringValue]) {
            ChangePIN * changePin = [[ChangePIN alloc]initChangePinOldpin:self.tfOldPin.stringValue Newpin:self.tfNewPin.stringValue];
            
            NSLog(@"***********%@*********",changePin);
            //            [[NSApplication sharedApplication]stopModal];
            
        }
        else{
            NSAlert *alert = [[NSAlert alloc] init];
            alert.alertStyle = NSAlertStyleWarning;
            [alert addButtonWithTitle:@"确定"];
            [alert addButtonWithTitle:@"取消"];
            alert.messageText = @"提示";
            alert.informativeText = @"你前后输入的新密码不一致";
            [alert runModal];
        }
    }
    else{
        NSAlert *alert = [[NSAlert alloc] init];
        alert.alertStyle = NSAlertStyleWarning;
        [alert addButtonWithTitle:@"确定"];
        [alert addButtonWithTitle:@"取消"];
        alert.messageText = @"提示";
        alert.informativeText = @"请输入密码";
        [alert runModal];
    }
}

-(NSSecureTextField*)getFocusView{
    if(_tfOldPin.currentEditor) return _tfOldPin;
    if(_tfNewPin.currentEditor) return _tfNewPin;
    if(_tfNewPin2.currentEditor) return _tfNewPin2;
    return nil;
}

-(void)doCloseWindow{
    [[NSApplication sharedApplication]stopModal];
}

#pragma mark - Private

-(void)controlTextDidChange:(NSNotification *)obj {
    int PasswordMaxLimit = 16;//限制密码设置最大长度
    if (self.tfOldPin == obj.object) {
    }
    if (self.tfNewPin == obj.object) {
        if ([[self.tfNewPin stringValue] length] > PasswordMaxLimit) {
            [self.tfNewPin setStringValue:[[self.tfNewPin stringValue] substringToIndex:PasswordMaxLimit]];
            NSAlert *alert = [[NSAlert alloc] init];
            alert.alertStyle = NSAlertStyleWarning;
            [alert addButtonWithTitle:@"确定"];
            [alert addButtonWithTitle:@"取消"];
            alert.messageText = @"提示";
            alert.informativeText = @"你的新密码输入过长";
            [alert runModal];
        }
        NSLog(@"***********%@*********",@"2");
    }
    if (self.tfNewPin2 == obj.object) {
        NSLog(@"***********%@*********",@"3");
    }
}

//-(void) controlTextDidEndEditing:(NSNotification *)obj {
//    int PasswordMinLimit = 6;
//    if (obj.object == self.tfOldPin) {
//        //currentPasswordTextField你要限制字符的输入框  还有的话继续添加
//        if ([[self.tfOldPin stringValue] length] < PasswordMinLimit) {
//            self.tfOldPin.stringValue = @"The number of characters less than the minimum limit!";  //提示框提示内容
//            self.tfOldPin.hidden = NO;  //提示框  看需要加不加
//            return;
//        }
//        else
//        {
//            self.tfOldPin.hidden = YES;
//        }
//    }
//}

@end
