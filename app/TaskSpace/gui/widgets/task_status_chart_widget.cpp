#include "task_status_chart_widget.h"

TaskStatusChartWidget::TaskStatusChartWidget(QWidget *parent) :
    QWidget(parent),
    m_chartTitle("Tasks Status Bar Chart"),
    m_setTitle("TasksCount"),
    m_categories(QStringList()),
    m_set(new QtCharts::QBarSet(m_setTitle)),
    m_series(new QtCharts::QBarSeries()),
    m_chart(new QtCharts::QChart()),
    m_chartView(new QtCharts::QChartView()),
    m_axisX(new QtCharts::QBarCategoryAxis()),
    m_axisY(new QtCharts::QValueAxis())
{
    m_series->append(m_set);

    m_chart->addSeries(m_series);;
    m_chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);

    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);

    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_chartView->setChart(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->addWidget(m_chartView);
    this->setLayout(mainLayout);

    this->updateChart();
}

void TaskStatusChartWidget::updateChart()
{
    //m_series->append(m_set);
    m_chart->setTitle(m_chartTitle);

    m_axisX->append(m_categories);
    m_axisY->setRange(0, this->maxItemsCount() +  1);

    m_chart->update();
    m_chartView->update();
}

QStringList TaskStatusChartWidget::categories() const
{
    return m_categories;
}

void TaskStatusChartWidget::setCategories(const QStringList &categories)
{
    m_categories = categories;
    //this->updateChart();
}

QtCharts::QBarSet *TaskStatusChartWidget::set() const
{
    return m_set;
}

void TaskStatusChartWidget::setSet(QList<int> taskCountSet)
{
    /*for(int i = 0; i < m_set->count(); i++)
    {
        m_set->remove(i);
    }*/

    for(auto taskCount : taskCountSet)
    {
        *m_set << taskCount;
    }
    //this->updateChart();
}

double TaskStatusChartWidget::maxItemsCount()
{
    double result = 0;
    for(int i = 0; i < m_set->count(); i++)
    {
        result = std::max(m_set->at(i), result);
    }
    return result;
}
