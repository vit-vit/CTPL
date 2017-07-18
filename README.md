CTPL
====

Modern and efficient C++ Thread Pool Library

A thread pool is a programming pattern for parallel execution of jobs, http://en.wikipedia.org/wiki/Thread_pool_pattern.

More specifically, there are some threads dedicated to the pool and a container of jobs. The jobs come to the pool dynamically. A job is fetched and deleted from the container when there is an idle thread. The job is then run on that thread.

A thread pool is helpful when you want to minimize time of loading and destroying threads and when you want to limit the number of parallel jobs that run simultaneously. For example, time consuming event handlers may be processed in a thread pool to make UI more responsive.

## Features:
- standard c++ language, tested to compile on MS Visual Studio 2013 (2012?), gcc 4.8.2 and mingw 4.8.1(with posix threads)
- simple but efficient solution, one header only, no need to compile a binary library
- query the number of idle threads and resize the pool dynamically
- one API to push to the thread pool any collable object: lambdas, functors, functions, result of bind expression
- callable objects with variadic number of parameters plus index of the thread running the object
- automatic template argument deduction
- get returned value of any type with standard c++ futures
- get fired exceptions with standard c++ futures
- use for any purpose under Apache license
- two variants, one depends on Boost Lockfree Queue library, http://boost.org, which is a header only library

## Sample usage

    void first(int id) { 
        std::cout << "hello from " << id << "\n";
    }
    
    struct Second {
        void operator()(int id) const {
            std::cout << "hello from " << id << "\n";
        }
    } second;
    
    void third(int id, const std::string& additional_param) {}
    
    int main() {
        // Create a thread pool with 2 threads
        ctpl::thread_pool pool(2);
        
        // Add a function to the pool
        pool.push(first);
        
        // Add a function with a parameter
        pool.push(third, "additional_parameter");
        
        // Use a lamda
        pool.push([](int id) {
                      std::cout << "hello from " << id << "\n";
                  });
        
        // Use a functor, reference
        pool.push(std::ref(second));
        
        // Use a functor, move constructor
        pool.push(std::move(second));
    }
