#include "burndown_chart_widget.h"

BurndownChartWidget::BurndownChartWidget(QWidget *parent) : QWidget(parent)
{
    QtCharts::QLineSeries *series0 = new QtCharts::QLineSeries();
    series0->setName("FirstLine");
    series0->append(0, 6);
    series0->append(2, 4);
    series0->append(3, 8);
    series0->append(7, 4);
    series0->append(10, 5);
    *series0 << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QtCharts::QLineSeries *series1 = new QtCharts::QLineSeries();
    series1->setName("SecondLine");
    for(int i = 0; i < 20; i++)
    {
       series1->append(i, i);
    }

    QtCharts::QLineSeries *series2 = new QtCharts::QLineSeries();
    series2->setName("ThirdLine");
    for(int i = 0; i < 5; i++)
    {
       series2->append(i, i*i);
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series0);
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->createDefaultAxes();
    chart->setTitle("Burndown chart");

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->addWidget(chartView);
    this->setLayout(mainLayout);
}
