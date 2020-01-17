#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>

#include "product.h"
#include "adddate.h"
#include "tablemenu.h"
#include "table.h"
#include "productmanagement.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    maindb = QSqlDatabase::addDatabase("QSQLITE");
    maindb.setDatabaseName("cal.db");
    maindb.open();
    QSqlQuery query(maindb);
    query.prepare("create table if not exists reservation (id integer primary key AUTOINCREMENT,"
                  "year integer,"
                  "month integer,"
                  "dayOfMonth integer,"
                  "table_number integer,"
                  "reservation_info varchar(300));");
    if(!query.exec()){
        qDebug()<<query.lastError();
    }
    query.prepare("create table if not exists product (product_name varchar(20) primary key,"
                  "product_price float);");
    if(!query.exec()){
        qDebug()<<query.lastError();
    }
    query.prepare("create table if not exists orders (id integer primary key AUTOINCREMENT,"
                  "date_of_order date,"
                  "total float);");
    if(!query.exec()){
        qDebug()<<query.lastError();
    }
    refreshProducts();

    maindb.close();
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(6);
    ui->tableWidget2->setColumnCount(6);
    ui->tableWidget2->setRowCount(5);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableHeader<<"Δ"<<"Τ"<<"Τ"<<"Π"<<"Π"<<"Σ"<<"Κ";
    combobox<<"Ιανουάριος"<<"Φεβρουάριος"<<"Μάρτιος"<<"Απρίλιος"<<"Μάιος"<<"Ιούνιος"<<"Ιούλιος"<<"Άυγουστος"<<"Σεμπτέμβριος"<<"Οκτώβριος"<<"Νοέμβριος"<<"Δεκέμβριος";
    ui->comboBox->addItems(combobox);
    active_date = QDate::currentDate();
    QString combostring;
    for (int i = active_date.year()-50; i<=active_date.year()+100;i++){
        ui->comboBox1->addItem(combostring.setNum(i));
    }
    ui->comboBox1->setCurrentIndex(50);
    ui->comboBox->setCurrentIndex(active_date.month()-1);
    ui->tableWidget->setHorizontalHeaderLabels(tableHeader);
    dayOfMonth=QDate::currentDate().day();
    redrawMonth();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setGeometry(0,0,0,ui->tableWidget->height()*12);
    ui->tableWidget2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    redrawTables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    properGeometry();
}
void MainWindow::redrawMonth(){
    QDate refresh_date(active_date.year(),active_date.month(),1);
    int week_day=refresh_date.dayOfWeek()-2,month_days=refresh_date.daysInMonth();
    ui->tableWidget->clearContents();
    maindb.open();
    QSqlQuery query(maindb);
    for(int i=1;i<=month_days;i++){
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i));
        ui->tableWidget->setItem((week_day+i)/7,(week_day+i)%7,item);

        query.prepare("select exists(select 1 from reservation where dayOfMonth=:dayOfMonth and year=:year and month=:month limit 1);");
        query.bindValue(":dayOfMonth",i);
        query.bindValue(":year",refresh_date.year());
        query.bindValue(":month",refresh_date.month());
        if(query.exec()){
            while (query.next()){
                if(query.value(0).toBool()){
                    item->setBackgroundColor("grey");
                }
            }
        }
        else{
            qDebug()<<"WEW"<<query.lastError();
        }
        if(i==dayOfMonth){
            ui->tableWidget->setCurrentCell((week_day+i)/7,(week_day+i)%7);
        }

    }
    maindb.close();


}

void MainWindow::redrawTables(){
    maindb.open();
    QSqlQuery query(maindb);
    ui->tableWidget2->clearContents();
    int reserved=0;
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            reserved=0;
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(i*6+j+1));
            ui->tableWidget2->setItem(i,j,item);
            query.prepare("select exists(select 1 from reservation where dayOfMonth=:dayOfMonth and year=:year and month=:month and table_number=:tablenumber limit 1);");
            query.bindValue(":dayOfMonth",dayOfMonth);
            query.bindValue(":year",active_date.year());
            query.bindValue(":month",active_date.month());
            query.bindValue(":tablenumber",i*6+j+1);
            if(query.exec()){
                while (query.next()){
                    if(query.value(0).toBool()){
                        item->setBackgroundColor("green");
                        reserved=1;
                    }
                }
            }
            else{
                qDebug()<<"WEW"<<query.lastError();
            }
            if(dayOfMonth==QDate::currentDate().day() && active_date.month()==QDate::currentDate().month() && active_date.year()==QDate::currentDate().year()){
                if(tableList[i*6+j].hasOrder()){
                    if(reserved==1){
                        item->setBackgroundColor("red");
                    }
                    else{
                        item->setBackgroundColor("blue");
                    }
                }
            }
        }
    }
    ui->orders->clear();
    tablesWithOrders.clear();
    for(int i=0;i<30;i++){
        if(tableList[i].hasOrder()){
            ui->orders->addItem("Τραπέζι: "+QString::number(i+1)+" Σύνολο:"+tableList[i].getSum());
            tablesWithOrders.append(i);
        }
    }
    maindb.close();

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    active_date.setDate(active_date.year(),index+1,1);
    redrawMonth();
}
void MainWindow::on_comboBox1_currentIndexChanged(int index)
{
    active_date.setDate(ui->comboBox1->itemText(index).toInt(),active_date.month(),1);
    redrawMonth();
}


void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if(!ui->tableWidget->item(row,column)==0){
        dayOfMonth=ui->tableWidget->item(row,column)->text().toInt();
        redrawTables();
    }
}

void MainWindow::on_tableWidget2_cellDoubleClicked(int row, int column)
{
    if(dayOfMonth==QDate::currentDate().day() && active_date.month()==QDate::currentDate().month() && active_date.year()==QDate::currentDate().year()){
        tableMenu *a = new tableMenu(this);
        a->setProdList(&productList);
        a->setActiveDate(active_date);
        a->setTableNumber(row*6+column+1);
        a->setDayOfMonth(dayOfMonth);
        a->setDatabase(maindb);
        a->setCurrentTable(&tableList[row*6+column]);
        a->show();
    }
    else{
        AddDate *a = new AddDate(this);
        a->setTable_number(row*6+column+1);
        a->setDatabase(maindb);
        a->setDate(active_date);
        a->setDayOfMonth(dayOfMonth);
        a->show();
    }

}


void MainWindow::on_pushButton_clicked()
{
    productmanagement *a = new productmanagement(this);
    a->setMaindb(maindb);
    a->show();

}

void MainWindow::setProductList(const QVector<Product *> &value)
{
    productList = value;
}
void MainWindow::refreshProducts(){
    maindb.open();
    QSqlQuery query(maindb);
    query.prepare("select * from product;");
    productList.clear();
    if(!query.exec()){
        qDebug()<<"WEW"<<query.lastError();
    }
    while(query.next()){
        Product* tempProd=new Product(query.value(0).toString(),query.value(1).toFloat());
        productList.append(tempProd);
        qDebug()<<tempProd->getProductName()<<tempProd->getProductPrice();
    }
    maindb.close();
}


void MainWindow::on_orders_clicked(const QModelIndex &index)
{
    tableMenu *a = new tableMenu(this);
    a->setProdList(&productList);
    a->setActiveDate(active_date);
    a->setTableNumber(tablesWithOrders[index.row()]);
    a->setDayOfMonth(dayOfMonth);
    a->setDatabase(maindb);
    a->setCurrentTable(&tableList[tablesWithOrders[index.row()]]);
    a->show();
}

void MainWindow::properGeometry(){
    for (int i = 0; i < 6; ++i) {
        ui->tableWidget->setRowHeight(i, ui->tableWidget->height()/6-6);
    }
    for (int i = 0; i < 5; ++i) {
        ui->tableWidget2->setRowHeight(i, ui->tableWidget2->height()/5-7);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    active_date = QDate::currentDate();
    dayOfMonth = QDate::currentDate().day();
    ui->comboBox1->setCurrentIndex(50);
    ui->comboBox->setCurrentIndex(active_date.month()-1);
    redrawMonth();
    redrawTables();
}
