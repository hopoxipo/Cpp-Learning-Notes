## 定义和初始化[std::string](https://en.cppreference.com/w/cpp/std::string/basic_std::string)对象
* 直接初始化和拷贝初始化
```cpp
std::string s("hello"); // 直接初始化
std::string s2 = "hello"; // 拷贝初始化
// 多个值的初始化尽量用直接初始化
std::string s3(10, 'c');
std::string s4 = std::string(10, 'c'); // 开销大
```

## 字符串转换
* char\*转std::string
```cpp
const char* s1 = "hello";
std::string s2(s1);
```
* std::string转char\*
```cpp
std::string s1("hello");
const char* s2 = s1.c_str();
```
* std::string转int
```cpp
int i = std::stoi(str);
```
* QString、std::string、const char\*的互转
```cpp
std::string str = qstr.toStdString();
QString qstr1 = QString::fromLocal8Bit(str.c_str()))
QString qstr2 = QString::fromStdString(str);
```
* 格式化到char[]
```cpp
char s[100];
int i = 1, j = 2;
int n = std::sprintf(s, "%d + %d = %d", i, j, i + j); // n == strlen(s)
std::cout << s; // 1 + 2 = 3
std::cout << n; // 9


char s2[100];
const char* str1 = "hello";
const char* str2 = " world";
std::sprintf(s2, "%s%s", str1, str2);
std::cout << s2; // hello world
```
* 用加法运算符连接，两侧必须至少有一个std::string对象，才会把另一个也转为std::string
```cpp
std::string s1 = "hello", s2 = "world";
std::string s3 = s1 + ", " + s2 + "\n";
std::string s4 = "hello" + ", " + s2; // 错误，"hello" + ", "
std::string s5 = s1 + ", " + "world"; // 正确
```

## 手动实现一个atoi（string到int的转换）
```cpp
int myAtoi(std::string s)
{
    long tar = 0;
    int n = 0;
    while(s[n] == ' ')
    {
        ++n;
        if(n == s.length()) return 0;
    }
    if(isalpha(s[n])) return 0;
    int flag = 1;
    if(s[n] == '+') ++n;
    else if(s[n] == '-')
    {
        flag = -1;
        ++n;
    }
    while(isdigit(s[n]))
    {
        tar = tar*10 + s[n] - '0';
        if(tar > INT_MAX) return (flag > 0)? INT_MAX : INT_MIN;
        ++n;
    }
    long res = flag * tar;
    if(res > INT_MAX) return INT_MAX;
    else if(res < INT_MIN) return INT_MIN;
    return res;
}
```

## size_type和std::size_t
* size_type是std::string和std::vector的长度类型，标准库中定义为unsigned
* std::size_t是cstddef头文件中的类型，也是unsigned
* std::vector下标类型为std::vector::size_type，数组则是std::size_t
* std::std::string::npos代表std::size_t的最大值，因为std::size_t为unsigned，所以-1就是最大值
```cpp
static const std::size_t npos = -1;
```

## 清空std::string
* clear删除内容使size为0，但capacity不变，用shrink_to_fit使capacity适应size
```cpp
std::string s("12345678901234567890");
std::cout << s.size() << " " << s.capacity() << '\n'; // 20 31
s.clear();
std::cout << s.size() << " " << s.capacity() << '\n'; // 0 31
s.shrink_to_fit();
std::cout << s.size() << " " << s.capacity() << '\n'; // 0 15
```

## 处理std::string对象中的字符
* cctype头文件包含了一组检测字符特性的函数
```cpp
isalnum(c); // 数字或字母
isalpha(c); // 字母
iscntrl(c); // 控制字符
isdigit(c); // 数字
isgraph(c); // 不是空格但可打印
islower(c); // 小写字母
isprint(c); // 是可打印字符（即c是空格或具有可视形式）
ispunct(c); // 标点
isspace(c); // 空白（空格、制表符、回车符、换行符等）
isupper(c); // 大写字母
isxdigit(c); // 十六进制数字
tolower(c); // 转小写
toupper(c); // 转大写
```

## 用[substr](https://en.cppreference.com/w/cpp/string/basic_string/substr)获取子字符串
```cpp
std::string s("hello world");
std::string s2 = s.substr(0, 5); // hello：从位置0开始的5个字符
std::string s3 = s.substr(6); // world：从位置6开始到结尾的字符
std::string s4 = s.substr(6, 100); // world
std::string s5 = s.substr(11); // s5为空
std::string s6 = s.substr(12); // 抛出out_of_range异常
std::size_t pos = str.find("world"); // pos = 6
std::string s7 = str.substr(pos); // world
```
* 用[std::boyer_moore_searcher](https://en.cppreference.com/w/cpp/utility/functional/boyer_moore_searcher)、[std::boyer_moore_horspool_searcher](https://en.cppreference.com/w/cpp/utility/functional/boyer_moore_horspool_searcher)搜索子串
```cpp
// s为目标字符串，p为子串，检索到则返回目标迭代器，否则返回尾后迭代器
auto it = std::search(s.begin(), s.end(),
    std::boyer_moore_horspool_searcher(p.begin(), p.end()));
```

## [\<cstring\>库函数](https://en.cppreference.com/w/cpp/header/cstring)
```cpp
char* strcpy( char* dest, const char* src );
char *strncpy( char *dest, const char *src, std::size_t count );
char *strcat( char *dest, const char *src );
char *strncat( char *dest, const char *src, std::size_t count );
std::size_t strlen( const char* str );
int strcmp( const char *lhs, const char *rhs );
void* memset( void* dest, int ch, std::size_t count );
// 常用于清空数组memset(buf, 0, sizeof(buf))
void* memcpy( void* dest, const void* src, std::size_t count );
```

## Manacher算法
* 求一个字符串的最长回文子串，最简单的方法是依次以每个字符为中心，向两边扩展，遍历出所有的回文子串，取其中最长的
* 偶数长度的子串中心点为两个相同字符，因此还需要以两个相同的字符为中心，向两边扩展
* 为了统一上面两种情况，一个做法是在每个字符之间插入一个符号，这样相当于在偶数长度子串中间插入了一个点
```cpp
#a#b#b#a#
012345678

0 # => #
1 a => #a#
2 # => #
3 b => #b#
4 # => b#b => #b#b# => a#b#b#a => #a#b#b#a#
5 b => #b#
6 # => #
7 a => #a#
8 # => #
```
* 引入一个数组，用于表示每个位置的最大回文半径，比如对上面的例子
```cpp
#a#b#b#a#
012345678

i    = 0 1 2 3 4 5 6 7 8
p[i] = 1 2 1 2 5 2 1 2 1
// 可以发现 p[i] - 1 就是原来的回文子串长度，最大回文子串长度就是 5 - 1 = 4
```
* 于是求最大回文子串就可以转化为求该字符串对应的数组p
* Manacher算法的优化点在于，对于一个`p[i]`，其之前的`p[j] (j < i)`已经求过了，`p[j]`的右边界为`j+p[j]`，将最大的右边界记为mx，mx对应的j记为id，这个右边界只会不断右移而不会回退
```cpp
#a#b#b#a#
012345678

i    = 0 1 2 3 4 5 6 7 8
p[i] = 1 2 1 2 5 2 1 2 1
mx   = 1 3 3 5 9 7 7 9 9
```
* 现在计算`p[i]`，i的位置有两种情况，一是与右边界相等，二是在右边界内
* 先考虑i在右边界内的情况。考虑i关于id的对称点j，因为`p[j]`已经计算过，根据`p[j]`的情况来考虑`p[i]`，此时有三种情况
```cpp
// i < mx
__________id__________mx
__j_______id_______i__mx // j为i关于id的对称点，j = id-(i-id) = 2*id-i
```
* 如果j的左边界超出了id的左边界
```cpp
// if(i < mx && p[j] > mx - i) p[i] = mx-i;
 ___________id___________mx
---j---_____id________i__mx // 必定p[i] = mx-i

// 如果p[i]右边界超出mx的话，即p[i] > mx-i
---j---_____id_____---i---mx // id的右边界应该超出mx，与mx是最大右边界的前提矛盾
```
* 如果j的左边界小于id的左边界
```cpp
// if(i < mx && p[j] < mx - i) p[i] = p[j];
___________id___________mx
_-j-_______id________i__mx // 必定p[i] = p[j]
```
* 如果j的左边界与id的左边界重合
```cpp
// if(i < mx && p[j] == mx - i) p[i] = p[j]; // 仍可能增加
___________id___________mx
--j--______id________i__mx // p[i] >= p[j]或mx-i，p[i]可以继续增加，还需要向两边扩展计算p[i]
```
* 综上
```cpp
if(i < mx)
{
    if(p[2*id-i] < mx - i) p[i] = p[2 * id - i];
    if(p[2*id-i] > mx - i) p[i] = mx - i;
    if(p[2*id-i] == mx - i)
    {
        p[i] = mx - i;
        while(str[i-p[i]] == str[i+p[i]]) ++p[i];
    }
}
```
* 第二种情况，如果当前的i抵达了右边界位置，即`i == mx`，则向两边扩展计算`p[i]`
```cpp
if(i == mx)
{
    p[i] = 1;
    while(str[i-p[i]] == str[i+p[i]]) ++p[i];
}
```
* 综合以上两种情况
```cpp
if(i < mx)
{
    if(p[2*id-i] < mx - i) p[i] = p[2 * id - i];
    if(p[2*id-i] > mx - i) p[i] = mx - i;
    if(p[2*id-i] == mx - i)
    {
        p[i] = mx - i;
        while(i >= p[i] && str[i-p[i]] == str[i+p[i]]) ++p[i]; // 需要确保i-p[i] >= 0防越界
    }
}
else if(i == mx)
{
    p[i] = 1;
    while(i >= p[i] && str[i-p[i]] == str[i+p[i]]) ++p[i];
}

// 上述代码可简化为
if(i < mx) p[i] = std::min(p[2 * id - i], mx - i);
else if(i == mx) p[i] = 1;
while(i >= p[i] && str[i-p[i]] == str[i+p[i]]) ++p[i];
```
* 最后每次对当前的i计算出`p[i]`后，再与右边界比较，如果超出当前最大右边界，则将其更新为i的右边界
```cpp
if(mx < i + p[i])
{
    id = i;
    mx = i + p[i];
}
```
* 最终代码如下
```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> Manacher(std::string s)
{
    std::string str{ "#" };
    for(auto& x : s)
    {
        str.push_back(x);
        str.push_back('#');
    }
    int sz = str.size();
    std::vector<int> p(sz); // 目标数组
    int start = 0; // 最长回文子串首字符在原字符串中的下标
    int maxLen = 0; // 最长回文子串的长度

    int id = 0;
    int mx = 0;

    for (int i = 0; i < sz; ++i)
    {
        if (i < mx) p[i] = std::min(p[2 * id - i], mx - i);
        else if(i == mx) p[i] = 1;
        while (i >= p[i] && str[i - p[i]] == str[i + p[i]]) ++p[i]; // i >= p[i]是为了防越界
        if (mx < i + p[i]) // 检查是否超出右边界
        {
            id = i;
            mx = i + p[i];
        }
        if(p[i] - 1 > maxLen) // p[i] - 1 是回文子串的长度
        {
            start = i;
            maxLen = p[i] - 1;
        }
    }
    std::cout << "string: " << s << "\n";
    std::cout << "longest palindrome substring: " << s.substr((start - p[start] + 1)/2, maxLen) << "\n";
    std::cout << "length: " << maxLen << "\n";
    return p;
}

int main()
{
    std::string s("abba");
    std::vector<int> p = Manacher(s);
    for (auto x : p) std::cout << x;
}
```
* 打印过程更易于理解
```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<int> Manacher(std::string s)
{
    std::string str{ "#" };
    for(auto& x : s)
    {
        str.push_back(x);
        str.push_back('#');
    }
    int sz = str.size();
    std::vector<int> p(sz);
    int start = 0;
    int maxLen = 0;

    int id = 0;
    int mx = 0;

    for (int i = 0; i < sz; ++i)
    {
        std::cout << str << " loop" << i << "\n";
        std::cout << std::string(i, ' ') << str[i] << "\n";
        if (i < mx)
        {
            std::cout << "i = " << i << " id = " << id << " mx = " << mx << "\n";
            std::cout << "i < mx\n";
            p[i] = std::min(p[2 * id - i], mx - i);
            std::cout << "2*id-i = " << 2*id - i << ", p[" << 2*id - i  << "] = " << p[2 * id - i] << ", mx - i = " << mx - i << "\n";
            std::cout << "p[" << i <<"] = " << p[i] << "\n\n";
        }
        else if(i == mx) {
            std::cout << "i = " << i << " id = " << id << " mx = " << mx << "\n";
            std::cout << "i == mx\n";
            p[i] = 1;
            std::cout << "p[" << i <<"] = " << p[i] << "\n\n";
        }
        while (i >= p[i] && str[i - p[i]] == str[i + p[i]])
        {
            ++p[i];
            std::cout << "++p[i]\n";
            std::cout << "p[" << i << "]=" << p[i] << "\n\n";
        }
        if (mx < i + p[i])
        {
            std::cout << "mx = " << mx << " i+p[i] = " << i + p[i] << "\n";
            std::cout << "mx < i+p[i]\n";
            id = i;
            mx = i + p[i];
            std::cout << "mx = " << mx << " id = " << id<< "\n\n";
        }
        if(p[i] - 1 > maxLen)
        {
            start = i;
            maxLen = p[i] - 1;
        }
    }
    std::cout << "string: " << s << "\n";
    std::cout << "longest palindrome substring: " << s.substr((start - p[start] + 1)/2, maxLen) << "\n";
    std::cout << "length: " << maxLen << "\n";
    return p;
}

int main()
{
    std::string s("abba");
    std::vector<int> p = Manacher(s);
    for (auto x : p) std::cout << x;
}
```
