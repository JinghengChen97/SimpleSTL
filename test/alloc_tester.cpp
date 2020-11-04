#include <iostream>
class a{
    enum{c = 1};
    int l;
    union obj{
        union obj* l;
        char p[1];
    };
};
int main() {
    std::cout << sizeof(a) << std::endl;
    return 0;
}
