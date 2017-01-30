#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QObject>
#include "common.h"

class ConfigFile : public QObject
{
    Q_OBJECT
public:
    ConfigFile();
    void readConfigFile();

public slots:
    void saveConfigFile();
    void setConfigFromConfigWindow(databaseConfig configFromWindow);
    void saveDatabaseFileName(QString fileName);
    databaseConfig getConfig();

signals:
    void sendMsgToUsr(QString message);
    void sendConfigToConfigWindow(databaseConfig config);
    void sendConfigToMainWindow(databaseConfig config);
    void sendConfigToDatabase(databaseConfig config);
    void sendConfigToFilterWindow(databaseConfig config);
    void sendConfigToEditWindow(databaseConfig config);

private:
    databaseConfig config;
};

#endif // CONFIGFILE_H
