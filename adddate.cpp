#include "adddate.h"
#include "tablemenu.h"
#include "ui_adddate.h"
#include "mainwindow.h"

AddDate::AddDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDate)
{
    ui->setupUi(this);
}

AddDate::~AddDate()
{
    delete ui;
}

void AddDate::setDatabase(QSqlDatabase &database){
    ThatDb=database;
    ThatDb.open();
    QSqlQuery query(ThatDb);
    query.prepare("select reservation_info from reservation where year=:year and month=:month and dayOfMonth=:dayOfMonth and table_number=:tablenumber");
    query.bindValue(":year",currentDate.year());
    query.bindValue(":month",currentDate.month());
    query.bindValue(":dayOfMonth",dayOfMonth);
    query.bindValue(":tablenumber",table_number);
    if(query.exec()){
        while (query.next()){
            qDebug()<<query.value(0).toString();
            ui->plainTextEdit->insertPlainText(query.value(0).toString());
        }
    }
    ThatDb.close();
}

void AddDate::on_pushButton_clicked()
{
    modifyDatabase();
}

void AddDate::modifyDatabase(){
    QString A;
    int id=(int)NULL;
    ThatDb.open();
    A=ui->plainTextEdit->toPlainText();
    QSqlQuery query(ThatDb);
    query.prepare("select id from reservation where year=:year and month=:month and dayOfMonth=:dayOfMonth and table_number=:tablenumber");
    query.bindValue(":year",currentDate.year());
    query.bindValue(":month",currentDate.month());
    query.bindValue(":dayOfMonth",dayOfMonth);
    query.bindValue(":tablenumber",table_number);
    if(query.exec()){
        while (query.next()){
            id=query.value(0).toInt();
        }
    }
    else{
        qDebug()<<query.lastError();
    }
    if(id!=0){
        query.prepare("INSERT OR REPLACE INTO reservation VALUES(:id,:year,:month,:dayOfMonth,:table_number,:reservation_info)");
        query.bindValue(":id",id);
        query.bindValue(":year",currentDate.year());
        query.bindValue(":month",currentDate.month());
        query.bindValue(":dayOfMonth",dayOfMonth);
        query.bindValue(":reservation_info",A);
        query.bindValue(":table_number",table_number);

        if(!query.exec()){
            qDebug()<<query.lastError();
        }
    }else{
        query.prepare("INSERT INTO reservation VALUES(NULL,:year,:month,:dayOfMonth,:table_number,:reservation_info)");
        query.bindValue(":year",currentDate.year());
        query.bindValue(":month",currentDate.month());
        query.bindValue(":dayOfMonth",dayOfMonth);
        query.bindValue(":reservation_info",A);
        query.bindValue(":table_number",table_number);

        if(!query.exec()){
            qDebug()<<query.lastError();
        }
    }
    query.prepare("delete from reservation where reservation_info=''");
    query.exec();
    ThatDb.close();

}
void AddDate::setDate(QDate showDate){
    currentDate=showDate;
}

void AddDate::setDayOfMonth(int tempDate){
    dayOfMonth=tempDate;
}

void AddDate::on_pushButton_2_clicked()
{
    this->close();
}

void AddDate::on_pushButton_3_clicked()
{
    modifyDatabase();
    this->close();
}

void AddDate::setTable_number(int value)
{
    table_number = value;
}
