#ifndef PRODUCTMANAGEMENT_H
#define PRODUCTMANAGEMENT_H

#include <QDialog>
#include <QtSql>
#include <QString>
#include "mainwindow.h"
#include "product.h"

namespace Ui {
class productmanagement;
}

class productmanagement : public QDialog
{
    Q_OBJECT

public:
    explicit productmanagement(QWidget *parent = 0);
    ~productmanagement();

    void setMaindb(const QSqlDatabase &value);
    void refreshProducts();

private slots:

    void on_currentProducts_clicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::productmanagement *ui;
    QSqlDatabase maindb;
    QVector<Product*> productList;
};

#endif // PRODUCTMANAGEMENT_H
