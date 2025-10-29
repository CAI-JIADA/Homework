#include<iostream>
#include<cmath>
using namespace std;
class Polynomial;
class Term {
	friend class Polynomial;//friend Polynomial��Term�i�H�s��Polynomial�������禡
	friend ostream& operator<<(ostream&, const Polynomial&);
	friend istream& operator>>(istream&, Polynomial&);
private:
	float coef;//�Y��
	int exp;//����
};
class Polynomial {
private:
	Term* termArray;//�ʺA�O����t�m�}�C�j�p
	int capacity;//�}�C�j�p
	int terms;//�D�s�V
	void resize(int newcapacity) {//�T�O�O���餣�|����
		Term* newArray = new Term[newcapacity];
		for (int i = 0; i < terms; i++) newArray[i] = termArray[i];
		delete[] termArray;
		termArray = newArray;
		capacity = newcapacity;
	}
public:
	friend class Term;
	Polynomial() {
		capacity = 2;//��l�}�C�j�p��2
		terms = 0;
		termArray = new Term[capacity];
	}
	~Polynomial() { delete[] termArray; }//�Ѻc�l����ɧR��termArray����O����
	friend istream& operator>>(istream& in, Polynomial& p) {//��J
		in >> p.terms;
		if (p.terms > p.capacity) p.resize(p.terms);
		for (int i = 0; i < p.terms; i++) {
			in >> p.termArray[i].coef >> p.termArray[i].exp;
		}
		return in;
	}
	friend ostream& operator<<(ostream& out, const Polynomial& p) {//��X
		for (int i = 0; i < p.terms; i++) {
			if (i > 0 && p.termArray[i].coef >= 0)
				out << " + ";
			out << p.termArray[i].coef << "X^" << p.termArray[i].exp;//��X�榡
		}
		return out;
	}
	Polynomial Add(const Polynomial& poly) {
		Polynomial result;//�h�����[�k�����G�|��b�o��
		int i = 0, j = 0;//i,j���O�ΨӥN��*this��poly
		while (i < terms && j < poly.terms) {
			if (result.terms >= result.capacity)//�Y���G�}�C�����j�N�X�R�e�q
				result.resize(result.capacity * 2);
			if (termArray[i].exp == poly.termArray[j].exp) {//�ۦP���ƫY�Ƭۥ[
				float c = termArray[i].coef + poly.termArray[j].coef;
				if (c != 0) {//�ۥ[������s�N�s�J���G
					result.termArray[result.terms].coef = c;
					result.termArray[result.terms].exp = termArray[i].exp;
					result.terms++;
				}
				i++; j++;
			}
			else if (termArray[i].exp > poly.termArray[j].exp) {//�p�Gthis���Ƥ���j�N�����s�J
				result.termArray[result.terms++] = termArray[i++];
			}
			else {//poly����j����s�J
				result.termArray[result.terms++] = poly.termArray[j++];
			}
		}
		while (i < terms) {//�B�zthis�ѤU���D�s��
			if (result.terms >= result.capacity)
				result.resize(result.capacity * 2);
			result.termArray[result.terms++] = termArray[i++];
		}
		while (j < poly.terms) {//�B�zpoly�ѤU���D�s��
			if (result.terms >= result.capacity)
				result.resize(result.capacity * 2);
			result.termArray[result.terms++] = poly.termArray[j++];
		}
		return result;//�^�ǵ��G
	}
	Polynomial Mult(const Polynomial& poly) const {
		Polynomial result;//�h�������k�����G�|��b�o��
		for (int i = 0; i < terms; i++) {//�~�j��this���C�@��,���j��poly���C�@��
			for (int j = 0; j < poly.terms; j++) {//�p��ⶵ�ۭ��᪺�s�Y�ƩM�s����
				float c = termArray[i].coef * poly.termArray[j].coef;//�Y�Ƭۭ�
				int e = termArray[i].exp + poly.termArray[j].exp;//���Ƭۥ[
				bool found = false;//�����O�_���ۦP����
				for (int k = 0; k < result.terms; k++) {//�ˬdresult�O�_���ۦP����,���N�X��
					if (result.termArray[k].exp == e) {
						result.termArray[k].coef += c;
						found = true;
						break;
					}
				}
				if (!found) {//�S���ۦP�s�W�@��term
					if (result.terms >= result.capacity)//�T�O�O�Ш���
						result.resize(result.capacity * 2);
					result.termArray[result.terms].coef = c;
					result.termArray[result.terms].exp = e;
					result.terms++;
				}
			}
		}
		return result;//�^�Ǭۭ����G
	}
	float Eval(float f) const {//�p��h�����b�Y�� x �Ȯɪ����G
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
	cout << A.Add(B)<<endl;//�ۥ[���G
	cout << A.Mult(B) <<endl;//�ۭ����G
	float x;
	cin >> x;
	cout << A.Eval(x);//�^��f(x)
	return 0;
}
