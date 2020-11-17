#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <vector>
#include <QtSql/QSqlDatabase>
#include "event.h"
#include "db_data.h"

class DBManager
{
private:
    QSqlDatabase m_db;

public:
    DBManager();
    ~DBManager(); //todo: delete m_db_manager; m_db_manager = nullptr; QSqlDatabase::removeDatabase( "tasklist" );

    vector <PriorityData> GetPriorityList( void );
    vector <TypeData> GetTypeTask( void );

    vector <Study> GetStudy( QDateTime date_start, QDateTime date_end);
    vector <Task> GetTask( QDateTime date_start, QDateTime date_end);
    vector <Meet> GetMeet( QDateTime date_start, QDateTime date_end);
    vector <Birthday> GetBirthday( QDateTime date_start, QDateTime date_end);

    void AddStudy( Study& item );
    void AddTask( Task& item );
    void AddMeet( Meet& item );
    void AddBirthday( Birthday& item );

    void DeleteData( vector <quint64> list_id );
    void RestoreData( vector <quint64> list_id );

    void ModifyStudy( Study& item );
    void ModifyTask( Task& item );
    void ModifyMeet( Meet& item );
    void ModifyBirthday( Birthday& item );

    void ActivateTask( Task& item );
    void DeactivateTask( Task&item );

//    void ExportData_CSV( const QString& file_name );
//    void ExportData_JSON( const QString& file_name );
};

#endif // DBMANAGER_H
