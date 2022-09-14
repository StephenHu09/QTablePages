#include "qtablepages.h"
#include "ui_qtablepages.h"
#include "pagenavigator.h"

#include <QDateTime>
#include <QDebug>

QTablePages::QTablePages(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QTablePages)
{
    ui->setupUi(this);

    m_titleName = QStringList();

    m_pageNavBar = new PageNavigator(this);
    layout()->addWidget(m_pageNavBar);

    connect(m_pageNavBar, &PageNavigator::SigCurrentPageChanged, this, &QTablePages::OnPageChanged);

    InitTableView();
}

QTablePages::~QTablePages()
{
    delete m_dataModel;
    delete m_pageNavBar;
    delete ui;
}

void QTablePages::InitTableForm(const QStringList &title, QList<QStringList> &data, int pageLines)
{
    qDebug() << "table start : " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (title.size() > 0) {
        // 标题初始化
        UpdateTableTitle(title);
    }

    if (pageLines > 0) {
        // 表格页面初始化（设置显示行数）
        UpdateTablePageLine();
    }

    if (data.size() > 0) {
        // 数据初始化
        m_dataList.clear();
        m_totalLines = data.size();
        for (int i = 0; i < (int)m_totalLines; i++) {
            m_dataList.append(data.at(i));
        }
    }

    // 导航栏初始化
    m_pageNavBar->InitNavbar(m_totalLines, pageLines);

    qDebug() << "table finish : " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

void QTablePages::UpdateTableTitle(const QStringList &titleList)
{
    m_titleName = titleList;
    m_dataModel->setColumnCount(m_titleName.count());
    m_dataModel->setHorizontalHeaderLabels(m_titleName);
}

void QTablePages::UpdateTablePageLine()
{
    m_pageLines = m_pageNavBar->GetPageLines();
    m_dataModel->setRowCount(m_pageLines);
}

void QTablePages::UpdateTableDataByRow(uint32_t row, QStringList &dataList)
{
    if (row < 0 || row >= m_totalLines) {
        qDebug() << "ERROR  invalid row number :" << row;
        return;
    }

    if (dataList.size() != m_titleName.count()) {
        qDebug() << "ERROR  invalid dataList :" << dataList;
        return;
    }

    // 更新list
    m_dataList.replace(row, dataList);

    uint32_t curStart = (m_curViewPage - 1) * m_pageLines;
    uint32_t curEnd = curStart + m_dataModel->rowCount();
    // 更新表格显示
    if (row >= curStart && row < curEnd) {
        UpdateTableShow(curStart, curEnd);
    }
}

void QTablePages::CleanTable()
{
    m_dataList.clear();
    m_viewData.clear();
    m_totalLines = 0;

    m_dataModel->removeRows(0, m_dataModel->rowCount());
    m_pageNavBar->ResetNavBar();

    m_pageLines = m_pageNavBar->GetPageLines();
    m_curViewPage = m_pageNavBar->GetCurPage();
}

QStringList QTablePages::GetTableTitle()
{
    return m_titleName;
}

QTableView* QTablePages::GetTableViewObj()
{
    return ui->tableView;
}

void QTablePages::InitTableView()
{
    // 声明数据模型
    m_dataModel = new QStandardItemModel();

    QStringList defTitle;
    defTitle << "T1" << "T2" << "T3";
    m_dataModel->setColumnCount(defTitle.count());
    m_dataModel->setHorizontalHeaderLabels(defTitle);

    // 绑定模型与显示界面
    ui->tableView->setModel(m_dataModel);

    // 设置表格属性
    ui->tableView->verticalHeader()->setVisible(false); // 隐藏默认序号列
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 内容不可编辑
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 整行选中
    ui->tableView->setAlternatingRowColors(true); // 奇偶行间色显示

    // 设置表头属性
    ui->tableView->horizontalHeader()->setStretchLastSection(true); // 最后列自动扩展
    ui->tableView->horizontalHeader()->setDefaultSectionSize(120); // 设置初始列宽
    // 固定第0列宽度
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0, 60);

//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 根据列内容来扩展列宽，可能填不满 无法手动调节
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 自动扩展列宽填满窗口 无法手动调节

    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->horizontalHeader()->setObjectName("tableViewHHeader");
    ui->tableView->horizontalHeader()->setHighlightSections(false); // 取消表头高亮

    return;
}

void QTablePages::ShowTableViewPage(int page)
{
    if (page <= 0 || page > m_pageNavBar->GetMaxPage()) {
        qDebug() << "ERROR  invalid page number :" << page;
        return;
    }

    m_pageNavBar->SetCurrentPage(page);
}

void QTablePages::UpdateTableShow(int start, int end)
{
    if (start > end) {
        return;
    }

    // 拷贝数据
    m_viewData.clear();
    for (int i = start; i < end; ++i) {
        m_viewData.append(m_dataList.at(i));
    }

    // 填充数据
    // qDebug() << "startIndex:" << start << ", endIndex:" << end;
    for (int i = 0; i < m_viewData.count(); i++) {
        QStringList tempData = m_viewData.at(i);
        for (int j = 0; j < (int)m_titleName.count(); j++) {
            m_dataModel->setItem(i, j, new QStandardItem(tempData.at(j)));
            m_dataModel->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0, 60);
}

void QTablePages::OnPageChanged(int index)
{
    qDebug() << "switch page to" << index;

    m_curViewPage = m_pageNavBar->GetCurPage();
    m_pageLines = m_pageNavBar->GetPageLines();

    // 计算需要展示的数据范围
    int startIndex = (m_curViewPage - 1) * m_pageLines;
    int endIndex = 0;

    // 判断是否到尾页
    if (startIndex + m_pageLines < m_totalLines) {
        endIndex = startIndex + m_pageLines;
        m_dataModel->setRowCount(m_pageLines);
    } else {
        endIndex = m_totalLines;
        m_dataModel->setRowCount(endIndex - startIndex);
    }

    UpdateTableShow(startIndex, endIndex);

    emit SigPageStateChanged(index);
}

void QTablePages::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    int pos = m_pageLines * (m_curViewPage - 1) + row;
    qDebug() << "tableView select dataList num：" << pos + 1;

    qDebug() << "dataList: " << m_dataList.at(pos);
}
