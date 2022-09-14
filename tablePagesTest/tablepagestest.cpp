#include "tablepagestest.h"
#include "ui_tablepagestest.h"

#include "qtablepages.h"

TablePagesTest::TablePagesTest(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TablePagesTest)
{
    ui->setupUi(this);

    QStringList headers;
    headers << QString("序号") << QString("用户名") << QString("用户ID") << QString("时间戳") <<  QString("设备") << QString("事件");

    /* 初始化数据填充 模拟 */
    QList<QStringList> sampleList;
    for (int i = 0; i < 20229; i++) {
        QStringList tempData;
        for (int col = 0; col < headers.size(); col++) {
            if (col == 0) {
                tempData.append(QString::fromLocal8Bit("%1").arg(i+1)); // 设置序号
            } else {
                tempData.append(QString::fromLocal8Bit("L%1_columns%2").arg(i+1).arg(col+1)); // 表格内容
            }
        }
        sampleList.append(tempData);
    }

    QTablePages *tab = new QTablePages();
    ui->centralwidget->layout()->addWidget(tab);
    tab->InitTableForm(headers, sampleList, 20);
}

TablePagesTest::~TablePagesTest()
{
    delete ui;
}

