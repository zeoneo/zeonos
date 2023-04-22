#include"hello.hpp"

char * x = "welcome prakash";

int get_1() {
    return 1;
}

class Hello
{
private:
    /* data */
public:
    Hello(/* args */);
    ~Hello();
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
        return x;
    }
}



