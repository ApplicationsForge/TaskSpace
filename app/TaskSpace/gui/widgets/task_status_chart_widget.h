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

    void updateChartWidget(QList<int> taskCountList, QStringList avaliableStatusesList);

private:
    QString m_setLabel = "Tasks Count";
    QString m_chartLabel = "Task status chart";

    static QtCharts::QChartView* buildChart(QString chartLabel,
                                            QString setLabel,
                                            QList<int> taskCountList,
                                            QStringList avaliableStatusesList,
                                            QWidget* parent = nullptr);

    static int maxListItem(QList<int> set);

signals:

public slots:
};

#endif // STATUS_CHART_WIDGET_H
