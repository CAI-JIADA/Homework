#include<iostream>
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

