* [QLayout](http://doc.qt.io/qt-5/qlayout.html)
* [QBoxLayout](http://doc.qt.io/qt-5/qboxlayout.html)
* [QGridLayout](http://doc.qt.io/qt-5/qgridlayout.html)
* [QFormLayout](http://doc.qt.io/qt-5/qformlayout.html)
* [All Layout](http://doc.qt.io/qt-5/layout.html)
* [QMargins](http://doc.qt.io/qt-5/qmargins.html)

## 简介
* Qt布局使用很简单
```cpp
// main.cpp
QVBoxLayout* layout = new QVBoxLayout(this);

layout->setMenuBar(menuBar);
layout->addWidget(label);

w.setLayout(layout);
w.show();
```
* 注意，菜单栏设置布局使用的是setMenuBar
* 不在main.cpp而在窗口的构造函数中直接添加控件实现时，发现布局不起作用，原因在于窗口有一个中央窗体，必须使用setCentralWidget在此之上布局
```cpp
QWidget* centralWidget = new QWidget();
centralWidget->setLayout(layout);
setCentralWidget(centralWidget);
```
* 完整的实现
```cpp
// tabtest.cpp（即MainWindow.cpp）
#include "tabtest.h"

tabtest::tabtest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // 菜单栏，因为使用了布局所以setGeometry会失效
    menuBar = new QMenuBar(this);
    // menuBar->setGeometry(QRect(0, 0, 400, 24));
    QMenu* menu = menuBar->addMenu(tr("&File"));
    actionNew = menu->addAction(tr("&New"));
    actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

    // 左侧导航
    tabWidget = new QTabWidget(this);
    dlg = new dlg1();
    QLabel *label = new QLabel();
    label->setText(u8"测试2");
    tabWidget->addTab(dlg, u8"页面1");
    tabWidget->addTab(label, u8"页面2");
    tabWidget->setTabPosition(QTabWidget::West);
    // tabWidget->setGeometry(QRect(0, 0, width(), height()));
    tabWidget->tabBar()->setStyle(new CustomTabStyle);

    // 最下方的一个label
    QLabel *label2 = new QLabel();
    label2->setText(u8"test label");

    // 垂直布局
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMenuBar(menuBar);
    layout->addWidget(tabWidget);
    layout->addWidget(label2);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}
```
* 布局也能直接用addLayout嵌套
```cpp
QHBoxLayout* layout1 = new QHBoxLayout(this);
QVBoxLayout* layout2 = new QVBoxLayout(this);
... // layout1添加内容
layout2->addLayout(layout1); // 把layout1的横排布局作为一个内容
... // layout2再添加其他内容
```

## 几种布局
* 所有的布局管理器
```cpp
QHBoxLayout：水平方向从左到右
QVBoxLayout：竖直方向从上到下
QGridLayout：栅格布局
QFormLayout：表格布局，每一行前面是一段文本，文本后接一个组件（通常是输入框）
QStackedLayout：层叠布局，允许组件 Z 轴层叠，做出类似于tabWidget效果
```
* [QBoxLayout](http://doc.qt.io/qt-5/qboxlayout.html)设置控件比例和间隙
```cpp
// 设置控件比例
void QBoxLayout::setStretch(int index, int stretch);
// 第一个参数为控件索引，第二个为延伸值
layout1->setStretch(0, 1);
layout1->setStretch(1, 2);
// 这样两个控件的比例为1：2，分别设置为5，10结果也一样

// 末尾填充，实现控件靠左
void QBoxLayout::addStretch(int stretch = 0); // 参数设置为1即可

// 在对应索引位置插入填充
void QBoxLayout::insertStretch(int index, int stretch = 0);

// 设置控件间隔
void QBoxLayout::setSpacing(int spacing);

// 在对应索引位置设置间隔
void QBoxLayout::insertSpacing(int index, int size);

// 利用QSpacerItem作为间隔
// 后两个参数是大小调整策略
QSpacerItem(int *w*, int *h*,
    QSizePolicy::Policy *hPolicy* = QSizePolicy::Minimum,
    QSizePolicy::Policy *vPolicy* = QSizePolicy::Minimum)

void QBoxLayout::addSpacerItem(QSpacerItem *spacerItem);
void QBoxLayout::insertSpacerItem(int index, QSpacerItem *spacerItem);
// 空隙尽可能扩展
layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
```
* [QGridLayout](http://doc.qt.io/qt-5/qgridlayout.html)
```cpp
void addItem(QLayoutItem *item, int row, int column,
    int rowSpan = 1, int columnSpan = 1, Qt::Alignment alignment = ...);
void addLayout(QLayout *layout, int row, int column, Qt::Alignment alignment = ...);
void addLayout(QLayout *layout, int row, int column,
    int rowSpan, int columnSpan, Qt::Alignment alignment = ...);

void addWidget(QWidget *widget, int row, int column, Qt::Alignment alignment = ...);
void addWidget(QWidget *widget, int fromRow, int fromColumn,
    int rowSpan, int columnSpan, Qt::Alignment alignment = ...);

void setHorizontalSpacing(int spacing);
void setVerticalSpacing(int spacing);

void setRowStretch(int row, int stretch); // 设置某行的比例
void setColumnStretch(int column, int stretch);

void setRowMinimumHeight(int row, int minSize);
void setColumnMinimumWidth(int column, int minSize);
```
* 让控件占QGridLayout的多个单元格
```cpp
btn1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
btn2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
btn3->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
mainLayout->addWidget(btn1, 0, 0, 1, 1); // btn1在位置(0, 0)并占1行1列
mainLayout->addWidget(btn2, 0, 1, 1, 1); // btn2在位置(0, 1)并占1行1列
mainLayout->addWidget(btn3, 1, 0, 1, 2); // btn在位置(1, 0)并占1行2列
```

* [QFormLayout](http://doc.qt.io/qt-5/qformlayout.html)
```cpp
void addRow(QWidget *label, QWidget *field);
void addRow(QWidget *label, QLayout *field);
void addRow(const QString &labelText, QWidget *field);
void addRow(const QString &labelText, QLayout *field);
void addRow(QWidget *widget);
void addRow(QLayout *layout);

void insertRow(int row, QWidget *label, QWidget *field);
void insertRow(int row, QWidget *label, QLayout *field);
void insertRow(int row, const QString &labelText, QWidget *field);
void insertRow(int row, const QString &labelText, QLayout *field);
void insertRow(int row, QWidget *widget);
void insertRow(int row, QLayout *layout);

void removeRow(int row);
void removeRow(QWidget *widget);
void removeRow(QLayout *layout);

void setFieldGrowthPolicy(QFormLayout::FieldGrowthPolicy policy); // 设置控件大小策略
```

| Constant | Value | Description |
|:-:|:-:|:-:|
| QFormLayout::FieldsStayAtSizeHint | 0 | 可以被拉伸不超过SizeHint值，这是QMacStyle的默认值 |
| QFormLayout::ExpandingFieldsGrow | 1 | 大小策略设置了水平的Expanding或MinimumExpanding的控件被尽可能拉伸占满布局，其他同FieldsStayAtSizeHint |
| QFormLayout::AllNonFixedFieldsGrow | 2 | 全部可以被拉伸占满布局，是大多数样式的默认策略 |

* 设置间距
```cpp
// QBoxlayout
void setSpacing(int spacing);

// QGridLayout和QFormLayout
void setHorizontalSpacing(int spacing);
void setVerticalSpacing(int spacing);
```

## [QLayout](http://doc.qt.io/qt-5/qlayout.html)
* 通过setSizeConstraint设置大小约束
```cpp
void setSizeConstraint(QLayout::SizeConstraint)
```

| Constant | Value | Description |
|:-:|:-:|:-:|
| QLayout::SetDefaultConstraint | 0 | The main widget's minimum size is set to [minimumSize](http://doc.qt.io/qt-5/qlayout.html#minimumSize)(), unless the widget already has a minimum size. |
| QLayout::SetFixedSize | 3 | The main widget's size is set to [sizeHint](http://doc.qt.io/qt-5/qlayoutitem.html#sizeHint)(); it cannot be resized at all. |
| QLayout::SetMinimumSize | 2 | The main widget's minimum size is set to [minimumSize](http://doc.qt.io/qt-5/qlayout.html#minimumSize)(); it cannot be smaller. |
| QLayout::SetMaximumSize | 4 | The main widget's maximum size is set to [maximumSize](http://doc.qt.io/qt-5/qlayout.html#maximumSize)(); it cannot be larger. |
| QLayout::SetMinAndMaxSize | 5 | The main widget's minimum size is set to [minimumSize](http://doc.qt.io/qt-5/qlayout.html#minimumSize)() and its maximum size is set to [maximumSize](http://doc.qt.io/qt-5/qlayout.html#maximumSize)(). |
| QLayout::SetNoConstraint | 1 | The widget is not constrained. |

* 通过setContentsMargins设置边距
```cpp
void setContentsMargins(int left, int top, int right, int bottom);
void QLayout::setContentsMargins(const QMargins &margins);
```
* [QMargins](http://doc.qt.io/qt-5/qmargins.html)
```cpp
QMargins(int left, int top, int right, int bottom)
```

## QLayout: Attempting to add QLayout XXX to XXX, which already has a layout
* 此警告原因在于多次使用setLayout
```cpp
btn1 = new QPushButton(this); // 1
btn1->setText("test");
QHBoxLayout* layout = new QHBoxLayout(this); // 2
layout->addWidget(btn1);

QWidget* centralWidget = new QWidget(this);
centralWidget->setLayout(layout); // 3
setCentralWidget(centralWidget);
```
* 1和2两次用this隐式设置了窗口布局，3又再次显式使用setLayout，解决方法是删掉2的this，或不使用3的setLayout方法

## 从布局中移除控件
* 移除
```cpp
while (layout->count()) layout->removeItem(layout->itemAt(layout->count()-1));
// 也可以用removeWidget
while (layout->count()) layout->removeWidget(layout->itemAt(layout->count()-1)->widget());
```
* 当前窗口为QMainWindow，将layout设为成员
```cpp
auto w = new QWidget;
layout = new QVBoxLayout;
myWidget = new MyWidget; // myWidget中包含hide()的表格
layout->addWidget(myWidget);
w->setLayout(layout);
setCentralWidget(w);

// 槽函数中移除
void dlg::slots()
{
    // layout->removeWidget(myWidget); // 移除后还需要close，实际直接close即可
    myWidget->close();
    myWidget = new MyWidget;
    // connect(this, &QPushButton::clicked, myWidget, &MyWidget::showTable);
    // 若要连接新的myWidget，必须写在此处，否则连接的是之前close的myWidget
    layout->addWidget(myWidget); // 再次加入即可
}
```
