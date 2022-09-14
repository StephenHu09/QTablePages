#ifndef PAGENAVIGATOR_H
#define PAGENAVIGATOR_H

#include <QWidget>

namespace Ui {
class PageNavigator;
}

class PageNavigator : public QWidget
{
    Q_OBJECT

public:
    explicit PageNavigator(QWidget *parent = nullptr);
    ~PageNavigator();

    void InitNavbar(int total, int pageCount = 10);
    void ResetNavBar();
    void SetCurrentPage(int page);
    void UpdatePages();

    uint32_t GetMaxPage() { return m_maxPage; }
    uint32_t GetPageLines() { return m_pageLines; }
    uint32_t GetCurPage() { return m_curPage; }

signals:
    void SigCurrentPageChanged(int index);

private:
    void HandlePageLinesChanged(int count);
    void UpdateButtonState();

private slots:
    void on_pagePerCount_currentIndexChanged(const QString &arg1);
    void on_tBtnSwitch_clicked();
    void on_tBtnPrevious_clicked();
    void on_tBtnNext_clicked();
    void on_tBtnFirst_clicked();
    void on_tBtnLast_clicked();

private:
    Ui::PageNavigator *ui;

    uint32_t m_totalRecord = 0;
    uint32_t m_maxPage = 1;
    uint32_t m_curPage = 1;
    uint32_t m_pageLines = 0; // 每页显示行数
};

#endif // PAGENAVIGATOR_H
