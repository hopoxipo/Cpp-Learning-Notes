* 元素类型不能是引用，因为指向引用的指针（迭代器）是非法的
```cpp
vector<int&> v; // 错误
```
* 可以保存指针，但指针必须先初始化再添加到容器
```cpp
int* p;
vector<int*> v { p }; // 错误：使用了未初始化的局部变量
v[0] = new int(42); // 错误
```
* 添加元素实际执行的是拷贝操作，即保存的指针与原始对象已经无关了
```cpp
int* p;
vector<int*> v;
v.push_back(p); // 这样做是可以的
v[0] = new int(42); // 并没有给*p赋值
std::cout << *v[0]; // 42
std::cout << *p; // 错误：p没有被初始化
```
* 迭代器类型分为iterator、const_iterator，对象是const时`begin()和end()`才返回const_iterator，要专门得到const_iterator则可以使用cbegin和cend
```cpp
vector<int> v(10);
auto it = cbegin(v); // it类型为vector<int>::const_iterator
```
* 迭代器转指针
```cpp
int* p = &*begin(v);
```
* 用数组初始化vector，只需要传入两个指针确定范围
```cpp
int arr[] = {0, 1, 2, 3, 4, 5};
std::vector<int> v(begin(arr), end(arr));
std::vector<int> v2(arr + 1, arr + 4); // arr[1]、arr[2]、arr[3]
```
* 删除指定元素
```cpp
std::vector<int> v{1, 2, 3, 2 };
while (auto it = v.begin(); it != end(v))
{
    if (*it == 2) it = v.erase(it);
    else ++it;
}
```
* 不同容器的迭代器不能比较
```cpp
auto f(std::vector<int> v) // 传值，拷贝一份容器
{
    return v.begin();
}

std::vector<int> v { 1, 2 };
if(v.begin() == f(v)) ... // 错误：函数返回的迭代器来自临时拷贝的容器
// 更大的问题是，临时容器对象在函数结束后被销毁，返回的迭代器根本不能使用
```

## 使用实例
* 矩阵乘法
```cpp
#include <iostream>
#include <vector>

int main()
{
    int m = 2; // 矩阵1行数
    int n = 2; // 矩阵1列数和矩阵2行数
    int r = 2; // 矩阵2列数
    int tmp;
    std::vector<int> row; // 用来临时保存行
    std::vector<std::vector<int>> matrix1;
    std::vector<std::vector<int>> matrix2;
    std::vector<std::vector<int>> result;
    // 矩阵1
    std::cout << "Input first(" << m << "*" << n << ")matrix:\n";
    for (int i = 1; i <= m; ++i)
    {
        std::cout << "Type in " << n << " values for row" << i
            << " separated by spaces:";
        for (int j = 1; j <= n; ++j)
        {
            std::cin >> tmp;
            row.push_back(tmp);
        }
        matrix1.push_back(row);
        row.clear();
    }
    // 矩阵2
    std::cout << "Input second(" << n << "*" << r << ")matrix:\n";
    for (int i = 1; i <= n; ++i)
    {
        std::cout << "Type in " << r << " values for row" << i
            << " separated by spaces:";
        for (int j = 1; j <= r; ++j)
        {
            std::cin >> tmp;
            row.push_back(tmp);
        }
        matrix2.push_back(row);
        row.clear();
    }
    // 计算结果
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < r; ++j)
        {
            tmp = 0;
            for (int k = 0; k < n; ++k) tmp += matrix1[i][k] * matrix2[k][j];
            row.push_back(tmp);
        }
        result.push_back(row);
        row.clear();
    }
    // 打印结果
    for (auto x : matrix1)
    {
        for (auto y : x) std::cout << y << " ";
        std::cout << '\n';
    }
    std::cout << "times\n";
    for (auto x : matrix2)
    {
        for (auto y : x) std::cout << y << " ";
        std::cout << '\n';
    }
    std::cout << "equals\n";
    for (auto x : result)
    {
        for (auto y : x) std::cout << y << " ";
        std::cout << '\n';
    }
}
```
* 大数阶乘
```cpp
#include <iostream>
#include <vector>

int main()
{
    std::vector<char> result; // 也可以用std::string
    // result.reserve(10000);
    result.emplace_back(1); // 默认值为1
    int n = 100;
    for (int i = 1; i <= n; ++i) // 依次计算i的阶乘
    {
        int carry = 0; // 进位值
        for (std::size_t j = 0; j < result.size(); ++j) // 依次计算结果的各个数位
        {
            int tmp = result[j] * i + carry;
            result[j] = tmp % 10;
            carry = tmp / 10; // 其他用于进位
        }
        while (carry > 0) // 如果还有多余的进位值依次置于高位
        {
            result.emplace_back(carry % 10); // 放置高位值
            carry /= 10;
        }
    }
    std::cout << n << "! = ";
    for (auto it = result.crbegin(); it != result.crend(); ++it)
    {
        std::cout << static_cast<int>(*it);
    }
}
```

## 二分查找
* 采用左闭右开的写法
```cpp
int binarySearch(std::vector<int>& v, int n)
{
    // 区间为[l, r)
    int l = 0;
    int r = v.size();
    int mid;
    while(l < r) // 条件保持一致
    {
        mid = l + (r-l)/2;
        if(v[mid] == n) return mid;
        if(v[mid] < n) l = mid + 1;
        if(v[mid] > n) r = mid; // 右开
    }
    return -1;
}

int main()
{
    std::vector<int> v { 1, 3, 5, 7, 9 };
    for(auto x : v)
    {
        std::cout << binarySearch(v, x); // 01234
    }
    std::cout << binarySearch(v, 2); // -1
}
```
* 第一个大于等于n的元素
```cpp
int lowerBound(std::vector<int>& v, int n)
{
    int l = 0;
    int r = v.size();
    int mid;
    while(l < r)
    {
        mid = l + (r-l)/2;
        if(v[mid] < n) l = mid + 1;
        // 以上与二分查找一样
        if(v[mid] >= n) r = mid; // 相等继续左压
    }
    return l; // 最终返回l（返回r也一样，l==r）：对于空vector则返回0
    // std::lower_bound对于空vector返回begin(v)或end(v)（空vector两者相等）
}

int main()
{
    std::vector<int> v { 1, 2, 2, 3 };
    std::cout << lowerBound(v, 1); // 0
    std::cout << lowerBound(v, 2); // 1
    std::cout << lowerBound(v, 3); // 3
    std::cout << lowerBound(v, 5); // 4：找不到则返回尾后
}
```
* 第一个大于n的元素
```cpp
int upperBound(std::vector<int>& v, int n)
{
    int l = 0;
    int r = v.size();
    int mid;
    while(l < r)
    {
        mid = l + (r-l)/2;
        if(v[mid] <= n) l = mid + 1; // 相等则右压
        if(v[mid] > n) r = mid;
    }
    return l;
}

int main()
{
    std::vector<int> v { 1, 2, 2, 3 };
    std::cout << upperBound(v, 1); // 1
    std::cout << upperBound(v, 2); // 3
    std::cout << upperBound(v, 3); // 4：找不到则返回尾后
    std::cout << upperBound(v, 5); // 4：找不到则返回尾后
}
```
* 从上面两个函数可以看出r是用来判断条件的，比如找到第一个满足某种条件的元素
```cpp
int f(std::vector<int>& v, int n)
{
    int l = 0;
    int r = v.size();
    int mid;
    while(l < r)
    {
        mid = l + (r-l)/2;
        if(check(mid)) r = mid;
        else l = mid + 1;
    }
    return l; // 找不到则返回尾后
}
```
* 也可以采用右闭
```cpp
int f(std::vector<int>& v, int n)
{
    // 区间为[l, r]
    int l = 0;
    int r = v.size() - 1; // 右闭
    int mid;
    while(l <= r) // 条件与右闭一致
    {
        mid = l + (r-l)/2;
        if(check(mid)) r = mid - 1; // 因为右闭所以要减1
        else l = mid + 1;
    }
    return l;
    // 找不到同样将返回尾后
}
```
