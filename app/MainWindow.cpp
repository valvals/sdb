#include "MainWindow.h"
#include "qobjectdefs.h"
#include "ui_MainWindow.h"
#include "PlotShow.h"
#include <QImage>
#include <QUrl>
#include <QClipboard>
#include <QQmlEngine>
#include <QQmlContext>
#include "QCustomPlot.h"
#include "Types.h"
#include "SqlUtility.h"
#include "../libs/DBJson.h"
#include "google_maps_url_maker.h"
//#include "UselessDataCutter.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,plot(new QCustomPlot)
    ,mp(new MapPointer)

{
    ui->setupUi(this);
    spectr_current_index = -1;
    plot->setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);
    plot->addAction(ui->action_copy_spectr);
    connect(ui->action_copy_spectr,SIGNAL(triggered()),SLOT(copySpectrToClipboard()));
    ui->verticalLayout_plot_area->addWidget(plot);
    dbm = new DataBaseModule(ui->tableView);
    dbm->model()->setTable("measurements");
    dbm->model()->select();
    ui->tableView->setModel(dbm->model());
    manual_adder_dialog = new ManualAdderDialog;
    base_values_adder_dialog = new BaseValuesAdderDialog;
    connect(base_values_adder_dialog,SIGNAL(generalTypeChanged()),manual_adder_dialog,SLOT(updateGeneralTypes()));
    connect(base_values_adder_dialog,SIGNAL(regionsChanged()),manual_adder_dialog,SLOT(updateRegions()));
    connect(base_values_adder_dialog,SIGNAL(placeTypeNameChanged()),manual_adder_dialog,SLOT(updatePlaceTypes()));
    connect(base_values_adder_dialog,SIGNAL(experimentChanged()),manual_adder_dialog,SLOT(updateExperiments()));
    connect(base_values_adder_dialog,SIGNAL(ownerChanged()),manual_adder_dialog,SLOT(updateOwners()));
    connect(base_values_adder_dialog,SIGNAL(captureTypeChanged()),manual_adder_dialog,SLOT(updateCaptureTypes()));
    connect(base_values_adder_dialog,SIGNAL(instrumentChanged()),manual_adder_dialog,SLOT(updateInstruments()));
    connect(base_values_adder_dialog,SIGNAL(spectralDataTypeChanged()),manual_adder_dialog,SLOT(updateSpectralDataTypes()));

    hideUnusedColumns();
    translateEnglishColumnsToRussian();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    connect(ui->tableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(itemSelectionChanged(QItemSelection,QItemSelection)));

    ui->quickWidget_map->engine()->rootContext()->setContextProperty("tracker", mp);
    ui->quickWidget_map->setSource(QUrl("qrc:/maps/map.qml"));
    fillComboBoxesFilters();
    connect(ui->action_add_to_db,SIGNAL(triggered()),manual_adder_dialog,SLOT(show()));
    connect(ui->action_edit_base_tables,SIGNAL(triggered()),base_values_adder_dialog,SLOT(show()));
    ui->tableWidget_metadata->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(manual_adder_dialog,SIGNAL(updateDB()),SLOT(refreshTable()));
    connect(base_values_adder_dialog,SIGNAL(updateDB()),SLOT(refreshTable()));
    if(ui->tableView->model()->rowCount()>0)ui->tableView->selectRow(0);
    ui->label_Image->addAction(ui->action_copy_image);
    ui->quickWidget_map->addAction(ui->action_copy_coordinates);
    connect(ui->action_copy_coordinates,SIGNAL(triggered()),SLOT(copyCoordinatesToClipboard()));
    connect(ui->action_copy_image,SIGNAL(triggered()),SLOT(copyImageToClipboard()));
    ui->tableView->addActions(ui->menu_copy_to_buffer->actions());
    connect(ui->action_copy_JSON,SIGNAL(triggered()),SLOT(copyJsonToClipboard()));
    QAction *openGoogleMapAction = new QAction;
    openGoogleMapAction->setText("открыть google maps в браузере");
    ui->quickWidget_map->addAction(openGoogleMapAction);
    ui->tableView->addAction(openGoogleMapAction);
    connect(openGoogleMapAction,SIGNAL(triggered()),SLOT(openGoogleMapWithForCurrentPosition()));
    QAction *createNewMeasurementBasedOnTheSelected = new QAction;
    createNewMeasurementBasedOnTheSelected->setText("создать новую запись на основе текущего измерения");
    ui->tableView->addAction(createNewMeasurementBasedOnTheSelected);
    connect(createNewMeasurementBasedOnTheSelected,SIGNAL(triggered()),SLOT(onRequestedToAddDataBasedOnSelected()));
    readSelectedData();
}

MainWindow::~MainWindow()
{
    delete manual_adder_dialog;
    delete base_values_adder_dialog;
    delete dbm;
    delete mp;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(manual_adder_dialog)manual_adder_dialog->close();
    if(base_values_adder_dialog)base_values_adder_dialog->close();
    event->accept();
}

void MainWindow::itemSelectionChanged(QItemSelection selected, QItemSelection deselected)
{
    Q_UNUSED(deselected)
    Q_UNUSED(selected)
    db_json::makeStructCleared(m_spectral_struct);
    readSelectedData();
    mp->positionChanged();
}

void MainWindow::on_pushButton_imageLeft_clicked()
{
    if(image_current_index !=0){
        --image_current_index;
        ui->label_Image->setPixmap(QPixmap::fromImage(images[image_current_index]));
        showCurrentImageIndex();
    }

}

void MainWindow::on_pushButton_imageRight_clicked()
{
    if(images.size()-1>image_current_index){
        ++image_current_index;
        ui->label_Image->setPixmap(QPixmap::fromImage(images[image_current_index]));
        showCurrentImageIndex();
    }
}

void MainWindow::showCurrentImageIndex()
{
    QString currentImage = QString("%1/%2").arg(image_current_index+1).arg(images.size());
    ui->label_image_current_text->setText(currentImage);
}

void MainWindow::showCurrentSpectrIndex()
{
    auto number_of_spectrs = m_spectral_struct.sd.values.size();
    QString currentSpectr = QString("%1/%2").arg(spectr_current_index+1).arg(number_of_spectrs);
    ui->label_spectr_current_text->setText(currentSpectr);
}

void MainWindow::fillComboBoxesFilters()
{
    QSqlQuery query;
    query.exec(sdb::kQuerySelectObjectNameFromObjects);
    ui->comboBox_surface->addItem("все поверхности");
    while(query.next()){
        ui->comboBox_surface->addItem(query.value(0).toString());
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    mp->positionChanged();
    ui->quickWidget_map->update();
    ui->quickWidget_map->repaint();
}

void MainWindow::on_pushButton_spectr_current_Left_clicked()
{
    if(spectr_current_index > 0){
        --spectr_current_index;
        auto device_name = m_spectral_struct.sd.atributes[spectr_current_index].instrument;
        ui->label_deviceName->setText(device_name);
        ps::showPlot(plot,
                     m_spectral_struct.sd.waves[spectr_current_index],
                     m_spectral_struct.sd.values[spectr_current_index],
                     m_spectral_struct.sd.atributes[spectr_current_index].type);
        showCurrentSpectrIndex();
    }
}

void MainWindow::on_pushButton_spectr_current_Right_clicked()
{
    auto number_of_spectrs = m_spectral_struct.sd.values.size();
    if(number_of_spectrs-1 > spectr_current_index){
        ++ spectr_current_index;
        auto device_name = m_spectral_struct.sd.atributes[spectr_current_index].instrument;
        ui->label_deviceName->setText(device_name);
        ps::showPlot(plot,
                     m_spectral_struct.sd.waves[spectr_current_index],
                     m_spectral_struct.sd.values[spectr_current_index],
                     m_spectral_struct.sd.atributes[spectr_current_index].type);
        showCurrentSpectrIndex();
    }
}

void MainWindow::translateEnglishColumnsToRussian()
{

    for(auto &&name:sdb::root_table_names){
        dbm->model()->setHeaderData(dbm->model()->fieldIndex(name.first),
                                    Qt::Horizontal, name.second);
    }
}

void MainWindow::hideUnusedColumns()
{
    for(auto &&name:sdb::hiden_columns){
        ui->tableView->setColumnHidden(dbm->model()->fieldIndex(name),true);
    }
}

void MainWindow::copyCoordinatesToClipboard()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(google_coordinates);
}

void MainWindow::on_pushButton_find_clicked()
{

    if(ui->lineEdit_find_text->text().isEmpty()){
        dbm->model()->setFilter("");
        dbm->model()->select();
        if(ui->comboBox_surface->currentText()!="все поверхности"){
            QString filter = QString("surface = '%1'").arg(ui->comboBox_surface->currentText());
            dbm->model()->setFilter(filter);
        }
        return;
    }

    QString filter = QString("location_name LIKE '%%1%'").arg(ui->lineEdit_find_text->text());
    if(ui->comboBox_surface->currentText()!="все поверхности"){
        filter.append(QString(" AND surface = '%1'").arg(ui->comboBox_surface->currentText()));
    }
    dbm->model()->setFilter(filter);
    dbm->model()->select();
}

void MainWindow::refreshTable()
{
    dbm->model()->select();
}

void MainWindow::copySpectrToClipboard()
{
    if(-1==spectr_current_index)readSelectedData();
    QString spectrData;
    for(int i=0;i<m_spectral_struct.sd.values[spectr_current_index].size();++i){
        spectrData.append(QString::number(m_spectral_struct.sd.waves[spectr_current_index][i])
                          + "\t" +
                          QString::number(m_spectral_struct.sd.values[spectr_current_index][i])
                          + "\n");
    }
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(spectrData);
}

void MainWindow::readSelectedData()
{
    auto selected = ui->tableView->selectionModel()->selection();
    auto ii = selected.first().indexes().first();
    auto row = ii.row();
    auto iv = ii.sibling(row,0);
    auto id = dbm->model()->data(iv).toInt();
    iv = ii.sibling(row,1);
    QString date_time = dbm->model()->data(iv).toString();
    m_spectral_struct.md.date_time = date_time;
    iv = ii.sibling(row,2);
    QString owner = dbm->model()->data(iv).toString();
    m_spectral_struct.md.owner = owner;
    iv = ii.sibling(row,3);
    QString experiment = dbm->model()->data(iv).toString();
    m_spectral_struct.md.experiment_name = experiment;
    iv = ii.sibling(row,4);
    QString capture_type = dbm->model()->data(iv).toString();
    m_spectral_struct.md.capture_type = capture_type;
    iv = ii.sibling(row,5);
    double sun_el_angle = dbm->model()->data(iv).toDouble();
    m_spectral_struct.md.sun_elevation_angle = sun_el_angle;
    iv = ii.sibling(row,6);
    double capt_angle = dbm->model()->data(iv).toDouble();
    m_spectral_struct.md.capture_angle = capt_angle;
    iv = ii.sibling(row,7);
    QString locationName = dbm->model()->data(iv).toString();
    m_spectral_struct.md.location.place_name = locationName;
    iv = ii.sibling(row,8);
    QString surface = dbm->model()->data(iv).toString();
    m_spectral_struct.md.classification.object_name = surface;

    QSqlQuery query;
    QString qS = QString(sdb::kQueryImageIds).arg(id);
    query.exec(qS);
    images.clear();
    image_current_index = 0;

    while (query.next()) {
        int imgId = query.value(0).toInt();
        QString qiS = QString(sdb::kQueryImageData).arg(imgId);
        QSqlQuery q;
        q.exec(qiS);

        while (q.next()) {
            QImage img;
            db_json::IMAGE_OBJECT img_obj;
            img.loadFromData(q.value(0).toByteArray());
            img_obj.type = q.value(1).toString();
            img_obj.description = q.value(2).toString();
            images.append(img);
            m_spectral_struct.md.images.append(img_obj);
        }
    }
    if(images.size()>0){
        showCurrentImageIndex();
        ui->label_Image->setPixmap(QPixmap::fromImage(images.at(0)));
    }else{
        ui->label_Image->setPixmap(QPixmap::fromImage(QImage(":/images/no_image_available.jpg")));
    }

    QSqlQuery spectrsQuery;
    QString qSpectrs = QString(sdb::kQuerySpectrData).arg(id);
    spectrsQuery.exec(qSpectrs);

    m_spectral_struct.sd.values.clear();
    m_spectral_struct.sd.waves.clear();
    m_spectral_struct.sd.atributes.clear();

    while (spectrsQuery.next()) {
        auto spectralDataType = spectrsQuery.value(0).toString();//кся спэя альбедо
        auto instrumentType = spectrsQuery.value(2).toString();  //прибор(ы)
        auto description = spectrsQuery.value(3).toString();     // дополнительные сведения
        db_json::SPECTRAL_ATRIBUTES sa = {instrumentType,spectralDataType,description};
        qDebug()<< "Описание: " << description <<instrumentType;
        m_spectral_struct.sd.atributes.append(sa);
        QString sd = spectrsQuery.value(1).toString();
        QStringList lines = sd.split("\n");
        QPair<QVector<double>,QVector<double>> data;
        QVector<double>x;
        QVector<double>y;
        for(auto &&value:lines){
            bool res;
            value.replace("\t"," ");
            QStringList values = value.split(" ");
            if(values.size()<2)continue;
            QString str = values.at(0);
            str.replace(",",".");
            x.append(str.toDouble(&res));
            str = values.at(1);
            str.replace(",",".");
            y.append(str.toDouble(&res));
        }

        data.first = x;
        data.second = y;
        m_spectral_struct.sd.waves.append(x);
        m_spectral_struct.sd.values.append(y);

    }
    spectr_current_index = 0;
    showCurrentSpectrIndex();
    if(m_spectral_struct.sd.values.size()>0){
        if(m_spectral_struct.sd.atributes.size()>=spectr_current_index+1){
            ui->label_deviceName->setText(m_spectral_struct.sd.atributes[spectr_current_index].instrument);
        }
        ps::showPlot(plot,
                     m_spectral_struct.sd.waves[spectr_current_index],
                     m_spectral_struct.sd.values[spectr_current_index],
                     m_spectral_struct.sd.atributes[spectr_current_index].type);
    }

    QSqlQuery classificationQuery;
    QString classQuery = QString(sdb::kQueryClassification).arg(id);
    classificationQuery.exec(classQuery);
    if (classificationQuery.next()) {
        auto className = classificationQuery.value(2).toString();
        auto generalType = classificationQuery.value(3).toString();
        m_spectral_struct.md.classification.class_name = className;
        m_spectral_struct.md.classification.general_type = generalType;
    }

    auto model = ui->tableWidget_metadata->model();
    model->setData(model->index(0,0),"Генеральный тип");
    model->setData(model->index(0,1),m_spectral_struct.md.classification.general_type);
    model->setData(model->index(1,0),"Класс");
    model->setData(model->index(1,1),m_spectral_struct.md.classification.class_name);

    QSqlQuery locationQ;
    QString locationQuery = QString(sdb::kQueryLocation).arg(locationName);
    locationQ.exec(locationQuery);

    if (locationQ.next()) {
        auto locationName = locationQ.value(0).toString();
        auto placeName = locationQ.value(1).toString();
        auto region = locationQ.value(2).toString();
        auto latitude = locationQ.value(3).toDouble();
        auto longitude = locationQ.value(4).toDouble();
        auto altitude = locationQ.value(5).toDouble();
        mp->setLatitude(latitude);
        mp->setLongitude(longitude);
        m_spectral_struct.md.location.local_name = locationName;
        m_spectral_struct.md.location.place_name  = placeName;
        m_spectral_struct.md.location.region_name = region;
        m_spectral_struct.md.location.latitude = latitude;
        m_spectral_struct.md.location.longitude = longitude;
        m_spectral_struct.md.location.altitude = altitude;
        google_coordinates = QString("%1,%2").arg(latitude).arg(longitude);
    }
    QSqlQuery placesQ;
    auto QueryStr = QString(sdb::kQuerySelectTypeNameFromPlacesForPlaceName).arg(m_spectral_struct.md.location.place_name);
    placesQ.exec(QueryStr);
    if(placesQ.next()){
        auto type = placesQ.value(0).toString();
        m_spectral_struct.md.location.place_type = type;
    }
    QSqlQuery air_conditions;
    QueryStr = QString(sdb::kQuerySelectAirConditionsForMeasurementId).arg(id);
    air_conditions.exec(QueryStr);
    if(air_conditions.next()){
        auto temperature = air_conditions.value(0).toDouble();
        m_spectral_struct.md.air_conditions.temperature = temperature;
        auto humidity = air_conditions.value(1).toDouble();
        m_spectral_struct.md.air_conditions.humidity = humidity;
    }
    model->setData(model->index(2,0),"Тип места");
    model->setData(model->index(2,1),m_spectral_struct.md.location.place_type);
    model->setData(model->index(3,0),"Широта");
    model->setData(model->index(3,1),m_spectral_struct.md.location.latitude);
    model->setData(model->index(4,0),"Долгота");
    model->setData(model->index(4,1),m_spectral_struct.md.location.longitude);
    model->setData(model->index(5,0),"Высота");
    model->setData(model->index(5,1),m_spectral_struct.md.location.altitude);
    model->setData(model->index(6,0),"Угол возвышения солнца");
    model->setData(model->index(6,1),m_spectral_struct.md.sun_elevation_angle);
    model->setData(model->index(7,0),"Угол съёмки");
    model->setData(model->index(7,1),m_spectral_struct.md.capture_angle);
    model->setData(model->index(8,0),"Температура");
    model->setData(model->index(8,1),ReplaceTextifUndefined(m_spectral_struct.md.air_conditions.temperature));
    model->setData(model->index(9,0),"Влажнось");
    model->setData(model->index(9,1),ReplaceTextifUndefined(m_spectral_struct.md.air_conditions.humidity));

    if(!m_spectral_struct.sd.atributes.empty()){
    model->setData(model->index(10,0),"Прибор");// TODO change instrument for different indexes
    model->setData(model->index(10,1),m_spectral_struct.sd.atributes[0].instrument);
    model->setData(model->index(11,0),"Тип данных");// TODO change instrument for different indexes
    model->setData(model->index(11,1),m_spectral_struct.sd.atributes[0].type);
    model->setData(model->index(12,0),"доп.спек.инфо");// TODO change instrument for different indexes
    model->setData(model->index(12,1),m_spectral_struct.sd.atributes[0].description);
    model->setData(model->index(13,0),"доп.изоб.инфо");// TODO change instrument for different indexes
    if(!m_spectral_struct.md.images.empty()){
    model->setData(model->index(13,1),m_spectral_struct.md.images[0].description);
    }
    }

}

void MainWindow::copyImageToClipboard()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setImage(ui->label_Image->pixmap()->toImage());
}

void MainWindow::copyJsonToClipboard()
{
    db_json::copyStructLikeJsonToClipboard(m_spectral_struct);
}


void MainWindow::openGoogleMapWithForCurrentPosition()
{


    /*std::string command = R"(start https://www.google.com/maps/@)";//53.8514852,27.4457041,400000m/data=!3m1!1e3?entry=ttu)";
    command.append(QString::number(m_spectral_struct.md.location.latitude).toStdString());
    command.append(",");
    command.append(QString::number(m_spectral_struct.md.location.longitude).toStdString());
    command.append(",10000m/data=!3m1!1e3?entry=ttu)");
    system(command.c_str());*/

    std::string command = "start ";
    command.append(maps_utility::makeGoogleUrl(m_spectral_struct.md.location.latitude,
                                              m_spectral_struct.md.location.longitude));


    system(command.c_str());
}

void MainWindow::onRequestedToAddDataBasedOnSelected()
{
    qDebug()<<"check create new on selected...";
    if(manual_adder_dialog->window()->isHidden())manual_adder_dialog->show();
    manual_adder_dialog->fillDataBasedOnSelectedMeasurements(m_spectral_struct);
}

QString MainWindow::ReplaceTextifUndefined(const double &param)
{
    if(param == -999)return "--";
    return QString::number(param);
}
