#include "dvddatabase.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>

DvdDatabase::DvdDatabase(QObject *parent) : QObject(parent)
{
    displayTable=NULL;
    databaseTable=NULL;
    init();
}

DvdDatabase::~DvdDatabase()
{
    delete databaseTable;
    delete displayTable;
}

bool DvdDatabase::readDataFile(){
    QFile file(dataFile);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        emit sendMsgToUsr("Błąd odczytu. Nie udało się otworzyć pliku do odczytu.");
        return false;
    }
    QTextStream in(&file);
    int databaseColumn=0;
    while (!in.atEnd())
    {
        QString line=in.readLine();
        if (line==";")
        {
            databaseColumn=0;
            numberOfItemsInDatabaseTable++;
            if (numberOfItemsInDatabaseTable>=sizeOfDatabaseTable-1)
                extendTable();
        }
        else if (databaseColumn>=howManyColumns)
        {
            databaseColumn++;
        }
        else
        {
            if (databaseColumn!=numberOfDateColumn-1)
                databaseTable[numberOfItemsInDatabaseTable][databaseColumn]=line;
            else
            {
                QDate dateControl;
                dateControl = dateControl.fromString(line,Qt::ISODate);
                if (dateControl.isValid())
                    databaseTable[numberOfItemsInDatabaseTable][databaseColumn]=line;
                else
                    databaseTable[numberOfItemsInDatabaseTable][databaseColumn]="";
            }
            databaseColumn++;
        }
    }
    file.close();
    return true;
}

void DvdDatabase::saveDataFile()
{
    QDate date(QDate::currentDate());
    QString fileName=date.toString(Qt::ISODate);
    fileName.append(".txt");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        emit sendMsgToUsr("Błąd zapisu. Nie udało się otworzyć pliku do zapisu.");
                return;
    }
    QTextStream out(&file);
    for (int i=0;i<numberOfItemsInDatabaseTable;i++)
    {
        for(int j=0;j<howManyColumns;j++)
        {
             out<<databaseTable[i][j]<<"\n";
        }
        out<<";"<<"\n";
    }
    emit saveLastDatabaseFileName(fileName);
}

bool DvdDatabase::init(){
    dataFile="database.txt";
    isTableToFilter=false;
    numberOfItemsInDatabaseTable=0;
    sizeOfDatabaseTable=INITIALROWSOFTABLE;
    howManyColumns=INITIALCOLUMNSOFTABLE;
    databaseTable = new QString*[INITIALROWSOFTABLE];
    displayTable = new QString*[INITIALROWSOFTABLE];
    for (int i=0;i<sizeOfDatabaseTable;i++)
    {
        databaseTable[i] = new QString[INITIALCOLUMNSOFTABLE];
        displayTable[i] = new QString[INITIALCOLUMNSOFTABLE];
    }
    return true;
}

void DvdDatabase::setConfigOfDatabase(databaseConfig config)
{
    howManyColumns=config.howManyColumns;
    dataFile=config.lastSavedFile;
    numberOfDateColumn=config.whichIsDateColumn;
    delete databaseTable;
    delete displayTable;
    numberOfItemsInDatabaseTable=0;
    sizeOfDatabaseTable=INITIALROWSOFTABLE;
    databaseTable = new QString*[INITIALROWSOFTABLE];
    displayTable = new QString*[INITIALROWSOFTABLE];
    for (int i=0;i<sizeOfDatabaseTable;i++)
    {
        databaseTable[i] = new QString[howManyColumns];
        displayTable[i] = new QString[howManyColumns];
    }
    readDataFile();
    displayAllDatabase();
}

void DvdDatabase::extendTable()
{
    QString **tempDatabaseTable;
    QString **tempDisplayTable;
    int newSizeOfDatabaseTable=sizeOfDatabaseTable*2;
    tempDatabaseTable = new QString*[newSizeOfDatabaseTable];
    tempDisplayTable = new QString*[newSizeOfDatabaseTable];
    for (int i=0;i<newSizeOfDatabaseTable;i++)
    {
        tempDatabaseTable[i]=new QString[howManyColumns];
        tempDisplayTable[i]=new QString[howManyColumns];
    }
    for (int i=0;i<numberOfItemsInDatabaseTable;i++)
    {
        for (int j=0;j<howManyColumns;j++)
        {
            tempDatabaseTable[i][j]=databaseTable[i][j];
            tempDisplayTable[i][j]=displayTable[i][j];
        }
    }
    delete databaseTable;
    delete displayTable;
    sizeOfDatabaseTable=newSizeOfDatabaseTable;
    databaseTable=tempDatabaseTable;
    displayTable=tempDisplayTable;
}

void DvdDatabase::displayStringFilteredDatabase(int column,QString textFromUser)
{
    int numberOfItemsToDisplay=0;
    for(int i=0;i<numberOfItemsInDatabaseTable;i++)
    {
        if (databaseTable[i][column].contains(textFromUser,Qt::CaseInsensitive))
        {
            for(int j=0;j<howManyColumns;j++)
            {
                displayTable[numberOfItemsToDisplay][j] = databaseTable[i][j];
            }
            numberOfItemsToDisplay++;
        }
    }
    setDisplayStringLastType(column,textFromUser);
    emit sendTableToDisplay(displayTable,numberOfItemsToDisplay);
}

void DvdDatabase::displayDateFilteredDatabase(int searchDaysToEndFromUser)
{
    QDate today = QDate::currentDate();
    int numberOfItemsToDisplay=0;
    for(int i=0;i<numberOfItemsInDatabaseTable;i++)
    {
        QDate dateToCompare = QDate::fromString(databaseTable[i][numberOfDateColumn-1],Qt::ISODate);
        if (!dateToCompare.isValid())
            continue;
        if (today<dateToCompare&&(today.daysTo(dateToCompare)<searchDaysToEndFromUser))
        {
            for(int j=0;j<howManyColumns;j++)
            {
                displayTable[numberOfItemsToDisplay][j] = databaseTable[i][j];
            }
            numberOfItemsToDisplay++;
        }
    }
    setDisplayDateLastType(searchDaysToEndFromUser);
    emit sendTableToDisplay(displayTable,numberOfItemsToDisplay);
}

void DvdDatabase::displayAllDatabase()
{
    int numberOfItemsToDisplay=0;
    for(int i=0;i<numberOfItemsInDatabaseTable;i++)
    {
        for(int j=0;j<howManyColumns;j++)
        {
            displayTable[i][j] = databaseTable[i][j];
        }
        numberOfItemsToDisplay++;
    }
    setDisplayAllLastType();
    emit sendTableToDisplay(displayTable,numberOfItemsToDisplay);
}

void DvdDatabase::editRowOfDatabase(int rowOnDisplayTable, QString *dataRow)
{
    int rowToSave;
    if (rowOnDisplayTable==-1)
    {
        rowToSave=numberOfItemsInDatabaseTable;
        numberOfItemsInDatabaseTable++;
    }
    else
        rowToSave=rowInDisplayTableToRowInDatabase(rowOnDisplayTable);
    for(int i=0;i<howManyColumns;i++)
    {
        databaseTable[rowToSave][i]=dataRow[i];
    }
    if ((sizeOfDatabaseTable-1)<=numberOfItemsInDatabaseTable)
        extendTable();
    displayInLastType();
}

void DvdDatabase::deleteRow(int rowOnDisplayTable)
{
    int rowInDatabaseToDelete=rowInDisplayTableToRowInDatabase(rowOnDisplayTable);
    if (rowInDatabaseToDelete==-1)
        return;
    for (int i=rowInDatabaseToDelete+1;i<numberOfItemsInDatabaseTable;i++)
    {
        for (int j=0;j<howManyColumns;j++)
        {
            databaseTable[i-1][j]=databaseTable[i][j];
        }
    }
    numberOfItemsInDatabaseTable--;
    displayInLastType();
}

int DvdDatabase::rowInDisplayTableToRowInDatabase(int rowOnDisplayTable)
{
    int rowInDatabase=-1;
    for (int i=0;i<numberOfItemsInDatabaseTable;i++)
    {
        bool isTheSame=true;
        for (int j=0;j<howManyColumns;j++)
        {
            if (displayTable[rowOnDisplayTable][j]!=databaseTable[i][j])
                isTheSame=false;
        }
        if (isTheSame)
        {
            rowInDatabase=i;
            return rowInDatabase;
        }
    }
    return rowInDatabase;
}

void DvdDatabase::setDisplayStringLastType(int column, QString textFromUser)
{
    lastColumnFormUser = column;
    lastTextFromUser = textFromUser;
    lastDisplay = displayStringFiltered;
}

void DvdDatabase::setDisplayDateLastType(int searchDaysToEndFromUser)
{
    lastSearchDateFromUser = searchDaysToEndFromUser;
    lastDisplay = displayDateFiltered;
}

void DvdDatabase::setDisplayAllLastType()
{
    lastDisplay = displayAll;
}

void DvdDatabase::displayInLastType()
{
    if (lastDisplay==displayAll)
        displayAllDatabase();
    else if (lastDisplay==displayStringFiltered)
        displayStringFilteredDatabase(lastColumnFormUser,lastTextFromUser);
    else if (lastDisplay==displayDateFiltered)
        displayDateFilteredDatabase(lastSearchDateFromUser);
}
