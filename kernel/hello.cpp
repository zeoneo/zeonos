#include"hello.hpp"

int get_1() {
    return 1;
}

extern "C" {
    int my_function(int arg1, int arg2) {
        return 0;
    }
}