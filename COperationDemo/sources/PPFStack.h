//
//  PPFStack.h
//  COperationDemo
//
//  Created by 潘鹏飞 on 2019/9/3.
//  Copyright © 2019 潘鹏飞. All rights reserved.
//

#ifndef PPFStack_h
#define PPFStack_h

#include <stdio.h>


typedef struct Data {
    /// 字符串头一个
    char opertor;
    /// 数字
    double number;
    /// 类型 1:字符串  2:表示浮点
    int type;
}StackData;

/**
 一个节点用于存数据
 */
typedef struct Node Node;

/// 栈
typedef struct {
    /**
     开头
     */
    Node *begin;
    /// 结尾
    Node *end;
    /// 长度
    int length;

} PPFStack;


/// 是否为空
int isEmpty(PPFStack *stack);
/// push一个字符串
void pushString(PPFStack *stack,const char c);
/// push一个浮点
void pushFloat(PPFStack *stack,double number);
/// push一个数据块
void pushStackData(PPFStack *stack,StackData *data);
/// pop出一个数据
StackData * pop(PPFStack *stack);
/// 顶部数据
StackData * top(PPFStack *stack);

/// 输出,头一个开始
void printStack(PPFStack *stack);

/// 释放栈
void freeStack(PPFStack *stack);

#endif /* PPFStack_h */
