#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QComboBox>
#include <QListWidget>
#include "budgetcontroller.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    BudgetController* m_pController;
    QCalendarWidget *m_pCal;
    QComboBox *m_pCombo;
    QListWidget *m_pList;
    void fillInTransactionData(Transaction* trans);
    void populateComboBox(QVector<Transaction*> trans);

    QVector<Transaction*> m_CurrentSelectionTransactions;

public slots:
    void onSelectionChanged();
    void onSelectedTransIndexChanged(int);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
