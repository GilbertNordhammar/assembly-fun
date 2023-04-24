#include <iostream>

extern "C" int __cdecl GetSum(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}

extern "C" void __cdecl HelloWorldCStyle()
{
    std::cout << "Hello World from outside ASM with C-style!" << std::endl;
}

void __cdecl HelloWorldCPlusPlusStyle()
{
    std::cout << "Hello World from outside ASM C++ style!" << std::endl;
}

namespace SomeNamespace
{
    void Foo()
    {
        std::cout << "Calling SomeNamespace::Foo()" << std::endl;
    }
}