#include "productmanagement.h"
#include "ui_productmanagement.h"

productmanagement::productmanagement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::productmanagement)
{
    ui->setupUi(this);
}

productmanagement::~productmanagement()
{
    delete ui;
}

void productmanagement::setMaindb(const QSqlDatabase &value)
{
    maindb = value;
    refreshProducts();
}

void productmanagement::refreshProducts()
{
    maindb.open();
    QSqlQuery query(maindb);
    query.prepare("select * from product;");
    if(!query.exec()){
        qDebug()<<"WEW"<<query.lastError();
    }
    productList.clear();
    while(query.next()){
        Product* tempProd=new Product(query.value(0).toString(),query.value(1).toFloat());
        productList.append(tempProd);
        qDebug()<<tempProd->getProductName()<<tempProd->getProductPrice();
    }
    maindb.close();
    ui->currentProducts->clear();
    for(int i=0;i<productList.length();i++){
        ui->currentProducts->addItem(productList[i]->getProductName()+" ("+QString::number(productList[i]->getProductPrice())+")");
    }
}


void productmanagement::on_currentProducts_clicked(const QModelIndex &index)
{
    ui->total->setText(QString::number(productList[index.row()]->getProductPrice()));
    ui->name->setText(productList[index.row()]->getProductName());
}

void productmanagement::on_pushButton_3_clicked()
{
    maindb.open();
    QSqlQuery query(maindb);
    query.prepare("insert or replace into product values(:name,:price);");
    query.bindValue(":name",ui->name->text());
    query.bindValue(":price",ui->total->text());
    if(!query.exec()){
        qDebug()<<"WEW"<<query.lastError();
    }
    maindb.close();
    refreshProducts();
    ((MainWindow*)(parent()))->refreshProducts();
}

void productmanagement::on_pushButton_2_clicked()
{
    qDebug()<<ui->currentProducts->currentRow();
    maindb.open();
    QSqlQuery query(maindb);
    query.prepare("delete from product where product_name=:name;");
    query.bindValue(":name",productList[ui->currentProducts->currentRow()]->getProductName());
    if(!query.exec()){
        qDebug()<<"WEW"<<query.lastError();
    }
    maindb.close();
    refreshProducts();
    ((MainWindow*)(parent()))->refreshProducts();

}

void productmanagement::on_pushButton_4_clicked()
{
    this->close();
}
