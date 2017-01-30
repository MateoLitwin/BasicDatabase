#include "mainwindow.h"
#include "dvddatabase.h"
#include "common.h"
#include "configfile.h"
#include "configwindow.h"
#include "filterwindow.h"
#include "editwindow.h"
#include "filterdatewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication aplication(argc,argv);
    ConfigFile config;
    DvdDatabase dataBase;
    MainWindow window;
    ConfigWindow configWindow;
    FilterWindow filterWindow;
    EditWindow editWindow;
    FilterDateWindow filterDateWindow;
    QObject::connect(&dataBase,SIGNAL(sendTableToDisplay(QString**,int)),
                     &window,SLOT(displayTable(QString**,int)));
    QObject::connect(&window,SIGNAL(saveDatabase()),
                     &dataBase,SLOT(saveDataFile()));
    QObject::connect(&dataBase,SIGNAL(sendMsgToUsr(QString)),
                     &window,SLOT(sendMsgToUsr(QString)));
    QObject::connect(&editWindow,SIGNAL(sendMsgToUsr(QString)),
                     &window,SLOT(sendMsgToUsr(QString)));
    QObject::connect(&configWindow,SIGNAL(sendConfigToSave(databaseConfig)),
                     &config,SLOT(setConfigFromConfigWindow(databaseConfig)));
    QObject::connect(&window,SIGNAL(showConfigWindow()),
                     &configWindow,SLOT(showWindow()));
    QObject::connect(&config,SIGNAL(sendConfigToConfigWindow(databaseConfig)),
                     &configWindow,SLOT(setConfigFromFile(databaseConfig)));
    QObject::connect(&config,SIGNAL(sendConfigToMainWindow(databaseConfig)),
                     &window,SLOT(chengeConfig(databaseConfig)));
    QObject::connect(&config,SIGNAL(sendConfigToDatabase(databaseConfig)),
                     &dataBase,SLOT(setConfigOfDatabase(databaseConfig)));
    QObject::connect(&dataBase,SIGNAL(saveLastDatabaseFileName(QString)),
                     &config,SLOT(saveDatabaseFileName(QString)));
    QObject::connect(&window,SIGNAL(showFilterStringWindow()),
                     &filterWindow,SLOT(showFilterStringWindow()));
    QObject::connect(&window,SIGNAL(showFilterDateWindow()),
                     &filterDateWindow,SLOT(showFilterDateWindow()));
    QObject::connect(&filterDateWindow,SIGNAL(displayFilteredDatabaseWithDate(int)),
                     &dataBase,SLOT(displayDateFilteredDatabase(int)));
    QObject::connect(&window,SIGNAL(showAllDatabase()),
                     &dataBase,SLOT(displayAllDatabase()));
    QObject::connect(&config,SIGNAL(sendConfigToFilterWindow(databaseConfig)),
                     &filterWindow,SLOT(setConfigOfWindow(databaseConfig)));
    QObject::connect(&window,SIGNAL(deleteRow(int)),
                     &dataBase,SLOT(deleteRow(int)));
    QObject::connect(&config,SIGNAL(sendConfigToEditWindow(databaseConfig)),
                     &editWindow,SLOT(chengeConfig(databaseConfig)));
    QObject::connect(&window,SIGNAL(showEditWindow(int,QString*)),
                     &editWindow,SLOT(showEditWindow(int,QString*)));
    QObject::connect(&editWindow,SIGNAL(sendDataToDatabase(int,QString*)),
                     &dataBase,SLOT(editRowOfDatabase(int,QString*)));
    QObject::connect(&filterWindow,SIGNAL(filterDatabase(int,QString)),
                     &dataBase,SLOT(displayStringFilteredDatabase(int,QString)));
    config.readConfigFile();
    window.show();
    return aplication.exec();
}
