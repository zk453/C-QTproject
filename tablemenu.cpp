#include "tablemenu.h"
#include "ui_tablemenu.h"
#include "adddate.h"
#include "mainwindow.h"

tableMenu::tableMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tableMenu)
{
    ui->setupUi(this);
}

tableMenu::~tableMenu()
{
    delete ui;
}

void tableMenu::on_reserveTable_clicked()
{
    AddDate *a = new AddDate(this);
    a->setDate(active_date);
    a->setTable_number(tableNumber);
    a->setDayOfMonth(dayOfMonth);
    a->setDatabase(database);
    a->show();
}

void tableMenu::setDayOfMonth(int value)
{
    dayOfMonth = value;
}

void tableMenu::setActiveDate(QDate date){
    active_date=date;
}


void tableMenu::setDatabase(QSqlDatabase &db){
    database=db;
}

void tableMenu::setTableNumber(int number){
    tableNumber=number;
}


void tableMenu::on_exitTable_clicked()
{
    ((MainWindow*)(parent()))->redrawTables();
    ((MainWindow*)(parent()))->redrawMonth();
    this->close();
}

void tableMenu::setProdList( QVector<Product*> *value)
{
    prodList = value;
    for(int i=0;i<prodList->length();i++){
        ui->products->addItem(prodList->operator[](i)->getProductName()+" ("+QString::number(prodList->operator[](i)->getProductPrice())+")");
    }
}

void tableMenu::setCurrentTable(Table *value)
{
    currentTable = value;
    for(int i=0;i<currentTable->getProductsOnTable().length();i++){
        ui->currentOrders->addItem(currentTable->getProductsOnTable().at(i)->getProductName()+" ("+QString::number(currentTable->getProductsOnTable().at(i)->getProductPrice())+")");
    }
    if(currentTable->hasOrder()){
        ui->total->setText(("Σύνολο: "+currentTable->getSum()));
    }
}

void tableMenu::setOrderedProducts( QVector<Product *> &value)
{
    orderedProducts = value;
}

void tableMenu::on_products_clicked(const QModelIndex &index)
{
    currentTable->addProduct(prodList->operator[](index.row()));
    ui->currentOrders->addItem(prodList->at(index.row())->getProductName()+" ("+QString::number(prodList->at(index.row())->getProductPrice())+")");
    ui->total->setText(("Σύνολο: "+currentTable->getSum()));
}

void tableMenu::on_currentOrders_clicked(const QModelIndex &index)
{
    currentTable->removeItem(index.row());
    ui->currentOrders->takeItem(index.row());
    ui->total->setText(("Σύνολο: "+currentTable->getSum()));

}

void tableMenu::on_finishOrder_clicked()
{
    database.open();
    QSqlQuery query(database);
    query.prepare("insert into orders(date_of_order,total) values(DateTime('now'),:total)");
    query.bindValue(":total",currentTable->getSum().toFloat());
    if(!query.exec()){
        qDebug()<<query.lastError();
    }
    currentTable->clearAll();
    database.close();
    ((MainWindow*)(parent()))->redrawTables();
    ((MainWindow*)(parent()))->redrawMonth();
    this->close();
}
