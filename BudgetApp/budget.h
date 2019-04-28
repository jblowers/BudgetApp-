#ifndef BUDGET_H
#define BUDGET_H

#include <QObject>
#include <QMap>
#include <QDate>
#include <QVector>
#include "day.h"
#include "transaction.h"


class Budget : public QObject
{
    Q_OBJECT
public:
    explicit Budget(QObject* parent = nullptr);
//    Budget(QDate start, QDate end);
    QVector<Transaction> m_TransactionData;
    QMap<QDate,Day> m_dayMap;
    QDate m_BudgetStartDate;
    QDate m_BudgetEndDate;
    QMap<QDate,QVector<Transaction*>> m_DateMap;

    void addTransaction(Transaction trans, bool bCheckDuplicate = false);
    bool removeTransaction(Transaction* pTrans);
    void removeTransaction(QDate date, int nIndex) { removeTransaction(m_DateMap[date][nIndex]); }
    bool checkAndRemoveDataForTransaction(Transaction* pTrans);

    QVector<Transaction>* getTransactionDataPointer() { return &m_TransactionData; }
    QDate getStartDate() { return m_BudgetStartDate; }
    QDate getEndDate() { return m_BudgetEndDate; }

    QVector<Transaction*> getTransactionsAtDate(QDate date) { return m_DateMap[date]; }
    void updateDateMap();

    // debug functions

    void loadDefaultBudget();

};

#endif // BUDGET_H
