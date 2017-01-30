#include "configfile.h"
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

ConfigFile::ConfigFile()
{

}

void ConfigFile::readConfigFile()
{
    QFile file("config.txt");
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        emit sendMsgToUsr("Błąd odczytu. Nie udało się otworzyć pliku z konfiguracją.");
        config.lastSavedFile="";
        config.howManyColumns=1;
        config.whichIsDateColumn=-1;
        return;
    }
    QTextStream in(&file);
    config.lastSavedFile=in.readLine();
    QString tempHowManyColumnsString=in.readLine();
    config.howManyColumns=tempHowManyColumnsString.toInt();
    QString tempWhichIsDateColumnString=in.readLine();
    config.whichIsDateColumn=tempWhichIsDateColumnString.toInt();
    for (int i=0;i<NUMCOLUMNSNAMES;i++)
    {
        config.columnNames[i]=in.readLine();
    }
    file.close();
    emit sendConfigToConfigWindow(config);
    emit sendConfigToMainWindow(config);
    emit sendConfigToFilterWindow(config);
    emit sendConfigToEditWindow(config);
    emit sendConfigToDatabase(config);
}

void ConfigFile::saveConfigFile()
{
    QFile file("config.txt");
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        emit sendMsgToUsr("Błąd zapisu. Nie udało się zapisać pliku z konfiguracją.");
                return;
    }
    QTextStream out(&file);
    out<<config.lastSavedFile<<"\n";
    out<<config.howManyColumns<<"\n";
    out<<config.whichIsDateColumn<<"\n";
    for (int i=0;i<NUMCOLUMNSNAMES;i++)
    {
        out<<config.columnNames[i]<<"\n";
    }
    file.close();
}

void ConfigFile::setConfigFromConfigWindow(databaseConfig configFromWindow)
{
    QString lastSavedFile=config.lastSavedFile;
    config=configFromWindow;
    config.lastSavedFile=lastSavedFile;
    saveConfigFile();
    emit sendConfigToMainWindow(config);
    emit sendConfigToFilterWindow(config);
    emit sendConfigToEditWindow(config);
    emit sendConfigToDatabase(config);
}

void ConfigFile::saveDatabaseFileName(QString fileName)
{
    config.lastSavedFile=fileName;
    saveConfigFile();
}

databaseConfig ConfigFile::getConfig()
{
    return config;
}
