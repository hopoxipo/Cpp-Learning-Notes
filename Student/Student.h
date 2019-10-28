#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::istream;
using std::ostream;

class Student {
    friend istream& read(istream&, Student&);
    friend ostream& print(ostream&, Student&);
    // friend Student& increase(Student&);
    friend void sorts(vector<Student>&);
public:
    Student() = default;
    Student(int i, string n, int j) :Number(i), Name(n), Grade(j) {}
    Student& increase();
    int getNumber() { return Number; }
    Student& update(istream&);
private:
    int Number;
    string Name;
    int Grade;
};

// 更新
Student& Student::update(istream& is)
{
    read(is, *this);
    return *this;
}

// 添加
Student& Student::increase()
{
    cout << "Please enter the imformation: \n";
    cin >> this->Number >> this->Name >> this->Grade;
    return *this;
}


istream& read(istream& is, Student& stu)
{
    is >> stu.Number >> stu.Name >> stu.Grade;
    return is;
}

ostream& print(ostream& os, Student& stu)
{
    os << stu.Number << " " << stu.Name << " " << stu.Grade << "\n";
    return os;
}

// 显示
void display()
{
    cout << "Please select your option: \n";
    cout << " A. Display " << "           "
        << " B. Increase " << "           "
        << " C. Delete " << "           "
        << " D. Update \n";
}



// 排序
void sorts(vector<Student>& stuvec) {
    std::sort(stuvec.begin(), stuvec.end(),
        [](Student& stu1, Student& stu2) { return stu1.Grade > stu2.Grade; });
}
 


// 查找
void finds(int num)
{
    
}


void sys(vector<Student> stuvec)
{
    display();
    while (true)
    {
        sorts(stuvec);
        char s = cin.get();
        switch (s) { // 控制传输跳过的实例化？？?
        case 'A': // 实现显示当前容器成员
        {
            for (auto stu : stuvec)
            {
                //ostream& os(cout << "_: \n"); // 如何设置无初始值的ostream参数变量
                ostream& os(cout); // ostream& os(cout << "\n");
                print(os, stu);
            }
            //cout << "xianshi \n";
            cout << "\n";
            display();
            break;
        }
        case 'B': // 添加操作
        {
            Student stu;
            stu.increase();
            stuvec.push_back(stu);
            //cout << "tianjia \n";
            cout << "\n";
            display();
            break;
        }
        case 'C': //  删除操作
        {
            cout << "Please enter the number of the student which you want to delete: \n";
            // int i = cin.get();  // char型
            int i;
            cin >> i;
            for (auto iter = stuvec.begin(); iter != stuvec.end();)
            {
                if (iter->getNumber() == i) iter = stuvec.erase(iter); // erase有坑
                else ++iter;
            }
            //cout << "shanchu \n";
            cout << "\n";
            display();
            break;
        }
        case 'D': // 修改操作
            cout << "Please enter the number of the student which you want to update: \n";
            int i;
            cin >> i;
            cout << "Please enter the new information: \n";
            for (auto iter = stuvec.begin(); iter != stuvec.end(); ++iter)
            {
                if (iter->getNumber() == i) iter->update(cin);
            }
            //cout << "xiugai \n";
            cout << "\n";
            display();
            break;
        }
    }
}
