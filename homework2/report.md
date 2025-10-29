# 41343145

作業二

## 解題說明

本題要求實作一個多項式(Polynomial)，以抽象資料型態方式儲存與操作多項式
多項式的加法與乘法運算
以動態陣列儲存，並能自動擴充容量

### 解題策略

1. 設計 Term 類別，儲存每一項的係數與指數。
2. Polynomial類別以動態陣列儲存所有非零項，並記錄容量(capacity)及項數(terms)。  
3. 運算子重載(>>、<<)使多項式可由cin/cout直接讀寫，簡化操作。
4. 使用(resize)防止記憶體溢位。

## 程式實作

以下為主要程式碼：

```
#include<iostream>
#include<cmath>
using namespace std;
class Polynomial;
class Term {
	friend class Polynomial;//friend Polynomial讓Term可以存取Polynomial的成員函式
	friend ostream& operator<<(ostream&, const Polynomial&);
	friend istream& operator>>(istream&, Polynomial&);
private:
	float coef;//係數
	int exp;//指數
};
class Polynomial {
private:
	Term* termArray;//動態記憶體配置陣列大小
	int capacity;//陣列大小
	int terms;//非零向
	void resize(int newcapacity) {//確保記憶體不會溢位
		Term* newArray = new Term[newcapacity];
		for (int i = 0; i < terms; i++) newArray[i] = termArray[i];
		delete[] termArray;
		termArray = newArray;
		capacity = newcapacity;
	}
public:
	friend class Term;
	Polynomial() {
		capacity = 2;//初始陣列大小為2
		terms = 0;
		termArray = new Term[capacity];
	}
	~Polynomial() { delete[] termArray; }//解構子執行時刪除termArray釋放記憶體
	friend istream& operator>>(istream& in, Polynomial& p) {//輸入
		in >> p.terms;
		if (p.terms > p.capacity) p.resize(p.terms);
		for (int i = 0; i < p.terms; i++) {
			in >> p.termArray[i].coef >> p.termArray[i].exp;
		}
		return in;
	}
	friend ostream& operator<<(ostream& out, const Polynomial& p) {//輸出
		for (int i = 0; i < p.terms; i++) {
			if (i > 0 && p.termArray[i].coef >= 0)
				out << " + ";
			out << p.termArray[i].coef << "X^" << p.termArray[i].exp;//輸出格式
		}
		return out;
	}
	Polynomial Add(const Polynomial& poly) {
		Polynomial result;//多項式加法的結果會放在這裡
		int i = 0, j = 0;//i,j分別用來代表*this及poly
		while (i < terms && j < poly.terms) {
			if (result.terms >= result.capacity)//若結果陣列不夠大就擴充容量
				result.resize(result.capacity * 2);
			if (termArray[i].exp == poly.termArray[j].exp) {//相同指數係數相加
				float c = termArray[i].coef + poly.termArray[j].coef;
				if (c != 0) {//相加不等於零就存入結果
					result.termArray[result.terms].coef = c;
					result.termArray[result.terms].exp = termArray[i].exp;
					result.terms++;
				}
				i++; j++;
			}
			else if (termArray[i].exp > poly.termArray[j].exp) {//如果this指數比較大就直接存入
				result.termArray[result.terms++] = termArray[i++];
			}
			else {//poly比較大直街存入
				result.termArray[result.terms++] = poly.termArray[j++];
			}
		}
		while (i < terms) {//處理this剩下的非零項
			if (result.terms >= result.capacity)
				result.resize(result.capacity * 2);
			result.termArray[result.terms++] = termArray[i++];
		}
		while (j < poly.terms) {//處理poly剩下的非零項
			if (result.terms >= result.capacity)
				result.resize(result.capacity * 2);
			result.termArray[result.terms++] = poly.termArray[j++];
		}
		return result;//回傳結果
	}
	Polynomial Mult(const Polynomial& poly) const {
		Polynomial result;//多項式乘法的結果會放在這裡
		for (int i = 0; i < terms; i++) {//外迴圈this的每一項,內迴圈poly的每一項
			for (int j = 0; j < poly.terms; j++) {//計算兩項相乘後的新係數和新指數
				float c = termArray[i].coef * poly.termArray[j].coef;//係數相乘
				int e = termArray[i].exp + poly.termArray[j].exp;//指數相加
				bool found = false;//紀錄是否有相同指數
				for (int k = 0; k < result.terms; k++) {//檢查result是否有相同指數,有就合併
					if (result.termArray[k].exp == e) {
						result.termArray[k].coef += c;
						found = true;
						break;
					}
				}
				if (!found) {//沒有相同新增一個term
					if (result.terms >= result.capacity)//確保記憶足夠
						result.resize(result.capacity * 2);
					result.termArray[result.terms].coef = c;
					result.termArray[result.terms].exp = e;
					result.terms++;
				}
			}
		}
		return result;//回傳相乘結果
	}
	float Eval(float f) const {//計算多項式在某個 x 值時的結果
		float sum = 0;
		for (int i = 0; i < terms; i++) {
			sum += termArray[i].coef * pow(f, termArray[i].exp);
		}
		return sum;
	}
};
int main(){
	Polynomial A, B;
	cin >> A;
	cin >> B;
	cout << A.Add(B)<<endl;//相加結果
	cout << A.Mult(B) <<endl;//相乘結果
	float x;
	cin >> x;
	cout << A.Eval(x);//回傳f(x)
	return 0;
}
```

## 效能分析

### 時間複雜度
1. Add:$O(n+m)$n、m 為兩多項式項數
2. Mult:$O(nm*k)$k 為暫時結果項數
3. Eval:$O(n)$

### 空間複雜度
1. Add:$O(n+m)$
2. Mult:$O(nm)$
3. Eval:$O(1)$
   
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

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
作業二

## 解題說明

本題要求使用遞迴方式列出所有可能的子集合。

### 解題策略
使用一個 char 陣列 S[] 儲存輸入的集合元素。
使用另一個 char 陣列 subset[] 暫存目前遞迴過程中產生的子集合。
為了讓重複的元素相鄰，先用```sort(S, S + n)```排序，若在遞迴中若發現連續相同元素，利用```if (i > index && S[i] == S[i - 1]) continue;```跳過重複組合。
## 程式實作

以下為主要程式碼：

```#include<iostream>
#include <algorithm>//排序用
using namespace std;
void powerset(char S[], int n, int index, char subset[], int subsetSize) {//index指目前遞迴到第幾個元素，subsetSize表示目前子集合中有多少元素
    // 輸出目前子集合
    cout << "(";
    for (int i = 0; i < subsetSize; i++) {
        cout << subset[i];
    }
    cout << ")" << endl;
    for (int i = index; i < n; i++) {//如果有重複元素跳過
        if (i > index && S[i] == S[i - 1])continue;
        subset[subsetSize] = S[i];
        powerset(S, n, i + 1, subset, subsetSize + 1);
    }
}
int main() {
	int n;
	cout << "請輸入n:";
	cin >> n;
	cout << endl<< "請輸入元素:";
	char* S = new char[n];//建立集合陣列
    char* subset = new char[n];//暫存目前正在生成的子集合
	for (int i = 0;i < n;i++) {
		cin >> S[i];
	}
    sort(S, S + n);//將重複元素排一起
	cout << endl<< "冪集合為：" ;
    powerset(S, n, 0, subset, 0);
    delete[] S;
    delete[] subset;
	return 0;
}
```

## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(n*2^n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

## 測試與驗證

### 測試案例

測試一: $n = 3$  $abc$
輸出:()(a)(ab)(abc)(ac)(b)(bc)(c)

測試二: $n = 3$  $abb$
輸出:()(a)(ab)(abb)(b)(bb)

### 結論

1. 程式能正確排列出元素的冪集合。  
2. 在有相同元素時不會出現重複的答案。 

## 申論及開發報告

1. **選擇遞迴的原因**

遞迴能以簡潔的方式展現所有組合，程式結構清楚，邏輯自然。

2. **動態記憶體配置**  

因為剛開始需要輸入元素的個數所以使用動態記憶體配置以節省空間

