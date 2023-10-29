#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <DataBaseModule.h>
#include "../libs/DBJson.h"
#include "MapPointer.h"
#include "QCustomPlot.h"
#include "ManualAdderDialog.h"
#include "BaseValuesAdderDialog.h"

class QImage;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void itemSelectionChanged(QItemSelection selected,
                              QItemSelection deselected);
    void on_pushButton_imageLeft_clicked();
    void on_pushButton_imageRight_clicked();
    void on_pushButton_spectr_current_Left_clicked();
    void on_pushButton_spectr_current_Right_clicked();   
    void copyCoordinatesToClipboard();
    void on_pushButton_find_clicked();
    void refreshTable();
    void copySpectrToClipboard();
    void readSelectedData();
    void copyImageToClipboard();
    void copyJsonToClipboard();
    void openGoogleMapWithForCurrentPosition();
    void onRequestedToAddDataBasedOnSelected();


private:
    Ui::MainWindow *ui;
    DataBaseModule *dbm;
    QCustomPlot *plot;
    MapPointer *mp;
    ManualAdderDialog *manual_adder_dialog;
    BaseValuesAdderDialog *base_values_adder_dialog;
    QVector<QImage> images;
    quint16 image_current_index;
    db_json::SPECTRAL_STRUCT m_spectral_struct;
    int spectr_current_index;
    QString google_coordinates;
    void translateEnglishColumnsToRussian();
    void hideUnusedColumns();
    void showCurrentImageIndex();
    void showCurrentSpectrIndex();
    void fillComboBoxesFilters();
    QString ReplaceTextifUndefined(const double &param);

    // QWidget interfaces
protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
