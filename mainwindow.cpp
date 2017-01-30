#include "mainwindow.h"
#include "filterwindow.h"
#include <QTableWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStatusBar>
#include <QHeaderView>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), tableWidget(new QTableWidget)
{
    setCentralWidget(tableWidget);
    setWindowOptions();
    setTableWidgetOptions();
    createActions();
    createStatusBar();
    displayedRows=0;
    displayedTable=NULL;
    sendedData=NULL;
}

void MainWindow::setWindowOptions()
{
    setMinimumSize(QSize(700, 450));
    setWindowTitle("Baza danych");
}

void MainWindow::setTableWidgetOptions()
{
    displayedColumns=INITIALCOLUMNSOFTABLE;
    if (tableWidget->columnCount() < displayedColumns)
        tableWidget->setColumnCount(displayedColumns);
    columnNames = new QTableWidgetItem*[displayedColumns];
    for (int i=0;i<displayedColumns;i++)
    {
        columnNames[i] = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(i, columnNames[i]);
        columnNames[i]->setText("-BRAK-");
    }
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setTabKeyNavigation(false);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu("Plik");
    QToolBar *fileToolBar = addToolBar("Plik");
    const QIcon saveIcon = QIcon::fromTheme("save", QIcon("icons/save.png"));
    QAction *saveAct = new QAction(saveIcon, "Zapisz", this);
    saveAct->setStatusTip("Zapisuje bazę");
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveDatabase);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon closeIcon = QIcon::fromTheme("close", QIcon("icons/close.png"));
    QAction *closeAct = new QAction(closeIcon, "Zamknij", this);
    closeAct->setStatusTip("Zamyka aplikację");
    connect(closeAct, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(closeAct);

    QMenu *editMenu = menuBar()->addMenu("Edycja");
    QToolBar *editToolBar = addToolBar("Edycja");
    const QIcon newIcon = QIcon::fromTheme("new", QIcon("icons/add.png"));
    QAction *newAct = new QAction(newIcon, "Dodaj wpis", this);
    newAct->setStatusTip("Dodadaje nowy wpis");
    connect(newAct, &QAction::triggered, this, &MainWindow::addSignal);
    editMenu->addAction(newAct);
    editToolBar->addAction(newAct);

    const QIcon editDataIcon = QIcon::fromTheme("editData", QIcon("icons/editData.png"));
    QAction *editDataAct = new QAction(editDataIcon, "Edytuj Data", this);
    editDataAct->setStatusTip("Edytuje dane płyty");
    connect(editDataAct, &QAction::triggered, this, &MainWindow::editDataSignal);
    editMenu->addAction(editDataAct);
    editToolBar->addAction(editDataAct);

    const QIcon editDeleteIcon = QIcon::fromTheme("delete", QIcon("icons/editDelete.png"));
    QAction *editDeleteAct = new QAction(editDeleteIcon, "Usuń wpis", this);
    editDeleteAct->setStatusTip("Usuwa zaznaczony wpis");
    connect(editDeleteAct, &QAction::triggered, this, &MainWindow::editDeleteSignal);
    editMenu->addAction(editDeleteAct);
    editToolBar->addAction(editDeleteAct);

    QMenu *filterMenu = menuBar()->addMenu("Filtruj");
    QToolBar *filterToolBar = addToolBar("Filtruj");

    const QIcon filterDateIcon = QIcon::fromTheme("filterDate", QIcon("icons/filterDate.png"));
    QAction *filterDateAct = new QAction(filterDateIcon, "Filtruj po zapisanej dacie", this);
    filterDateAct->setStatusTip("Wyszukuje w bazie po ilości dni do zapisanej daty");
    connect(filterDateAct, &QAction::triggered, this, &MainWindow::filterDateSignal);
    filterMenu->addAction(filterDateAct);
    filterToolBar->addAction(filterDateAct);

    const QIcon filterDataIcon = QIcon::fromTheme("filterData", QIcon("icons/filterData.png"));
    QAction *filterDataAct = new QAction(filterDataIcon, "Filtruj po Danych", this);
    filterDataAct->setStatusTip("Wyszukuje w bazie w kolumnach");
    connect(filterDataAct, &QAction::triggered, this, &MainWindow::filterDataSignal);
    filterMenu->addAction(filterDataAct);
    filterToolBar->addAction(filterDataAct);

    const QIcon filterNoneIcon = QIcon::fromTheme("filterNone", QIcon("icons/filterDelete.png"));
    QAction *filterNoneAct = new QAction(filterNoneIcon, "Usuń filtrację", this);
    filterNoneAct->setStatusTip("Usuń wszystkie wyszukiwania");
    connect(filterNoneAct, &QAction::triggered, this, &MainWindow::filterNoneSignal);
    filterMenu->addAction(filterNoneAct);
    filterToolBar->addAction(filterNoneAct);

    QMenu *configMenu = menuBar()->addMenu("Konfiguracja");
    QToolBar *configToolBar = addToolBar("Konfiguracja");
    const QIcon configIcon = QIcon::fromTheme("config", QIcon("icons/config.png"));
    QAction *configAct = new QAction(configIcon, "Konfiguruj bazę", this);
    configAct->setStatusTip("Konfiguruje bazę według wytycznych");
    connect(configAct, &QAction::triggered, this, &MainWindow::showConfigWindow);
    configMenu->addAction(configAct);
    configToolBar->addAction(configAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Gotowy");
}

void MainWindow::displayTable(QString **displayTable, int numberOfItemsToDisplay)
{
    deleteOldContent();
    createNewTableItems(numberOfItemsToDisplay);
    for (int i=0;i<numberOfItemsToDisplay;i++)
    {
        for (int j=0;j<displayedColumns;j++)
        {
            displayedTable[i][j].setData(0,displayTable[i][j]);
        }
    }
    displayedRows=numberOfItemsToDisplay;
}

void MainWindow::deleteOldContent()
{
    tableWidget->clearContents();
    for (int i=0;i<displayedRows;i++)
    {
        tableWidget->removeRow(0);
    }
    if (displayedTable!=NULL)
    {
        delete displayedTable;
    }
}

void MainWindow::createNewTableItems(int numberOfItemsToDisplay)
{
    if (numberOfItemsToDisplay>0)
    {
        displayedTable = new QTableWidgetItem*[numberOfItemsToDisplay];
        for (int i=0;i<numberOfItemsToDisplay;i++)
        {
            displayedTable[i] = new QTableWidgetItem[displayedColumns];
        }
        for (int i=0;i<numberOfItemsToDisplay;i++)
        {
            tableWidget->insertRow(i);
            for (int j=0;j<displayedColumns;j++)
            {
                tableWidget->setItem(i,j,&displayedTable[i][j]);
            }
        }
    }
    else
    {
        displayedTable = NULL;
    }
}

void MainWindow::sendMsgToUsr(QString message)
{
    QMessageBox msg(this);
    msg.setText(message);
    msg.exec();
}

int MainWindow::askUser(QString text)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgBox.exec();
    return ret;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (askUser("Czy zapisać zmiany przed wyjściem?")==QMessageBox::Ok)
    {
        emit saveDatabase();
    }
    event->accept();
}

void MainWindow::addSignal()
{
    emit showEditWindow(-1);
}

void MainWindow::editDataSignal()
{
    if (sendedData!=NULL)
        delete sendedData;
    if (displayedTable==NULL)
    {
        sendMsgToUsr(tr("Brak wiersza do edycji"));
        return;
    }
    sendedData = new QString[displayedColumns];
    for(int i=0;i<displayedColumns;i++)
    {
        sendedData[i]=displayedTable[tableWidget->currentRow()][i].text();
    }
    emit showEditWindow(tableWidget->currentRow(),sendedData);
}

void MainWindow::editDeleteSignal()
{
    if (displayedTable==NULL)
    {
        sendMsgToUsr(tr("Brak wiersza do usunięcia"));
        return;
    }
    int ret = askUser("Czy napewno chcesz usunąć?");
    if (ret == QMessageBox::Ok)
        emit deleteRow(tableWidget->currentRow());
    return;
}

void MainWindow::filterDataSignal()
{
    emit showFilterStringWindow();
}

void MainWindow::filterDateSignal()
{
    if (config.whichIsDateColumn!=-1)
        emit showFilterDateWindow();
    else
        sendMsgToUsr("Brak wybranej kolumny z datami");
}

void MainWindow::filterNoneSignal()
{
    emit showAllDatabase();
}

void MainWindow::chengeConfig(databaseConfig configFromFile)
{
    displayedColumns=configFromFile.howManyColumns;
    tableWidget->setColumnCount(displayedColumns);
    delete columnNames;
    columnNames = new QTableWidgetItem*[displayedColumns];
    for(int i=0;i<displayedColumns;i++)
    {
        columnNames[i] = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(i, columnNames[i]);
        columnNames[i]->setText(configFromFile.columnNames[i]);
    }
    config=configFromFile;
}
