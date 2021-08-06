#include<iuom>
#include<uom>
#include<iostream>
using namespace inventory;
using namespace data_layer;
UnitOfMeasurement::UnitOfMeasurement()
{
}
UnitOfMeasurement::UnitOfMeasurement(int code,string title)
{
}
UnitOfMeasurement::UnitOfMeasurement(const UnitOfMeasurement &other)
{
}
UnitOfMeasurement::~UnitOfMeasurement()
{
}
void UnitOfMeasurement::setCode(int code)
{
}
int UnitOfMeasurement::getCode()
{
}
void UnitOfMeasurement::setTitle(string title)
{
}
string UnitOfMeasurement::getTitle()
{
}
abc::IUnitOfMeasurement UnitOfMeasurement::operator=(const IUnitOfMeasurement &other)
{
}
int UnitOfMeasurement::operator==(const IUnitOfMeasurement &other)
{
}
int UnitOfMeasurement::operator<(const IUnitOfMeasurement &other)
{
}
int UnitOfMeasurement::operator>(const IUnitOfMeasurement &other)
{
}
int UnitOfMeasurement::operator<=(const IUnitOfMeasurement &other)
{
}
int UnitOfMeasurement::operator>=(const IUnitOfMeasurement &other)
{
}
int UnitOfMeasurement::operator!=(const IUnitOfMeasurement &other)
{
}
int main()
{
 abc::IUnitOfMeasurement *uom=new UnitOfMeasurement();
 return 0;
} 