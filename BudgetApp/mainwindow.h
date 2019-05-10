#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QComboBox>
#include <QListWidget>
#include <QFileDialog>
#include <QThread>
#include "budgetcontroller.h"


Q_DECLARE_METATYPE(Transaction*)
Q_DECLARE_METATYPE(QVector<Transaction*>)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QThread m_ControllerThread;
    BudgetController* m_pController;
    QCalendarWidget *m_pCal;
    QComboBox *m_pCombo;
    QListWidget *m_pList;
    void fillInTransactionData(Transaction* trans);
    void populateTransactionList(QVector<Transaction*> trans);
    void updateSelectedTransaction(int nIndex);
    int m_nCurrentTransactionIndex;

    QVector<Transaction*> m_CurrentSelectionTransactions;

public slots:
    void onSelectionChanged();
    void onSelectedTransIndexChanged(int);
    void onRemoveSelectedPressed();
    void onSaveTransactionPressed();
    void onSaveBrowsePressed();
    void onSaveButtonPressed();
    void onLoadButtonPressed();
    void onLoadBrowsePressed();
    void LogToGuiWindow(QString);
    void onUpdateGuiTransactions(QVector<Transaction*>);
    void onControllerRequestUpdateGui() { emit RequestSelectedDate(m_pCal->selectedDate()); } // new data update requested from controller

signals:
    void SaveBudgetToJsonFile(QString);
    void LoadBudgetFromJsonFile(QString);
    void RemoveSelected();
    void RequestSelectedDate(QDate);
    void SelectedTransactionChanged(int);
    void RequestSaveTransaction(Transaction);
//    void

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
