#include<iostream>
#include <algorithm>//�Ƨǥ�
using namespace std;
void powerset(char S[], int n, int index, char subset[], int subsetSize) {//index���ثe���j��ĴX�Ӥ����AsubsetSize��ܥثe�l���X�����h�֤���
    // ��X�ثe�l���X
    cout << "(";
    for (int i = 0; i < subsetSize; i++) {
        cout << subset[i];
    }
    cout << ")" << endl;
    for (int i = index; i < n; i++) {//�p�G�����Ƥ������L
        if (i > index && S[i] == S[i - 1])continue;
        subset[subsetSize] = S[i];
        powerset(S, n, i + 1, subset, subsetSize + 1);
    }
}
int main() {
	int n;
	cout << "�п�Jn:";
	cin >> n;
	cout << endl<< "�п�J����:";
	char* S = new char[n];//�إ߶��X�}�C
    char* subset = new char[n];//�Ȧs�ثe���b�ͦ����l���X
	for (int i = 0;i < n;i++) {
		cin >> S[i];
	}
    sort(S, S + n);//�N���Ƥ����Ƥ@�_
	cout << endl<< "�����X���G" ;
    powerset(S, n, 0, subset, 0);
    delete[] S;
    delete[] subset;
	return 0;
}

