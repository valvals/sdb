#ifndef MANUALADDERDIALOG_H
#define MANUALADDERDIALOG_H

#include <QWidget>
#include <QCustomPlot.h>
#include "../libs/DBJson.h"

class QComboBox;
class QListWidgetItem;
class QListWidget;



namespace Ui {
class ManualAdderDialog;
}

class ManualAdderDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ManualAdderDialog(QWidget *parent = nullptr);
    ~ManualAdderDialog();
    void fillDataBasedOnSelectedMeasurements(const db_json::SPECTRAL_STRUCT &spec_str);

private slots:
    void on_comboBox_general_type_currentIndexChanged(const QString &arg1);
    void on_comboBox_class_currentIndexChanged(const QString &arg1);
    void on_comboBox_region_name_currentIndexChanged(const QString &arg1);
    void on_comboBox_geographic_name_currentIndexChanged(const QString &arg1);
    void on_comboBox_place_type_currentIndexChanged(const QString &arg1);
    void on_pushButton_choose_image_button_clicked();
    void on_pushButton_add_data_to_DB_clicked();
    void on_pushButton_choose_spectral_data_clicked();
    void on_tableWidget_spectral_data_itemClicked(QTableWidgetItem *item);
    void on_tableWidget_images_itemClicked(QTableWidgetItem *item);

    void deleteImageFromTable();
    void deleteSpectrFromTable();

    void updateGeneralTypes();
    void updateRegions();
    void updatePlaceTypes();
    void updateOwners();
    void updateExperiments();
    void updateCaptureTypes();
    void updateInstruments();
    void updateSpectralDataTypes();
    void updateImageTypes();


signals:
    void updateDB();

private:
    Ui::ManualAdderDialog *ui;
    QCustomPlot *plot;
    struct SpectrForDB{
        QVector<double>waves;
        QVector<double>values;
        QByteArray raw_data;
        QString instrument;
        QString data_type;
        QString description;
    };
    struct ImageForDB{
        QByteArray imageData;
        QString type;
        QString description;
    };
    QHash <QString,ImageForDB>  m_images_for_DB;
    QHash <QString,SpectrForDB> m_spectrs_for_db;

    void updateComboboxes();

    inline void deleteSelectedItemsFromList(QTableWidget *widget);
    inline bool isUniqueFieldExists(const QString &table_name,
                                    const QString &column_name,
                                    const QString &key_name);

};

#endif // MANUALADDERDIALOG_H
