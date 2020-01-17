#include "product.h"

Product::Product()
{
    productName="";
    productPrice=0;

}

Product::Product(QString prodname,float prodprice):productName(prodname),productPrice(prodprice)
{}

void Product::setProductName(QString name)
{
    productName=name;
}

void Product::setProductPrice(float price)
{
    productPrice=price;
}

QString Product::getProductName()
{
    return productName;
}

float Product::getProductPrice()
{
    return productPrice;
}
