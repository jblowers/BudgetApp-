#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QComboBox>
#include <QListWidget>
#include <QFileDialog>
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
    void populateTransactionList(QVector<Transaction*> trans);
    void updateGuiData();
    int m_nCurrentTransactionIndex;

    QVector<Transaction*> m_CurrentSelectionTransactions;

public slots:
    void onSelectionChanged();
    void onSelectedTransIndexChanged(int);
    void onRemoveSelectedPressed();
    void onSaveTransactionPressed();
    void onSaveBrowsePressed();
    void onSaveButtonPressed();
    void LogToGuiWindow(QString);

signals:
    void SaveBudgetToJsonFile(QString);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
