#include "task_status_chart_widget.h"

TaskStatusChartWidget::TaskStatusChartWidget(QWidget *parent) :
    QWidget(parent),
    m_chartTitle("Tasks Status Bar Chart"),
    m_setTitle("TasksCount"),
    m_categories(QStringList()),
    m_set(new QtCharts::QBarSet(m_setTitle)),
    m_chart(new QtCharts::QChart()),
    m_chartView(new QtCharts::QChartView())
{
    this->showChart();
}

void TaskStatusChartWidget::showChart()
{
    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
    series->append(m_set);

    m_chart->addSeries(series);
    m_chart->setTitle(m_chartTitle);
    m_chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
    axisX->append(m_categories);
    m_chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setRange(0, m_set->count());
    m_chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setChart(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->addWidget(m_chartView);
    this->setLayout(mainLayout);
}

QStringList TaskStatusChartWidget::categories() const
{
    return m_categories;
}

void TaskStatusChartWidget::setCategories(const QStringList &categories)
{
    m_categories = categories;
}

QtCharts::QBarSet *TaskStatusChartWidget::set() const
{
    return m_set;
}

void TaskStatusChartWidget::setSet(QList<int> taskCountSet)
{
    for(auto taskCount : taskCountSet)
    {
        *m_set << taskCount;
    }
}
