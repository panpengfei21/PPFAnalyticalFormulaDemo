//
//  PPFTest.c
//  COperationDemo
//
//  Created by 潘鹏飞 on 2019/9/10.
//  Copyright © 2019 潘鹏飞. All rights reserved.
//

#include "PPFTest.h"
#include "PPFAnalyticalFormula.h"
#include "string.h"

void test(void) {
    
    char *array = "1+1";
    int error = 0;
    double res = 0;
    
    PPFAnalyticalFormula(array, (int)sizeof(array), &error, &res);
    
    if (error) {
        printf("出错 error = %d\n",error);
    }else{
        printf("%s = %f/n",array,res);
    }
}
