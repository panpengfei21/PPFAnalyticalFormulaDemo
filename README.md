# PPFAnalyticalFormulaDemo
 解析公式并计算公式字符串

## 怎么引入
pod 'PPFAnalyticalFormula'

## 怎么用

#### 公式
```
let str = "((1-2*7))/ ( 1+1) *4+( 1 * 9 )-(23*10-2)"
```
#### 解析
```
let s = UnsafeMutablePointer(mutating: NSString(string: str).utf8String)
var error:Int32 = 0
var res:Double = 0;
        
PPFAnalyticalFormula(s, Int32(str.utf8CString.count), &error,&res)
if error == 0 {
   print("\(str) = \(res)\n");
}else{
   print("出错\n");
}

```
#### 结果
```
((1-2*7))/ ( 1+1) *4+( 1 * 9 )-(23*10-2) = -245.0
```
