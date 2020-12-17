#include "exception.h"

Exception::Exception(int err)
{
    error_number = err;
}
void Exception::show()
{
    QString msg;
    switch (error_number)
    {
        case 1:
            msg = "Обраний проміжок часу зайнято";
            break;
        case 2:
            msg = "Подію не знайдено";
            break;
        case 3:
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
    }
    ErrorDialog errDialog(msg);
    errDialog.exec();
}
