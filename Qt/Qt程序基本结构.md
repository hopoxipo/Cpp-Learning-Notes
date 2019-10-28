* 以VS的空项目为例，新建一个Qt界面程序。为了更深入地认识，这里不使用默认的空窗口，而是添加一个`Qt Widget Form File`，Widget.ui
* 双击`Widget.ui`文件就会打开Designer界面，可在界面中拖动控件编辑程序。这里使用Designer添加一个pushButton
* 编译`Widget.ui`文件，生成`ui_Widget.h`文件，将其导入到项目中
* `ui_Widget.h`文件内容如下
```cpp
/********************************************************************************
** Form generated from reading UI file 'Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(400, 300);
        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(100, 80, 75, 23));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", nullptr));
        pushButton->setText(QApplication::translate("Form", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
```
* 可以发现在Designer界面添加的按钮，实际上会在`setupUi`函数中生成对应的代码，所以`setupUi`可以简单理解为构造函数，它的作用就是把Designer界面内容显示出来
* 这里还需要解释的是`namespace Ui`，其中的`class Form`其实就是一个继承了`Ui_Form`的空类。之后将在`Widget.h`中声明一个指向`Ui::Form`的指针，用于操控界面。虽然`Form`是空类，但它继承了`Ui_Form`，因此`Ui_Form`中的所有非私有的内容都能用。`Form`这个类名就是Designer中默认生成的objectName，如果把objectName改成test，保存后编译，`Form`就会变为test
* 把按钮从Designer中删掉，objectName改回为`Form`，重新编译后，`ui_Widget.h`内容如下（QPushButton相关代码被删除）
```cpp
/********************************************************************************
** Form generated from reading UI file 'Widget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(400, 300);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
```

* 随后添加一个头文件`Widget.h`和源文件`Widget.cpp`（也可以取其他文件名）
* `Widget.h`内容如下，先做一个namespace的前置声明，private作用域中添加了一个指向`Ui::Form`类型的指针`ui`，可以将其理解为指向界面的指针。在源文件中用它可以直接访问界面中的控件，如访问之前添加过的按钮，只需要`ui->pushButton`。这种分离接口和实现的手法即[PImpl手法](https://en.cppreference.com/w/cpp/language/pimpl)，好处就是不需要包含`ui_Widget.h`，降低了耦合性（每次Designer界面改变都会重新编译`ui_Widget.h`，而不包含此头文件，就不需要一起重新编译）
```cpp
#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow> // 窗口派生自QMainWindow

namespace Ui { // 前置声明命名空间
    class Form;
}

class widget : public QMainWindow // 继承
{
    Q_OBJECT // 必须添加的宏

public:
    explicit widget(QWidget *parent = 0); // 必须指定显式构造
    ~widget();

private:
    Ui::Form *ui; // 指向Designer界面的指针
};

#endif // WIDGET_H
```
* `Q_OBJECT`本质是一个宏
```cpp
#define Q_OBJECT \
public: \
    Q_OBJECT_CHECK \
    QT_WARNING_PUSH \
    Q_OBJECT_NO_OVERRIDE_WARNING \
    static const QMetaObject staticMetaObject; \
    virtual const QMetaObject *metaObject() const; \
    virtual void *qt_metacast(const char *); \
    virtual int qt_metacall(QMetaObject::Call, int, void **); \
    QT_TR_FUNCTIONS \
private: \
    Q_OBJECT_NO_ATTRIBUTES_WARNING \
    Q_DECL_HIDDEN_STATIC_METACALL static void qt_static_metacall(QObject *, QMetaObject::Call, int, void **); \
    QT_WARNING_POP \
    struct QPrivateSignal {}; \
    QT_ANNOTATE_CLASS(qt_qobject, "")
```
* QObject类是Qt所有类的基类，只要是QObject的子类就要在第一行代码写上`Q_OBJECT`，它提供信号槽机制、国际化机制、Qt自行实现的反射机制，但即使不使用这些操作最好也添加`Q_OBJECT`以防出错。moc（可以理解为一种预处理器）会对标记了`Q_OBJECT`的头文件（不对.cpp文件）进行特殊处理，生成`moc_`前缀的文件
* `Widget.cpp`内容如下，这里调用`setupUi`相当于`ui_Widget.h`中的构造函数，用于显示`Widget.ui`界面的内容
```cpp
#include "Widget.h"
#include "ui_Widget.h"

widget::widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

widget::~widget()
{
    delete ui;
}
```
* 利用`ui`指针也可以访问控件来修改内容，比如在Designer界面中的按钮控件`pushButton`文本默认是`PushButton`，在`Widget.cpp`中使用`ui->pushButton->setText`即可修改按钮文本，同理也可以为其添加其他控件
```cpp
#include "Widget.h"
#include "ui_Widget.h"

widget::widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->pushButton->setText("new");
}

widget::~widget()
{
    delete ui;
}
```
* 也可以不通过`ui`指针，也就是不利用Desiner界面，直接用代码生成，比如直接在`Widget.cpp`的构造函数中添加控件
```cpp
widget::widget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* btn = new QPushButton(this);
    btn->setText("test");
    layout->addWidget(btn);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}
```
* 最后在`main.cpp`中包含`Widget.h`，声明一个`Widget`对象并对其调用`show()`来显示此窗口
```cpp
#include "Widget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    widget w;
    w.show();
    return a.exec();
}
```
