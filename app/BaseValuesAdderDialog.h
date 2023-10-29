#ifndef BASEVALUESADDERDIALOG_H
#define BASEVALUESADDERDIALOG_H

#include <QWidget>

namespace Ui {
class BaseValuesAdderDialog;
}

class BaseValuesAdderDialog : public QWidget
{
    Q_OBJECT

public:
    explicit BaseValuesAdderDialog(QWidget *parent = nullptr);
    ~BaseValuesAdderDialog();

signals:
    void updateDB();
    void regionsChanged();
    void generalTypeChanged();
    void placeTypeNameChanged();
    void experimentChanged();
    void captureTypeChanged();
    void instrumentChanged();
    void ownerChanged();
    void spectralDataTypeChanged();

private slots:
    void on_stackedWidget_currentChanged(int arg1);
    void on_pushButton_back_clicked();
    void on_pushButton_forward_clicked();
    void on_pushButton_add_new_object_clicked();

private:
    Ui::BaseValuesAdderDialog *ui;
    QStringList classificationTableFields;
    QStringList locationTableFields;
    QStringList measurementTableFields;
    void getNextIndex(int &index);
    QString getTableAndFieldCreationTree();
    void updateGeneralTypes();
    void updateClasses();
    void updateRegions();
    void updatePlaceTypes();
    void updateGeographicNames();
    void updateAllObjects();
};

#endif // BASEVALUESADDERDIALOG_H
