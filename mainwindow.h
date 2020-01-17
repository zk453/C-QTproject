#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <adddate.h>
#include <QMainWindow>
#include <QResizeEvent>
#include <QDateTime>
#include <QtSql>
#include <QTableWidget>
#include "product.h"
#include "table.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event);
    void resizeInitializer();
    void redrawMonth();
    void redrawTables();
    void refreshProducts();
    void properGeometry();
    void setProductList(const QVector<Product *> &value);

private slots:

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox1_currentIndexChanged(int index);

    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget2_cellDoubleClicked(int row, int column);

    void on_pushButton_clicked();


    void on_orders_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Table tableList[30];
    QStringList tableHeader,combobox;
    QDate active_date;
    QString path=".";
    QSqlDatabase maindb;
    QVector<Product*> productList;
    QVector<int> tablesWithOrders;
    int dayOfMonth;
};

#endif // MAINWINDOW_H
