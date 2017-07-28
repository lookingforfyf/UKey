//
//  HextoInt.cpp
//  UKey管理工具(国信安泰)
//
//  Created by 范云飞 on 2017/6/30.
//  Copyright © 2017年 nist. All rights reserved.
//

#include "HextoInt.h"
int HextoInt(char * str){
    int t;
    long sum=0;
    for(int i=0;str[i];i++){
        if(str[i]<='9')
            t=str[i]-'0';
        else
            t=str[i]-'A'+10;
        sum=sum*16+t;
    }
    return (int)sum;
}
