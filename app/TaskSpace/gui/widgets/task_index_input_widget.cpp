#include "task_index_input_widget.h"

TaskIndexInputWidget::TaskIndexInputWidget(QWidget *parent) :
    QWidget(parent),
    m_indexInput(new QtMaterialTextField(this))
{
    QVBoxLayout* containerLayout = new QVBoxLayout(this);
    containerLayout->setContentsMargins(0, 0, 0, 0);
        m_indexInput->setText("");
        m_indexInput->setLabel("Index:");
        m_indexInput->setLabelFontSize(16);
        m_indexInput->setInkColor(QColor("#333"));
        m_indexInput->setPlaceholderText("Enter the task index, plaese.");
        m_indexInput->setFont(QFont("Roboto", 16, QFont::Normal));
        containerLayout->addWidget(m_indexInput);
    this->setLayout(containerLayout);
}

void TaskIndexInputWidget::getResult()
{
    QString rawIndex = m_indexInput->text();

    bool ok = false;
    size_t index = size_t(rawIndex.toUInt(&ok));
    if(ok)
    {
        emit this->indexSelected(index);
    }

    m_indexInput->clear();
}
