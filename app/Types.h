#ifndef TYPES_H
#define TYPES_H
#include <QString>
#include <QPair>
#include <QVector>

namespace sdb{

const QVector<QPair<QString,QString>> root_table_names{ {"date_time",   "Дата Время"},
                                                        {"owner",        "Правообладатель"},
                                                        {"experiment",   "Эксперимент"},
                                                        {"capture_type", "Тип съёмки"},
                                                        {"location_name","Местоположение"},
                                                        {"surface",      "Тип поверхности"}
                                                       };

const QVector<QString>hiden_columns {
                                       "id",
                                       "sun_elevation_angle",
                                       "capturing_angle"
                                     };

}


#endif // TYPES_H
