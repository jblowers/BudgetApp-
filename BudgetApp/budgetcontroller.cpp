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
    emit LogToGui("Attempting to remove " + date.toString());
    m_budget.removeTransaction(date,nIndex);

//    m_budget.removeTransaction(date,nIndex);

}

void BudgetController::onRequestTransactionsAt(QDate date)
{
    auto transactions = m_budget.getTransactionsAtDate(date);
    // make copy of current day's transactions; to be edited locally in gui, then when requested, will be saved to the current budget
    m_CurrentDisplayedTransactions = copyOfTransactions(transactions);

    emit updateGuiTransactions(m_CurrentDisplayedTransactions);


//    auto day = m_budget.getDay(date);
//    day->getTransactionList();

}

QVector<Transaction*> BudgetController::copyOfTransactions(QVector<Transaction*> transactions)
{
    qDebug("Copying transaction -> not super efficient if called a lot...");
    QVector<Transaction*> copyT;
    Transaction* newTrans;
    foreach(Transaction* trans,transactions) {
        newTrans = new Transaction();
        newTrans->setDate(trans->date());
        newTrans->setTitle(trans->title());
        newTrans->setDescription(trans->description());
        newTrans->setTransactionType(trans->type());
        newTrans->setValue(trans->value());
        copyT.append(newTrans);
    }
    return copyT;

}

void BudgetController::onRemoveSelected()
{
    removeTransaction(QDate::currentDate(),0);
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
