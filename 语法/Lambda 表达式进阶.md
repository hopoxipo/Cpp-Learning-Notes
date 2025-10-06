* **Lambda 表达式**，这里说几点需要注意的点及其扩展
  * 使用初始化捕获时（C++14），注意用到的局部变量如果没被捕获，不能直接使用，如：
    ```cpp 
        int x = 10;
        auto init_capture = [y = x + 5]() {  // 创建新变量 y，初始化为 x+5
            std::cout << "y = " << y << std::endl;
            //std::cout << "x = " << x << std::endl; // 不能直接使用 x 变量
        };
        init_capture(); // 输出: y = 15
    ```
    要在函数体内使用 x 必须将其捕获如：`auto init_capture = [y = x + 5, x](){/../};`
  * 不支持修改值捕获参数，想要更改其值需加 **mutable** 关键字，但修改的只是该变量副本，源值并不会改变。
    ```cpp
        int x = 10;

        // 没有 mutable：值捕获的变量是 const 的
        auto non_mutable = [x]() {
            // x++; // 错误：不能修改值捕获的变量
        };

        // 使用 mutable：可以修改值捕获的变量（修改的是副本）
        auto mutable_lambda = [x]() mutable {
            x++;
            std::cout << "Internal x: " << x << std::endl;
        };

        std::cout << "Before: " << x << std::endl; // 输出: Before: 10
        mutable_lambda(); // 输出: Internal x: 11
        mutable_lambda(); // 输出: Internal x: 12
        std::cout << "After: " << x << std::endl;  // 输出: After: 10（外部变量未改变）
    ```

    为什么每次调用 `mutable_lambda` 时，x 的值会保持上一次修改后的值呢？因为 **Lambda 表达式实际上是一个函数对象（仿函数）**，**当我们编写一个 Lambda 表达式时，编译器会生成一个匿名的类，这个类重载了函数调用运算符 operator()**。捕获的变量成为这个类的成员变量。对于值捕获的变量，相当于在函数对象内部有一个成员变量，该成员变量通过外部变量初始化。每次调用 Lambda 表达式时，都是在同一个函数对象上调用 `operator()`。因此，如果 Lambda 被声明为 mutable，那么每次调用都可以修改它自己的成员变量，从而保持状态。
    以上面的例子为例，实际上会被编译器转换为类似这样的代码：
    ```cpp
        // 编译器生成的匿名类
        class __AnonymousLambdaClass {
        private:
            int x;  // 捕获的变量的副本
        public:
            // 构造函数：用外部变量初始化内部成员
            __AnonymousLambdaClass(int external_x) : x(external_x) {}
            
            // 函数调用运算符（因为有 mutable，所以不是 const）
            void operator()() {
                x++;
                std::cout << "Internal x: " << x << std::endl;
            }
        };

        // 实际的使用
        int x = 10;
        __AnonymousLambdaClass mutable_lambda(x);  // 创建对象，初始化内部 x = 10

        mutable_lambda();  // 调用 operator()，内部 x 变成 11
        mutable_lambda();  // 再次调用，内部 x 变成 12（同一个对象）
    ```

    但要注意在循环或函数中创建新的 lambda 就是不同的对象，如：
    ```cpp
        #include <iostream>

        void demo() {
            int x = 10;
            
            for (int i = 0; i < 3; i++) {
                // 每次循环都创建新的 lambda 对象
                auto lambda = [x]() mutable {
                    x++;
                    std::cout << "x = " << x << std::endl;
                };
                lambda();  // 每次都输出 11（每个新对象都从 x = 10 开始）
            }
        }

        int main() {
            demo();
            return 0;
        }
    ```

  * Lambda 表达式可立即调用 IILE
    ```cpp
    // 立即调用的 lambda 表达式 (IILE - Immediately Invoked Lambda Expression)
    int result = [](int a, int b) {
        return a * a + b * b;
    }(3, 4); // 立即调用，传递参数 3 和 4

    std::cout << result << std::endl; // 输出: 25

    // 用于复杂初始化
    auto config = []() -> std::map<std::string, int> {
        std::map<std::string, int> config_map;
        config_map["timeout"] = 1000;
        config_map["retries"] = 3;
        config_map["port"] = 8080;
        return config_map;
    }(); // 立即调用，返回配置映射
    ``` 
  * Lambda 类型、储存注意事项
    ```cpp
        // 每个 lambda 都有唯一的、编译器生成的类型
        auto lambda1 = []() {};
        auto lambda2 = []() {};  // lambda1 和 lambda2 类型不同

        // lambda1 和 lambda2 的类型不同，即使代码完全相同
        static_assert(!std::is_same_v<decltype(lambda1), decltype(lambda2)>);

        // 可以使用 std::function 进行类型擦除
        std::function<void()> func1 = lambda1;
        std::function<void()> func2 = lambda2;  // 现在类型相同了

        // 判断 lambda 是否捕获了变量
        auto capturing = [x = 1]() {};    // 有捕获
        auto non_capturing = []() {};     // 无捕获

        // 无捕获的 lambda 可以转换为函数指针
        void (*func_ptr)() = non_capturing;  // 正确
        // void (*func_ptr2)() = capturing;  // 错误：有捕获的 lambda 不能转换
    ```
 * 现在介绍一下 Lambda 表达式的递归用法
   * 结合 std::function 实现递归
     ```cpp
     #include <iostream>
     #include <functional>

     int main()
     {
        std::function<int(int)> func;
        func = [&func](int n) -> int{
          if (n <= 1) return 1;
          return n * func(n - 1);
        };

        std::cout << func(5) << "\n";
        return 0;
     }
     ``` 
