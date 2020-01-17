#include "table.h"

QVector<Product *> Table::getProductsOnTable()
{
    return productsOnTable;
}

void Table::setProductsOnTable(QVector<Product *> &value)
{
    productsOnTable = value;
}
void Table::addProduct(Product* &a){
    productsOnTable.append(a);
}

void Table::removeItem(int a){
    productsOnTable.remove(a);
}

void Table::clearAll()
{
   productsOnTable.clear();
}


Table::Table()
{

}

bool Table::hasOrder(){
    return !getProductsOnTable().empty();
}

QString Table::getSum()
{
    float sum;
    for(int i=0;i<productsOnTable.size();i++)
    {
        sum+=productsOnTable[i]->getProductPrice();
    }
    return QString::number(sum);
}
