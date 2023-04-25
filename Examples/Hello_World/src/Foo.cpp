#include <iostream>

extern "C" void __cdecl Tjo(int a, int b)
{

}

extern "C" void __cdecl Moo(int a, int b, int c)
{
    Tjo(a, b);
}

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
        std::cout << "SomeNamespace::Foo()" << std::endl;
    }
}