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
    /*qDebug() << mimeData << mimeData->text() << mimeData->data("application/x-item");
    qDebug() << mimeData->isWidgetType() << mimeData->formats()
             << mimeData->data("application/x-qabstractitemmodeldatalist")
             << mimeData->data("application/x-qt-mime-type-name")
             << mimeData->hasUrls() << mimeData->hasColor()
             << mimeData->hasFormat("application/x-qt-mime-type-name")
             << mimeData->hasFormat("application/x-qabstractitemmodeldatalist");*/

    QByteArray encoded = mimeData->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        /* do something with the data */
        QString text = roleDataMap.first().toString();
        qDebug() << roleDataMap << text;

        emit this->dropAction(text);
    }

    /*if (mimeData->hasFormat("application/x-item"))
    {
        QString text = mimeData->data("application/x-item");
        if (!text.isEmpty())
        {
            //emit this->dropAction(text);
            //QListWidgetItem* item = new QListWidgetItem(text);
            //this->insertItem(0, item);
            //this->addItem(item);
        }
        emit this->dropAction(text);
        event->acceptProposedAction();
    }
    QListWidget::dropEvent(event);*/

    /*if (event->mimeData()->hasFormat("application/x-item"))
    {
        event->accept();
        event->setDropAction(Qt::MoveAction);
        QListWidgetItem *item = new QListWidgetItem(this);
        QString name = event->mimeData()->data("application/x-item");
        item->setText(name);
        //item->setIcon(QIcon(":/images/iString")); //set path to image
        this->addItem(item);
        emit dropAction(name);
    }
    else
    {
        event->ignore();
    }*/
}

/*void MyListWidget::startDrag(Qt::DropActions supportedActions)
{
    QListWidgetItem* item = currentItem();
    QMimeData* mimeData = new QMimeData;
    QByteArray ba;
    ba = item->text().toUtf8();
    mimeData->setData("application/x-item", ba);
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        delete takeItem(row(item));
        //emit itemDroped();
    }
}

void MyListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-item"))
        event->accept();
    else
        event->ignore();
}

void MyListWidget::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasFormat("application/x-item") && e->source() != this) {
        e->setDropAction(Qt::MoveAction);
        e->accept();
    } else
        e->ignore();
}*/

/*Qt::DropAction MyListWidget::supportedDropActions()
{
    return Qt::MoveAction;
}*/

void MyListWidget::keyReturnPressed(QModelIndex selectedItemIndex)
{
    int currentItemRow = selectedItemIndex.row();
    if(currentItemRow >= 0 && currentItemRow < this->count())
    {
        emit clicked(selectedItemIndex);
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
