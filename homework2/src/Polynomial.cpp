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
