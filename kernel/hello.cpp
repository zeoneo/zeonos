#include <zeonos/kernel/hello.hpp>

char * x = "welcome zeo in CPP world :)";

char * get_1() {
    return x;
}

class Hello
{
private:
    /* data */
public:
    Hello(/* args */);
    ~Hello();
    char * print_hello() {
        return get_1();
    }
};

Hello::Hello(/* args */)
{
}

Hello::~Hello()
{
}

extern "C" {
    char * my_function(int arg1, int arg2) {
        Hello y = Hello();
        return y.print_hello();
    }
}



