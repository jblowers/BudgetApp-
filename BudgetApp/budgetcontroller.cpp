#include "budgetcontroller.h"

BudgetController::BudgetController(QObject* parent)
{
    m_budget.loadDefaultBudget();


}


void BudgetController::setDateRange(QDate start, QDate end)
{
//    Transaction trans;

//    QDate it = start;
//    while ( it < end ) {
//        trans.setTitle(QString::number(it.day()));
//        trans.setDescription(it.toString());
//        trans.setDate(it);
//        trans.setTransactionType(Transaction::Transaction_Unknown);
//        m_TransMap.insert(trans.date(),trans);
//        it = it.addDays(1);
//    }

}
