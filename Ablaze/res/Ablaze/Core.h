#pragma once

#ifdef ABLAZE_PLATFORM_WINDOWS
#ifdef ABLAZE_BUILD_DLL
#define ABLAZE_API _declspec(dllexport)
#else
#define ABLAZE_API _declspec(dllimport)
#endif // ABLAZE_BUILT_IN
#else
#error Ablaze only supports Windows!
#endif // ABLAZE_PlATFORM_WINDOW

#ifdef ABLAZE_ENABLE_ASSERTS
#define ABLAZE_ASSERTS(x,...) {if(!(x)){ABLAZE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
#define ABLAZE_CORE_ASSERTS(x,...) {if(!(x)){ABLAZE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
#else
#define ABLAZE_ASSERTS(X,...)
#define ABLAZE_CORE_ASSERTS(X,...)
#endif // ABLAZE_ENABLE_ASSERTS

#define BIT(x) (1<<x)//将数字1左移x位，相当于乘以2的x次方

//&成员函数;this表示成员函数属于哪个对象;占位符
#define BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)
// void fun_1(int x,int y,int z) {
//std::cout << "print: x = " << x << ", y = " << y << ", z = " << z << std::endl;
//}
//f1的类型为 function<void(int, int, int)>
//auto f1 = std::bind(fun_1, 1, 2, 3); 					//表示绑定函数 fun 的第一，二，三个参数值为： 1 2 3
//f1(); 													//print: x=1,y=2,z=3

//auto f2 = std::bind(fun_1, std::placeholders::_1, std::placeholders::_2, 3);
//表示绑定函数 fun 的第三个参数为 3，而fun 的第一，二个参数分别由调用 f2 的第一，二个参数指定
//f2(1, 2);												//print: x=1,y=2,z=3

//auto f3 = std::bind(fun_1, std::placeholders::_2, std::placeholders::_1, 3);
//表示绑定函数 fun 的第三个参数为 3，而fun 的第一，二个参数分别由调用 f3 的第二，一个参数指定
//注意： f2  和  f3 的区别。
//f3(1, 2);												//print: x=2,y=1,z=3