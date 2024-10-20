# Qt Learning

***
 

## QMessageBox

```qt
void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(NULL,"提示","是否退出系统" , QMessageBox::Yes|QMessageBox::No|QMessageBox::YesAll , QMessageBox::Yes);
}

/*
 *  1、第一个父类控件指针
 *  2、标题名称
 *  3、信息提示框的内容
 *  4、信息提示框里面有多少个按钮
 *  5、默认按下Enter时使用的按钮,(不设置则默认第一个)
*/
```


### QMessageBox 的四种用法
- information:提示框
- critical(批评、错误等)
- warning (提醒)
- question(问号框)
- about ('默认提示消息框')

---
## 表格
### tableWidget
我们设置了5x5的表格
~~~qt
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置表格
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(5);

    QStringList strList;
    strList<<"学号"<<"姓名"<<"性别"<<"分数"<<"备注";
    
    //使用标签设置水平标题
    ui->tableWidget->setHorizontalHeaderLabels(strList);
 
}
~~~

然后在主函数调用初始化表格展示函数
~~~qt
void MainWindow::Init_TableWidget_Data()
{
    QList<QString> StrId;
    StrId<<"J1000"<<"J1001"<<"J1002"<<"J1003"<<"J1004";

    for(int irow = 0 ; irow <5 ; irow++)
    {
        int icolumn = 0 ;
        // 表格内容对象
        QTableWidgetItem *pitem = new QTableWidgetItem(StrId.at(irow));

        ui->tableWidget->setItem(irow,icolumn,pitem); 
    }
}
~~~

完整代码
~~~qt
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置表格
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);

    QStringList strList;
    strList << "学号" << "姓名" << "性别" << "分数" << "备注";
    ui->tableWidget->setHorizontalHeaderLabels(strList);

    // 初始化表格数据
    Init_TableWidget_Data();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init_TableWidget_Data()
{
//    QList<QString> StrId;
//    StrId << "J1000" << "J1001" << "J1002" << "J1003" << "J1004";

//    for (int irow = 0; irow < 5; irow++)
//    {
//        int icolumn = 0;
//        QTableWidgetItem *pitem = new QTableWidgetItem(StrId.at(irow));
//        ui->tableWidget->setItem(irow, icolumn, pitem);
//    }
}

void MainWindow::on_DelButton_clicked()
{
    // 获取当前选中的行
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow != -1) {
        // 删除选中的行
        ui->tableWidget->removeRow(currentRow);
        qDebug() << "Row" << currentRow << "deleted";
    } else {
        QMessageBox::warning(this, "警告", "请先选择一行删除！");
    }
}

void MainWindow::on_AddButton_clicked()
{
    // 添加一行
    int newRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(newRow);

    // 获取输入框内容
    QString id = ui->lineEdit_ID->text();
    QString name = ui->lineEdit_Name->text();
    QString gender = ui->radioButton_Man->isChecked() ? "男" : "女";
    QString grade = ui->lineEdit_Grade->text();
    QString remark = ui->lineEdit_ReMark->text();

    // 将内容添加到表格
    ui->tableWidget->setItem(newRow, 0, new QTableWidgetItem(id));
    ui->tableWidget->setItem(newRow, 1, new QTableWidgetItem(name));
    ui->tableWidget->setItem(newRow, 2, new QTableWidgetItem(gender));
    ui->tableWidget->setItem(newRow, 3, new QTableWidgetItem(grade));
    ui->tableWidget->setItem(newRow, 4, new QTableWidgetItem(remark));

    qDebug() << "Added row:" << newRow << id << name << gender << grade << remark;
}

void MainWindow::on_lineEdit_ID_textEdited(const QString &arg1)
{
    qDebug() << "ID text edited: " << arg1;
}

void MainWindow::on_lineEdit_Name_textEdited(const QString &arg1)
{
    qDebug() << "Name text edited: " << arg1;
}

void MainWindow::on_radioButton_Man_toggled(bool checked)
{
    if (checked) {
        qDebug() << "Gender selected: 男";
    }
}

void MainWindow::on_radioButton_Woman_toggled(bool checked)
{
    if (checked) {
        qDebug() << "Gender selected: 女";
    }
}

void MainWindow::on_lineEdit_Grade_textEdited(const QString &arg1)
{
    qDebug() << "Grade text edited: " << arg1;
}

void MainWindow::on_lineEdit_ReMark_textEdited(const QString &arg1)
{
    qDebug() << "Remark text edited: " << arg1;
}

~~~
**以下是 QLineEdit 类中的函数说明，它们用于控制和操作 Qt 中的文本输入框。QLineEdit 提供了许多功能，包括文本编辑、文本光标的控制、自动补全等。**
~~~
构造函数和析构函数
*QLineEdit(QWidget parent = Q_NULLPTR)
构造一个文本框，父类为 parent。

*QLineEdit(const QString &contents, QWidget parent = Q_NULLPTR)
构造一个带有初始文本 contents 的文本框，父类为 parent。

~QLineEdit()
析构函数，销毁该 QLineEdit 对象。

行为和动作
*void addAction(QAction action, ActionPosition position)
向文本框添加一个操作按钮，按钮位置由 position 指定。

QAction addAction(const QIcon &icon, ActionPosition position)*
添加一个带有图标的操作按钮，并返回该操作的指针。

文本对齐
Qt::Alignment alignment() const
返回文本框中当前的文本对齐方式。
光标和文本操作
void backspace()
删除光标前的一个字符，相当于按下退格键。

QCompleter completer() const*
获取用于自动补全的 QCompleter 对象。

QMenu createStandardContextMenu()*
创建标准的上下文菜单，比如复制、剪切、粘贴等。

void cursorBackward(bool mark, int steps = 1)
光标向后移动 steps 步。如果 mark 为 true，则选中移动的文本。

void cursorForward(bool mark, int steps = 1)
光标向前移动 steps 步。如果 mark 为 true，则选中移动的文本。

Qt::CursorMoveStyle cursorMoveStyle() const
返回光标移动的样式，可能的值有 LogicalMoveStyle 和 VisualMoveStyle。

int cursorPosition() const
返回当前光标在文本中的位置。

int cursorPositionAt(const QPoint &pos)
返回指定位置 pos 处的光标位置。

void cursorWordBackward(bool mark)
光标向后移动一个单词。

void cursorWordForward(bool mark)
光标向前移动一个单词。

文本删除与选择
void del()
删除光标后的一个字符，相当于按下 Delete 键。

void deselect()
取消选中的文本。

显示与输入设置
QString displayText() const
返回文本框中显示的文本，通常用于密码输入框显示星号等替换符。

bool dragEnabled() const
返回是否启用拖放功能。

EchoMode echoMode() const
返回文本的显示模式，可能的值包括正常显示、密码隐藏等。

光标位置控制
void end(bool mark)
将光标移至文本末尾。如果 mark 为 true，则选中文本。

**void getTextMargins(int *left, int *top, int right, int bottom) const
返回文本框内的文本边距。

输入验证与其他功能
bool hasAcceptableInput() const
返回当前输入的内容是否符合验证规则。

bool hasFrame() const
返回文本框是否有边框。

bool hasSelectedText() const
返回是否有选中文本。

void home(bool mark)
将光标移至文本的开头。

QString inputMask() const
返回当前的输入掩码。

void insert(const QString &newText)
在光标位置插入新的文本。

bool isClearButtonEnabled() const
返回是否启用清除按钮。

bool isModified() const
返回文本框的内容是否被修改过。

bool isReadOnly() const
返回文本框是否是只读状态。

bool isRedoAvailable() const
返回是否可以执行重做操作。

bool isUndoAvailable() const
返回是否可以执行撤销操作。

文本长度与占位符
int maxLength() const
返回文本框可输入的最大字符数。

QString placeholderText() const
返回占位符文本，当文本框为空时显示的提示文本。

文本选择
QString selectedText() const
返回选中的文本。

int selectionStart() const
返回选中文本的起始位置。

文本对齐与样式
void setAlignment(Qt::Alignment flag)
设置文本的对齐方式。

void setClearButtonEnabled(bool enable)
设置是否启用清除按钮。

*void setCompleter(QCompleter c)
设置自动补全的对象。

void setCursorMoveStyle(Qt::CursorMoveStyle style)
设置光标移动的样式。

void setCursorPosition(int)
设置光标位置。

拖放与显示
void setDragEnabled(bool b)
设置是否启用拖放功能。

void setEchoMode(EchoMode)
设置文本显示模式（例如：普通文本、密码隐藏等）。

void setFrame(bool)
设置是否显示文本框的边框。

输入掩码与验证器
void setInputMask(const QString &inputMask)
设置输入掩码，用于限制用户输入特定格式的文本。

void setMaxLength(int)
设置可输入的最大字符数。

void setModified(bool)
设置文本框内容是否被修改的状态。

void setPlaceholderText(const QString &)
设置占位符文本。

void setReadOnly(bool)
设置文本框是否为只读状态。

void setSelection(int start, int length)
设置选中的文本。

void setTextMargins(int left, int top, int right, int bottom)
设置文本框内的文本边距。

void setTextMargins(const QMargins &margins)
设置文本框的边距。

*void setValidator(const QValidator v)
设置输入验证器，限制文本框输入的内容符合特定规则。

文本操作和验证器
QString text() const
返回文本框中的文本。

QMargins textMargins() const
返回文本框内的文本边距。

const QValidator * validator() const
返回当前的输入验证器。
~~~