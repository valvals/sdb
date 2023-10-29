#ifndef SQLUTILITY_H
#define SQLUTILITY_H


class QString;
class QStringList;
class QComboBox;

namespace sdb{

extern const char kQuerySelectAllRegions[];
extern const char kQuerySelectAllClasses[];
extern const char kQuerySelectAllPlaceNames[];
extern const char kQuerySelectAllPlaceTypes[];
extern const char kQueryImageIds[];
extern const char kQueryImageData[];
extern const char kQuerySpectrData[];
extern const char kQueryLocation[];
extern const char kQueryClassification[];
extern const char kQuerySelectGeneralTypes[];
extern const char kQuerySelectTypeNameFromPlacesForPlaceName[];
extern const char kQuerySelectAirConditionsForMeasurementId[];
extern const char kQuerySelectObjectNameFromObjects[];


bool insertValueToDB(const QString &query_str,
                     const QString &bind_id,
                     const QString &value);

bool insertValuesToDB(const QString &query_str,
                     const QStringList &bind_ids,
                     const QStringList &values);

bool insertLocationObject(const QString &location_name,
                          const QString &geo_name, const QString &region_name,
                          const double &latitude,
                          const double &longitude,
                          const double &altitude);

bool insertValuesToCombobox(const QString &queryS,
                            QComboBox *cb);

}
#endif // SQLUTILITY_H
