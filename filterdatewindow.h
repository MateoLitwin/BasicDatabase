#ifndef FILTERDATEWINDOW_H
#define FILTERDATEWINDOW_H

#include <QWidget>
#include "common.h"

class QLineEdit;
class QDialogButtonBox;
class QGroupBox;
class QVBoxLayout;

class FilterDateWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FilterDateWindow(QWidget *parent = 0);

signals:
    void displayFilteredDatabaseWithDate(int searchDaysToEndFromUser);

public slots:
    void showFilterDateWindow();

private slots:
    void prepareAndSendFilter();

private:
    void setWindowOptions();
    void createFilterWindow();
    void createComboBoxLine();
    void createButtonBox();
    void createMainLayout();
    QVBoxLayout *mainLayout;
    QGroupBox *comboBoxGroup;
    QLineEdit *searchData;
    QDialogButtonBox *buttonBoxDialog;
};

#endif // FILTERDATEWINDOW_H
