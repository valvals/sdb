#include "ManualAdderDialog.h"

#include <QtSql>
#include "MessageReporter.h"

#include "PlotShow.h"
#include "ui_ManualAdderDialog.h"
#include "SqlUtility.h"
//#include "UselessDataCutter.h"
//#include "memory.h"
using namespace sdb;

ManualAdderDialog::ManualAdderDialog(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ManualAdderDialog)
{
    ui->setupUi(this);
    plot = ui->widget;
    plot->setMinimumWidth(400);
    plot->setMinimumHeight(400);
    ui->tableWidget_images->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget_images->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    updateComboboxes();
    QAction *action_delete_image_from_table = new QAction;
    action_delete_image_from_table->setText("Удалить изображение");
    ui->tableWidget_images->addAction(std::move(action_delete_image_from_table));
    connect(action_delete_image_from_table,SIGNAL(triggered()),SLOT(deleteImageFromTable()));

    QAction *action_delete_spectr_from_table = new QAction;
    action_delete_spectr_from_table->setText("Удалить спектр");
    ui->tableWidget_spectral_data->addAction(std::move(action_delete_spectr_from_table));
    connect(action_delete_spectr_from_table,SIGNAL(triggered()),SLOT(deleteSpectrFromTable()));
}

ManualAdderDialog::~ManualAdderDialog()
{
    delete ui;
}

void ManualAdderDialog::updateComboboxes()
{
    updateGeneralTypes();
    updatePlaceTypes();
    updateRegions();
    updateCaptureTypes();
    updateOwners();
    updateExperiments();
    updateInstruments();
    updateSpectralDataTypes();
    updateImageTypes();
}

void ManualAdderDialog::deleteSelectedItemsFromList(QTableWidget *widget)
{
    QList<QTableWidgetItem*> items = widget->selectedItems();
    if(items.empty())return;
    widget->removeRow(items[0]->row());
}

bool ManualAdderDialog::isUniqueFieldExists(const QString &table_name,
                                            const QString &column_name,
                                            const QString &key_name)
{
    QSqlQuery querySelect;
    querySelect.prepare("SELECT * FROM " + table_name + " WHERE " + column_name + " = :Id");
    querySelect.bindValue(":Id", key_name);

    if(!querySelect.exec())
    {
        qDebug() << querySelect.lastError().text();
        return false;
    }
    else
    {
        return querySelect.next();
    }
    return true;
}

void ManualAdderDialog::fillDataBasedOnSelectedMeasurements(const db_json::SPECTRAL_STRUCT &spec_str)
{
    ui->dateTimeEdit->setDateTime(QDateTime::fromString(spec_str.md.date_time,"yyyy-MM-dd hh:mm:ss"));
    ui->comboBox_owners->setCurrentText(spec_str.md.owner);
    ui->comboBox_experiment_name->setCurrentText(spec_str.md.experiment_name);
    ui->comboBox_capture_type->setCurrentText(spec_str.md.capture_type);
    ui->doubleSpinBox_sun_elevation->setValue(spec_str.md.sun_elevation_angle);
    ui->doubleSpinBox_capturing_angle->setValue(spec_str.md.capture_angle);

    ui->comboBox_general_type->setCurrentText(spec_str.md.classification.general_type);
    ui->comboBox_class->setCurrentText(spec_str.md.classification.class_name);
    ui->comboBox_object->setCurrentText(spec_str.md.classification.object_name);

    ui->doubleSpinBox_altitude->setValue(spec_str.md.location.altitude);
    ui->doubleSpinBox_latitude->setValue(spec_str.md.location.latitude);
    ui->doubleSpinBox_longitude->setValue(spec_str.md.location.longitude);
    ui->comboBox_region_name->setCurrentText(spec_str.md.location.region_name);
    ui->comboBox_place_type->setCurrentText(spec_str.md.location.place_type);
    ui->comboBox_geographic_name->setCurrentText(spec_str.md.location.place_name);
    ui->lineEdit_location_name->setText(spec_str.md.location.local_name);

    ui->comboBox_region_name->setCurrentText(spec_str.md.location.region_name);
}

void ManualAdderDialog::on_comboBox_general_type_currentIndexChanged(const QString &arg1)
{
    QString qS = QString("SELECT class_name FROM classes WHERE general_type_name = '%1'").arg(arg1);
    insertValuesToCombobox(qS,ui->comboBox_class);
}

void ManualAdderDialog::on_comboBox_class_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_object->clear();
    QString qS = QString("SELECT object_name FROM objects WHERE class_name = '%1'").arg(arg1);
    insertValuesToCombobox(qS,ui->comboBox_object);
}

void ManualAdderDialog::on_comboBox_region_name_currentIndexChanged(const QString &arg1)
{
    QString qS = QString("SELECT place_name FROM location WHERE region_name = '%1'").arg(arg1);
    QSqlQuery query;
    QStringList place_names;
    query.exec(qS);
    while(query.next()){
        qDebug()<<"Location name: "<<query.value(0).toString();
        QString place_name = query.value(0).toString();
        if(!place_names.contains(place_name))place_names.append(place_name);
    }
    ui->comboBox_geographic_name->clear();
    if(place_names.isEmpty())return;
    ui->comboBox_geographic_name->addItems(place_names);
}

void ManualAdderDialog::on_comboBox_geographic_name_currentIndexChanged(const QString &arg1)
{
    QString qS = QString("SELECT type_name FROM places WHERE name = '%1'").arg(arg1);
    QSqlQuery query;
    QString type_name;
    query.exec(qS);
    while(query.next()){
        qDebug()<<"type name: "<<query.value(0).toString();
        type_name = query.value(0).toString();
    }
    ui->comboBox_place_type->setCurrentText(type_name);
}

void ManualAdderDialog::on_comboBox_place_type_currentIndexChanged(const QString &arg1)
{
    //if(ui->comboBox_->count()==0)return;
    QString tS = QString("SELECT location.place_name FROM location INNER JOIN places ON location.place_name = places.name "
   "WHERE location.region_name = '%1' AND places.type_name = '%2'")
            .arg(ui->comboBox_region_name->currentText())
            .arg(arg1);
    QStringList type_name;
    QSqlQuery query;
    query.exec(tS);
    while(query.next()){
        qDebug()<<"location.place_name: "<<query.value(0).toString();
        QString pn = query.value(0).toString();
        if(!type_name.contains(pn))type_name.append(pn);
    }
    if(type_name.isEmpty()){
        ui->comboBox_geographic_name->clear();
        return;
    }

    QString qS = QString("SELECT name FROM places WHERE type_name = '%1'").arg(arg1);
    ui->comboBox_geographic_name->clear();
    insertValuesToCombobox(qS,ui->comboBox_geographic_name);

}

void ManualAdderDialog::on_pushButton_choose_image_button_clicked()
{
    static QString imglastChoosedPath = "/home";
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Open Image"),
                                                 imglastChoosedPath,
                                                 tr("Image Files(*.png *.jpg *.bmp *.JPEG)"));
    QFile file(fileName);

    if(file.exists()){
        imglastChoosedPath = fileName;
        if(!file.open(QIODevice::ReadOnly))return;
        ui->label_add_image->setPixmap(QPixmap::fromImage(QImage(fileName)));
        auto rc = ui->tableWidget_images->model()->rowCount();
        ui->tableWidget_images->insertRow(rc);
        ui->tableWidget_images->setItem(rc,0,new QTableWidgetItem(fileName));
        ui->tableWidget_images->setItem(rc,1,new QTableWidgetItem(ui->comboBox_image_types->currentText()));
        ui->tableWidget_images->setItem(rc,2,new QTableWidgetItem(ui->lineEdit_image_comments->text()));
        ui->lineEdit_image_comments->clear();
        file.close();
    }

}

void ManualAdderDialog::deleteImageFromTable()
{
    deleteSelectedItemsFromList(ui->tableWidget_images);
    ui->label_add_image->clear();
}

void ManualAdderDialog::deleteSpectrFromTable()
{
    deleteSelectedItemsFromList(ui->tableWidget_spectral_data);
    plot->graph(0)->data().data()->clear();
    plot->replot();
}

void ManualAdderDialog::on_pushButton_add_data_to_DB_clicked()
{
    //TODO сделать проверку, что все данные заполнены и спектры валидны
    using namespace sdb;
    if(ui->comboBox_object->currentText().isEmpty()){
        showWarning(NO_OBJECT_FILLED);
        return;
    }
    else if(ui->lineEdit_location_name->text().isEmpty()){
        showWarning(NO_LOCATION_NAME_FILLED);
        return;
    }else if(ui->tableWidget_spectral_data->model()->rowCount()==0){
        showWarning(NO_SPECTRAL_DATA_ADDED);
        return;
    }

    QString qS = QString("INSERT INTO measurements (date_time,"
                                                   "owner,"
                                                   "experiment,"
                                                   "capture_type,"
                                                   "sun_elevation_angle,"
                                                   "capturing_angle,"
                                                   "location_name,"
                                                   "surface"
                                                   ") VALUES ('%1', '%2', '%3', '%4', %5, %6, '%7', '%8')")
            .arg(ui->dateTimeEdit->text())
            .arg(ui->comboBox_owners->currentText())
            .arg(ui->comboBox_experiment_name->currentText())
            .arg(ui->comboBox_capture_type->currentText())
            .arg(ui->doubleSpinBox_sun_elevation->value())
            .arg(ui->doubleSpinBox_capturing_angle->value())
            .arg(ui->lineEdit_location_name->text().toLower())
            .arg(ui->comboBox_object->currentText());

    //TODO Check if location is not exists add it to location table before insert data
    if(!isUniqueFieldExists("location","location_name",ui->lineEdit_location_name->text().toLower())){
        QSqlQuery locationQuery;
        locationQuery.prepare("INSERT INTO location"
        " (location_name, place_name, region_name, latitude, longitude, altitude) VALUES (:ln,:pn,:rn,:lat,:lon,:alt)");
        locationQuery.bindValue(":ln",ui->lineEdit_location_name->text().toLower());
        locationQuery.bindValue(":pn",ui->comboBox_geographic_name->currentText().toLower());
        locationQuery.bindValue(":rn",ui->comboBox_region_name->currentText().toLower());
        locationQuery.bindValue(":lat",ui->doubleSpinBox_latitude->value());
        locationQuery.bindValue(":lon",ui->doubleSpinBox_longitude->value());
        locationQuery.bindValue(":alt",ui->doubleSpinBox_altitude->value());
        qDebug()<<"INSERT Location query result: "<<locationQuery.exec()<<" " <<locationQuery.lastError();
    }else{
        //TODO show message that location is already exists
        showWarning(LOCATION_NAME_IS_NOT_UNIQUE);
        return;
    };

    QSqlQuery query;
    qDebug()<<" Test insert data to DB: "<<query.exec(qS);

    auto last__measurements_id = query.lastInsertId().toLongLong();
    while(query.next()){
        qDebug()<<"last_id.measurements: "<<last__measurements_id;
    }

    for(int i=0;i<ui->tableWidget_images->model()->rowCount();++i){
        auto img_file_name = ui->tableWidget_images->model()->index(i,0).data().toString();
        auto img_type = ui->tableWidget_images->model()->index(i,1).data().toString();
        auto img_desc = ui->tableWidget_images->model()->index(i,1).data().toString();
        QFile imageFile(img_file_name);
        long long last_images_id = 0;
        if (imageFile.open(QIODevice::ReadOnly)) {
            QByteArray imageData = imageFile.readAll();
            imageFile.close();

            QSqlQuery query;
            query.prepare("INSERT INTO images (data, image_type, description) VALUES (:data, :image_type, :description)");
            query.bindValue(":data", imageData);
            query.bindValue(":image_type", img_type);
            query.bindValue(":description", img_desc);

            if (query.exec()) {
                qDebug() << "Изображение с дополнительными данными успешно добавлено в базу данных.";
                last_images_id = query.lastInsertId().toLongLong();
            } else {
                qDebug() << "Ошибка при добавлении изображения с дополнительными данными:" << query.lastError().text();
            }
        } else {
            qDebug() << "Ошибка при открытии файла изображения:" << imageFile.errorString();
        }

        QSqlQuery imageQuery;
        imageQuery.prepare("INSERT INTO images_measurements(measurements_id, image_id) VALUES (:m_id, :i_id )");
        imageQuery.bindValue(":m_id",last__measurements_id);
        imageQuery.bindValue(":i_id",last_images_id);
        qDebug()<<"image query debug output: "<<imageQuery.exec();
    }

    for(int i=0;i<ui->tableWidget_spectral_data->model()->rowCount();++i){
        QSqlQuery spectrQuery;
        spectrQuery.prepare("INSERT INTO spectral_data_measurements"
    "(measurements_id, data_type, data, instrument, description) VALUES (:id,:dt,:data,:instr,:descr)");
        spectrQuery.bindValue(":id",last__measurements_id);
        auto item = ui->tableWidget_spectral_data->model()->index(i,0).data().toString();
        auto spectr_container = m_spectrs_for_db.value(item);
        spectrQuery.bindValue(":dt",spectr_container.data_type);
        spectrQuery.bindValue(":instr",spectr_container.instrument);
        spectrQuery.bindValue(":descr",spectr_container.description);
        spectrQuery.bindValue(":data",spectr_container.raw_data);
        qDebug()<<"spectr query debug output: "<<spectrQuery.exec();
    }
    showInfo(NEW_OBJECT_SUCCESSFULLY_ADDED_TO_DB);
    emit updateDB();
}

void ManualAdderDialog::on_pushButton_choose_spectral_data_clicked()
{
    using namespace sdb;
    static QString lastChoosedPath = "/home";
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Open Spectr"),
                                                 lastChoosedPath,
                                                 tr("Spectr Files(*.json *.txt)"));
    lastChoosedPath = fileName;
    bool parsing_result = true;
    QVector<double>xValues;
    QVector<double>yValues;
    QString sanitized_raw_data;
    if(!QFile(fileName).exists()||fileName.isEmpty())return;
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {

        if(line.contains(",")){
            line.replace(",",".");
            //db_errors::showWarning(db_errors::WARNINGS::COMMA_DECIMAL);
        }  //унификация формата

        if(line.contains("\t")){
            if(line[line.length()-1]=='\t'){
                line.replace(line.length()-2,'\t',"");
            }// it's really strange situation but it was created by someone
            line.replace("\t"," ");

            //db_errors::showWarning(db_errors::WARNINGS::TAB_SEPARATOR);
        }//унификация формата
        if(line.isEmpty()){
            showError(ERRORS::LINE_IS_EMPTY);
            break;
        }
        if(!line.contains(" ")){
            showError(ERRORS::NO_SPACE_IN_LINE_ERROR);
            parsing_result = false;
            break;
        }else{ //для множественных пробелов
            QStringList spaces = line.split(" ");
            line = spaces.first() + " " + spaces.last();

        }

        if(line.isEmpty()){
            parsing_result = false;
            showError(ERRORS::NO_SPACE_IN_LINE_ERROR);
            break;
        }
        QStringList elements = line.split(" ");
        if(elements.size()<2){
            parsing_result = false;
            showError(ERRORS::NO_2_ELEMNTS_ERROR);
            break;
        }

        double xValue = elements.at(0).toDouble(&parsing_result);
        double yValue = elements.at(1).toDouble(&parsing_result);
        line.append("\n");
        qDebug()<<line;
        qDebug()<<"xValue: "<<xValue
               <<"yValue: "<<yValue
              <<"convertion to double: "<<parsing_result;
        xValues.push_back(xValue);
        yValues.push_back(yValue);
        sanitized_raw_data.append(line);
        line = in.readLine();
    }

    if(parsing_result){
        ps::showPlot(plot,
                     xValues,
                     yValues,
                     ui->comboBox_spectral_data_type->currentText());
    }
    SpectrForDB spectr_for_db;
    spectr_for_db.data_type = ui->comboBox_spectral_data_type->currentText();
    spectr_for_db.description = ui->lineEdit_spectral_data_comments->text();
    spectr_for_db.instrument = ui->comboBox_instrument_type->currentText();
    spectr_for_db.waves = yValues;
    spectr_for_db.values = xValues;
    spectr_for_db.raw_data = sanitized_raw_data.toLatin1();
    m_spectrs_for_db.insert(fileName,spectr_for_db);
    ui->lineEdit_spectral_data_comments->clear();
    file.close();
    auto rc = ui->tableWidget_spectral_data->model()->rowCount();
    ui->tableWidget_spectral_data->insertRow(rc);
    ui->tableWidget_spectral_data->setItem(rc,0,new QTableWidgetItem(fileName));
    ui->tableWidget_spectral_data->setItem(rc,1,new QTableWidgetItem(spectr_for_db.instrument));
    ui->tableWidget_spectral_data->setItem(rc,2,new QTableWidgetItem(spectr_for_db.data_type));
    ui->tableWidget_spectral_data->setItem(rc,3,new QTableWidgetItem(spectr_for_db.description));

}

void ManualAdderDialog::on_tableWidget_images_itemClicked(QTableWidgetItem *item)
{

    QString path = ui->tableWidget_images->model()->index(item->row(),0).data().toString();
    ui->label_add_image->setPixmap(QPixmap::fromImage(QImage(path)));
}


void ManualAdderDialog::updateGeneralTypes()
{
    auto qS = "SELECT type_name FROM general_type";
    insertValuesToCombobox(qS,ui->comboBox_general_type);
}

void ManualAdderDialog::updateRegions()
{
    auto qS = "SELECT region_name FROM regions";
    insertValuesToCombobox(qS,ui->comboBox_region_name);
}

void ManualAdderDialog::updatePlaceTypes()
{
    auto qS = "SELECT place_type_name FROM place_types";
    insertValuesToCombobox(qS,ui->comboBox_place_type);
}

void ManualAdderDialog::updateOwners()
{
    auto qS = "SELECT name FROM owner";
    insertValuesToCombobox(qS,ui->comboBox_owners);
}

void ManualAdderDialog::updateExperiments()
{
    auto qS = "SELECT name FROM experiment";
    insertValuesToCombobox(qS,ui->comboBox_experiment_name);
}

void ManualAdderDialog::updateCaptureTypes()
{
    auto qS = "SELECT name FROM capture_type";
    insertValuesToCombobox(qS,ui->comboBox_capture_type);
}

void ManualAdderDialog::updateInstruments()
{
    auto qS = "SELECT name FROM instrument_type";
    insertValuesToCombobox(qS,ui->comboBox_instrument_type);
}

void ManualAdderDialog::updateSpectralDataTypes()
{
    auto qS = "SELECT name FROM spectral_data_types";
    insertValuesToCombobox(qS,ui->comboBox_spectral_data_type);
}

void ManualAdderDialog::updateImageTypes()
{
    auto qS = "SELECT type_name FROM image_types";
    insertValuesToCombobox(qS,ui->comboBox_image_types);
}


void ManualAdderDialog::on_tableWidget_spectral_data_itemClicked(QTableWidgetItem *item)
{
    QString path = ui->tableWidget_spectral_data->model()->index(item->row(),0).data().toString();
    auto selected_spectr = m_spectrs_for_db.value(path);
    ps::showPlot(plot,
                 selected_spectr.values,
                 selected_spectr.waves,
                 ui->comboBox_spectral_data_type->currentText());
}
