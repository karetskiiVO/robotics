#include "clientspage.h"
#include "ui_clientspage.h"
#include <QKeyEvent>
#include <blocksareawidget.h>

ClientsPage::ClientsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientsPage)
{
    ui->setupUi(this);
    ui->pageSplitter->setCollapsible(1, false);
    qApp->installEventFilter(this);
    connect(ui->addButton, &QToolButton::clicked, this, &ClientsPage::addClient);
    connect(ui->clientsList, &QListWidget::currentItemChanged, this, [this]()
    {
       const auto currentRow = ui->clientsList->currentRow();
       if (auto widgetAt = this->ui->clientsStack->widget(currentRow); widgetAt != nullptr)
       {
           this->ui->clientsStack->setCurrentIndex(currentRow);
       }
    });
}

ClientsPage::~ClientsPage()
{
    delete ui;
}

void ClientsPage::addClient()
{
    const auto itemsCount = ui->clientsList->count() + 1;
    const auto newItem = new QListWidgetItem(QString("Client %1").arg(itemsCount));
    const auto newWidget = new BlocksAreaWidget();
    ui->clientsList->addItem(newItem);
    ui->clientsStack->addWidget(newWidget);
    ui->clientsList->setCurrentItem(newItem);
    ui->clientsStack->setCurrentWidget(newWidget);
    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    ui->clientsList->editItem(newItem);
    newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
}

void ClientsPage::deleteClient(QListWidgetItem* item)
{
    if (item == nullptr)
    {
        return;
    }
    const auto row = ui->clientsList->row(item);
    if (row < ui->clientsList->count()-1)
    {
        ui->clientsList->setCurrentRow(row+1);
    }
    ui->clientsList->removeItemWidget(item);
    if (const auto widgetAt = ui->clientsStack->widget(row); widgetAt != nullptr)
    {
        ui->clientsStack->removeWidget(widgetAt);
        delete widgetAt;
    }

    delete item;
}


bool ClientsPage::eventFilter(QObject *obj, QEvent *event)
{
    auto current = ui->clientsList->currentItem();
    if (obj != ui->clientsList || current == nullptr)
    {
        return QWidget::eventFilter(obj, event);
    }
    if (event->type() == QEvent::KeyPress)
    {
        switch (auto *keyEvent = dynamic_cast<QKeyEvent *>(event); keyEvent->key())
        {
        case Qt::Key_Delete:
            deleteClient(current);
            break;
        case Qt::Key_F2:
            current->setFlags(current->flags() | Qt::ItemIsEditable);
            ui->clientsList->editItem(current);
            current->setFlags(current->flags() & ~Qt::ItemIsEditable);
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, event);
}