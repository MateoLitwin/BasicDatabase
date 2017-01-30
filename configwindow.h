#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include "common.h"

class QGroupBox;
class QComboBox;
class QDialogButtonBox;
class QLineEdit;

class ConfigWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWindow(QWidget *parent = 0);

signals:
    void sendConfigToSave(databaseConfig config);

private slots:
    void prepareAndSendConfigToSave();
    void showWindow();
    void setConfigFromFile(databaseConfig config);

private:
    void setWindowOptions();
    void createComboBoxLine();
    void createColumnNamesLine();
    void createButtonBox();
    QGroupBox *comboBoxGroup;
    QGroupBox *columnNamesBox;
    QComboBox *comboBoxNumbersOfColumns;
    QComboBox *comboBoxNumberOfTimeColumn;
    QLineEdit **linesWithColumsNames;
    QDialogButtonBox *buttonBoxDialog;
};

#endif // CONFIGWINDOW_H
