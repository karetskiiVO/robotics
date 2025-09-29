#ifndef CLIENTSPAGE_H
#define CLIENTSPAGE_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class ClientsPage;
}

class ClientsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ClientsPage(QWidget *parent = nullptr);
    ~ClientsPage();

private slots:
    void addClient();
private:
    Ui::ClientsPage *ui;

    bool eventFilter(QObject *obj, QEvent *event) override;
    void deleteClient(QListWidgetItem* item);
};

#endif // CLIENTSPAGE_H
