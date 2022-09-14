#ifndef TABLEPAGESTEST_H
#define TABLEPAGESTEST_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TablePagesTest; }
QT_END_NAMESPACE

class TablePagesTest : public QMainWindow
{
    Q_OBJECT

public:
    TablePagesTest(QWidget *parent = nullptr);
    ~TablePagesTest();

private:
    Ui::TablePagesTest *ui;
};
#endif // TABLEPAGESTEST_H
