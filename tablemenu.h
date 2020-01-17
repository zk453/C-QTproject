#ifndef TABLEMENU_H
#define TABLEMENU_H

#include <QDialog>
#include <QDate>
#include <QtSql>
#include <QListWidgetItem>
#include <QVector>
#include "product.h"
#include "table.h"

namespace Ui {
class tableMenu;
}

class tableMenu : public QDialog
{
    Q_OBJECT

public:
    explicit tableMenu(QWidget *parent = 0);
    ~tableMenu();
    void setActiveDate(QDate date);
    void setDatabase(QSqlDatabase &db);
    void setTableNumber(int number);

    void setDayOfMonth(int value);

    void setProdList(QVector<Product*> *value);

    void setOrderedProducts( QVector<Product *> &value);

    void setCurrentTable(Table *value);

private slots:
    void on_reserveTable_clicked();

    void on_exitTable_clicked();

    void on_products_clicked(const QModelIndex &index);

    void on_currentOrders_clicked(const QModelIndex &index);

    void on_finishOrder_clicked();

private:
    Ui::tableMenu *ui;
    QDate active_date;
    QSqlDatabase database;
    int tableNumber;
    int dayOfMonth;
    QVector<Product*> *prodList;
    QVector<Product*> orderedProducts;
    Table* currentTable;
};

#endif // TABLEMENU_H
