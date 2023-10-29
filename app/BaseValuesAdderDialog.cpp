#include "BaseValuesAdderDialog.h"

#include "ui_BaseValuesAdderDialog.h"
#include "QDebug"
#include "QSqlQuery"
#include "SqlUtility.h"
#include "Types.h"
#include "Constants.h"
#include "MessageReporter.h"

namespace  {

enum PAGES{
    CHOOSE_TABLE_PAGE = 0,
    CHOOSE_FIELD_PAGE = 1,
    CHOOSE_GENERAL_TYPE_PAGE = 2,
    CHOOSE_CLASS_PAGE = 3,
    CHOOSE_REGION_PAGE = 4,
    CHOOSE_GEO_NAME_PAGE = 5,
    CHOOSE_PLACE_TYPE_PAGE = 6,
    INPUT_NEW_OBJECT_PAGE = 7,
    LAT_LON_ALT_PAGE = 8
};

enum CREATION_VARIANT{
    ADD_GENERAL_TYPE,
    ADD_CLASS,
    ADD_OBJECT,
    ADD_REGION,
    ADD_GEO_NAME,
    ADD_LOCATION_TYPE,
    ADD_LOCATION_NAME,
    ADD_EXPERIMENT_NAME,
    ADD_INSTRUMENT,
    ADD_SPECTRAL_DATA_TYPE,
    ADD_CAPTURE_TYPE,
    ADD_OWNER
} CREATION_VAR;

QHash<CREATION_VARIANT,PAGES> page_provider
{
    {ADD_GENERAL_TYPE,INPUT_NEW_OBJECT_PAGE},
    {ADD_LOCATION_TYPE, INPUT_NEW_OBJECT_PAGE},
    {ADD_REGION,INPUT_NEW_OBJECT_PAGE},
    {ADD_CLASS,CHOOSE_GENERAL_TYPE_PAGE},
    {ADD_OBJECT,CHOOSE_GENERAL_TYPE_PAGE},
    {ADD_LOCATION_NAME,CHOOSE_REGION_PAGE},
    {ADD_GEO_NAME,CHOOSE_PLACE_TYPE_PAGE},
    {ADD_EXPERIMENT_NAME,INPUT_NEW_OBJECT_PAGE},
    {ADD_CAPTURE_TYPE,INPUT_NEW_OBJECT_PAGE},
    {ADD_SPECTRAL_DATA_TYPE,INPUT_NEW_OBJECT_PAGE},
    {ADD_INSTRUMENT,INPUT_NEW_OBJECT_PAGE},
    {ADD_OWNER,INPUT_NEW_OBJECT_PAGE}
};
int currentPageIndx = CHOOSE_TABLE_PAGE;
} // end namespace

using namespace sdb;

BaseValuesAdderDialog::BaseValuesAdderDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseValuesAdderDialog)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->comboBox_table_name->addItems({sdb::kCreationTreeTableClassification,
                                       sdb::kCreationTreeTableLocation,
                                       sdb::kCreationTreeTableMeasurements
                                      });
    locationTableFields << sdb::kCreationTreeRegion
                        << sdb::kCreationTreePlaceType
                        << sdb::kCreationTreeGeographicName
                        << sdb::kCreationTreeUniqueLocationName;

    classificationTableFields << sdb::kCreationTreeGeneralType
                              << sdb::kCreationTreeClass
                              << sdb::kCreationTreeObject;

    measurementTableFields << sdb::kCreationTreeOwners
                           << sdb::kCreationTreeExperiment
                           << sdb::kCreationTreeCaptureTypes
                           << sdb::kCreationTreeInstrument
                           << sdb::kCreationTreeSpectralDataTypes;

    updateAllObjects();
}

BaseValuesAdderDialog::~BaseValuesAdderDialog()
{
    delete ui;
}

void BaseValuesAdderDialog::on_stackedWidget_currentChanged(int arg1)
{
    PAGES page = (PAGES)arg1;
    QString common_description;
    QString creation_tree;
    switch (page) {
    case LAT_LON_ALT_PAGE:
        ui->label_common_description->setText("Выберите географическое имя");
        insertValuesToCombobox(QString("SELECT name FROM places WHERE type_name = '%1'")
                               .arg(ui->comboBox_place_types->currentText()),
                               ui->comboBox_geo_name);
        creation_tree.append(QString("%1%2%3(%4)%5%6(%7)").arg(getTableAndFieldCreationTree(),
                                                               sdb::kCreationTreeArrow,
                                                               sdb::kCreationTreeRegion,
                                                               ui->comboBox_regions->currentText(),
                                                               sdb::kCreationTreeArrow,
                                                               sdb::kCreationTreePlaceType,
                                                               ui->comboBox_place_types->currentText()));
        break;
    case CHOOSE_TABLE_PAGE:
        ui->label_common_description->setText("выбор таблицы");
        creation_tree = "";
        break;
    case CHOOSE_FIELD_PAGE:
        ui->label_common_description->setText("Выбор параметра");
        creation_tree = ui->comboBox_table_name->currentText();
        ui->comboBox_field_name->clear();
        if(ui->comboBox_table_name->currentText() == sdb::kCreationTreeTableClassification){
            ui->comboBox_field_name->addItems(classificationTableFields);
        }else if(ui->comboBox_table_name->currentText() == sdb::kCreationTreeTableLocation){
            ui->comboBox_field_name->addItems(locationTableFields);
        }else if(ui->comboBox_table_name->currentText() == sdb::kCreationTreeTableMeasurements){
            ui->comboBox_field_name->addItems(measurementTableFields);
        }
        break;


        // На этом этапе происходит добавление нового объекта в БД
    case INPUT_NEW_OBJECT_PAGE:
        creation_tree = getTableAndFieldCreationTree();
        switch (CREATION_VAR) {
        case ADD_LOCATION_NAME:
            common_description ="добавление уникального места";
            creation_tree.append(sdb::kCreationTreeArrow);
            creation_tree.append(QString("%1(%2)%3%4(%5)%6%7 (%8 ш:%9 д:%10 в:%11)")
                                 .arg(sdb::kCreationTreeRegion)//1
                                 .arg(ui->comboBox_regions->currentText())//2
                                 .arg(sdb::kCreationTreeArrow)//3
                                 .arg(sdb::kCreationTreePlaceType)//4
                                 .arg(ui->comboBox_place_types->currentText())//5
                                 .arg(sdb::kCreationTreeArrow)//6
                                 .arg(sdb::kCreationTreeGeographicName)//7
                                 .arg(ui->comboBox_geo_name->currentText())//8
                                 .arg(QString::number(ui->doubleSpinBox_latitude->value()))//9
                                 .arg(QString::number(ui->doubleSpinBox_longitude->value()))//10
                                 .arg(QString::number(ui->doubleSpinBox_altitude->value())));//11

            break;
        case ADD_GENERAL_TYPE:
            common_description ="добавление генерального типа";
            break;
        case ADD_CLASS:
            common_description ="добавление класса";
            creation_tree.append(sdb::kCreationTreeArrow);
            creation_tree.append(sdb::kCreationTreeGeneralType);
            creation_tree.append(QString("(%1)").arg(ui->comboBox_general_types->currentText()));
            break;
        case ADD_OBJECT:
            common_description ="добавление классификационного объекта";
            creation_tree.append(sdb::kCreationTreeArrow);
            creation_tree.append(sdb::kCreationTreeGeneralType);
            creation_tree.append(QString("(%1)%2%3(%4)").arg(ui->comboBox_general_types->currentText(),
                                                             sdb::kCreationTreeArrow,
                                                             sdb::kCreationTreeClass,
                                                             ui->comboBox_choose_class->currentText()));
            break;
        case ADD_REGION:
            common_description ="добавление региона";
            break;
        case ADD_GEO_NAME:
            common_description ="добавление географического имени";
            creation_tree.append(sdb::kCreationTreeArrow);
            creation_tree.append(sdb::kCreationTreePlaceType);
            creation_tree.append(QString("(%1)").arg(ui->comboBox_place_types->currentText()));
            break;
        case ADD_LOCATION_TYPE:
            common_description ="добавление типа места";
            break;
        case ADD_EXPERIMENT_NAME:
            common_description ="добавление эксперимента";
            break;
        case ADD_INSTRUMENT:
            common_description ="добавление инструмента";
            break;
        case ADD_SPECTRAL_DATA_TYPE:
            common_description ="добавление типа спектральных данных";
            break;
        case ADD_CAPTURE_TYPE:
            common_description ="добавление типа съёмки";
            break;
        case ADD_OWNER:
            common_description ="добавление владельца";
            break;
        };
        ui->label_common_description->setText(common_description);
        break;



    case CHOOSE_GENERAL_TYPE_PAGE:
        ui->label_common_description->setText("Выберите генеральный тип");
        creation_tree = getTableAndFieldCreationTree();
        break;
    case CHOOSE_CLASS_PAGE:
        ui->label_common_description->setText("Выберите класс");
        creation_tree = getTableAndFieldCreationTree();
        creation_tree.append(QString("%1%2(%3)").arg(sdb::kCreationTreeArrow,
                                                     sdb::kCreationTreeGeneralType,
                                                     ui->comboBox_general_types->currentText()));
        insertValuesToCombobox(QString("SELECT class_name FROM classes WHERE general_type_name = '%1'").
                               arg(ui->comboBox_general_types->currentText()),
                               ui->comboBox_choose_class);
        break;
    case CHOOSE_REGION_PAGE:
        ui->label_common_description->setText("Выберите регион");
        creation_tree = getTableAndFieldCreationTree();
        break;
    case CHOOSE_GEO_NAME_PAGE:
        ui->label_common_description->setText("Выберите географическое имя");
        break;
    case CHOOSE_PLACE_TYPE_PAGE:
        ui->label_common_description->setText("Выберите тип места");
        creation_tree = getTableAndFieldCreationTree();
        if(CREATION_VAR == ADD_LOCATION_NAME){
            creation_tree.append(QString("%1%2(%3)").arg(sdb::kCreationTreeArrow,
                                                         sdb::kCreationTreeRegion,
                                                         ui->comboBox_regions->currentText()));
        }
        break;
    }

    // Отображение на каком этапе находится пользователь
    ui->label_tree_creation->setText(creation_tree);
}

void BaseValuesAdderDialog::on_pushButton_back_clicked()
{
    PAGES currentPage = (PAGES)ui->stackedWidget->currentIndex();
    switch(currentPage){
    case CHOOSE_TABLE_PAGE:// this is the end and we do not need to move it back
        break;
    case CHOOSE_FIELD_PAGE:
        currentPageIndx = CHOOSE_TABLE_PAGE;
        break;
    case CHOOSE_GENERAL_TYPE_PAGE:
        if(CREATION_VAR == ADD_CLASS || CREATION_VAR == ADD_OBJECT){
            currentPageIndx = CHOOSE_TABLE_PAGE;
        }
        break;
    case CHOOSE_CLASS_PAGE:
        if(CREATION_VAR == ADD_OBJECT){
            currentPageIndx = CHOOSE_GENERAL_TYPE_PAGE;
        }
        break;
    case CHOOSE_REGION_PAGE:
        if(CREATION_VAR == ADD_LOCATION_NAME){
            currentPageIndx = CHOOSE_FIELD_PAGE;
        }
        break;
    case CHOOSE_GEO_NAME_PAGE:

        break;
    case CHOOSE_PLACE_TYPE_PAGE:
        if(CREATION_VAR == ADD_GEO_NAME){
            currentPageIndx = CHOOSE_FIELD_PAGE;
        }else
            if(CREATION_VAR == ADD_LOCATION_NAME){
                currentPageIndx = CHOOSE_REGION_PAGE;
            }
        break;
    case INPUT_NEW_OBJECT_PAGE:
        if(CREATION_VAR == ADD_GENERAL_TYPE){
            currentPageIndx = CHOOSE_TABLE_PAGE;
        }
        else if(CREATION_VAR == ADD_LOCATION_TYPE){
            currentPageIndx = CHOOSE_TABLE_PAGE;
        }
        else if(CREATION_VAR == ADD_REGION){
            currentPageIndx = CHOOSE_TABLE_PAGE;
        }
        else if(CREATION_VAR == ADD_EXPERIMENT_NAME){
            currentPageIndx = CHOOSE_FIELD_PAGE;
        }
        else if(CREATION_VAR == ADD_OWNER){
            currentPageIndx = CHOOSE_FIELD_PAGE;
        }
        else if(CREATION_VAR == ADD_INSTRUMENT){
            currentPageIndx = CHOOSE_FIELD_PAGE;
        }
        else if(CREATION_VAR == ADD_CAPTURE_TYPE){
            currentPageIndx = CHOOSE_FIELD_PAGE;
        }
        else if(CREATION_VAR == ADD_SPECTRAL_DATA_TYPE){
            currentPageIndx = CHOOSE_FIELD_PAGE;
        }
        else if(CREATION_VAR == ADD_CLASS){
            currentPageIndx = CHOOSE_GENERAL_TYPE_PAGE;
        }else if(CREATION_VAR == ADD_GEO_NAME){
            currentPageIndx = CHOOSE_PLACE_TYPE_PAGE;
        }else if(CREATION_VAR == ADD_OBJECT){
            currentPageIndx = CHOOSE_CLASS_PAGE;
        }else if(CREATION_VAR == ADD_LOCATION_NAME){
            currentPageIndx = LAT_LON_ALT_PAGE;
        }
        break;
    case LAT_LON_ALT_PAGE:
        if(CREATION_VAR == ADD_LOCATION_NAME){
            currentPageIndx = CHOOSE_PLACE_TYPE_PAGE;
        }
        break;
    }
    ui->stackedWidget->setCurrentIndex(currentPageIndx);
}

void BaseValuesAdderDialog::on_pushButton_forward_clicked()
{
    if(currentPageIndx == INPUT_NEW_OBJECT_PAGE)return;
    if(currentPageIndx == CHOOSE_TABLE_PAGE){
        currentPageIndx = CHOOSE_FIELD_PAGE;
        ui->stackedWidget->setCurrentIndex(currentPageIndx);
        return;
    }else {
        getNextIndex(currentPageIndx);
    }
    ui->stackedWidget->setCurrentIndex(currentPageIndx);
}

void BaseValuesAdderDialog::on_pushButton_add_new_object_clicked()
{
    using namespace sdb;
    auto newObjectName = ui->lineEdit_new_object_name->text();
    bool result = false;

    switch (CREATION_VAR) {

    case ADD_LOCATION_NAME:
        result = insertLocationObject(newObjectName,
                                      ui->comboBox_geo_name->currentText(),
                                      ui->comboBox_regions->currentText(),
                                      ui->doubleSpinBox_latitude->value(),
                                      ui->doubleSpinBox_longitude->value(),
                                      ui->doubleSpinBox_altitude->value());
        break;

    case ADD_GENERAL_TYPE:
        result = insertValueToDB("INSERT INTO general_type (type_name) VALUES (:tn)",
                                 ":tn", newObjectName);
        updateGeneralTypes();
        emit generalTypeChanged();
        break;

    case ADD_CLASS:
        result = insertValuesToDB("INSERT INTO classes (class_name,general_type_name) VALUES (:cn,:gt)",
                                  {":cn",":gt"},
                                  {newObjectName,
                                   ui->comboBox_general_types->currentText()
                                  });
        updateClasses();
        break;

    case ADD_OBJECT:
        result = insertValuesToDB("INSERT INTO objects (object_name, class_name) VALUES (:on,:cn)",
                                  {":on",":cn"},
                                  {newObjectName,
                                   ui->comboBox_choose_class->currentText()
                                  });
        break;

    case ADD_REGION:
        result = insertValueToDB("INSERT INTO regions (region_name) VALUES (:rn)",
                                 ":rn",
                                 newObjectName);
        updateRegions();
        emit regionsChanged();
        break;

    case ADD_GEO_NAME:
        result = insertValuesToDB("INSERT INTO places (name,type_name) VALUES (:name,:tn)",
                                  {":name",":tn"},
                                  {newObjectName,
                                   ui->comboBox_place_types->currentText()
                                  });
        updateGeographicNames();
        break;

    case ADD_LOCATION_TYPE:
        result = insertValueToDB("INSERT INTO place_types (place_type_name) VALUES (:ptn)",
                                 ":ptn",
                                 newObjectName);
        updatePlaceTypes();
        emit placeTypeNameChanged();
        break;
    case ADD_EXPERIMENT_NAME:
        result = insertValueToDB("INSERT INTO experiment(name) VALUES (:en)",
                                 ":en",
                                 newObjectName);
        emit experimentChanged();
        break;
    case ADD_INSTRUMENT:
        result = insertValueToDB("INSERT INTO instrument_type(name) VALUES (:in)",
                                 ":in",
                                 newObjectName);
        emit instrumentChanged();
        break;
    case ADD_SPECTRAL_DATA_TYPE:
        result = insertValueToDB("INSERT INTO spectral_data_types(name) VALUES (:sdt)",
                                 ":sdt",
                                 newObjectName);
        emit spectralDataTypeChanged();
        break;
    case ADD_CAPTURE_TYPE:
        result = insertValueToDB("INSERT INTO capture_type(name) VALUES (:ct)",
                                 ":ct",
                                 newObjectName);
        emit captureTypeChanged();
        break;
    case ADD_OWNER:
        result = insertValueToDB("INSERT INTO owner(name) VALUES (:on)",
                                 ":on",
                                 newObjectName);
        emit ownerChanged();
        break;
    }
    if(result){
        showInfo(NEW_OBJECT_SUCCESSFULLY_ADDED_TO_DB);
        ui->lineEdit_new_object_name->clear();
    }
    else {
        showError(FAIL_TO_ADD_OBJECT_TO_DB);
    }
}

void BaseValuesAdderDialog::getNextIndex(int &index)
{
    auto field_name = ui->comboBox_field_name->currentText();
    auto table_name = ui->comboBox_table_name->currentText();
    if(index == CHOOSE_FIELD_PAGE){
        if(table_name == sdb::kCreationTreeTableClassification){
            if(field_name == sdb::kCreationTreeGeneralType){
                CREATION_VAR = ADD_GENERAL_TYPE;
            }else if(field_name == sdb::kCreationTreeClass){
                CREATION_VAR = ADD_CLASS;
            }else if(field_name == sdb::kCreationTreeObject){
                CREATION_VAR = ADD_OBJECT;
            }
        }else if(table_name == sdb::kCreationTreeTableLocation){

            if(field_name == sdb::kCreationTreeRegion){
                CREATION_VAR = ADD_REGION;
            }else if(field_name == sdb::kCreationTreePlaceType){
                CREATION_VAR = ADD_LOCATION_TYPE;
            }else if(field_name == sdb::kCreationTreeGeographicName){
                CREATION_VAR = ADD_GEO_NAME;
            }else if(field_name == sdb::kCreationTreeUniqueLocationName){
                CREATION_VAR = ADD_LOCATION_NAME;
            }
        }else if(table_name == sdb::kCreationTreeTableMeasurements){
            if(field_name == sdb::kCreationTreeOwners){
                CREATION_VAR = ADD_OWNER;
            }else if(field_name == sdb::kCreationTreeExperiment){
                CREATION_VAR = ADD_EXPERIMENT_NAME;
            }else if(field_name == sdb::kCreationTreeCaptureTypes){
                CREATION_VAR = ADD_CAPTURE_TYPE;
            }else if(field_name == sdb::kCreationTreeInstrument){
                CREATION_VAR = ADD_INSTRUMENT;
            }else if(field_name == sdb::kCreationTreeSpectralDataTypes){
                CREATION_VAR = ADD_SPECTRAL_DATA_TYPE;
            }
        }
        index = page_provider.value(CREATION_VAR);
        return;
    }else if(index == CHOOSE_GENERAL_TYPE_PAGE && CREATION_VAR == ADD_OBJECT){
        index = CHOOSE_CLASS_PAGE;
        return;
    }else if(index == CHOOSE_CLASS_PAGE && CREATION_VAR == ADD_OBJECT)
    {
        index = INPUT_NEW_OBJECT_PAGE;
        return;

    }else if(index == CHOOSE_REGION_PAGE && CREATION_VAR == ADD_LOCATION_NAME){
        index = CHOOSE_PLACE_TYPE_PAGE;
        return;
    }
    else if(index == CHOOSE_PLACE_TYPE_PAGE && CREATION_VAR == ADD_LOCATION_NAME){
        index = LAT_LON_ALT_PAGE;
        return;
    }else if(index == LAT_LON_ALT_PAGE){
        index = INPUT_NEW_OBJECT_PAGE;
    }
    else if(index == CHOOSE_GEO_NAME_PAGE || index == CHOOSE_PLACE_TYPE_PAGE){
        index = INPUT_NEW_OBJECT_PAGE;
        return;
    }else if(index == CHOOSE_GENERAL_TYPE_PAGE || index == CHOOSE_REGION_PAGE){
        index = INPUT_NEW_OBJECT_PAGE;
        return;
    }

}

QString BaseValuesAdderDialog::getTableAndFieldCreationTree()
{
    return ui->comboBox_table_name->currentText() +
            sdb::kCreationTreeArrow +
            ui->comboBox_field_name->currentText();
}

void BaseValuesAdderDialog::updateGeneralTypes()
{
    insertValuesToCombobox(sdb::kQuerySelectGeneralTypes,
                           ui->comboBox_general_types);
}

void BaseValuesAdderDialog::updateClasses()
{
    insertValuesToCombobox(sdb::kQuerySelectAllClasses,
                           ui->comboBox_choose_class);
}

void BaseValuesAdderDialog::updateRegions()
{
    insertValuesToCombobox(sdb::kQuerySelectAllRegions,
                           ui->comboBox_regions);
}

void BaseValuesAdderDialog::updatePlaceTypes()
{
    insertValuesToCombobox(sdb::kQuerySelectAllPlaceTypes,
                           ui->comboBox_place_types);
}

void BaseValuesAdderDialog::updateGeographicNames()
{

    insertValuesToCombobox(sdb::kQuerySelectAllPlaceNames,
                           ui->comboBox_places);
}

void BaseValuesAdderDialog::updateAllObjects()
{
    updateGeneralTypes();
    updateClasses();
    updateRegions();
    updatePlaceTypes();
    updateGeographicNames();
}

