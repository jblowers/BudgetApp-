#ifndef BUDGET_H
#define BUDGET_H

#include <QObject>
#include <QMap>
#include <QDate>
#include <QVector>
#include "day.h"
#include "transaction.h"


class Budget
{
//    Q_OBJECT
public:
    Budget(QObject* parent = nullptr);
//    Budget();
//    ~Budget();
//    Budget(QVector<Day> dayVec);
    Budget(QDate start, QDate end);
    QMap<QDate,Day> m_dayMap;
    Day getDay(QDate date);
    void addTransactionToDate(QDate date, Transaction trans);

    QVector<Transaction>* getTransactionDataPointer() { return &m_TransactionData; }
    QVector<Transaction> m_TransactionData;
    QDate getStartDate() { return m_BudgetStartDate; }
    QDate m_BudgetStartDate;
    QDate getEndDate() { return m_BudgetEndDate; }
    QDate m_BudgetEndDate;

    QVector<Transaction*> getTransactionsAtDate(QDate date) { return m_DateMap[date];}


    QMap<QDate,QVector<Transaction*>> m_DateMap;
    void updateDateMap();


    // debug functions

    void loadDefaultBudget();

};

#endif // BUDGET_H
