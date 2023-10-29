#ifndef USELESSDATACUTTER_H
#define USELESSDATACUTTER_H
#include <QVector>

namespace udc{
enum EQUIPMENT_TYPE {
  FSR_02,
  PSR_700,
  PSR_1300
};

void cutUselessData(EQUIPMENT_TYPE equipment, QVector<double> &waves, QVector<double> &values);

}

#endif // USELESSDATACUTTER_H
