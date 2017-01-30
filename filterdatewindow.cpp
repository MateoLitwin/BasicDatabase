#include "filterdatewindow.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGroupBox>


FilterDateWindow::FilterDateWindow(QWidget *parent) : QWidget(parent)
{
    setWindowOptions();
    createFilterWindow();
}

void FilterDateWindow::setWindowOptions()
{
    //setMaximumSize(QSize(500, 500));
    setWindowTitle(tr("Parametry filtrowania wyników"));
}

void FilterDateWindow::createFilterWindow()
{
    createComboBoxLine();
    createButtonBox();
    createMainLayout();
}

void FilterDateWindow::createComboBoxLine()
{
    comboBoxGroup = new QGroupBox(tr("Parametry filtracji"));
    QFormLayout *horizontalLayout = new QFormLayout;
    horizontalLayout->addRow(new QLabel(tr("Wpisz liczbę dni wyszukiwania od dziś:")), searchData = new QLineEdit);
    searchData->setInputMask("9999");
    comboBoxGroup->setLayout(horizontalLayout);
}

void FilterDateWindow::createButtonBox()
{
    buttonBoxDialog = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel );
    connect(buttonBoxDialog,SIGNAL(accepted()),this,SLOT(prepareAndSendFilter()));
    connect(buttonBoxDialog,SIGNAL(rejected()),this,SLOT(close()));
}

void FilterDateWindow::createMainLayout()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(comboBoxGroup);
    mainLayout->addWidget(buttonBoxDialog);
    setLayout(mainLayout);
}

void FilterDateWindow::prepareAndSendFilter()
{
    QString textToFind = searchData->text();
    int daysToFindFromToday = textToFind.toInt();
    emit displayFilteredDatabaseWithDate(daysToFindFromToday);
    this->close();
}

void FilterDateWindow::showFilterDateWindow()
{
    this->show();
}
