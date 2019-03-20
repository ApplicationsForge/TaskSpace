#include "task_viewer_widget.h"

TaskViewerWidget::TaskViewerWidget(QWidget *parent) :
    QWidget(parent),
    m_titleTextField(new QtMaterialTextField(this)),
    m_descriptionTextEdit(new QMarkdownTextEdit(this)),
    m_inputLocked(true)
{
    QVBoxLayout* containerLayout = new QVBoxLayout(this);
        QLabel* taskTitleLabel = new QLabel("Title:", this);
        taskTitleLabel->setFont(QFont("Roboto", 16, QFont::Normal));
        containerLayout->addWidget(taskTitleLabel);

        m_titleTextField->setText("");
        //taskTitleTextField->setLabel("Title");
        m_titleTextField->setLabelFontSize(16);
        m_titleTextField->setLabelColor(QColor("#333"));
        m_titleTextField->setInkColor(QColor("#333"));
        m_titleTextField->setTextColor(QColor("#333"));
        //taskTitleTextField->setStyleSheet("QtMaterialTextField { background-color: transparent; }");
        m_titleTextField->setPlaceholderText("Please, type a title for your task.");
        m_titleTextField->setFont(QFont("Roboto", 16, QFont::Normal));
        m_titleTextField->setShowInputLine(false);
        containerLayout->addWidget(m_titleTextField);

        /*QWidget* taskFormWidget = new QWidget(containerWidget);
            QFormLayout* taskFormWidgetLayout = new QFormLayout(taskFormWidget);
                QLineEdit* taskTitleLineEdit = new QLineEdit(taskFormWidget);
                taskTitleLineEdit->setText(task.title());
                taskTitleLineEdit->setEnabled(false);
                taskFormWidgetLayout->addRow("Title", taskTitleLineEdit);
            taskFormWidget->setLayout(taskFormWidgetLayout);
        containerLayout->addWidget(taskFormWidget);*/

        QLabel* taskDescriptionLabel = new QLabel("Description:", this);
        taskDescriptionLabel->setFont(QFont("Roboto", 16, QFont::Normal));
        containerLayout->addWidget(taskDescriptionLabel);

        //descriptionTextEdit->setStyleSheet("QMarkdownTextEdit { border: 1px solid #dfdfdf; background-color: #efefef; }");
        m_descriptionTextEdit->setStyleSheet("QMarkdownTextEdit { border: 1px solid transparent; }");
        m_descriptionTextEdit->setPlaceholderText("Please, type a short description for your task (markdown supported).");
        containerLayout->addWidget(m_descriptionTextEdit);
    this->setLayout(containerLayout);
    this->setContentsMargins(0, 0, 0, 0);
}

void TaskViewerWidget::setTaskTitle(QString title)
{
    m_titleTextField->setText(title);
}

void TaskViewerWidget::setInputLocked(bool inputLocked)
{
    m_inputLocked = inputLocked;

    m_titleTextField->setEnabled(!m_inputLocked);
    m_descriptionTextEdit->setEnabled(!m_inputLocked);
}

void TaskViewerWidget::changeLockStatus()
{
    this->setInputLocked(!m_inputLocked);
}
