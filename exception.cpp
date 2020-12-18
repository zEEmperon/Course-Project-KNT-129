#include "exception.h"
#include <QCoreApplication>

Exception::Exception(int err)
{
    error_number = err;
}
void Exception::show()
{
    QString msg;
    bool needed_close = false;
    switch (error_number)
    {
        case 1:
            msg = "Обраний проміжок часу зайнято";
            break;
        case 2:
            msg = "Подію не знайдено";
            break;
        case 3:
            needed_close = true;
            msg = "Вихід за границю масиву";
            break;
        case 4:
            msg = "Не всі поля заповнені";
            break;
        case 5:
            msg = "Час вказаний некоректно";
            break;
        case 6:
            msg = "Кінцевий термін сдачі недійсний";
            break;
        case 7:
            msg = "Задача вже активована";
            break;
        case 8:
            msg = "Задача ще не активована";
            break;
        case 9:
            needed_close = true;
            msg = "Відкриття бази даних";
            break;
        case 10:
            needed_close = true;
            msg = "Витягання пріоритетів з БД";
            break;
        case 11:
            needed_close = true;
            msg = "Витягання типів подій з БД";
            break;
        case 12:
            needed_close = true;
            msg = "Витягання інформації з БД";
            break;
        case 13:
            msg = "Додавання інформації до БД";
            break;
        case 14:
            msg = "Видалення інформації з БД";
            break;
        case 15:
            msg = "Змінення інформації у БД";
            break;
        case 16:
            msg = "Активація задачі у БД";
            break;
        case 17:
            msg = "Деактивація задачі у БД";
            break;
        case 18:
            msg = "Експорт у файл";
            break;
        case 19:
            msg = "Помилка відкриття файлу";
            break;
    }
    ErrorDialog errDialog(msg);
    errDialog.exec();
    if( needed_close )
        QCoreApplication::exit( error_number );
}
