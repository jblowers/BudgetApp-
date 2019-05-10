#ifndef BUDGETCONTROLLER_H
#define BUDGETCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include "budget.h"

#define DEFAULT_BUDGET_FILE "D:/John/test_budget_file.json"

class BudgetController : public QObject
{
    Q_OBJECT
public:
    BudgetController(QObject* parent = nullptr);
    virtual ~BudgetController();

signals:
    void updateTransactionGui();
    void LogToGui(QString);
    void updateGuiTransactions(QVector<Transaction*>);
    void RequestUpdateGui();

public:

    Budget m_budget;
    QVector<Transaction*> m_CurrentDisplayedTransactions;
    int m_nCurrentSelectedIndex = -1;

    QVector<Transaction*> copyOfTransactions(QVector<Transaction*> transactions);

    QDate getStartDate() { return m_budget.getStartDate(); }
    QDate getEndDate() { return m_budget.getEndDate(); }
    void removeTransaction(QDate date,int nIndex);
    void removeTransaction(Transaction* pTrans) {
        emit LogToGui("Removing Transaction" + pTrans->title());
        qDebug("removeTransaction(ptrans) %d",pTrans);
//        m_budget.removeTransaction(pTrans);
//        emit updateTransactionGui();
    }

    void addTransaction(Transaction trans) { m_budget.addTransaction(trans); }
    QVector<Transaction*> getTransactionVector(QDate date) { return m_budget.getTransactionsAtDate(date); }

    void saveBudgetToFile(QString strFile);
    void loadBudgetFromFile(QString jsonFile);

public slots:
    void onSaveBudgetToJsonFileRequested(QString);
    void onLoadBudgetFromJsonFileRequested(QString);
    void onRemoveSelected();
    void onRequestTransactionsAt(QDate);
    void onSelectedTransactionChanged(int);
    void onSaveSelected(Transaction);
};

#endif // BUDGETCONTROLLER_H
