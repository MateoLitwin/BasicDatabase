#include "configwindow.h"
#include <QBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QCheckBox>

ConfigWindow::ConfigWindow(QWidget *parent) : QWidget(parent)
{
    setWindowOptions();
    createComboBoxLine();
    createColumnNamesLine();
    createButtonBox();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(comboBoxGroup);
    mainLayout->addWidget(columnNamesBox);
    mainLayout->addWidget(buttonBoxDialog);
    setLayout(mainLayout);
}

void ConfigWindow::setWindowOptions()
{
    //setMaximumSize(QSize(500, 500));
    setWindowTitle(tr("Konfiguracja bazy"));
}

void ConfigWindow::createComboBoxLine()
{
    comboBoxGroup = new QGroupBox(tr("Parametry bazy"));
    QFormLayout *horizontalLayout = new QFormLayout;
    comboBoxNumbersOfColumns=new QComboBox;
    comboBoxNumberOfTimeColumn=new QComboBox;
    for (int i=0;i<NUMCOLUMNSNAMES;i++)
    {
        comboBoxNumbersOfColumns->addItem(tr("%1").arg(i+1));
        comboBoxNumberOfTimeColumn->addItem(tr("%1").arg(i+1));
    }
    comboBoxNumberOfTimeColumn->addItem(tr("Brak"));
    horizontalLayout->addRow(new QLabel(tr("Ilość kolumn")), comboBoxNumbersOfColumns);
    horizontalLayout->addRow(new QLabel(tr("Kolumna daty")), comboBoxNumberOfTimeColumn);
    comboBoxGroup->setLayout(horizontalLayout);
}

void ConfigWindow::createColumnNamesLine()
{
    columnNamesBox = new QGroupBox("Nazwy kolumn");
    QFormLayout *varticalLayout = new QFormLayout;
    linesWithColumsNames = new QLineEdit*[NUMCOLUMNSNAMES];
    for (int i=0;i<NUMCOLUMNSNAMES;i++)
    {
        varticalLayout->addRow(new QLabel(tr("Kolumna %1").arg(i+1)), linesWithColumsNames[i] = new QLineEdit);
    }
    columnNamesBox->setLayout(varticalLayout);
}

void ConfigWindow::createButtonBox()
{
    buttonBoxDialog = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel );
    connect(buttonBoxDialog,SIGNAL(accepted()),this,SLOT(prepareAndSendConfigToSave()));
    connect(buttonBoxDialog,SIGNAL(rejected()),this,SLOT(close()));
}

void ConfigWindow::prepareAndSendConfigToSave()
{
    databaseConfig sendedConfig;
    sendedConfig.howManyColumns = (comboBoxNumbersOfColumns->currentIndex()+1);
    if (comboBoxNumberOfTimeColumn->currentIndex()<=comboBoxNumbersOfColumns->currentIndex())
        sendedConfig.whichIsDateColumn = (comboBoxNumberOfTimeColumn->currentIndex()+1);
    else
    {
        sendedConfig.whichIsDateColumn = -1;
        comboBoxNumberOfTimeColumn->setCurrentIndex(9);
    }
    for (int i=0;i<NUMCOLUMNSNAMES;i++)
    {
        sendedConfig.columnNames[i] = linesWithColumsNames[i]->text();
    }
    emit sendConfigToSave(sendedConfig);
    this->close();
}

void ConfigWindow::showWindow()
{
    this->show();
}

void ConfigWindow::setConfigFromFile(databaseConfig config)
{
    comboBoxNumbersOfColumns->setCurrentIndex(config.howManyColumns-1);
    if (config.whichIsDateColumn!=-1)
        comboBoxNumberOfTimeColumn->setCurrentIndex(config.whichIsDateColumn-1);
    else
        comboBoxNumberOfTimeColumn->setCurrentIndex(9);
    for (int i=0;i<NUMCOLUMNSNAMES;i++)
    {
         linesWithColumsNames[i]->setText(config.columnNames[i]);
    }
}
