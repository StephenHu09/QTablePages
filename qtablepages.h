#ifndef QTABLEPAGES_H
#define QTABLEPAGES_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class QTablePages; }
QT_END_NAMESPACE

class PageNavigator;

class QTablePages : public QWidget
{
    Q_OBJECT

public:
    QTablePages(QWidget *parent = nullptr);
    ~QTablePages();

    /* 表格初始化, 包括页面设置：
     * title 标题栏， pageLines 每页显示行数（按预制格式）
     * data 数据列表(QList形式), 一个成员代表一行数据 QStringList类型
     */
    void InitTableForm(const QStringList &title, QList<QStringList> &data, int pageLines);
    void UpdateTableTitle(const QStringList &titleList);
    void UpdateTablePageLine();
    void UpdateTableDataByRow(uint32_t row, QStringList &dataList);
    void CleanTable();

    QStringList GetTableTitle();
    QTableView* GetTableViewObj();

    uint32_t GetAllLines() { return m_totalLines; }
    uint32_t GetPageLines() { return m_pageLines; }
    uint32_t GetCurPage() { return m_curViewPage; }

private:
    /* 表格属性初始化 */
    void InitTableView();
    /* 主动设置当前显示页面，页面刷新由导航栏触发 */
    void ShowTableViewPage(int page);
    /* 表格显示刷新 */
    void UpdateTableShow(int start, int end);

signals:
    void SigPageStateChanged(int page);

private slots:
    void OnPageChanged(int index);

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::QTablePages *ui;

    // 导航栏
    PageNavigator *m_pageNavBar = Q_NULLPTR;
    // 表格模型
    QStandardItemModel *m_dataModel = Q_NULLPTR;

    QStringList m_titleName;     // 标题栏 名称list
    uint32_t m_totalLines = 0;   // 总行数
    uint32_t m_pageLines = 0;    // 每页显示行数
    uint32_t m_curViewPage = 0;  // 当前页 从1开始

    // 列表数据来源
    QList<QStringList> m_dataList; // 所有数据
    QList<QStringList> m_viewData; // 当前页展示数据

};
#endif // QTABLEPAGES_H
