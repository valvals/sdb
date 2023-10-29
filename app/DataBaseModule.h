#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include <QObject>
#include <QtSql>
#include <QtWidgets>


class DataBaseModule : public QObject
{
    Q_OBJECT

public:
    DataBaseModule(QTableView *tableView);

    ~DataBaseModule();

    QSqlRelationalTableModel *model() const;

signals:
    void testDBresult(QString);

private:
    QSqlDatabase m_db;
    QSqlRelationalTableModel *m_model;
};

#endif // DATABASEMODULE_H
