#include <iostream>
#include <set>

class Object {
    template<class iterator>
    bool operator()(iterator begin_1, iterator end_1,
                    iterator begin_2, iterator end_2);
};

template<class iterator, class Object>
std::set<Object> DryFunc(iterator begin , iterator end , Object object) {
    std::set<Object> new_set;
};

int main() {
    std::set setix;
    std::set::iterator begin = setix.begin();
    std::set::iterator end = setix.end();
    Object x;
    DryFunc(begin,end,x);
    return 0;
}
