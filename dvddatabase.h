#ifndef DVDDATABASE_H
#define DVDDATABASE_H

#include <QObject>
#include "common.h"

class DvdDatabase : public QObject
{
    Q_OBJECT
public:
    explicit DvdDatabase(QObject *parent = 0);
    ~DvdDatabase();

signals:
    void sendTableToDisplay(QString **displayTable,int numberOfItemsToDisplay);
    void sendMsgToUsr(QString message);
    void saveLastDatabaseFileName(QString fileName);

public slots:
    void saveDataFile();
    bool readDataFile();
    void setConfigOfDatabase(databaseConfig config);
    void displayStringFilteredDatabase(int column,QString textFromUser);
    void displayDateFilteredDatabase(int searchDaysToEndFromUser);
    void displayAllDatabase();
    void deleteRow(int rowOnDisplayTable);
    void editRowOfDatabase(int rowOnDisplayTable, QString *dataRow);

private:
    enum lastDisplayDatabaseType{displayAll=0,displayStringFiltered=10,displayDateFiltered=20};
    lastDisplayDatabaseType lastDisplay;
    QString lastTextFromUser;
    int lastColumnFormUser;
    int lastSearchDateFromUser;
    int howManyColumns;
    int numberOfDateColumn;
    QString **databaseTable;
    QString **displayTable;
    int numberOfItemsInDatabaseTable;
    int sizeOfDatabaseTable;
    bool init();
    void extendTable();
    void displayInLastType();
    void setDisplayStringLastType(int column, QString textFromUser);
    void setDisplayDateLastType(int searchDaysToEndFromUser);
    void setDisplayAllLastType();
    int rowInDisplayTableToRowInDatabase(int rowOnDisplayTable);
    bool isTableToFilter;
    QString dataFile;
};

#endif // DVDDATABASE_H
