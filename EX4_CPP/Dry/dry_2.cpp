#include <iostream>
using namespace std;

class A {
public:
    void print(A &x) const {
        cout << "a" << endl;
    }

    void print(const A &x) const {
        cout << "b" << endl;
    }

    class InnerClass {
    public:
        void print(const A &x) const {
            cout << "c" << endl;
        }
    };
};

void f(const A::InnerClass &a, const A &x) {
    a.print(x);
}

int main() {
    A a;
    A a2;
    A::InnerClass y;
    f(y,a2);
    return 0;
}