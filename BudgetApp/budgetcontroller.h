#ifndef BUDGETCONTROLLER_H
#define BUDGETCONTROLLER_H

#include <QObject>
#include <QMap>
#include "budget.h"

class BudgetController
{
public:
    BudgetController(QObject* parent = nullptr);

//    QMap<QDate,Transaction> m_TransMap;

    Budget m_budget;

    void setDateRange(QDate start,QDate end);

    QDate getStartDate() { return m_budget.getStartDate(); }
    QDate getEndDate() { return m_budget.getEndDate(); }

    QVector<Transaction*> getTransaction(QDate date) { return m_budget.getTransactionsAtDate(date); }

};

#endif // BUDGETCONTROLLER_H
