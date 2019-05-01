#include "budgetcontroller.h"

BudgetController::BudgetController(QObject* parent) :
    QObject(parent),
    m_budget(parent)
{

//    m_budget.loadDefaultBudget();
    saveBudgetToFile("");
}

BudgetController::~BudgetController()
{
    //end
}

void BudgetController::onSaveBudgetToJsonFileRequested(QString jsonFile)
{
    saveBudgetToFile(jsonFile);
}

void BudgetController::removeTransaction(QDate date,int nIndex)
{
    qDebug("Attempting to remove %s index %d",date.toString().toStdString().c_str(),nIndex);
    m_budget.removeTransaction(date,nIndex);

}

void BudgetController::saveBudgetToFile(QString strFile)
{
    if(strFile.isEmpty()) {
        strFile = "D:/John/test_budget_file.json";
    }
    QFile saveFile(strFile);

       if (!saveFile.open(QIODevice::WriteOnly)) {
           qWarning("Couldn't open save file.");
       }

       QJsonObject budgetObject;
       m_budget.SaveToJson(budgetObject);
       QJsonDocument saveDoc(budgetObject);
       saveFile.write(saveDoc.toJson());


}
