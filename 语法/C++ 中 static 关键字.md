# 关于 **Static**
**Static** 作为 C++ 修饰符是用来控制变量的存储方式和可见性的。它主要有以下几个作用：
* **局部静态对象**
  * 某些时候，有必要令局部变量的生命周期贯穿函数调用及之后的时间。可以将局部变量定义成 `static` 类型从而获得这样的对象。
局部静态对象在程序的执行路径第一次经过对象定义语句时初始化，并且直到程序终止才被销毁，在此期间即使对象所在函数
结束执行也不会对它有影响。
    举个例子，下面的函数统计它自己被调用了多少次：
    ```
        size_t count_calls()
        {
            static size_t ctr = 0;      // 调用结束后，这个值仍然有效
            return ++ctr;
        }
        int main()
        {
            for (size_t i = 0; i != 10; ++i)
                cout << count_calls() << endl;
            return 0;
        }
    ```
    这段程序将输出从 1 到 10 （包括 10 在内） 的数字。



* **类的静态成员**
 有的时候类需要它的一些成员与类本省直接相关，而不是与类的各个对象保持关联。
  * **声明静态成员**
  我们通过在成员声明之前加上关键字 *Static* 使得其与类关联在一起。
  ```
      class Account {
      public:
          void calculate() { amount += amount * interestRate; }     // 成员函数不用通过作用域运算符就能直接使用静态成员
          static double rate() { return interestRate; }
          static void rate(double);
      private:
          std::string owner;
          double amount;
          static double interestRate;
          static double initRate();
      };
  ```
  类的静态成员函数不与任何对象绑定在一起，它们也不包含 *this* 指针。
  * **使用类的静态成员**
  我们可以使用作用域运算符 **::** 直接访问静态成员：
  ```
      double r;
      r = Account::rate();
  ```
  虽然静态成员不属于类的某个对象，但我们仍然可以使用类的对象、引用或者指针来访问静态成员成员：
  ```
      Account ac1;
      Account* ac2 = &ac1;
      //调用静态成员函数 rate 的等价形式
      r = ac1.rate();       // 通过 Account 的对象或引用
      r = ac2->rate();      // 通过指向 Account 对象的指针
  ```
  * **定义静态成员**
  和其他成员函数一样，我们既可以在类的内部也可以在类的外部定义静态成员函数。
    * **类外**
    当在类的外部定义静态成员时，不能重复 static 关键字， **该关键字只出现在类内部的声明语句** ：
    ```
        void Account::rate9double newRate)
        {
            interestRate = newRate;
        }
    ```
    因为静态数据成员不属于类的任何一个对象，这意味着他们不是由类的构造函数初始化的。一般来讲，我们不能在类的内部初始化
    静态成员。相反，必须在类的外部定义和初始化每个静态册灰姑娘元。和其他对象一样，一个静态数据成员只能定义一次。一旦
    它被定义，就将一直存在于程序的整个生命周期中。
    ```
    double Account::interestRate = initRate();
    ```
    * **静态成员的类内初始化**
    通常情况下，类的静态成员不应该在类的内部初始化。然而，我们可以为静态成员提供 const 整数类型的类内初始值，
    不过要求静态成员必须是字面值常量类型的 constexpr 。初始值必须是常量表达式。例如，我们可以用一个初始化了
    的静态数据成员指定数组成员的维度：
    ```
        class Account {
        public:
            static double rate() { return interestRate; }
            static void rate(double);
        private:
            static constexpr int period = 30;     // period 是常量表达式
            double daily_tbl[period];
        };
    ```
    如果某个静态成员的应用场景仅限于编译器看了一替换它的值的情况，则一个初始化的 const 或 cosnstexpr static 不需要分别
    定义。否则，如果将它用域值不饿能替换的场景中，则该成员必须有一条定义语句。例如，如果 period 的唯一用途就是定义 daily_tbl 的
    维度，则不需要在 Account 外面专门定义 period。但当需要把 Account::period 传递给一个接受 const int& 的函数时，必须定义
    period。如果在类的内部提供了一个初始值，则成员的定义不能再指定一个初始值了：
    ```
    //一个不带初始值的静态成员的定义
    constexpr int Account::period;      //初始值在类的定义内提供
    ```
    综上所述，即使一个常量静态数据成员在类内部被初始化了，通常情况下也应该在类的外部定义以下该成员。
  * **静态成员能用于某些场景，而普通成员不能**
   静态成员独立于任何对象，静态数据成员可以是不完全类型。**特别的** ：
   * 静态数据成员的类型可以就是它所属的类类型，而非静态数据成员则受到限制，只能声明成它所属类的指针或引用：  
    ```
        class Bar {
        public:
            // ...
        private:
            static Bar mem1;        // 正确：静态成员可以是不完全类型
            Bar* mem2;              // 正确：指针成员可以是不完全类型
            Bar mem3;               // 错误：数据成员必须是完全类型
        };
   ```

   * 静态成员可以做为默认实参，而非静态数据成员不能作为默认实参（因为它的值本身属于对象的一部分，这么做的结果是无法真正提供一个对象以便从中获取成员的值，最终将引发错误）：    
   ```
      class Screen {
      public:
          // bkground 表示一个在类中稍后定义的静态成员
          Screen& clear(char  = bkground);
      private:
          static const char bkground;
      };
   ```
