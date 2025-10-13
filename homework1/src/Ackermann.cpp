#include <iostream>
using namespace std;
int Ackermann(int m,int n){//遞迴函式
	if (m == 0) return n + 1;
	else if (n == 0)return Ackermann(m - 1, 1);
	else return Ackermann(m - 1, Ackermann(m, n - 1));
}
int main() {
	int m, n;
	cin >> m>> n;//輸入Ackermann的m和n
	cout << Ackermann(m, n) << endl;//呼叫遞迴函式
	return 0;
}