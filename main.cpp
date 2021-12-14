#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <complex>
#include <cmath>
#include <iomanip>
#include <numbers>
#include <map>

void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise)
{
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum);  // Notify future
}
 
void do_work(std::promise<int> barrier)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    barrier.set_value(1);
}

void f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 1 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void f2(int& n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 2 executing\n";
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

class foo
{
public:
    void bar()
    {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 3 executing\n";
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    int n = 0;
};

class baz
{
public:
    void operator()()
    {
        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread 4 executing\n";
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    int n = 0;
};

int main()
{
    // Demonstrate using promise<int> to transmit a result between threads.
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(accumulate, numbers.begin(), numbers.end(),
                            std::move(accumulate_promise));
 
    // future::get() will wait until the future has a valid result and retrieves it.
    // Calling wait() before get() is not needed
    //accumulate_future.wait();  // wait for result
    std::cout << "Резултатът е = " << accumulate_future.get() << '\n';
    work_thread.detach();  // wait for thread completion
 
    // Demonstrate using promise<void> to signal state between threads.
    std::promise<int> barrier;
    std::future<int> barrier_future = barrier.get_future();
    std::thread new_work_thread(do_work, std::move(barrier));
//    std::cout << "result2=" << barrier_future.get() << '\n';
    new_work_thread.detach();
    std::cout << "result2=" << barrier_future.get() << '\n';
    std::cout << "Execution finished" << std::endl;
    std::cout << "Изпълнението завърши !" << std::endl;
    std::cout << "Hardware concurrency : " << std::thread::hardware_concurrency() << std::endl;
    std::string someString = "A";
    char *test = someString.data();
    char *end;
    std::bitset<8> someBin = strtol(test,&end,16);
    std::cout << someBin.to_string() << std::endl;
    std::cout << someBin.to_ulong() << std::endl;
    std::cout << end << std::endl;
    std::cout << "Някакъв тест !" << std::endl;

    using namespace std::complex_literals;
    std::cout << std::fixed << std::setprecision(1);

    std::complex<double> z1 = 1i * 1i;     // imaginary unit squared
    std::cout << "i * i = " << z1 << '\n';

    std::complex<double> z2 = std::pow(1i, 2); // imaginary unit squared
    std::cout << "pow(i, 2) = " << z2 << '\n';

    const double PI = std::numbers::pi; //std::acos(-1); // or std::numbers::pi in C++20
    std::complex<double> z3 = std::exp(1i * PI); // Euler's formula
    std::cout << "exp(i * pi) = " << z3 << '\n';

    std::complex<double> z4 = 1. + 2i, z5 = 1. - 2i; // conjugates
    std::cout << "(1+2i)*(1-2i) = " << z4*z5 << '\n';
    std::cout << "Край !" << std::endl;

    std::cout << "Някакъв тест !" << std::endl;

    std::cout << "Последният елемент от Вектора преди сортирането е : " << numbers.back() << std::endl;

    std::sort(numbers.begin(),numbers.end(),std::greater());

    std::cout << "Последният елемент от Вектора след сортирането е : " << numbers.back() << std::endl;

    std::cout << "Малко текст на кирилилица !" <<  std::endl;

    int n = 0;
    foo f;
    baz b;
    std::thread t1; // t1 is not a thread
    std::thread t2(f1, n + 1); // pass by value
    std::thread t3(f2, std::ref(n)); // pass by reference
    std::thread t4(std::move(t3)); // t4 is now running f2(). t3 is no longer a thread
    std::thread t5(&foo::bar, &f); // t5 runs foo::bar() on object f
    std::thread t6(b); // t6 runs baz::operator() on a copy of object b
    t2.join();
    t4.join();
    t5.join();
    t6.join();
    std::cout << "Final value of n is " << n << '\n';
    std::cout << "Final value of f.n (foo::n) is " << f.n << '\n';
    std::cout << "Final value of b.n (baz::n) is " << b.n << '\n';

}