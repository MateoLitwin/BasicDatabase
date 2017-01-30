#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QWidget>
#include "common.h"

class QTableWidget;
class QTableWidgetItem;
class QDialogButtonBox;
class QGroupBox;
class QGridLayout;
class QVBoxLayout;
class QLineEdit;

class EditWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EditWindow(QWidget *parent = 0);

signals:
    void editRowOfDatabase(QString *dataRow,int row);
    void addRowToDatabase(QString *dataRow);
    void sendDataToDatabase(int row,QString *sendedData);
    void sendMsgToUsr(QString message);

public slots:
    void chengeConfig(databaseConfig configFromFile);
    void showEditWindow(int row,QString *data=NULL);

private slots:
    void prepareAndSendToDatabase();

private:
    int displayedColumns;
    databaseConfig config;
    QGroupBox *editBoxGroup;
    QDialogButtonBox *buttonBoxDialog;
    int currentRow;
    QString *sendedDataToDatabase;
    QGridLayout *editWindowLayout;
    QVBoxLayout *mainLayout;
    QLineEdit **editedData;
    void setWindowOptions();
    void createEditWidgetLine();
    void createButtonBox();
    void displayTable(QString *displayTable=NULL);
    void setMainLayout();
    void setEditGroupBox();
    void deleteOldLayout();
};

#endif // EDITWINDOW_H
