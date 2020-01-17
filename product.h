#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>


class Product
{
private:
    QString productName;
    float productPrice;
public:
    Product();
    Product(QString prodname,float prodprice);
    void setProductName(QString name);
    void setProductPrice(float price);
    QString getProductName();
    float getProductPrice();
};

#endif // PRODUCT_H
