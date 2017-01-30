#ifndef COMMON_H
#define COMMON_H

#include <QString>

static const int NUMCOMBOBOXES=1;
static const int NUMCOLUMNSNAMES=9;
static const int INITIALROWSOFTABLE=10;
static const int INITIALCOLUMNSOFTABLE=10;

struct databaseConfig
{
    QString lastSavedFile;
    int howManyColumns;
    int whichIsDateColumn;
    QString columnNames[NUMCOLUMNSNAMES];
};

#endif // COMMON_H
