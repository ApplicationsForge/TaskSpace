#include "task_viewer_widget.h"

TaskViewerWidget::TaskViewerWidget(QWidget *parent) :
    QWidget(parent),
    m_taskIndex(-1),
    m_titleWidget(new QtMaterialTextField(this)),
    m_descriptionWidget(new QMarkdownTextEdit(this)),
    m_datePickerWidget(new QCalendarWidget(this)),
    m_withoutDateCheckBoxWidget(new QtMaterialCheckBox(this)),
    m_estimatedTimeWidget(new QTimeEdit(this)),
    m_actualTimeWidget(new QTimeEdit(this)),
    m_editingEnable(true)
{
    QVBoxLayout* containerLayout = new QVBoxLayout(this);
    containerLayout->setContentsMargins(0, 0, 0, 0);
        /*QLabel* taskTitleLabel = new QLabel("Title:", this);
        taskTitleLabel->setFont(QFont("Roboto", 16, QFont::Normal));
        containerLayout->addWidget(taskTitleLabel);*/

        m_titleWidget->setText("");
        m_titleWidget->setLabel("Title:");
        m_titleWidget->setLabelFontSize(16);
        m_titleWidget->setInkColor(QColor("#333"));
        //m_titleTextField->setStyleSheet("QtMaterialTextField { background-color: transparent; }");
        m_titleWidget->setPlaceholderText("Enter the task title, plaese.");
        m_titleWidget->setFont(QFont("Roboto", 16, QFont::Normal));
        //m_titleTextField->setShowInputLine(false);
        containerLayout->addWidget(m_titleWidget);

        /*QWidget* taskFormWidget = new QWidget(containerWidget);
            QFormLayout* taskFormWidgetLayout = new QFormLayout(taskFormWidget);
                QLineEdit* taskTitleLineEdit = new QLineEdit(taskFormWidget);
                taskTitleLineEdit->setText(task.title());
                taskTitleLineEdit->setEnabled(false);
                taskFormWidgetLayout->addRow("Title", taskTitleLineEdit);
            taskFormWidget->setLayout(taskFormWidgetLayout);
        containerLayout->addWidget(taskFormWidget);*/

        /*QLabel* taskDescriptionLabel = new QLabel("Description:", this);
        taskDescriptionLabel->setFont(QFont("Roboto", 16, QFont::Normal));
        containerLayout->addWidget(taskDescriptionLabel);*/

        QWidget* baseInformationContainerWidget = new QWidget(this);
            QHBoxLayout* baseInformationContainerWidgetLayout = new QHBoxLayout(baseInformationContainerWidget);
            baseInformationContainerWidgetLayout->setContentsMargins(0, 0, 0, 0);
                m_descriptionWidget->setParent(baseInformationContainerWidget);
                m_descriptionWidget->setStyleSheet("QMarkdownTextEdit { border: 1px solid transparent; }");
                m_descriptionWidget->setPlaceholderText("You could enter a short description for your task (markdown supported).");
                baseInformationContainerWidgetLayout->addWidget(m_descriptionWidget);

                QWidget* timeSettingsContainerWidget = new QWidget(baseInformationContainerWidget);
                    QVBoxLayout* timeSettingsContainerWidgetLayout = new QVBoxLayout(timeSettingsContainerWidget);
                    timeSettingsContainerWidgetLayout->setContentsMargins(0, 0, 0, 0);
                        QLabel* dueToDateLabel = new QLabel("DueTo date:", timeSettingsContainerWidget);
                        dueToDateLabel->setFont(QFont("Roboto", 14, QFont::Normal));
                        timeSettingsContainerWidgetLayout->addWidget(dueToDateLabel);

                        m_datePickerWidget->setParent(timeSettingsContainerWidget);
                        timeSettingsContainerWidgetLayout->addWidget(m_datePickerWidget);

                        m_withoutDateCheckBoxWidget->setParent(timeSettingsContainerWidget);
                        m_withoutDateCheckBoxWidget->setText("Task without due to date");
                        m_withoutDateCheckBoxWidget->setFont(QFont("Roboto", 14, QFont::Normal));
                        m_withoutDateCheckBoxWidget->setCheckedColor(QColor("#333"));
                        timeSettingsContainerWidgetLayout->addWidget(m_withoutDateCheckBoxWidget);

                        QLabel* estimatedTimeLabel = new QLabel("Estimated Time (hh:mm):", timeSettingsContainerWidget);
                        estimatedTimeLabel->setFont(QFont("Roboto", 14, QFont::Normal));
                        timeSettingsContainerWidgetLayout->addWidget(estimatedTimeLabel);

                        m_estimatedTimeWidget->setParent(timeSettingsContainerWidget);
                        timeSettingsContainerWidgetLayout->addWidget(m_estimatedTimeWidget);

                        QLabel* actualTimeLabel = new QLabel("Actual Time (hh:mm):", timeSettingsContainerWidget);
                        actualTimeLabel->setFont(QFont("Roboto", 14, QFont::Normal));
                        timeSettingsContainerWidgetLayout->addWidget(actualTimeLabel);

                        m_actualTimeWidget->setParent(timeSettingsContainerWidget);
                        timeSettingsContainerWidgetLayout->addWidget(m_actualTimeWidget);

                    timeSettingsContainerWidget->setLayout(timeSettingsContainerWidgetLayout);
                baseInformationContainerWidgetLayout->addWidget(timeSettingsContainerWidget);

            baseInformationContainerWidget->setLayout(baseInformationContainerWidgetLayout);
        containerLayout->addWidget(baseInformationContainerWidget);
    this->setLayout(containerLayout);
}

long TaskViewerWidget::taskIndex() const
{
    return m_taskIndex;
}

void TaskViewerWidget::setTaskIndex(long taskIndex)
{
    m_taskIndex = taskIndex;
}

void TaskViewerWidget::setEditingEnable(bool enable)
{
    m_editingEnable = enable;

    m_titleWidget->setEnabled(m_editingEnable);
    m_descriptionWidget->setReadOnly(!m_editingEnable);
    m_datePickerWidget->setEnabled(m_editingEnable);
    m_withoutDateCheckBoxWidget->setEnabled(m_editingEnable);
    m_estimatedTimeWidget->setEnabled(m_editingEnable);
    m_actualTimeWidget->setEnabled(m_editingEnable);
}

void TaskViewerWidget::setTaskTitle(const QString &title)
{
    m_titleWidget->setText(title);
}

void TaskViewerWidget::setTaskDescription(const QString &description)
{
    m_descriptionWidget->setText(description);
}

void TaskViewerWidget::setTaskDueToDate(const QDate &dueToDate)
{
    m_datePickerWidget->setSelectedDate(dueToDate);
}

void TaskViewerWidget::setTaskDueToDateEnabled(bool dueToDateEnabled)
{
    m_withoutDateCheckBoxWidget->setChecked(!dueToDateEnabled);
}

void TaskViewerWidget::setTaskEstimatedTime(const QTime &estimatedTime)
{
    m_estimatedTimeWidget->setTime(estimatedTime);
}

void TaskViewerWidget::setTaskActualTime(const QTime &actualTime)
{
    m_actualTimeWidget->setTime(actualTime);
}

void TaskViewerWidget::saveTaskData()
{
    QString title = m_titleWidget->text();
    QString description = m_descriptionWidget->toPlainText();
    QDate dueToDate = m_datePickerWidget->selectedDate();
    bool dueToDateEnabled = !(m_withoutDateCheckBoxWidget->isChecked());
    QTime estimatedTime = m_estimatedTimeWidget->time();
    QTime actualTime = m_actualTimeWidget->time();

    if(title.length() <= 0)
    {
        QMessageBox(QMessageBox::Warning, "Empty title", "Please type a title for your task").exec();
        this->setEditingEnable(false);
        return;
    }

    if(m_taskIndex >= 0)
    {
        emit this->taskUpdated(size_t(m_taskIndex),
                               title,
                               description,
                               dueToDate,
                               dueToDateEnabled,
                               estimatedTime,
                               actualTime);
    }
    else
    {
        emit this->taskCreated(title, description);
    }
}

void TaskViewerWidget::changeEditingEnableStatus()
{
    this->setEditingEnable(!m_editingEnable);
}

void TaskViewerWidget::enableEditing()
{
    this->setEditingEnable(true);
}

void TaskViewerWidget::disableEditing()
{
    this->setEditingEnable(false);
}

