#include "editwindow.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDateTime>

EditWindow::EditWindow(QWidget *parent) : QWidget(parent)
{
    displayedColumns=INITIALCOLUMNSOFTABLE;
    sendedDataToDatabase=NULL;
    editedData=NULL;
    setWindowOptions();
    createEditWidgetLine();
    createButtonBox();
    setMainLayout();

}

void EditWindow::setWindowOptions()
{
    //setMaximumSize(QSize(500, 500));
    setMaximumHeight(130);
    setMinimumSize(QSize(800,130));
    setWindowTitle(tr("Edycja Bazy"));
}

void EditWindow::createEditWidgetLine()
{
    editWindowLayout = new QGridLayout;
    for (int i=0;i<INITIALCOLUMNSOFTABLE;i++)
    {
        editWindowLayout->addWidget(new QLabel("-brak-"),0,i);
        editWindowLayout->addWidget(new QLineEdit,1,i);
    }
    setEditGroupBox();
}

void EditWindow::createButtonBox()
{
    buttonBoxDialog = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel );
    connect(buttonBoxDialog,SIGNAL(accepted()),this,SLOT(prepareAndSendToDatabase()));
    connect(buttonBoxDialog,SIGNAL(rejected()),this,SLOT(close()));
}

void EditWindow::setEditGroupBox()
{
    editBoxGroup = new QGroupBox(tr("Edycja tablicy danych:"));
    editBoxGroup->setLayout(editWindowLayout);
}

void EditWindow::setMainLayout()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(editBoxGroup);
    mainLayout->addWidget(buttonBoxDialog);
    setLayout(mainLayout);
}

void EditWindow::deleteOldLayout()
{
    delete mainLayout;
    delete editBoxGroup;
}

void EditWindow::displayTable(QString *displayTable)
{
    if (displayTable==NULL)
    {
        displayTable=new QString[displayedColumns];
    }
    for (int i=0;i<displayedColumns;i++)
    {
        editedData[i]->setText(displayTable[i]);
        if (i==config.whichIsDateColumn-1)
            editedData[i]->setInputMask("0000-00-00");
    }
}

void EditWindow::chengeConfig(databaseConfig configFromFile)
{
    displayedColumns=configFromFile.howManyColumns;
    deleteOldLayout();
    if (editedData!=NULL)
        delete editedData;
    editWindowLayout = new QGridLayout;
    editedData = new QLineEdit*[displayedColumns];
    for (int i=0;i<displayedColumns;i++)
    {
        editWindowLayout->addWidget(new QLabel(configFromFile.columnNames[i]),0,i);
        editWindowLayout->addWidget(editedData[i] = new QLineEdit,1,i);
    }
    setEditGroupBox();
    setMainLayout();
    config=configFromFile;
}

void EditWindow::showEditWindow(int row, QString *data)
{
    if ((row==-1)&&data==NULL)
    {
        currentRow=-1;
        displayTable();
        this->show();
    }
    else if ((row!=-1)&&data!=NULL)
    {
        currentRow=row;
        displayTable(data);
        this->show();
    }
}

void EditWindow::prepareAndSendToDatabase()
{
    if (config.whichIsDateColumn>0&&config.whichIsDateColumn<=displayedColumns)
    {
        QDate dateCheck;
        dateCheck = QDate::fromString(editedData[config.whichIsDateColumn-1]->text(),Qt::ISODate);
        if (!dateCheck.isValid())
        {
            emit sendMsgToUsr("Niepoprawna data, wprowadź poprawną");
            return;
        }
    }
    if (sendedDataToDatabase!=NULL)
        delete sendedDataToDatabase;
    sendedDataToDatabase = new QString[displayedColumns];
    for(int i=0;i<displayedColumns;i++)
    {
        sendedDataToDatabase[i]=editedData[i]->text();
    }
    emit sendDataToDatabase(currentRow,sendedDataToDatabase);
    this->close();
}
