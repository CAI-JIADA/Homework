# 41343145

作業三

## 解題說明

本題要求用鏈結串列來表示一元多項式，並能進行多項式的輸入、輸出、加法、減法、乘法與代入計算。
作業同時要求實作以下四個模板類別：
1. `template <class T> class ChainNode`
2. `template <class T> class Chain`
3. `template <class T> class ChainIterator`
4. `template <class T> class Polynomial`

### 解題策略

1. 使用遞迴函式讓程式可讀性提高
2. 透過 $Ackermann(m, n)$ 呼叫遞迴函式，最後輸出結果。  
3. 每次遞迴根據條件分支決定往更深層呼叫或是回傳結果。
4. 終止條件為 $m == 0$ 。

## 程式實作

以下為主要程式碼：

```#include <iostream>
using namespace std;
int Ackermann(int m,int n){//遞迴函式
	if (m == 0) return n + 1;
	else if (n == 0)return Ackermann(m - 1, 1);
	else return Ackermann(m - 1, Ackermann(m, n - 1));
}
int main() {
	int m, n;
	cout << "輸入Ackermann的m和n:";
	cin >> m>> n;//輸入Ackermann的m和n
	cout << Ackermann(m, n) << endl;//呼叫遞迴函式
	return 0;
}
```

## 效能分析

1. 時間複雜度：程式的時間複雜度遠大於 $2^n$。
2. 空間複雜度：空間複雜度為 $O(n)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $m$ $n$ | 預期輸出 | 實際輸出 |
|----------|---------------|----------|----------|
| 測試一   | $m = 1$ $n = 1$     | 3        | 3        |
| 測試二   | $m = 2$ $n = 3$     | 9        | 9        |
| 測試三   | $m = 4$ $n = 2$     | 異常拋出  | 異常拋出  |

### 結論

1. 程式能正確計算 $Ackermann(m, n)$ 的答案。
2. $Ackermann$ 函數成長數度極快以至於過大數字程式無法處理。  

## 申論及開發報告

1. **為何使用遞迴**
   
除了題目本身需求外 $Ackermann$ 函數的定義本身是遞迴形式，因此以遞迴程式最直觀。 

2. **函式特性**  
$Ackermann$ 函數的成長極快，超過一定值（如 m ≥ 4, n ≥ 2）會導致遞迴過深而異位。


