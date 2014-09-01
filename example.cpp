#include <ctpl.h>
#include <iostream>
#include <string>


void first(int id) {
    std::cout << "hello from " << id << ", function\n";
}

int main(int argc, char **argv) {
    //char * asd = new char();
    ctpl::thread_pool p(2 /* two threads in the pool */);

    p.copy(first);  // function
    p.push(std::ref(first));  // function
    //p.push(first);  // function. May not work for your compiler

    struct Second {
        Second(const std::string & s) { std::cout << "ctor\n"; this->s = s; }
        Second(Second && c) { std::cout << "move ctor\n"; s = std::move(c.s); }
        Second(const Second & c) { std::cout << "copy ctor\n"; this->s = c.s; };
        void operator()(int id) const {
            std::cout << "hello from " << id << ' ' << this->s << '\n';
        }

    private:
        std::string s;
    } second(", functor");

    p.push(std::ref(second));  // functor, reference
    p.copy(second);  // functor, copy ctor
    //p.push(second);  // functor, reference. May not work for your compiler
    p.push(std::move(second));  // functor, move ctor
    
    std::string s = ", lambda";
    p.push([s](int id){  // lambda
        std::cout << "hello from " << id << ' ' << s << '\n';
    });
    p.push([s](int id){  // lambda
        std::cout << "hello from " << id << ' ' << s << '\n';
    });

    auto f = p.pop();
    if (f) {
        std::cout << "poped function from the pool ";
        (*f)(0);
    }
    // change the number of treads in the pool
    p.resize(1);

    std::string s2 = "result";
    auto f1 = p.push([s2](int){
        return s2;
    });
    // other code here
    //...
    std::cout << "returned " << f1.get() << '\n';

    auto f2 = p.push([](int){
        throw std::exception();
    });
    // other code here
    //...
    try {
        f2.get();
    }
    catch (std::exception & e) {
        std::cout << "caught exception\n";
    }

    // get thread 0
    auto & th = p.get_thread(0);

    return 0;
}
