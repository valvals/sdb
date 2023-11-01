#include "SqlUtility.h"
#include <QString>
#include <QtSql>
#include <QSqlQuery>
#include <QComboBox>


namespace sdb{

constexpr char kQueryInsertIntoLocation[] = "INSERT INTO location (location_name, place_name, region_name, latitude,longitude, altitude) VALUES (:ln,:pn,:rn,:lat,:long,:alt)";

const char kQuerySelectAllRegions[] = "SELECT region_name FROM regions";
const char kQuerySelectAllClasses[] = "SELECT class_name FROM classes";
const char kQuerySelectAllPlaceNames[] = "SELECT name FROM places";
const char kQuerySelectAllPlaceTypes[] = "SELECT place_type_name FROM place_types";

const char kQueryImageIds[] = "SELECT image_id FROM images_measurements WHERE measurements_id == %1";
const char kQueryImageData[] = "SELECT data, image_type, description FROM images WHERE id == %1";
const char kQuerySpectrData[] = "SELECT data_type, data, instrument, description FROM spectral_data_measurements WHERE measurements_id == %1";
const char kQueryLocation[] = "SELECT location_name, place_name, region_name, latitude, longitude, altitude FROM location WHERE location_name ='%1'";
const char kQueryClassification[] = "SELECT location_name,objects.object_name,objects.class_name,classes.general_type_name FROM measurements INNER JOIN objects ON objects.object_name = measurements.surface INNER JOIN classes ON classes.class_name = objects.class_name WHERE measurements.id = %1";
const char kQuerySelectGeneralTypes[] = "SELECT type_name FROM general_type";
const char kQuerySelectTypeNameFromPlacesForPlaceName[] = "SELECT type_name FROM places WHERE name ='%1'";
const char kQuerySelectAirConditionsForMeasurementId[] ="SELECT temperature, humidity FROM air_conditions_measurements WHERE measurement_id =%1";
const char kQuerySelectObjectNameFromObjects[] = "SELECT object_name FROM objects";


bool insertValueToDB(const QString &query_str,
                     const QString &bind_id,
                     const QString &value)
{
    QSqlQuery query;
    query.prepare(query_str);
    query.bindValue(bind_id,value);
    return query.exec();
}

bool insertValuesToCombobox(const QString &queryS,
                            QComboBox *cb)
{
    cb->clear();
    QSqlQuery query;
    auto result = query.exec(queryS);
    QStringList items;
    while(query.next()){
        auto value = query.value(0).toString();
        if(!items.contains(value))items.append(value);
    }
    cb->addItems(items);
    return result;
}

bool insertValuesToDB(const QString &query_str,
                      const QStringList &bind_ids,
                      const QStringList &values)
{
    if(bind_ids.size()!=values.size())return false;
    QSqlQuery query;
    query.prepare(query_str);
    for(int i=0;i<bind_ids.size();++i){
    query.bindValue(bind_ids[i],values[i]);
    }
    return query.exec();
}

bool insertLocationObject(const QString &location_name,
                          const QString &geo_name,
                          const QString &region_name,
                          const double &latitude,
                          const double &longitude,
                          const double &altitude)
{
    QSqlQuery query;
    query.prepare(kQueryInsertIntoLocation);
    query.bindValue(":ln",location_name);
    query.bindValue(":pn",geo_name);
    query.bindValue(":rn",region_name);
    query.bindValue(":lat",latitude);
    query.bindValue(":long",longitude);
    query.bindValue(":alt", altitude);
    auto result = query.exec();
    qDebug()<<"Query error: "<<query.lastError() <<result;
    return result;
}


} // end namespace sql_utility
