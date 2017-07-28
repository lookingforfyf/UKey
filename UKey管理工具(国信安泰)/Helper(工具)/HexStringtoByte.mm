//
//  HexStringtoByte.cpp
//  UKey管理工具(国信安泰)
//
//  Created by 范云飞 on 2017/6/30.
//  Copyright © 2017年 nist. All rights reserved.
//

#include "HexStringtoByte.h"

void hexstringtobyte(char *in, unsigned char *out);
void hexstringtobyte(char *in, unsigned char *out) {
    int len = (int)strlen(in);
    char *str = (char *)malloc(len);
    memset(str, 0, len);
    memcpy(str, in, len);
    for (int i = 0; i < len; i+=2) {
        //小写转大写
        if(str[i] >= 'a' && str[i] <= 'f') str[i] = str[i] & ~0x20;
        if(str[i+1] >= 'a' && str[i] <= 'f') str[i+1] = str[i+1] & ~0x20;
        //处理第前4位
        if(str[i] >= 'A' && str[i] <= 'F')
            out[i/2] = (str[i]-'A'+10)<<4;
        else
            out[i/2] = (str[i] & ~0x30)<<4;
        //处理后4位, 并组合起来
        if(str[i+1] >= 'A' && str[i+1] <= 'F')
            out[i/2] |= (str[i+1]-'A'+10);
        else
            out[i/2] |= (str[i+1] & ~0x30);
    }
    free(str);
}
