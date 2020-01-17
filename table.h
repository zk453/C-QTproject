#ifndef TABLE_H
#define TABLE_H

#include "product.h"
#include <QVector>
#include <QString>


class Table
{
private:
    QVector<Product*> productsOnTable;

public:
    Table();
    QVector<Product *> getProductsOnTable();
    void addProduct(Product* &a);
    void setProductsOnTable(QVector<Product *> &value);
    void removeItem(int a);
    void clearAll();
    bool hasOrder();
    QString getSum();
};

#endif // TABLE_H
