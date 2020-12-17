#include "dbmanager.h"
#include <QDebug>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextCodec>
#include <stdexcept>

//----------------------------------------------------------------------------
DBManager::DBManager()
{
    qDebug() << __func__ << " +";

    QString fname = QDir::currentPath() + "\\db\\taskdb.db";

    m_db = QSqlDatabase::addDatabase( "QSQLITE", "tasklist" );
    m_db.setDatabaseName( fname );

    //можешь, пожалуйста, все мои throw std::runtime_error(...) тоже в catch отправлять?

    //тут try {m_db.open();}, if - catch
        //відкриття бази даних

    if( !m_db.open() )
    {
            throw std::runtime_error( QString("DB \"%1\" openning error!").arg( fname ).toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
DBManager::~DBManager()
{
    qDebug() << __func__ << " +";

    if( m_db.isOpen() )
        m_db.close();

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
vector <PriorityData> DBManager::GetPriorityList( void )
{
    qDebug() << __func__ << " +";

    vector <PriorityData> result;

    QString str = "select id, weight, name from Priority order by weight asc";

    QSqlQuery query( m_db );

    //тут if - try, где else - catch
        //відкриття бази даних: витягання пріоритетів задач

    if( query.exec( str ) )
    {
        PriorityData item;
        while( query.next() )
        {
            item.id = query.value(0).toULongLong();
            item.weight = query.value(1).toUInt();
            item.name = QString::fromUtf8( query.value(2).toByteArray().data() );
            result.push_back( item );
        }
    }
    else
        throw std::runtime_error( QString("Select from priority error!").toStdString() );

    qDebug() << __func__ << " -";

    return result;
}

//----------------------------------------------------------------------------
vector <TypeData> DBManager::GetTypeTask( void )
{
    qDebug() << __func__ << " +";

    vector <TypeData> result;

    QString str = "select id, name from Type_task order by id asc";

    QSqlQuery query( m_db );

    //тут if - try, где else - catch
        //відкриття бази даних: витягання типів подій

    if( query.exec( str ) )
    {
        TypeData item;
        while( query.next() )
        {
            item.id = query.value(0).toULongLong();
            item.name = QString::fromUtf8( query.value(1).toByteArray().data() );
            result.push_back( item );
        }
    }
    else
        throw std::runtime_error( QString("Select from type task error!").toStdString() );

    qDebug() << __func__ << " -";

    return result;
}

//----------------------------------------------------------------------------
vector <Study> DBManager::GetStudy( QDateTime date_start, QDateTime date_end)
{
    qDebug() << __func__ << " +";

    vector <Study> result;
    QString str = QString("select Task_list.ID, Task_list.Type_task, Type_task.Name as type_name,"
                          "Task_list.Date_start, Task_list.Date_end, Task_list.Name, Task_list.Place"
                          "  from Task_list, Type_task"
                          " where Task_list.Type_task = Type_task.ID"
                          "   and Date_start >= %1 and Date_end <= %2"
                          "   and Task_list.Type_task = %3"
                          "   and Deleted = 0"
                          " order by Date_start asc").
                          arg( date_start.toTime_t() ).arg( date_end.toTime_t() ).arg( EVENT_STUDY );

    QSqlQuery query( m_db );

    //тут if - try, если нет - catch
        //відкриття бази даних: витягання подій "Навчання"

    if( query.exec( str ) )
    {
        while( query.next() )
        {
            Study item( QDateTime::fromTime_t( query.value(3).toULongLong() ),
                        QTime( QDateTime::fromTime_t( query.value(3).toULongLong() ).time() ),
                        QTime( QDateTime::fromTime_t( query.value(4).toULongLong() ).time() ),
                        QString::fromUtf8( query.value(5).toByteArray().data() ),
                        QString::fromUtf8( query.value(6).toByteArray().data() ),
                        query.value(1).toULongLong() );
            item.SetID( query.value(0).toULongLong() );
            result.push_back( item );
        }
    }

    qDebug() << __func__ << " -";
    return result;
}

//----------------------------------------------------------------------------
vector <Task> DBManager::GetTask( QDateTime date_start, QDateTime date_end)
{
    qDebug() << __func__ << " +";

    vector <Task> result;
    QString str = QString("select Task_list.ID, Task_list.Type_task, Type_task.Name as type_name,"
                          " Task_list.Date_start,"
                          " Task_list.Date_end, Task_list.Name, Task_list.Priority,"
                          " Task_list.Time_Notification, Task_list.Work_time,"
                          " Priority.Name as priority_name, Priority.Weight,"
                          " case when Task_list.Date_start is null then 0 else 1 end as valid_st_dt"
                          " from Task_list, Priority, Type_task"
                          " where Task_list.Priority = Priority.ID"
                          "   and Task_list.Type_task = Type_task.ID"
                          "   and Date_end >= %1 and Date_end <= %2"
                          "   and Task_list.Type_task = %3"
                          "   and Deleted = 0"
                          " order by Date_start asc, Priority.Weight desc").
                          arg( date_start.toTime_t() ).arg( date_end.toTime_t() ).arg( EVENT_TASK );

    QSqlQuery query( m_db );

    //тут if - try, если нет - catch
        //відкриття бази даних: витягання подій "Задачі"

    if( query.exec( str ) )
    {
        while( query.next() )
        {
            Task item( (query.value(11).toUInt() ? QDateTime::fromTime_t( query.value(3).toULongLong() ) : QDateTime()),
                       QDateTime::fromTime_t( query.value(4).toULongLong() ),
                       query.value(10).toULongLong(),
                       QString::fromUtf8( query.value(5).toByteArray().data() ),
                       QDateTime::fromTime_t( query.value(7).toULongLong() ),
                       query.value(1).toULongLong(), query.value(6).toULongLong() );
            item.SetID( query.value(0).toULongLong() );
            item.SetWorkTime( query.value(8).toULongLong() );

            if( item.getDate().isValid() )
                item.SetIsActive( true );
            else
                item.SetIsActive( false );

            result.push_back( item );
        }
    }

    qDebug() << __func__ << " -";
    return result;
}

//----------------------------------------------------------------------------
vector <Meet> DBManager::GetMeet( QDateTime date_start, QDateTime date_end)
{
    qDebug() << __func__ << " +";

    vector <Meet> result;
    QString str = QString("select Task_list.ID, Task_list.Type_task, Type_task.Name as type_name,"
                          "Task_list.Date_start, Task_list.Date_end, Task_list.Name,"
                          "Task_list.Time_Notification, Task_list.Place"
                          " from Task_list, Type_task"
                          " where Task_list.Type_task = Type_task.ID"
                          "   and Date_start >= %1 and Date_end <= %2"
                          "   and Task_list.Type_task = %3"
                          "   and Deleted = 0"
                          " order by Date_start asc").
                          arg( date_start.toTime_t() ).arg( date_end.toTime_t() ).arg( EVENT_MEET );

    QSqlQuery query( m_db );

    //тут if - try, если нет - catch
        //відкриття бази даних: витягання подій "Зустрічі"

    if( query.exec( str ) )
    {
        while( query.next() )
        {
            Meet item( QDateTime::fromTime_t( query.value(3).toULongLong() ),
                       QTime( QDateTime::fromTime_t( query.value(3).toULongLong() ).time() ),
                       QTime( QDateTime::fromTime_t( query.value(4).toULongLong() ).time() ),
                       QString( query.value(5).toString() ), QString( query.value(7).toString() ),
                       QDateTime::fromTime_t( query.value(6).toULongLong() ),
                       query.value(1).toULongLong() );
            item.SetID( query.value(0).toULongLong() );
            result.push_back( item );
        }
    }

    qDebug() << __func__ << " -";
    return result;
}

//----------------------------------------------------------------------------
vector <Birthday> DBManager::GetBirthday( QDateTime date_start, QDateTime date_end)
{
    qDebug() << __func__ << " +";

    vector <Birthday> result;
    QString str = QString("select Task_list.ID, Task_list.Type_task, Type_task.Name as type_name,"
                          "Task_list.Date_start, Task_list.Name, Task_list.Time_notification"
                          "  from Task_list, Type_task"
                          " where Task_list.Type_task = Type_task.ID"
                          "   and Date_start >= %1 and Date_start <= %2"
                          "   and Task_list.Type_task = %3"
                          "   and Deleted = 0"
                          " order by Date_start asc").
                          arg( date_start.toTime_t() ).arg( date_end.toTime_t() ).arg( EVENT_BIRTHDAY );

    QSqlQuery query( m_db );

    //тут if - try, если нет - catch
        //відкриття бази даних: витягання подій "Дні народження"

    if( query.exec( str ) )
    {
        while( query.next() )
        {
            Birthday item( QDateTime::fromTime_t( query.value(3).toULongLong() ),
                           QString::fromUtf8( query.value(4).toByteArray().data() ),
                           QDateTime::fromTime_t( query.value(5).toULongLong() ),
                           query.value(1).toULongLong() );
            item.SetID( query.value(0).toULongLong() );
            result.push_back( item );
        }
    }

    qDebug() << __func__ << " -";
    return result;
}

//----------------------------------------------------------------------------
void DBManager::AddStudy( Study& item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "insert into Task_list"
                           " (Type_task, Date_start, Date_end, Name, Place)"
                           " values (%1, %2, %3, \"%4\", \"%5\")" ).arg( EVENT_STUDY ).
                           arg( QDateTime( item.getDate().date(), item.getTimeBeg() ).toTime_t() ).
                           arg( QDateTime( item.getDate().date(), item.getTimeEnd() ).toTime_t() ).
                           arg( item.getName() ).arg( item.getPlace() );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: додавання події "Навчання"

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("AddStudy error!").toStdString() );
    }

    item.SetID( query.lastInsertId().toULongLong() );

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::AddTask( Task& item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );

    QString str = QString( "insert into Task_list"
                           " (Type_task, Date_start, Date_end, Name, Priority, Time_notification, Work_time)"
                           " values (%1, %2, %3, \"%4\", %5, %6, %7)" ).arg( EVENT_TASK ).
                           arg( item.getDate().isValid() ? QString::number( item.getDate().toTime_t() ) : "NULL" ).
                           arg( item.getTimeDeadline().toTime_t() ).
                           arg( item.getName() ).arg( item.GetPriorityID() ).
                           arg( item.getTimeNotific().toTime_t() ).arg( item.getWorkTime() );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: додавання події "Задачі"

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("AddTask error!").toStdString() );
    }

    item.SetID( query.lastInsertId().toULongLong() );

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::AddMeet( Meet& item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "insert into Task_list"
                           " (Type_task, Date_start, Date_end, Name, Place, Time_notification)"
                           " values (%1, %2, %3, \"%4\", \"%5\", %6)" ).arg( EVENT_MEET ).
                           arg( QDateTime( item.getDate().date(), item.getTimeBeg()).toTime_t() ).
                           arg( QDateTime( item.getDate().date(), item.getTimeEnd() ).toTime_t() ).
                           arg( item.getName() ).arg( item.getPlace() ).arg( item.getTimeNotific().toTime_t() );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: додавання події "Зустрічі"

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("AddMeet error!").toStdString() );
    }

    item.SetID( query.lastInsertId().toULongLong() );

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::AddBirthday( Birthday& item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "insert into Task_list"
                           " (Type_task, Date_start, Name, Time_notification)"
                           " values (%1, %2, \"%3\", %4)" ).arg( EVENT_BIRTHDAY ).
                           arg( item.getDate().toTime_t() ).arg( item.getName() ).
                           arg( item.getTimeNotific().toTime_t() );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: додавання події "Дні народження"

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("AddBirthday error!").toStdString() );
    }

    item.SetID( query.lastInsertId().toULongLong() );

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::DeleteData( vector <quint64> list_id )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString list_id_str;
    for( auto it = list_id.begin(); it != list_id.end(); ++it )
    {
        list_id_str += QString( "%1%2" ).arg( !list_id_str.isEmpty() ? ", " : "" ).arg( *it );
    }
    QString str = QString( "update Task_list"
                           "   set Deleted = 1"
                           " where Task_list.ID in (%1)" ).arg( list_id_str );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: видалення події

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("Delete error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::RestoreData( vector <quint64> list_id )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString list_id_str;
    for( auto it = list_id.begin(); it != list_id.end(); ++it )
    {
        list_id_str += QString( "%1%2" ).arg( !list_id_str.isEmpty() ? ", " : "" ).arg( *it );
    }
    QString str = QString( "update Task_list"
                           "   set Deleted = 0"
                           " where Task_list.ID in (%1)" ).arg( list_id_str );

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("Restore error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::ModifyStudy( Study& item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "update Task_list"
                           "   set Date_start = %1,"
                           "       Date_end = %2,"
                           "       Name = %3,"
                           "       Place = %4"
                           " where Task_list.ID in (%5)" ).
                           arg( QDateTime( item.getDate().date(), item.getTimeBeg() ).toTime_t() ).
                           arg( QDateTime( item.getDate().date(), item.getTimeEnd() ).toTime_t() ).
                           arg( item.getName() ).arg( item.getPlace() ).arg( item.GetID() );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: змінення події "Навчання"

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("ModifyStudy error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::ModifyTask( Task& item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "update Task_list"
                           "   set Date_end = %2,"
                           "       Name = \"%3\","
                           "       Priority = %4,"
                           "       Time_notification = %5"
                           " where Task_list.ID in (%6)" ).
                           arg( item.getTimeDeadline().toTime_t() ).
                           arg( item.getName() ).arg( item.GetPriorityID() ).
                           arg( item.getTimeNotific().toTime_t() ).arg( item.GetID() );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: змінення події "Задачі"

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("ModifyTask error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::ModifyMeet( Meet& item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "update Task_list"
                           "   set Date_start = %1,"
                           "       Date_end = %2,"
                           "       Name = \"%3\","
                           "       Place = \"%4\","
                           "       Time_notification = %5"
                           " where Task_list.ID in (%6)" ).
                           arg( QDateTime( item.getDate().date(), item.getTimeBeg()).toTime_t() ).
                           arg( QDateTime( item.getDate().date(), item.getTimeEnd() ).toTime_t() ).
                           arg( item.getName() ).arg( item.getPlace() ).
                           arg( item.getTimeNotific().toTime_t() ).arg( item.GetID());

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: змінення події "Зустрічі"

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("ModifyMeet error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::ModifyBirthday( Birthday& item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "update Task_list"
                           "   set Date_start = %1,"
                           "       Name = \"%2\","
                           "       Time_notification = %3"
                           " where Task_list.ID in (%4)" ).
                           arg( item.getDate().toTime_t() ).arg( item.getName() ).
                           arg( item.getTimeNotific().toTime_t() ).arg( item.GetID());

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: змінення події "Дні народження"

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("ModifyBirthday error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::ActivateTask( Task &item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "update Task_list"
                           "   set Date_start = %1"
                           " where Task_list.ID in (%2)" ).
                           arg( item.getDate().toTime_t() ).arg( item.GetID());

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: активація задачі

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("ActivateTask error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::DeactivateTask( Task &item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "update Task_list"
                           "   set Date_start = NULL,"
                           "       Work_time = %1"
                           " where Task_list.ID in (%2)" ).
                           arg( item.getWorkTime() ).arg( item.GetID());

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: дезактивація задачі

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("DeactivateTask error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
vector <Lessons> DBManager::GetLessons( vector <Lessons>& lessons )
{
    qDebug() << __func__ << " +";

    QString str = QString("select ID, Name, Week_day, Lesson_number from Schedule order by ID asc");

    vector <Lessons> result;
    QSqlQuery query( m_db );

    //тут if - try, если нет - catch
        //відкриття бази даних: витягання "Розклад занять"

    if( query.exec( str ) )
    {
        Lessons item;
        while( query.next() )
        {
            item.id = query.value(0).toULongLong();
            item.name = QString::fromUtf8( query.value(1).toByteArray().data() );
            item.week_day = query.value(2).toUInt();
            item.lesson_number = query.value(3).toUInt();
            result.push_back( item );
        }
    }
    else
        throw std::runtime_error( QString("Select from lessons error!").toStdString() );

    qDebug() << __func__ << " -";

    lessons.resize(result.size());
    for (int i=0; i<int(result.size()); i++)
    {
        lessons[i].id = result[i].id;
        lessons[i].name = result[i].name;
        lessons[i].week_day = result[i].week_day;
        lessons[i].lesson_number = result[i].lesson_number;
    }

    return result;
}

//----------------------------------------------------------------------------
vector <Hometask> DBManager::GetHometask( QDate day )
{
    qDebug() << __func__ << " +";

    QString str = QString("select ID, Name, Delivery_day, Completed"
                          "  from Hometask"
                          " where Delivery_day = %1"
                          " order by ID asc").arg( QDateTime( day ).toSecsSinceEpoch() );

    vector <Hometask> result;
    QSqlQuery query( m_db );

    //тут if - try, если нет - catch
        //відкриття бази даних: витягання "Домашнє завдання"

    if( query.exec( str ) )
    {
        Hometask item;
        while( query.next() )
        {
            item.id = query.value(0).toULongLong();
            item.name = QString::fromUtf8( query.value(1).toByteArray().data() );
            item.delivery_day = QDateTime::fromSecsSinceEpoch( query.value(2).toULongLong() ).date();
            query.value(3).toUInt() ? item.completed = true : item.completed = false;
            result.push_back( item );
        }
    }
    else
        throw std::runtime_error( QString("Select from hometask error!").toStdString() );

    qDebug() << __func__ << " -";

    return result;
}

//----------------------------------------------------------------------------
void DBManager::AddLesson( Lessons &item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "insert into Schedule"
                           " (Name, Week_day, Lesson_number)"
                           " values (\"%1\", %2, %3)" ).
                           arg( item.name ).arg( item.week_day ).
                           arg( item.lesson_number );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: додавання заняття до розкладу

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("AddLesson error!").toStdString() );
    }

    item.id = query.lastInsertId().toULongLong();

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::AddHometask( Hometask &item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "insert into Hometask"
                           " (Name, Delivery_day, Completed)"
                           " values (\"%1\", %2, %3)" ).
                           arg( item.name ).arg( QDateTime( item.delivery_day ).toSecsSinceEpoch() ).
                           arg( item.completed ? 1 : 0 );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: додавання домашнього завдання

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("AddHometask error!").toStdString() );
    }

    item.id = query.lastInsertId().toULongLong();

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::ModifyLesson( Lessons &item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "update Schedule"
                           "   set Name = \"%1\","
                           "       Week_day = %2,"
                           "       Lesson_number = %3"
                           " where ID = %4" ).
                           arg( item.name ).arg( item.week_day ).
                           arg( item.lesson_number ).arg( item.id );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: змінення заняття у розкладі

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("ModifyLesson error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::ModifyHometask( Hometask &item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "update Hometask"
                           "   set Name = \"%1\","
                           "       Delivery_day = %2,"
                           "       Completed = %3"
                           " where ID = %4" ).
                           arg( item.name ).arg( QDateTime( item.delivery_day ).toSecsSinceEpoch() ).
                           arg( item.completed ? 1 : 0 ).arg( item.id );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: змінення домашнього завдання

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("ModifyHometask error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::DeleteLesson( Lessons &item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "delete from Schedule"
                           " where ID = %1" ).arg( item.id );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: видалення заняття з розкладу

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("DeleteLesson error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::DeleteHometask( Hometask &item )
{
    qDebug() << __func__ << " +";

    QSqlQuery query( m_db );
    QString str = QString( "delete from Hometask"
                           " where ID = %1" ).arg( item.id );

    //тут try {query.exec( str );}, если нет - catch
        //відкриття бази даних: видалення домашнього завдання

    if( !query.exec( str ) )
    {
        throw std::runtime_error( QString("DeleteHometask error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}


//----------------------------------------------------------------------------
void DBManager::ExportData_CSV( const QString &file_name, QChar delimeter )
{
    qDebug() << __func__ << " +";

    QString str = QString("select Task_list.ID, Type_task.Name as type_name, Task_list.Name,"
                          " (CASE Task_list.Place"
                          "       WHEN Task_list.Place IS NULL THEN ''"
                          "       ELSE Task_list.Place END) as place,"
                          " (CASE Task_list.Work_time"
                          "       WHEN Task_list.Work_time IS NULL THEN 0"
                          "       ELSE Task_list.Work_time END) as worktime,"
                          " (CASE Task_list.Date_start"
                          "       WHEN Task_list.Date_start IS NULL THEN 0"
                          "       ELSE Task_list.Date_start END) as date_start,"
                          " (CASE Task_list.Date_end"
                          "       WHEN Task_list.Date_end IS NULL THEN 0"
                          "       ELSE Task_list.Date_end END) as date_end,"
                          " (CASE Task_list.Priority"
                          "       WHEN Task_list.Priority IS NULL THEN ''"
                          "       ELSE (select Priority.Name from Priority where Priority.ID=Task_list.Priority) END) as priority_name,"
                          " (CASE Task_list.Priority"
                          "       WHEN Task_list.Priority IS NULL THEN 0"
                          "       ELSE (select Priority.Weight from Priority where Priority.ID=Task_list.Priority) END) as priority_weight,"
                          " (CASE Task_list.Time_notification"
                          "       WHEN Task_list.Time_notification IS NULL THEN 0"
                          "       ELSE Task_list.Time_notification END) as time_notification"
                          "  from Task_list, Type_task"
                          " where Task_list.Type_task = Type_task.ID"
                          "   and Deleted = 0"
                          " order by Task_list.Type_task, date_start, date_end asc, priority_weight desc");

    QSqlQuery query( m_db );

    //тут if - try, где else - catch
        //відкриття бази даних: експорт у файл CSV

    if( query.exec( str ) )
    {
        QFile file( file_name );

        //тут try {file.open(QIODevice::WriteOnly | QIODevice::Text);}, если нет - catch
            //помилка відкриття файлу

        if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
                throw std::runtime_error( QString("ExportData_CSV: OpenFile error for %1!").arg( file_name ).toStdString() );

        QTextStream out( &file );

        while( query.next() )
        {
            QString item = QString( "%1%11\"%2\"%11%3%11%4%11\"%5\"%11%6%11\"%7\"%11\"%8\"%11%9%11%10" ).
                                   arg( query.value(0).toULongLong() ).
                                   arg( QString::fromUtf8( query.value(1).toByteArray().data() ) ).
                                   arg( query.value(5).toULongLong() ? QDateTime::fromTime_t( query.value(5).toULongLong() ).toString("yyyy.MM.dd hh:mm:ss")
                                                                     : "NULL" ).
                                   arg( query.value(6).toULongLong() ? QDateTime::fromTime_t( query.value(6).toULongLong() ).toString("yyyy.MM.dd hh:mm:ss")
                                                                     : "NULL" ).
                                   arg( query.value(2).toString() ).
                                   arg( query.value(8).toULongLong() ).
                                   arg( query.value(7).toString() ).
                                   arg( query.value(3).toString() ).
                                   arg( query.value(9).toULongLong() ? QDateTime::fromTime_t( query.value(9).toULongLong() ).toString("yyyy.MM.dd hh:mm:ss")
                                                                     : "NULL" ).
                                   arg( query.value(4).toULongLong() ).
                                   arg( delimeter );

            out << item << '\n';
        }
        file.close();
    }
    else
        throw std::runtime_error( QString("ExportData_CSV error!").toStdString() );

    qDebug() << __func__ << " -";
}

//----------------------------------------------------------------------------
void DBManager::ExportData_JSON( const QString& file_name )
{
    qDebug() << __func__ << " +";

    QString str = QString("select Task_list.ID, Type_task.Name as type_name, Task_list.Name,"
                          " (CASE Task_list.Place"
                          "       WHEN Task_list.Place IS NULL THEN ''"
                          "       ELSE Task_list.Place END) as place,"
                          " (CASE Task_list.Work_time"
                          "       WHEN Task_list.Work_time IS NULL THEN 0"
                          "       ELSE Task_list.Work_time END) as worktime,"
                          " (CASE Task_list.Date_start"
                          "       WHEN Task_list.Date_start IS NULL THEN 0"
                          "       ELSE Task_list.Date_start END) as date_start,"
                          " (CASE Task_list.Date_end"
                          "       WHEN Task_list.Date_end IS NULL THEN 0"
                          "       ELSE Task_list.Date_end END) as date_end,"
                          " (CASE Task_list.Priority"
                          "       WHEN Task_list.Priority IS NULL THEN ''"
                          "       ELSE (select Priority.Name from Priority where Priority.ID=Task_list.Priority) END) as priority_name,"
                          " (CASE Task_list.Priority"
                          "       WHEN Task_list.Priority IS NULL THEN 0"
                          "       ELSE (select Priority.Weight from Priority where Priority.ID=Task_list.Priority) END) as priority_weight,"
                          " (CASE Task_list.Time_notification"
                          "       WHEN Task_list.Time_notification IS NULL THEN 0"
                          "       ELSE Task_list.Time_notification END) as time_notification"
                          "  from Task_list, Type_task"
                          " where Task_list.Type_task = Type_task.ID"
                          "   and Deleted = 0"
                          " order by Task_list.Type_task, date_start, date_end asc, priority_weight desc");

    QSqlQuery query( m_db );

    //тут if - try, где else - catch
        //відкриття бази даних: експорт у файл JSON

    if( query.exec( str ) )
    {
        QFile file( file_name );

        //тут try {file.open(QIODevice::WriteOnly | QIODevice::Text);}, если нет - catch
            //помилка відкриття файлу

        if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
                throw std::runtime_error( QString("ExportData_JSON: OpenFile error for %1!").arg( file_name ).toStdString() );

        QTextStream out( &file );

        out << "{\n    \"export_data\":\n                 [";
        bool first = true;

        while( query.next() )
        {
            QString item = QString( "%1                     { \"id\": %2, \"type\": \"%3\", "
                                    "\"name\": \"%4\", \"date_start\": \"%5\", \"date_end\": \"%6\", "
                                    "\"priority_weight\": %7, \"priority_name\": \"%8\", "
                                    "\"place\": \"%9\", \"time_notification\": \"%10\", \"work_time\": %11 }" ).
                                   arg( first ? "\n" : ",\n" ).arg( query.value(0).toULongLong() ).
                                   arg( QString::fromUtf8( query.value(1).toByteArray().data() ) ).
                                   arg( query.value(2).toString() ).
                                   arg( query.value(5).toULongLong() ? QDateTime::fromTime_t( query.value(5).toULongLong() ).toString("yyyy.MM.dd hh:mm:ss")
                                                                     : "NULL" ).
                                   arg( query.value(6).toULongLong() ? QDateTime::fromTime_t( query.value(6).toULongLong() ).toString("yyyy.MM.dd hh:mm:ss")
                                                                     : "NULL" ).
                                   arg( query.value(8).toULongLong() ).
                                   arg( query.value(7).toString() ).
                                   arg( query.value(3).toString() ).
                                   arg( query.value(9).toULongLong() ? QDateTime::fromTime_t( query.value(9).toULongLong() ).toString("yyyy.MM.dd hh:mm:ss")
                                                                     : "NULL" ).
                                   arg( query.value(4).toULongLong() );
            out << item;
            first = false;
        }
        out << "\n                 ]\n}";
    }
    else
    {
        throw std::runtime_error( QString("ExportData_JSON error!").toStdString() );
    }

    qDebug() << __func__ << " -";
}
