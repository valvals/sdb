#include "UnitTests.h"
#include "../libs/DBJson.h"

namespace{
using namespace db_json;
bool isJSonObjectAndSpectralStructIdentical(QString &error_field,
                                            const QJsonObject &jo,
                                            const SPECTRAL_STRUCT &spectral_struct){
    if(jo.find("meta_data") == jo.end()){
        error_field = "meta_data";
        return false;
    }
    auto md = jo.find("meta_data").value().toObject();

    if(spectral_struct.md.date_time != md.find("date_time").value().toString()){
        error_field = "date_time";
        return false;
    }
    if(spectral_struct.md.owner != md.find("owner").value().toString()){
        error_field = "owner";
        return false;
    }
    if(spectral_struct.md.sun_elevation_angle != md.find("sun_elevation_angle").value().toDouble()){
        error_field = "sun_elevation_angle";
        return false;
    }
    if(spectral_struct.md.capture_angle != md.find("capture_angle").value().toDouble()){
        error_field = "capture_angle";
        return false;
    }
    if(spectral_struct.md.experiment_name != md.find("experiment_name").value().toString()){
        error_field = "experiment_name";
        return false;
    }

    if(spectral_struct.md.capture_type != md.find("capture_type").value().toString()){
        error_field = "capture_type";
        return false;
    }
    if(md.find("classification") == md.end()){
        error_field = "classification";
        return false;
    }
    auto classification = md.find("classification")->toObject();

    if(spectral_struct.md.classification.general_type != classification.find("general_type")->toString()){
        error_field = "general_type";
        return false;
    }
    if(spectral_struct.md.classification.class_name != classification.find("class_name")->toString()){
        error_field = "class_name";
        return false;
    }
    if(spectral_struct.md.classification.object_name != classification.find("object_name")->toString()){
        error_field = "object_name";
        return false;
    }
    if(md.find("location") == md.end()){
        error_field = "location";
        return false;
    }
    auto location = md.find("location")->toObject();

    if(spectral_struct.md.location.latitude != location.find("latitude").value().toDouble()){
        error_field = "latitude";
        return false;
    }
    if(spectral_struct.md.location.longitude != location.find("longitude").value().toDouble()){
        error_field = "longitude";
        return false;
    }
    if(spectral_struct.md.location.altitude != location.find("altitude").value().toDouble()){
        error_field = "altitude";
        return false;
    }
    if(spectral_struct.md.location.local_name != location.find("local_name").value().toString()){
        error_field = "local_name";
        return false;
    }
    if(spectral_struct.md.location.place_name != location.find("place_name").value().toString()){
        error_field = "place_name";
        return false;
    }
    if(spectral_struct.md.location.place_type != location.find("place_type").value().toString()){
        error_field = "place_type";
        return false;
    }
    if(spectral_struct.md.location.region_name != location.find("region_name").value().toString()){
        error_field = "region_name";
        return false;
    }

    if(md.find("images") == md.end()){
        error_field = "images";
        return false;
    }
    auto images = md.find("images")->toArray();
    if(images.size() != spectral_struct.md.images.size()){
        error_field = "images size";
        return false;
    }
    for(int i=0; i<images.size(); ++i){
        auto image_object = images[i].toObject();
        auto image_struct = spectral_struct.md.images[i];

        if(image_object.find("name")->toString() != image_struct.name){
            error_field = "images name " + QString::number(i);
            return false;
        };
        if(image_object.find("type")->toString() != image_struct.type){
            error_field = "images type " + QString::number(i);
            return false;
        };
        if(image_object.find("description")->toString() != image_struct.description){
            error_field = "images description " + QString::number(i);
            return false;
        };

    };
    if(spectral_struct.md.relief_type != md.find("relief_type").value().toString()){
        error_field = "relief_type";
        return false;
    }

    if(md.find("fraction") == md.end()){
        error_field = "fraction";
        return false;
    }
    auto fraction = md.find("fraction").value().toObject();
    if(spectral_struct.md.fraction.name != fraction.find("name")->toString()){
        error_field = "fraction name";
        return false;
    }
    if(spectral_struct.md.fraction.from != fraction.find("from")->toDouble()){
        error_field = "fraction from";
        return false;
    }
    if(spectral_struct.md.fraction.to != fraction.find("to")->toDouble()){
        error_field = "fraction to";
        return false;
    }
    if(spectral_struct.md.fraction.unit != fraction.find("unit")->toString()){
        error_field = "fraction unit";
        return false;
    }


    if(md.find("air_conditions") == md.end()){
        error_field = "air_conditions";
        return false;
    }
    auto air_conditions = md.find("air_conditions")->toObject();
    if(spectral_struct.md.air_conditions.temperature != air_conditions.find("temperature").value().toDouble()){
        error_field = "air_conditions temperature";
        return false;
    }
    if(spectral_struct.md.air_conditions.humidity != air_conditions.find("humidity").value().toDouble()){
        error_field = "air_conditions humidity";
        return false;
    }


    if(md.find("wheather_conditions") == md.end()){
        error_field = "wheather_conditions";
        return false;
    }
    auto wheather_conditions = md.find("wheather_conditions").value().toObject();
    if(spectral_struct.md.wheather_conditions.clouds_level != wheather_conditions.find("clouds_level")->toDouble()){

        error_field = "wheather_conditions clouds_level";
        return false;
    }
    if(spectral_struct.md.wheather_conditions.wind != wheather_conditions.find("wind")->toDouble()){

        error_field = "wheather_conditions wind";
        return false;
    }
    if(spectral_struct.md.wheather_conditions.direction != wheather_conditions.find("direction")->toString()){

        error_field = "wheather_conditions direction";
        return false;
    }

    if(jo.find("spectral_data") == jo.end()){
        error_field = "spectral_data";
        return false;
    }
    auto sd = jo.find("spectral_data").value().toObject();

    auto atributes = sd.find("atributes")->toArray();
    if(spectral_struct.sd.atributes.size() != atributes.size()){
        error_field = "spectral_data atributes size";
        return false;
    }
    for(int i=0; i<atributes.size(); ++i){
        auto instr = atributes[i].toObject().value("instrument");
        auto type = atributes[i].toObject().value("type");
        auto description = atributes[i].toObject().value("description");
        if(spectral_struct.sd.atributes[i].instrument != instr.toString()){
            error_field = "spectral_data instrument";
            return false;
        }
        if(spectral_struct.sd.atributes[i].type != type.toString()){
            error_field = "spectral_data type";
            return false;
        }
        if(spectral_struct.sd.atributes[i].description != description.toString()){
            error_field = "spectral_data description";
            return false;
        }
    }

    auto waves = sd.find("waves").value().toArray();
    if(waves.size()!=spectral_struct.sd.waves.size()){
        error_field = "spectral_data waves size";
        return false;
    }

    for(int i=0; i<waves.size(); ++i){
        auto wave_line = waves[i].toArray();
        if(spectral_struct.sd.waves[i].size()!=wave_line.size()){
            error_field = "spectral_data waves line size" + QString::number(i);
            return false;
        }
        for(int j=0; j<wave_line.size(); ++j){
            if(wave_line[j]!=spectral_struct.sd.waves[i][j]){
                error_field = QString("spectral_data waves value %1 %2").arg(i,j);
                return false;
            }
        }

    };

    if(sd.find("values") == sd.end()){
        error_field = "values";
        return false;
    }
    auto values = sd.find("values").value().toArray();
    for(int i=0; i<values.size(); ++i){
        auto value_line = values[i].toArray();
        if(spectral_struct.sd.values[i].size() != value_line.size()){
            error_field = "spectral_data values line size" + QString::number(i);
            return false;
        }
        for(int j=0; j<value_line.size(); ++j){
            if(value_line[j] != spectral_struct.sd.values[i][j]){
                error_field = QString("spectral_data values value %1 %2").arg(i,j);
                return false;
            }
        }
    };
    return true;
};

void fillValidSpectralStruct(SPECTRAL_STRUCT &spectral_struct){
    spectral_struct.md.date_time = "2023/5/17 16:05:17";
    spectral_struct.md.owner = "НИИ ПФП";
    spectral_struct.md.sun_elevation_angle = 48;
    spectral_struct.md.capture_angle = 0;
    spectral_struct.md.classification.general_type = "Горы";
    spectral_struct.md.classification.class_name = "вулканические";
    spectral_struct.md.classification.object_name = "песчаные, шлаковые отложения вулканической природы";
    spectral_struct.md.location.latitude = 53.06;
    spectral_struct.md.location.longitude = 27.25;
    spectral_struct.md.location.altitude = 430;
    spectral_struct.md.location.local_name = "уникальное имя точки съёмки";
    spectral_struct.md.location.place_name = "Уруп";
    spectral_struct.md.location.place_type = "остров";
    spectral_struct.md.location.region_name = "Курильские острова";
    spectral_struct.md.images = {{"image_name_1","обзорный снимок","подспутниковый полигон №1"},
                                 {"image_name_2","поле зрения",""}
                                };
    spectral_struct.md.relief_type = "склон";
    spectral_struct.md.fraction.name = "мелкозернистый песок";
    spectral_struct.md.fraction.from = 0.1;
    spectral_struct.md.fraction.to = 2;
    spectral_struct.md.fraction.unit = "mm";
    spectral_struct.md.air_conditions.temperature = 24;
    spectral_struct.md.air_conditions.humidity = 86;
    spectral_struct.md.wheather_conditions.clouds_level = 9;
    spectral_struct.md.wheather_conditions.wind = 1;
    spectral_struct.md.wheather_conditions.direction = "NE";
    spectral_struct.sd.atributes = {
        {"ФСР-02","кся","специфика съёмки"},
        {"ПСР-700","кся","специфика съёмки"},
        {"ПСР-1300","кся","специфика съёмки"}
    };
    spectral_struct.sd.waves = {
        {323.05, 323.439},
        {323.09, 323.44},
        {323.015, 323.45}
    };
    spectral_struct.sd.values = {
        {0.4, 0.77},
        {0.6, 0.8},
        {0.35, 0.98}
    };

};

} // end namespace


UnitTests::UnitTests()
{
}

void UnitTests::initTestCase()
{
    // Инициализация перед запуском всех тестов
}

void UnitTests::cleanupTestCase()
{
    // Очистка после выполнения всех тестов
}

void UnitTests::init()
{
    // Инициализация перед каждым тестом
}

void UnitTests::cleanup()
{
    // Очистка после каждого теста
}

void UnitTests::testLoadJsonFromFile()
{
    bool result;
    QJsonObject jo;
    result =  db_json::getJsonObjectFromFile(QDir::currentPath()+"/SPECTRAL_JSON.json",jo);
    QVERIFY(result);
    //TODO check that all nodes in the JSON file exists
    QVERIFY2(jo.find("meta_data").value().isObject(),"check meta_data node error");
    QVERIFY2(jo.find("spectral_data").value().isObject(), "check spectral_data node error");
}

void UnitTests::testConvertionFromJsonObjectToSpectralStruct()
{

    QString error;
    db_json::SPECTRAL_STRUCT sp_str;
    QJsonObject jo;
    db_json::getJsonObjectFromFile("SPECTRAL_JSON.json",jo);
    db_json::getStructFromJsonObject(sp_str,jo);
    QVERIFY2(isJSonObjectAndSpectralStructIdentical(error,jo,sp_str),error.toLatin1());
}

void UnitTests::testSaveStructToJsonFile()
{
    const QString path = QDir::currentPath().append("\\test.json");
    db_json::SPECTRAL_STRUCT spectral_struct;
    fillValidSpectralStruct(spectral_struct);
    db_json::saveStructToJsonFile(path,spectral_struct,QJsonDocument::Indented);
    QJsonObject testJson;
    db_json::getJsonObjectFromFile(path,testJson);
    QString error;
    QVERIFY2(isJSonObjectAndSpectralStructIdentical(error,testJson,spectral_struct),error.toLatin1());
}

void UnitTests::testIsJsonObjectAndStructIdentical()
{
    db_json::SPECTRAL_STRUCT sp_str;
    QJsonObject jo;
    db_json::getJsonObjectFromFile(QDir::currentPath()+"/SPECTRAL_JSON.json",jo);
    db_json::getStructFromJsonObject(sp_str,jo);
    QString error;
    QVERIFY2(isJSonObjectAndSpectralStructIdentical(error,jo,sp_str),error.toLatin1());
}

QTEST_MAIN(UnitTests)
