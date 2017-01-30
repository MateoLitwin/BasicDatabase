#include "filterwindow.h"
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGroupBox>

FilterWindow::FilterWindow(QWidget *parent) : QWidget(parent)
{
    setWindowOptions();
    createFilterWindow();
}

void FilterWindow::setWindowOptions()
{
    //setMaximumSize(QSize(500, 500));
    setWindowTitle(tr("Parametry filtrowania wyników"));
}

void FilterWindow::createFilterWindow()
{
    createComboBoxLine();
    createButtonBox();
    createMainLayout();
}

void FilterWindow::createComboBoxLine()
{
    comboBoxGroup = new QGroupBox(tr("Parametry filtracji"));
    QFormLayout *horizontalLayout = new QFormLayout;
    horizontalLayout->addRow(new QLabel(tr("Po której kolumnie szukać:")), filterType = new QComboBox);
    horizontalLayout->addRow(new QLabel(tr("Wpisz szukaną frazę:")), searchData = new QLineEdit);
    comboBoxGroup->setLayout(horizontalLayout);
}

void FilterWindow::createButtonBox()
{
    buttonBoxDialog = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel );
    connect(buttonBoxDialog,SIGNAL(accepted()),this,SLOT(prepareAndSendFilter()));
    connect(buttonBoxDialog,SIGNAL(rejected()),this,SLOT(close()));
}

void FilterWindow::createMainLayout()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(comboBoxGroup);
    mainLayout->addWidget(buttonBoxDialog);
    setLayout(mainLayout);
}

void FilterWindow::setConfigOfWindow(databaseConfig configFromFile)
{
    config=configFromFile;
    filterType->clear();
    setComboBoxNames();
}

void FilterWindow::setComboBoxNames()
{
    for (int i=0;i<config.howManyColumns;i++)
    {
        filterType->addItem(config.columnNames[i]);
    }
}

void FilterWindow::prepareAndSendFilter()
{
    int columnNumber = filterType->currentIndex();
    QString textToFind = searchData->text();
    emit filterDatabase(columnNumber,textToFind);
    this->close();
}

void FilterWindow::showFilterStringWindow()
{
    this->show();
}
