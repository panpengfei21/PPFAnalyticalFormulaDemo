//
//  ParsingStringOperations.c
//  COperationDemo
//
//  Created by 潘鹏飞 on 2019/9/3.
//  Copyright © 2019 潘鹏飞. All rights reserved.
// 

#include "PPFAnalyticalFormula.h"
#include "PPFStack.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

#define EPSILON 0.000001 //根据精度需要

/// 运算符号
#define ADD ('+')
#define SUB ('-')
#define MUL ('*')
#define DIV ('/')
#define LPar ('(')
#define RPar (')')
#define DOT ('.')

/**
 是否是零
 */
int isZero(double num) {
    return fabs(num - 0) < EPSILON ? 1 : 0;
}


/**
 检查字符串是否为数字,只有一个'-'号不能算数字
 */
int checkNumber(char *num,int length) {
    if (length == 0) {
        return 0;
    }
    int dotNum = 0;
    for (int i = 0; i < length; i++) {
        char ch = num[i];
        if (i == 0 && (ch == ADD || ch == SUB) && length > 1) {
            continue;
        }
        if (i == 0 && ch == DOT) {
            return 0;
        }
        if (ch == DOT) {// 如果是点
            dotNum++;
            if (dotNum >= 2) {
                return 0;
            }
        }else if (ch < '0' || ch > '9'){//如果非数字
            return 0;
        }
    }
    return 1;
}

/**
 运算
 
 @param lhs 左边数字
 @param op 运算符
 @param rhs 右边数字
 @param error 错误信息  0表示无错
 @return 运算结果
 */
double operator(double lhs,char op,double rhs,int *error) {
    *error = 0;
    switch (op) {
        case ADD:  return lhs + rhs;
        case SUB:  return lhs - rhs;
        case MUL:  return lhs * rhs;
        case DIV:{
            if (isZero(rhs)) {
                *error = 4;
                printf("除数不能为零,error = 4.\n");
                return 0;
            }else{
                return lhs / rhs;
            }
        }
            break;
        default:
            *error = 5;
            printf("%c不能作为运算符.\n",op);
            return 0;
    }
}



/// 运算前面的 secOP只能为加,减,乘,除
void operatorPre(PPFStack *stack,double rhs,char secOP,int *error) {
    *error = 0;
    StackData *opData = pop(stack);
    for (; opData && opData->type == 1 && opData->opertor != LPar; opData = pop(stack)) {///如果栈顶一个运算符
        switch (secOP) {
            case ADD:
            case SUB:{
                StackData *lhsData = pop(stack);
                if (!lhsData || lhsData->type != 2) {
                    *error = 6;
                    freeStack(stack);
                    printf("程序运行出错.这里不应该不是数字 error = 6");
                    return;
                }
                double lhs = lhsData->number;
                double res = operator(lhs, opData->opertor, rhs, error);
                free(opData);
                free(lhsData);
                
                if (*error != 0) {
                    freeStack(stack);
                    return;
                }
                rhs = res;
            }
                break;
            case DIV:
            case MUL:{
                switch (opData->opertor) {
                    case ADD:
                    case SUB:{///乘除前一个运算符是加或减,就push到栈里
                        goto end;
                    }
                        break;
                    case DIV:
                    case MUL:{///乘除前一个运算符也是乘除,就先运算前一个的运算
                        StackData *lhsData = pop(stack);
                        if (!lhsData || lhsData->type != 2) {
                            *error = 6;
                            freeStack(stack);
                            printf("程序运行出错.这里不应该不是数字 error = 6");
                            return;
                        }
                        double lhs = lhsData->number;
                        double res = operator(lhs, opData->opertor, rhs, error);
                        free(opData);
                        free(lhsData);
                        if (*error != 0) {
                            freeStack(stack);
                            return;
                        }
                        rhs = res;
                    }
                        break;
                    default:
                        *error = 11;
                        freeStack(stack);
                        printf("这里出错%c不能作为运算符 error = 11",opData->opertor);
                        return;
                }
            }
                break;
            default:{
                *error = 12;
                freeStack(stack);
                printf("这里出错%c不能作为运算符 error = 12",secOP);
                return;
            }
        }
    }
end:
    if (opData) {
        pushStackData(stack, opData);
    }
    pushFloat(stack, rhs);
    pushString(stack, secOP);
    return;
}

/**
 增加字符去字符串,这里没有错误检查,调用时要注意,字符串长度问题

 @param list 目标字符串
 @param ch 被增加的字符
 @param index 要加在哪
 */
void addCharToList(char *list,char ch,int* index) {
    list[*index] = ch;
    list[*index + 1] = '\0';
    *index = *index + 1;
}


/**
 把一对括号里的公式取出来,转成一个字符串
 */
void stackToStringInParenthesis(PPFStack *stack,char *list,int length) {
    
    PPFStack *tmpStack = malloc(sizeof(PPFStack));
    tmpStack->length = 0;
    tmpStack->begin = NULL;
    tmpStack->end = NULL;

    StackData *t = pop(stack);
    int index = 0;
    while (t && (t->type != 1 || t->opertor != LPar)) {
        pushStackData(tmpStack, t);
        t = pop(stack);
    }
    
    if (t) {
        pushStackData(stack, t);
    }
    t = pop(tmpStack);
    while (t) {
        switch (t->type) {
            case 1://字符
            {
                strncpy(&list[index], &t->opertor, 1);
                index++;
            }
                break;
            case 2:{
                char *tmp = (char *)calloc(sizeof(char), length);
                sprintf(tmp, "%.6lf",t->number);
                strncpy(&list[index], tmp, strlen(tmp));
                index += strlen(tmp);
                free(tmp);
            }
                break;
            default:
                break;
        }
        free(t);
        t = pop(tmpStack);
    }
    free(tmpStack);
}


/**
 解析公式,计算并返回结果
 
 @param array 公式字符串,只包括数字,点,加,减,乘,除,小括号,空格
 @param length 公式长度
 @param error 错误代码  0:无错  非0:出错
 @param res 计算结果
 */
void PPFAnalyticalFormula(char * array,int length,int *error,double *res) {
    *error = 0;
    
    PPFStack *stack = (PPFStack *)malloc(sizeof(PPFStack));
    stack->length = 0;
    stack->begin = NULL;
    stack->end = NULL;
    
    // 如果是数字或点,l
    char *num = (char *)calloc(sizeof(char),length + 1);
    int numLen = 0;
    /// 前一个是右括号
    int preIsRPar = 0;
    
    for (int i = 0; i < length; i++) {
        char tmpChar = array[i];
        if (tmpChar == ' ') {
            continue;
        }
        if ( tmpChar >= '0' && tmpChar <= '9') { // 如果是数字
            if (preIsRPar) {
                printf("数字不能直接加右括号后面. error = 8\n");
                *error = 8;  //出错
                freeStack(stack);
                return;
            }else{
                addCharToList(num, array[i], &numLen);
            }
        }else if (tmpChar == DOT) { // 如果是'.'
            if (numLen == 0) {
                printf("数字不能以\".\"开头. error = 1\n");
                *error = 1;  //出错
                freeStack(stack);
                return;
            }else{
                addCharToList(num, array[i], &numLen);
            }
        }
        else if (tmpChar == ADD || tmpChar == SUB || tmpChar == DIV || tmpChar == MUL)
        {
            if (numLen == 0) {//说明'*','/','+','-'的前一个单元不是数字.
                StackData *data = top(stack);
                if (data && data->type == 1) {
                    if (tmpChar == SUB) {///如果前面一个运算是(,就把
                        addCharToList(num, tmpChar, &numLen);
                    }else{
                        printf("\"%c\"不能和\"%c\"放在一起..error = 3\n",tmpChar,data->opertor);
                        *error = 3;
                        freeStack(stack);
                        return;
                    }
                }else if (!data){
                    if (tmpChar == DIV || tmpChar == MUL) {
                        printf("\"%c\"不能放在首位.error = 2\n",tmpChar);
                        *error = 2;
                        freeStack(stack);
                        return;
                    }else{
                        addCharToList(num, tmpChar, &numLen);
                    }
                }else if (data->type == 2){ //如果栈顶是一个数字
                    double rhs = pop(stack)->number;
                    operatorPre(stack, rhs, tmpChar, error);
                    if (*error != 0) {
                        freeStack(stack);
                        return;
                    }
                }
                else{
                    pushString(stack, tmpChar);
                }
            }else{ // 说明'*','/','+','-','+','-'有一个数字单元.
                if (!checkNumber(num, numLen)) {
                    *error = 5;
                    printf("%s不是数字.error = 5.",num);
                    freeStack(stack);
                    return;
                }
                double rhs = atof(num);
                numLen = 0;
                operatorPre(stack, rhs, tmpChar, error);
                if (*error != 0) {
                    freeStack(stack);
                    return;
                }
            }
            preIsRPar = 0;
        }
        else if (tmpChar == LPar) //如果是左括号
        {
            if (numLen != 0) {
                *error = 6;
                freeStack(stack);
                printf("数字%s后不能直接放括号,请放运算符.error = 6",num);
                return;
            }
            if (preIsRPar) {
                *error = 10;
                freeStack(stack);
                printf("左括号不能直接右括号后面.error = 10");
                return;
            }
            pushString(stack, tmpChar);
        }
        else if (tmpChar == RPar) // 右括号
        {
            double rhs;
            if (checkNumber(num, numLen)) {
                rhs = atof(num);
                numLen = 0;
            }else if (preIsRPar){
                StackData *rhsData = pop(stack);
                rhs = rhsData->number;
            }else{
                *error = 7;
                freeStack(stack);
                printf("右括号前应该为数字.error = 7");
                return;
            }
            preIsRPar = 1;
            
            char *tmp = calloc(sizeof(char), 100);
            memset(tmp,'\0',100);
            
            pushFloat(stack, rhs);
            numLen = 0;
            stackToStringInParenthesis(stack, tmp, 100);
            
            PPFAnalyticalFormula(tmp, 100 , error, &rhs);

            ///检查栈顶是不是左括号
            if (isEmpty(stack) || top(stack)->type != 1 || top(stack)->opertor != LPar) {
                *error = 11;
                freeStack(stack);
                printf("左右括号没有成对出现.error = 11\n");
                return;
            }
            StackData *l = pop(stack);
            free(l);
            pushFloat(stack, rhs);
        }
        else
        {
            if (tmpChar == '\0') {
                double rhs;
                if (checkNumber(num, numLen)) {
                    rhs = atof(num);
                }else if (preIsRPar) {
                    StackData *rhsData = pop(stack);
                    rhs = rhsData->number;
                }else{
                    *error = 5;
                    freeStack(stack);
                    printf("%s不是数字.error = 5.",num);
                    return;
                }

                operatorPre(stack, rhs, ADD, error);
                
                StackData *opData = pop(stack);
                free(opData);
                StackData *resData = pop(stack);
                *res = resData->number;
                
                free(stack);
                free(resData);
                return;
            }else{
                *error = 6;
                freeStack(stack);
                printf("公式中存在不合法字符:%c\n.error = 6 ",tmpChar);
                return;
            }
        }
    }
}



