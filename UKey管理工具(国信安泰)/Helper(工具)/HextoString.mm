//
//  HextoString.cpp
//  UKey管理工具(国信安泰)
//
//  Created by 范云飞 on 2017/6/30.
//  Copyright © 2017年 nist. All rights reserved.
//

#include "HextoString.h"
char a2x(char ch);

char * HexToString(char * str1 ,char * str2){
    int i;
    
    i = 0;
    while(str1[i])
    {
        str2[i >> 1] = (a2x(str1[i]) << 4) | a2x(str1[i + 1]);
        i += 2;
    }
    str2[i >> 1] = '\0';
    return str2;
}

char a2x(char ch)
{
    switch(ch)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
        case 'a':
            return 10;
        case 'B':
        case 'b':
            return 11;
        case 'C':
        case 'c':
            return 12;
        case 'D':
        case 'd':
            return 13;
        case 'E':
        case 'e':
            return 14;
        case 'F':
        case 'f':
            return 15;
        default:
            break;;
    }
    
    return 0;
}


NSMutableString * HexToChar(char * str){
    
//    char st[]="39323030303031360000000000000000000000000000000000000000000000006D6F62696C65206B657900000000000000000000000000000000000000000000";
    int ch[strlen(str)/2];
    char c[strlen(str)/2];
    NSMutableString * resultString = [[NSMutableString alloc]init];
    printf("十六进制 十进制   字符\n");
    for(int i=0;i<strlen(str);i=i+2)
    {
        
        ch[i/2]=((str[i]<64?str[i]-48:str[i]-55)*16)+(str[i+1]<64?str[i+1]-48:str[i+1]-55);
        c[i/2]=ch[i/2];
        printf("  %c%c   ",str[i],str[i+1]);
        printf("  %5d",ch[i/2]);
        printf("  %5c\n",c[i/2]);
        
        [resultString appendString:[NSString stringWithFormat:@"%c",c[i/2]]];
        
    }
        NSLog(@"########################%@#########################",resultString);

    return resultString;
}
