#ifndef ADDDATE_H
#define ADDDATE_H

#include <QDialog>
#include <QtSql>
#include <QTableWidget>

namespace Ui {
class AddDate;
}

class AddDate : public QDialog
{
    Q_OBJECT

public:
    explicit AddDate(QWidget *parent = 0);
    void setDatabase(QSqlDatabase &database);
    void setDate(QDate showDate);
    void setDayOfMonth(int tempDate);
    void modifyDatabase();
    ~AddDate();

    void setTable_number(int value);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::AddDate *ui;
    QWidget *qparent;
    int dayOfMonth;
    QDate currentDate;
    QSqlDatabase ThatDb;
    int table_number;
};

#endif // ADDDATE_H
