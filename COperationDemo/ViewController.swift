//
//  ViewController.swift
//  COperationDemo
//
//  Created by 潘鹏飞 on 2019/9/3.
//  Copyright © 2019 潘鹏飞. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()
    }


    @IBAction func tap(_ sender: Any) {
        test(str: "((1-2*7))/ ( 1+1) *4+( 1 * 9 )-(23*10-2)")
//        test(str: "(1-2*7)/1*4")
//        test(str: "((1-2*7))/(2+1)")
    }
    
    func test(str:String) {
        let s = UnsafeMutablePointer(mutating: NSString(string: str).utf8String)
        var error:Int32 = 0
        var res:Double = 0;
        
        PPFAnalyticalFormula(s, Int32(str.utf8CString.count), &error,&res)
        if error == 0 {
            print("\(str) = \(res)\n");
        }else{
            print("出错\n");
        }
    }
}

