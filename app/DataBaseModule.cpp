#include "DataBaseModule.h"
#include "MessageReporter.h"
#include <QSqlDriver>

DataBaseModule::DataBaseModule(QTableView* tableView)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("spectra_db");
    m_db.open();
    if(m_db.lastError().type() != QSqlError::NoError)
        sdb::showError(sdb::DATA_BASE_ERROR);
    m_model = new QSqlRelationalTableModel(tableView, m_db);
}

DataBaseModule::~DataBaseModule()
{
    m_db.close();
    delete m_model;
}

QSqlRelationalTableModel *DataBaseModule::model() const
{
    return m_model;
}
