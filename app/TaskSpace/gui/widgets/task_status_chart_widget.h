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

    void updateChartWidget(QList<int> taskCountList, QStringList statuses);

private:
    QString m_setLabel = "Task statuses";
    QString m_chartLabel = "Task status chart";

    QtCharts::QChartView* buildChart(QString chartLabel, QString setLabel, QList<int> taskCountList, QStringList statuses);

signals:

public slots:
};

#endif // STATUS_CHART_WIDGET_H
