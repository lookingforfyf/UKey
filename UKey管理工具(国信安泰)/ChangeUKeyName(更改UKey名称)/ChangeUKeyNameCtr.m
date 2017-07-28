//
//  ChangeUKeyNameCtr.m
//  Ukeytools
//
//  Created by wuyangfan on 17/5/27.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "ChangeUKeyNameCtr.h"
#import "ChangeName.h"//修改UKey名称
@interface ChangeUKeyNameCtr ()<NSTextFieldDelegate>

@property (weak) IBOutlet NSTextField *labName;
@property (weak) IBOutlet NSButton *btnOK;
@property (weak) IBOutlet NSButton *btnCancel;
@property (weak) IBOutlet NSTextField *tfName;

@end

@implementation ChangeUKeyNameCtr

#pragma mark - Life

- (void)windowDidLoad {
    [super windowDidLoad];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(doCloseWindow) name:NSWindowWillCloseNotification object:nil];
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    self.tfName.delegate = self;
}

-(void)setLanguage:(BOOL)isCN{
    if(isCN){
        self.labName.stringValue = @"新UKey名称(最大16个字符)";
        self.btnOK.title = @"确定";
        self.btnCancel.title = @"取消";
        self.window.title = @"更改UKey名称";
    }else{
        self.labName.stringValue = @"New UKey Name(16 Characters Maximun)";
        self.btnOK.title = @"OK";
        self.btnCancel.title = @"Cancel";
        self.window.title = @"Change UKey Name";
    }
}

- (IBAction)doCancel:(id)sender {
    [self.window close];
}

- (IBAction)doOK:(id)sender {
    if (self.tfName.stringValue.length>0) {
        ChangeName * chaneName = [[ChangeName alloc]initChangeName:self.tfName.stringValue];
        NSLog(@"***********%@*********",chaneName);
    }
    else{
        NSAlert *alert = [[NSAlert alloc] init];
        alert.alertStyle = NSAlertStyleWarning;
        [alert addButtonWithTitle:@"确定"];
        [alert addButtonWithTitle:@"取消"];
        alert.messageText = @"提示";
        alert.informativeText = @"你的新UKey名称还未设置";
        [alert runModal];
    }
    //  [self.window close];
}

-(void)doCloseWindow{
    [[NSApplication sharedApplication] stopModal];
}


#pragma mark - Private

-(void)controlTextDidChange:(NSNotification *)obj {
    int PasswordMaxLimit = 16;//限制密码设置最大长度
    if (self.tfName == obj.object) {
        if ([[self.tfName stringValue] length] > PasswordMaxLimit) {
            [self.tfName setStringValue:[[self.tfName stringValue] substringToIndex:PasswordMaxLimit]];
            NSAlert *alert = [[NSAlert alloc] init];
            alert.alertStyle = NSAlertStyleWarning;
            [alert addButtonWithTitle:@"确定"];
            [alert addButtonWithTitle:@"取消"];
            alert.messageText = @"提示";
            alert.informativeText = @"你的新UKey名称设置过长";
            [alert runModal];
        }
    }
}


@end
