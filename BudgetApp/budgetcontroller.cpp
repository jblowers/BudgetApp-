#include "budgetcontroller.h"

BudgetController::BudgetController(QObject* parent) :
    QObject(parent),
    m_budget(parent)
{

//    m_budget.loadDefaultBudget();

}

BudgetController::~BudgetController()
{
    //end
}


void BudgetController::removeTransaction(QDate date,int nIndex)
{
    qDebug("Attempting to remove %s index %d",date.toString().toStdString().c_str(),nIndex);
    m_budget.removeTransaction(date,nIndex);

}
