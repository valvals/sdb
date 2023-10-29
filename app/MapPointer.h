#ifndef MAPPOINTER_H
#define MAPPOINTER_H

#include <QObject>

class MapPointer: public QObject
{
    Q_OBJECT
    Q_PROPERTY(double getPosLat READ getPosLat NOTIFY positionChanged)
    Q_PROPERTY(double getPosLon READ getPosLon NOTIFY positionChanged)
public:
    MapPointer();
    double getPosLat() const;
    double getPosLon() const;
    void setLatitude (double value);
    void setLongitude(double value);

signals:
    void positionChanged();

private:
    double latitude;
    double longitude;
};

#endif // MAPPOINTER_H
