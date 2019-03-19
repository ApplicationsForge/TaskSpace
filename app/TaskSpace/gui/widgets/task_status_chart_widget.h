#ifndef STATUS_CHART_WIDGET_H
#define STATUS_CHART_WIDGET_H

#include <QWidget>
#include <QChartView>
#include <QAreaSeries>
#include <QVBoxLayout>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>

class TaskStatusChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskStatusChartWidget(QWidget *parent = nullptr);

    void showChart();

    QStringList categories() const;
    void setCategories(const QStringList &categories);

    QtCharts::QBarSet *set() const;
    void setSet(QList<int> taskCountSet);

private:
    QString m_chartTitle;
    QString m_setTitle;
    QStringList m_categories;
    QtCharts::QBarSet *m_set;
    QtCharts::QChart *m_chart;
    QtCharts::QChartView *m_chartView;

signals:

public slots:
};

#endif // STATUS_CHART_WIDGET_H
