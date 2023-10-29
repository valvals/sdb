#include "MapPointer.h"

MapPointer::MapPointer():latitude(0),longitude(0)
{

}

double MapPointer::getPosLat() const
{
    return latitude;
}

double MapPointer::getPosLon() const
{
    return longitude;
}

void MapPointer::setLatitude(double value)
{
    latitude = value;
}

void MapPointer::setLongitude(double value)
{
    longitude = value;
}
