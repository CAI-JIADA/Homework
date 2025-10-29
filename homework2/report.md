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
1. Add: $O(n+m)$ n、m 為兩多項式項數
2. Mult: $O(nm*k)$ k 為暫時結果項數
3. Eval: $O(n)$

### 空間複雜度
1. Add: $O(n+m)$
2. Mult: $O(nm)$
3. Eval: $O(1)$
   
## 測試與驗證

### 測試案例

| 測試案例 | 輸入多項式 A | 輸入多項式 B | 測試 x | 預期輸出1 |  預期輸出2 | 實際輸出1 | 實際輸出2 |
|----------|--------------|--------------|--------|--------------------|-------------------------------|-------------------|----------|
| 測試一   | 3 5 2 3 1 7 0 | 2 3 2 -5 0   | 2      |8X^2 + 3X^1 + 2X^0 | 15X^4-4X^2 + 9X^3-15X^1-35X^0 | 8X^2 + 3X^1 + 2X^0 |15X^4-4X^2 + 9X^3-15X^1-35X^0|


### 結論

1. 程式能正確計算 $Polynomial$ 的加法乘法和 $f(x)$ 。
2. 因為兩個多項式的指數不一定都一樣所以隨時會有記憶體溢位的問題，要隨時確保記憶體空間足夠。  

## 申論及開發報告

1. **動態陣列**
因多項式項數不固定需動態分配與擴充，可以節省記憶體空間。

2. **運算子重載**  
使用friend function進行運算子重載，確保可以直接用cin/cout輸入輸出多項式。

