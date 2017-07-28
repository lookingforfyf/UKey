//
//  ViewController.m
//  Ukeytools
//
//  Created by wuyangfan on 17/5/25.
//  Copyright © 2017年 nist. All rights reserved.
//

#import "ViewController.h"
#import <SecurityInterface/SFCertificateView.h>
#import <SecurityInterface/SFCertificatePanel.h>
//NSWindow
#import "ChangePinCtr.h"
#import "UKeyInfoCtr.h"
#import "ChangeUKeyNameCtr.h"

//NSView
#import "CustomView.h"
//调用方法
#import "GetDeviceInfo.h" //读取设备列表
//#import "LibrarySingleton.h"

@interface ViewController()<NSTableViewDelegate,NSTableViewDataSource,NSComboBoxDataSource,NSComboBoxDelegate,NSTextFieldDelegate,NSTabViewDelegate>
{
    BOOL isCN;
    void *hLibrary;
    void *pTemp;
}
@property (weak) IBOutlet NSTableView *tableView;
@property (weak) IBOutlet NSTabView *tabView;
@property (weak) IBOutlet NSPopUpButton *lanSelect;
@property (weak) IBOutlet NSBox *box;

@property (weak) IBOutlet NSComboBox *comboBox;

#pragma mark UKey tab
@property (weak) IBOutlet NSButton *btnChangePin;
@property (weak) IBOutlet NSButton *btnChangeName;
@property (weak) IBOutlet NSButton *btnUKeyInfo;

#pragma mark topbar
@property (weak) IBOutlet NSTextField *labSelectUKey;
@property (weak) IBOutlet NSTextField *labSelectLan;

#pragma mark Exit button
@property (weak) IBOutlet NSButton *btnExit;


#pragma mark 证书tab

//@property (weak) IBOutlet NSTableColumn *certType;
@property (weak) IBOutlet NSTableColumn *certFrom;
@property (weak) IBOutlet NSTableColumn *certTo;
@property (weak) IBOutlet NSTableColumn *certBegin;
@property (weak) IBOutlet NSTableColumn *certEnd;
@property (weak) IBOutlet NSButton *btnShowCert;
@property (weak) IBOutlet NSButton *btnDelCert;

#pragma mark 关于tab
@property (weak) IBOutlet NSTextField *companyName;
@property (weak) IBOutlet NSTextField *ukeyTool;
@property (weak) IBOutlet NSTextField *copyright;

#pragma mark NSComboBox 数据源
@property (strong, nonatomic) NSMutableArray <NSString*>* comBoxArray;
@property (copy, nonatomic) NSString * comboBoxString;//获取选中的值

#pragma mark 证书数据源

@property (strong, nonatomic) NSMutableArray <NSString *>* certificateArray;

@end

@implementation ViewController

#pragma mark - Life

- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableView.doubleAction = @selector(doubleClick);
    // Do any additional setup after loading the view.
    
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(closeWindow) name:NSWindowWillCloseNotification object:nil];
    
    self.tableView.delegate = self;
    self.tableView.dataSource = self;
    [self.tabView selectTabViewItemAtIndex:0];
    
    self.tabView.delegate = self;
    isCN = YES;
    [self initLib];
    
    
    
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}

#pragma mark - init
-(void)initLib{
    self.certFrom.identifier = @"Subject";
    self.certTo.identifier = @"Issue";
    self.certBegin.identifier = @"Start time";
    self.certEnd.identifier = @"End time";
    
    
#pragma mark ------------------- 获取设备的SN和LABEL--------------------
    
    
    GetDeviceInfo * deviceInfo = [[[GetDeviceInfo alloc]init]initDeviceInfo];
    if (deviceInfo.array.count==0) {
        self.btnChangePin.enabled = NO;
        self.btnUKeyInfo.enabled = NO;
        self.btnChangeName.enabled = NO;
    }
    else{
        self.btnChangePin.enabled = YES;
        self.btnUKeyInfo.enabled = YES;
        self.btnChangeName.enabled = YES;
        
        // NSComboBox
        
        NSString * str = [NSString stringWithFormat:@"%@%@",[deviceInfo.array objectAtIndex:0],[deviceInfo.array objectAtIndex:1]];
        
        self.comBoxArray = [NSMutableArray arrayWithObject:str];
        self.comboBox.delegate = self;
        self.comboBox.dataSource = self;
        [self.comboBox selectItemAtIndex:0];
    }
    
}

- (IBAction)changeLan:(id)sender {
    if(isCN && _lanSelect.selectedTag == 1){
        //如果是中文，选择英文，那么修改为英文
        isCN = NO;
        [self doChangeLan];
        return;
    }
    if(!isCN && _lanSelect.selectedTag == 0){
        //当前是英文，但是选择了中文
        isCN = YES;
        [self doChangeLan];
    }
}

-(void)doChangeLan{
    if(isCN){
        //顶部按钮
        self.labSelectUKey.stringValue = @"选择UKey";
        self.labSelectLan.stringValue = @"选择语言";
        
        //退出按钮
        self.btnExit.title = @"退出";
        
        //tab title
        [self.tabView tabViewItemAtIndex:0].label = @"UKey";
        [self.tabView tabViewItemAtIndex:1].label = @"证书";
        [self.tabView tabViewItemAtIndex:2].label = @"关于";
        
        //ukey tab
        self.box.title = @"UKey操作";
        self.btnChangePin.title = @"更改UKey密码";
        self.btnChangeName.title = @"更改UKey名称";
        self.btnUKeyInfo.title = @"查看UKey信息";
        
        //cert tab
        //        self.certType.title = @"证书类型";
        self.certFrom.title = @"持有者";
        self.certTo.title = @"颁发者";
        self.certBegin.title = @"起始日期";
        self.certEnd.title = @"截止日期";
        self.btnDelCert.title = @"删除证书";
        self.btnShowCert.title = @"查看证书";
        [self.tableView reloadData];
        
        //about tab
        self.ukeyTool.stringValue = @"UKey管理工具V1.0";
        self.companyName.stringValue = @"国信安泰（武汉）科技有限公司";
    }else{
        
        //顶部按钮
        self.labSelectUKey.stringValue = @"UKey";
        self.labSelectLan.stringValue = @"Languages";
        
        //退出按钮
        self.btnExit.title = @"Exit";
        
        //tab title
        [self.tabView tabViewItemAtIndex:0].label = @"UKey";
        [self.tabView tabViewItemAtIndex:1].label = @"Certificate";
        [self.tabView tabViewItemAtIndex:2].label = @"About";
        
        //ukey tab
        self.box.title = @"UKey Operation";
        self.btnChangePin.title = @"Change UKey\nPassword";
        self.btnChangeName.title = @"Change UKey Name";
        self.btnUKeyInfo.title = @"Show UKey Info";
        
        //cert tab
        //        self.certType.title = @"Type";
        self.certFrom.title = @"Subject";
        self.certTo.title = @"Issue";
        self.certBegin.title = @"Start time";
        self.certEnd.title = @"End time";
        self.btnDelCert.title = @"Delete Certificate";
        self.btnShowCert.title = @"Show Certificate";
        [self.tableView reloadData];
        
        //about tab
        self.ukeyTool.stringValue = @"UKey Tool V1.0";
        self.companyName.stringValue = @"NIST Co.,Ltd.";
        
    }
}

#pragma mark - Target
-(void)closeWindow{
    NSLog(@"closeWindow");
    [[NSApplication sharedApplication]stopModalWithCode:1];
}

-(void)doubleClick{
    NSLog(@"doubleClick row:%ld",(long)self.tableView.selectedRow);
    
}

- (IBAction)delCert:(id)sender {
    NSInteger row = self.tableView.selectedRow;
    if(row>=0 && row<self.tableView.numberOfRows){
        
    }
}
- (IBAction)showCert:(id)sender {
    NSString *thePath = [[NSBundle mainBundle]
                         pathForResource:@"bank" ofType:@"der"];
    //NSData *data = [[NSData alloc]initWithContentsOfFile:thePath];
    NSData *data = [NSData dataWithContentsOfFile:thePath];
    
    SecCertificateRef ref = SecCertificateCreateWithData(NULL, (__bridge CFDataRef)data);
    [[SFCertificatePanel sharedCertificatePanel] runModalForCertificates:[NSArray arrayWithObject:(__bridge id _Nonnull)(ref)] showGroup:NO];
}

- (IBAction)exitApp:(id)sender {
    [[NSApplication sharedApplication] stop:nil];
}

- (IBAction)changePin:(id)sender {
    ChangePinCtr *cpc = [[ChangePinCtr alloc]initWithWindowNibName:@"ChangePinCtr"];
    [cpc.window center];
    [cpc setLan:_lanSelect.selectedTag==0];
    [[NSApplication sharedApplication] runModalForWindow:cpc.window];
}

- (IBAction)changeName:(id)sender {
    ChangeUKeyNameCtr *cpc = [[ChangeUKeyNameCtr alloc]initWithWindowNibName:@"ChangeUKeyNameCtr"];
    [cpc.window center];
    [cpc setLanguage:_lanSelect.selectedTag==0];
    [[NSApplication sharedApplication] runModalForWindow:cpc.window];
}
- (IBAction)showUKeyInfo:(id)sender {
    UKeyInfoCtr *cpc = [[UKeyInfoCtr alloc]initWithWindowNibName:@"UKeyInfoCtr"];
    [cpc.window center];
    [cpc setLanguage:_lanSelect.selectedTag==0];
    [[NSApplication sharedApplication] runModalForWindow:cpc.window];
}

#pragma mark - NSTableViewDataSourceDelegate
-(NSInteger)numberOfRowsInTableView:(NSTableView *)tableView{
    return 1;
}

-(id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row{
    return @"";
}

- (NSTableRowView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row{
    NSString * Id = @"cellId";
    CustomView * view = [tableView makeViewWithIdentifier:Id owner:self];
    if (!view) {
        view = [[CustomView alloc]initWithFrame:CGRectMake(0, 0, tableColumn.width, 50)];
    }
    if ([tableColumn.identifier isEqualToString:@"Subject"]) {
        view.content = [self.certificateArray objectAtIndex:0];
    }
    else if ([tableColumn.identifier isEqualToString:@"Issue"]) {
        view.content = [self.certificateArray objectAtIndex:1];
    }
    else if ([tableColumn.identifier isEqualToString:@"Start time"]) {
        view.content = [self.certificateArray objectAtIndex:3];
    }
    else{
        view.content = [self.certificateArray objectAtIndex:2];
    }
    
    return view;
}

- (CGFloat)tableView:(NSTableView *)tableView heightOfRow:(NSInteger)row{
    return 40;
}

- (void)tableViewSelectionDidChange:(NSNotification *)notification{
    NSTableView * tableView = notification.object;
    NSInteger index = tableView.selectedRow;
    NSLog(@"***********%ld*********",(long)index);
}

- (BOOL)tableView:(NSTableView *)tableView shouldSelectRow:(NSInteger)row{
    return YES;
}

- (BOOL)tableView:(NSTableView *)tableView shouldSelectTableColumn:(NSTableColumn *)tableColumn{
    return YES;
}

#pragma mark - NSComboBoxDelegate
- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)aComboBox {
    return self.comBoxArray.count;
}

- (id)comboBox:(NSComboBox *)aComboBox objectValueForItemAtIndex:(NSInteger)loc {
    return self.comBoxArray[loc];
}

/**
 获取选中的UKey的值
 */
- (void)comboBoxSelectionDidChange:(NSNotification *)notification{
    NSLog(@"***********%@*********",[(NSComboBox *)(notification.object) objectValue]);
}

#pragma mark - NSTabViewDelegate
- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(nullable NSTabViewItem *)tabViewItem{
    if ([tabView indexOfTabViewItem:tabViewItem]==0) {
        NSLog(@"***********%@*********",@"0");
    }
    else if ([tabView indexOfTabViewItem:tabViewItem]==1){
        
        NSString *thePath = [[NSBundle mainBundle]
                             pathForResource:@"bank" ofType:@"der"];
        NSData *data = [NSData dataWithContentsOfFile:thePath];
        
        SecCertificateRef ref = SecCertificateCreateWithData(NULL, (__bridge CFDataRef)data);
        
        self.certificateArray = (NSMutableArray *) [self stringFromCerificateWithLongwindedDescription:ref];
        NSLog(@"***********%@*********",[self stringFromCerificateWithLongwindedDescription:ref]);
        [self.tableView reloadData];
    }
    else if ([tabView indexOfTabViewItem:tabViewItem]==2){
        NSLog(@"***********%@*********",@"2");
    }
}

#pragma mark - reload
- (IBAction)comboBox:(NSComboBox *)sender {
    [self.comboBox reloadData];
}

#pragma mark - settter

- (NSMutableArray *)certificateArray{
    if (!_certificateArray) {
        _certificateArray = [[NSMutableArray alloc]init];
    }
    return _certificateArray;
}

- (NSMutableArray *)comBoxArray{
    if (!_comBoxArray) {
        _comBoxArray = [[NSMutableArray alloc]init];
    }
    return _comBoxArray;
}

#pragma mark - 解析证书

- (NSArray*)stringFromCerificateWithLongwindedDescription:(SecCertificateRef) certificateRef {
    
    NSMutableArray * certificateArray = [[NSMutableArray alloc]init];
    
    if (certificateRef == NULL)
        return @"";
    NSTimeInterval time = [[NSDate date] timeIntervalSince1970];
    
    NSDateFormatter* fomat = [[NSDateFormatter alloc] init];
    [fomat setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSLog(@"当前时间:%f",time);
    CFStringRef commonNameRef;
    OSStatus status;
    if ((status=SecCertificateCopyCommonName(certificateRef, &commonNameRef)) != errSecSuccess) {
        NSLog(@"Could not extract name from cert: %@",
              SecCopyErrorMessageString(status, NULL));
        return @"Unreadable cert";
    };
    
    CFStringRef summaryRef = SecCertificateCopySubjectSummary(certificateRef);
    if (summaryRef == NULL)
        summaryRef = CFRetain(commonNameRef);
    NSLog(@"-->%@",summaryRef);
    CFErrorRef error;
    
    const void *keys[] = { kSecOIDX509V1SubjectName, kSecOIDX509V1IssuerName,kSecOIDX509V1ValidityNotAfter,kSecOIDX509V1ValidityNotBefore };
    const void *labels[] = { "Subject", "Issuer" };
    CFArrayRef keySelection = CFArrayCreate(NULL, keys , sizeof(keys)/sizeof(keys[0]), &kCFTypeArrayCallBacks);
    
    CFDictionaryRef vals = SecCertificateCopyValues(certificateRef, keySelection,&error);
    NSMutableString *longDesc = [[NSMutableString alloc] init];
    
    for(int i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        CFDictionaryRef dict = CFDictionaryGetValue(vals, keys[i]);
        CFArrayRef values = CFDictionaryGetValue(dict, kSecPropertyKeyValue);
        
        NSDictionary  *subdic = (__bridge_transfer  NSDictionary*)dict;
        if ([[subdic objectForKey:@"label"] isEqualToString:@"Subject Name"] ) {
            NSArray * array = [[subdic objectForKey:@"value"] copy];
            
            NSString * subjectName = [(NSDictionary *)([array objectAtIndex:9]) objectForKey:@"value"];
            [certificateArray addObject:subjectName];
        }
        if ([[subdic objectForKey:@"label"] isEqualToString:@"Issuer Name"] ) {
            NSArray * array = [[subdic objectForKey:@"value"] copy];
            
            NSString * subjectName = [(NSDictionary *)([array objectAtIndex:2]) objectForKey:@"value"];
            [certificateArray addObject:subjectName];
        }
        
        NSLog(@"***********%@*********",dict);
        if (values == NULL)
            continue;
        if(CFArrayGetTypeID() != CFGetTypeID(values)){
            double time =((__bridge NSString*)values).doubleValue;
            NSDate* date = [NSDate dateWithTimeIntervalSinceReferenceDate:time];
            if ([[subdic objectForKey:@"label"] isEqualToString:@"Not Valid After"] ) {
                [certificateArray addObject:[fomat stringFromDate:date]];
            }
            if ([[subdic objectForKey:@"label"] isEqualToString:@"Not Valid Before"] ) {
                [certificateArray addObject:[fomat stringFromDate:date]];
            }
            NSLog(@"%@,time=%f",[fomat stringFromDate:date],time);
            continue;
        }
        [self printCFArray:values];
        //        [longDesc appendFormat:@"%s:%@\n\n", labels[i], [self stringFromDNwithSubjectName:values]];
    }
    
    //    CFRelease(vals);
    //    CFRelease(summaryRef);
    //    CFRelease(commonNameRef);
    
    return certificateArray;
    //    return longDesc;
}

-(void)printCFArray:(CFArrayRef) arr{
    CFIndex count = CFArrayGetCount(arr);
    NSLog(@"开始print:%ld---------->",count);
    for(CFIndex n=0;n<count;n++){
        CFDictionaryRef dict = CFArrayGetValueAtIndex(arr, n);
        if(CFGetTypeID(dict) != CFDictionaryGetTypeID()) continue;
        CFStringRef str = CFDictionaryGetValue(dict, kSecPropertyKeyValue);
        NSLog(@"%@",(__bridge NSString*)str);
        
    }
    NSLog(@"");
    NSLog(@"");
    
}



@end
