#include "my_list_widget.h"

MyListWidget::MyListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void MyListWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    QModelIndex selectedItemIndex = this->currentIndex();
    int keyPressed = keyEvent->key();

    switch (keyPressed) {
    case Qt::Key_Return:
    {
        keyReturnPressed(selectedItemIndex);
        break;
    }
    case Qt::Key_Up:
    {
        keyUpPressed(selectedItemIndex);
        break;
    }
    case Qt::Key_Down:
    {
        keyDownPressed(selectedItemIndex);
        break;
    }
    default:
    {
        break;
    }
    }
}

void MyListWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    //qDebug() << mimeData << mimeData->formats();

    QByteArray encoded = mimeData->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        QString text = roleDataMap.first().toString();
        emit this->dropAction(text);
    }
    event->acceptProposedAction();
}

void MyListWidget::keyReturnPressed(QModelIndex selectedItemIndex)
{
    int currentItemRow = selectedItemIndex.row();
    if(currentItemRow >= 0 && currentItemRow < this->count())
    {
        emit this->doubleClicked(selectedItemIndex);
    }
}

void MyListWidget::keyUpPressed(QModelIndex selectedItemIndex)
{
    int currentItemRow = selectedItemIndex.row();
    int rowsCount = this->count();
    if(currentItemRow > 0)
    {
        this->setCurrentRow(currentItemRow - 1);
    }
    else
    {
        if(currentItemRow == 0)
        {
            this->setCurrentRow(rowsCount - 1);
        }
        else
        {
            this->setCurrentRow(0);
        }
    }
}

void MyListWidget::keyDownPressed(QModelIndex selectedItemIndex)
{
    int currentItemRow = selectedItemIndex.row();
    int rowsCount = this->count();
    if(currentItemRow >= 0)
    {
        if(currentItemRow == rowsCount - 1)
        {
            this->setCurrentRow(0);
        }
        else
        {
            this->setCurrentRow(currentItemRow + 1);
        }
    }
    else
    {
        this->setCurrentRow(rowsCount - 1);
    }
}
