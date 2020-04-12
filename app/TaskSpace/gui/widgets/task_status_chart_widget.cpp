#include "task_status_chart_widget.h"

TaskStatusChartWidget::TaskStatusChartWidget(QWidget *parent) : QWidget(parent)
{

}

void TaskStatusChartWidget::updateChartWidget(QList<int> taskCountList, QStringList avaliableStatusesList)
{
    qDeleteAll(this->children());

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
        QtCharts::QChartView* chartWidget = TaskStatusChartWidget::buildChart(m_chartLabel, m_setLabel, taskCountList, avaliableStatusesList, this);
        mainLayout->addWidget(chartWidget);
    this->setLayout(mainLayout);
}

QtCharts::QChartView* TaskStatusChartWidget::buildChart(QString chartLabel, QString setLabel, QList<int> taskCountList, QStringList avaliableStatusesList, QWidget* parent)
{
    QtCharts::QBarSet *set = new QtCharts::QBarSet(setLabel);
    for(auto item : taskCountList)
    {
        *set << item;
    }

    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
    series->append(set);

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle(chartLabel);
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
    axisX->append(avaliableStatusesList);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setRange(0, TaskStatusChartWidget::maxListItem(taskCountList) + 1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart, parent);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

int TaskStatusChartWidget::maxListItem(QList<int> list)
{
    int result = 0;
    for(auto item : list)
    {
        result = std::max(item, result);
    }
    return result;
}
