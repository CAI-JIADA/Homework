#include <iostream>
using namespace std;

//實作1 ChainNode
template <class T>
class ChainNode {
public:
    T data;// 資料
    ChainNode<T>* link;// 下一個節點
    ChainNode() : data(), link(nullptr) {}
    ChainNode(const T& d, ChainNode<T>* next = nullptr) : data(d), link(next) {}
};
template <class T> class Chain;
template <class T> class ChainIterator;

//實作2 Chain
template <class T>
class Chain {
private:
    ChainNode<T>* header; //頭節點
public:
    using iterator = ChainIterator<T>; //iterator = ChainIterator<T>  讓後面主程式可以Chain<int>::iterator xHere = x.Begin();
    Chain() : header(new ChainNode<T>()) {// 建立 header並讓header->link
        header->link = header;
    }
    Chain(const Chain& other) : Chain() {
        ChainNode<T>* last = header;//last指向目前最後一個節點
        for (ChainNode<T>* p = other.header->link; p != other.header; p = p->link) {// 從第一個資料節點走到header
            last->link = new ChainNode<T>(p->data, header);//在尾端新增節點 並讓新節點link 指回本串列header
            last = last->link;
        }
    }
    const Chain& operator=(const Chain& other) {
        if (this == &other) return *this;
        ChainNode<T>* p = header->link;
        while (p != header) {//清空舊資料節點
            ChainNode<T>* nxt = p->link;
            delete p;
            p = nxt;
        }
        header->link = header;
        ChainNode<T>* last = header;
        for (ChainNode<T>* q = other.header->link; q != other.header; q = q->link) {
            last->link = new ChainNode<T>(q->data, header);
            last = last->link;
        }
        return *this;
    }
    ~Chain() {//解構 刪掉所有資料節點最後刪 header
        ChainNode<T>* p = header->link;
        while (p != header) {
            ChainNode<T>* nxt = p->link;
            delete p;
            p = nxt;
        }
        delete header;
        header = nullptr;
    }
    iterator Begin() { return iterator(header->link, header); }//Begin：回傳第一個資料節點的ChainIterator(I)（若空串列，Begin==End）
    iterator End() { return iterator(header, header); }// End：回傳指向 header 的 ChainIterator(I)（用作終止位置）
};

//實作3 ChainIterator
template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;//目前節點
    ChainNode<T>* head;//頭節點
public:
    ChainIterator(ChainNode<T>* c = nullptr, ChainNode<T>* h = nullptr): current(c), head(h) {
    }
    T& operator*() const { return current->data; }
    ChainIterator& operator++() {//移動到下一個節點
        current = current->link;
        return *this;
    }
    bool operator!=(const ChainIterator& rhs) const { return current != rhs.current; }
    friend class Chain<T>;//讓Chain可以存取ChainIterator的成員
};

//實作四 Polynomial
template <class T>
class Polynomial {
private:
    struct TermNode {
        int coef;        // 係數
        int exp;         // 次方
        TermNode* link;  // 下一個節點
        TermNode(int c = 0, int e = 0, TermNode* n = nullptr) : coef(c), exp(e), link(n) {}
    };
    TermNode* header;
    void Clear() {
        TermNode* p = header->link;
        while (p != header) {
            TermNode* nxt = p->link;
            delete p;
            p = nxt;
        }
        header->link = header;
    }
    void AddTerm(int c, int e) {
        if (c == 0) return;
        TermNode* prev = header;
        TermNode* cur = header->link;
        while (cur != header && cur->exp > e) {//由大到小插入
            prev = cur;
            cur = cur->link;
        }
        if (cur != header && cur->exp == e) {//若同次方存在合併係數
            cur->coef += c;
            if (cur->coef == 0) {//合併後 coef==0 則刪除該項
                prev->link = cur->link;
                delete cur;
            }
            return;
        }
        TermNode* n = new TermNode(c, e, cur);//插入新節點
        prev->link = n;
    }
public:
    Polynomial() : header(new TermNode()) {
        header->link = header;
    }

    Polynomial(const Polynomial& a) : Polynomial() {
        TermNode* last = header;
        for (TermNode* p = a.header->link; p != a.header; p = p->link) {
            last->link = new TermNode(p->coef, p->exp, header);
            last = last->link;
        }
    }
    const Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;
        Clear();

        TermNode* last = header;
        for (TermNode* p = a.header->link; p != a.header; p = p->link) {
            last->link = new TermNode(p->coef, p->exp, header);
            last = last->link;
        }
        return *this;
    }
    ~Polynomial() {
        Clear();//刪除所有節點(包括header)
        delete header;
        header = nullptr;
    }
    friend istream& operator>>(istream& is, Polynomial& x) {//讀入多項式
        int n;
        if (!(is >> n)) return is;//n：項數,c：係數,e：次方
        x.Clear();
        for (int i = 0; i < n; ++i) {
            int c, e;
            is >> c >> e;
            x.AddTerm(c, e);
        }
        return is;
    }
    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        TermNode* p = x.header->link;
        if (p == x.header) {//沒有任何項
            os << "0";
            return os;
        }
        bool first = true;
        while (p != x.header) {
            int c = p->coef;
            int e = p->exp;
            //第一項如果係數是負的要先輸出- 正的不輸出+
            if (first) {
                if (c < 0) os << "-";
            }
            else {
                if (c < 0) os << "-";
                else os << "+";
            }
            //印絕對值係數（避免出現兩個副號）
            int absC = (c < 0) ? -c : c;
            os << absC << "x^" << e;
            first = false;
            p = p->link;
        }
        return os;
    }
    Polynomial operator+(const Polynomial& b) const {//加法
        Polynomial r;
        TermNode* pa = header->link;
        TermNode* pb = b.header->link;
        while (pa != header && pb != b.header) {//只要兩邊都還沒走完，就一直比exp來合併
            if (pa->exp > pb->exp) {//A的次方比較大
                r.AddTerm(pa->coef, pa->exp);
                pa = pa->link;
            }
            else if (pa->exp < pb->exp) {//B的次方比較大
                r.AddTerm(pb->coef, pb->exp);
                pb = pb->link;
            }
            else {//一樣大
                r.AddTerm(pa->coef + pb->coef, pa->exp);
                pa = pa->link;
                pb = pb->link;
            }
        }
        while (pa != header) { r.AddTerm(pa->coef, pa->exp); pa = pa->link; }
        while (pb != b.header) { r.AddTerm(pb->coef, pb->exp); pb = pb->link; }
        return r;
    }
    Polynomial operator-(const Polynomial& b) const {//減法
        Polynomial r;
        TermNode* pa = header->link;
        TermNode* pb = b.header->link;
        while (pa != header && pb != b.header) {
            if (pa->exp > pb->exp) {//A的次方比較大
                r.AddTerm(pa->coef, pa->exp);
                pa = pa->link;
            }
            else if (pa->exp < pb->exp) {//B的次方比較大
                r.AddTerm(-pb->coef, pb->exp);
                pb = pb->link;
            }
            else {//一樣大
                r.AddTerm(pa->coef - pb->coef, pa->exp);
                pa = pa->link;
                pb = pb->link;
            }
        }
        while (pa != header) { r.AddTerm(pa->coef, pa->exp); pa = pa->link; }
        while (pb != b.header) { r.AddTerm(-pb->coef, pb->exp); pb = pb->link; }
        return r;
    }
    Polynomial operator*(const Polynomial& b) const {//乘法
        Polynomial r;
        for (TermNode* pa = header->link; pa != header; pa = pa->link) {//每一項互乘 再加進結果
            for (TermNode* pb = b.header->link; pb != b.header; pb = pb->link) {
                r.AddTerm(pa->coef * pb->coef, pa->exp + pb->exp);
            }
        }
        return r;
    }
    float Evaluate(T x) const {//計算多項式在x的值
        auto fpow = [](T a, int e) -> T {
            T r = (T)1;
            while (e > 0) {
                if (e & 1) r = r * a;
                a = a * a;
                e >>= 1;
            }
            return r;
            };
        T sum = (T)0;
        for (TermNode* p = header->link; p != header; p = p->link) {
            sum = sum + (T)p->coef * fpow(x, p->exp);
        }
        return (float)sum;
    }
};
int main() {
    Chain<int> x;
    Chain<int>::iterator xHere = x.Begin();
    Chain<int>::iterator xEnd = x.End();
    (void)xHere; (void)xEnd;
    Polynomial<float> A, B;
    cin >> A >> B;
    cout << "A+B=" << (A + B) << "\n";
    cout << "A-B=" << (A - B) << "\n";
    cout << "A*B=" << (A * B) << "\n";
    float a;
    cin >> a;
    cout << "E(" << a << ")=" << A.Evaluate(a) << "\n";
    return 0;
}