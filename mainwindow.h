#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"

class QTableWidgetItem;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:
    void saveDatabase();
    void showConfigWindow();
    void showFilterStringWindow();
    void showFilterDateWindow();
    void showEditWindow(int row,QString *data=NULL);
    void showAllDatabase();
    void deleteRow(int row);

public slots:
    void displayTable(QString **displayTable,int numberOfItemsToDisplay);
    void sendMsgToUsr(QString message);
    void chengeConfig(databaseConfig configFromFile);

private:
    QTableWidget *tableWidget;
    QTableWidgetItem **displayedTable;
    QTableWidgetItem **columnNames;
    QString *sendedData;
    int displayedColumns;
    int displayedRows;
    databaseConfig config;
    void setWindowOptions();
    void setTableWidgetOptions();
    void createActions();
    void createStatusBar();
    void actionHandling(int actionType);
    int askUser(QString text);
    void deleteOldContent();
    void createNewTableItems(int numberOfItemsToDisplay);
    void closeEvent(QCloseEvent *event);
    void addSignal();
    void editDataSignal();
    void editDeleteSignal();
    void filterDataSignal();
    void filterDateSignal();
    void filterNoneSignal();
};

#endif // MAINWINDOW_H
