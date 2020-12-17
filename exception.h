#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include "errordialog.h"
using namespace std;

class Exception
{
private:
    int error_number;
public:
    Exception(int err);
    void show();
};

#endif // EXCEPTION_H
