#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QWidget>
#include "common.h"

class QComboBox;
class QLineEdit;
class QDialogButtonBox;
class QGroupBox;
class QVBoxLayout;

class FilterWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FilterWindow(QWidget *parent = 0);

signals:
    void filterDatabase(int column,QString textFromUser);

public slots:
    void setConfigOfWindow(databaseConfig configFromFile);
    void showFilterStringWindow();

private slots:
    void prepareAndSendFilter();

private:
    void setWindowOptions();
    void createFilterWindow();
    void createComboBoxLine();
    void createButtonBox();
    void createMainLayout();
    void setComboBoxNames();
    QVBoxLayout *mainLayout;
    QGroupBox *comboBoxGroup;
    QComboBox *filterType;
    QLineEdit *searchData;
    QDialogButtonBox *buttonBoxDialog;
    databaseConfig config;
};

#endif // FILTERWINDOW_H
