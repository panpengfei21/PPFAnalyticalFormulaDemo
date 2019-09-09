//
//  PPFStack.c
//  COperationDemo
//
//  Created by 潘鹏飞 on 2019/9/3.
//  Copyright © 2019 潘鹏飞. All rights reserved.
//

#include "PPFStack.h"
#include "string.h"
#include "stdlib.h"


/**
 一个节点用于存数据
 */
struct Node {
    /// 数据源
    StackData *data;
    /// 下一个节点
    struct Node *next;
    /// 前一个节点
    struct Node *pre;
};

/// 顶部数据
StackData * top(PPFStack *stack){
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->end->data;
}

/// 是否为空
int isEmpty(PPFStack *stack) {
    return stack->length == 0 ? 1 : 0;
}

/// push 一个node
void pushNode(PPFStack *stack,Node *node) {
    stack->length++;
    if (stack->begin) {
        stack->end->next = node;
        node->pre = stack->end;
        stack->end = node;
    }else{
        stack->begin = node;
        stack->end = node;
    }
}
/// push一个数据块
void pushStackData(PPFStack *stack,StackData *data) {
    /// 初始化一个节点
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    node->pre  = NULL;
    
    pushNode(stack,node);
}


// pop出一个数据
StackData * pop(PPFStack *stack) {
    if (isEmpty(stack)) {
        return NULL;
    }else{
        if (stack->length == 1) {
            StackData *r = stack->end->data;
            free(stack->end);
            
            stack->begin = NULL;
            stack->end = NULL;
            stack->length--;
            return r;
        }else{
            Node *r = stack->end;
            stack->end = stack->end->pre;
            stack->end->next = NULL;
            
            StackData *rData = r->data;
            free(r);
            stack->length--;
            return rData;
        }
    }
}


/// push一个字符串
void pushString(PPFStack *stack,const char c) {
    /// 初始化一个栈数据
    StackData *data = (StackData *)malloc(sizeof(StackData));
    
    data->opertor = c;
    data->type = 1;
    
    pushStackData(stack, data);
}
/// push一个浮点
void pushFloat(PPFStack *stack,double number) {
    /// 初始化一个栈数据
    StackData *data = (StackData *)malloc(sizeof(StackData));
    data->number = number;
    data->type = 2;
    
    pushStackData(stack, data);
}

/// 打印栈,从底端开始
void printStack(PPFStack *stack) {
    if (isEmpty(stack)) {
        printf("stack is empty.\n");
        return;
    }
    Node *node = stack->begin;
    while (node) {
        StackData *data = node->data;
        if (data->type == 1) {
            printf("Char:%c  ",data->opertor);
        }else{
            printf("Float:%.2f  ",data->number);
        }
        node = node->next;
    }
    printf("\n");
}

