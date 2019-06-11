#include "budgetcontroller.h"

BudgetController::BudgetController(QObject* parent) :
    QObject(parent),
    m_budget(parent)
{

//    m_budget.loadDefaultBudget();
//    saveBudgetToFile("");
}

BudgetController::~BudgetController()
{
    //end
}

void BudgetController::onSaveBudgetToJsonFileRequested(QString jsonFile)
{
    saveBudgetToFile(jsonFile);
}

void BudgetController::onLoadBudgetFromJsonFileRequested(QString jsonFile)
{
    emit LogToGui("BudgetController::onLoadBudgetFromJsonFileRequested( " + jsonFile + " )");
    loadBudgetFromFile(jsonFile);

}

void BudgetController::loadBudgetFromFile(QString strFile)
{

    emit LogToGui("\tBudgetController::loadBudgetFromFile( " + strFile + " )");
//    if(strFile.isEmpty()) {
//        strFile = DEFAULT_BUDGET_FILE;
//    }
    QFile loadFile(strFile);

    if (!loadFile.open(QIODevice::ReadOnly)) {
       qWarning("Couldn't open load file.");
       emit LogToGui("Couldn't open load file: " + strFile);
       return;
    }
    QByteArray byteArr = loadFile.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(byteArr);
//    doc.fromBinaryData(byteArr);

    QJsonObject jsonObj = doc.object();
//    Budget b(m_budget.LoadFromJson(jsonObj));
    m_budget.LoadFromJson(jsonObj);

}

void BudgetController::onRequestTransactionsAt(QDate date)
{
    emit LogToGui("BudgetController::onRequestTransactionsAt( " + date.toString() + " )");
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

void BudgetController::onSaveSelected(Transaction trans)
{
    auto transactions = m_budget.getTransactionsAtDate(trans.date());
//    transactions[m_nCurrentSelectedIndex]-> // need to search for already existing transaction and replace with new one...

}

void BudgetController::onRemoveSelected()
{
    emit LogToGui("\tBudgetController::onRemoveSelected()");
    if(m_nCurrentSelectedIndex != -1) {
        QDate date = m_CurrentDisplayedTransactions[m_nCurrentSelectedIndex]->date();
        removeTransaction(date,m_nCurrentSelectedIndex);
    } else {
        emit LogToGui("Invalid selected index, could not remove.");
    }
}

void BudgetController::removeTransaction(QDate date,int nIndex)
{
    emit LogToGui("BudgetController::removeTransaction( " + date.toString() + ", " + QString::number(nIndex) + " )");
    qDebug("Attempting to remove %s index %d",date.toString().toStdString().c_str(),nIndex);
    m_budget.removeTransaction(date,nIndex);
    emit RequestUpdateGui();
}

void BudgetController::onSelectedTransactionChanged(int nIndex)
{
    emit LogToGui("BudgetController::onSelectedTransactionChanged( " + QString::number(nIndex) + " )");
    m_nCurrentSelectedIndex = nIndex;
}

void BudgetController::saveBudgetToFile(QString strFile)
{
    emit LogToGui("\tBudgetController::saveBudgetToFile( " + strFile + " )");
    if(strFile.isEmpty()) {
        strFile = DEFAULT_BUDGET_FILE;
    }
    QFile saveFile(strFile);

    if (!saveFile.open(QIODevice::WriteOnly)) {
       qWarning("Couldn't open save file.");
       emit LogToGui("Couldn't open save file: " + strFile);
       return;
    }

    QJsonObject budgetObject;
    m_budget.SaveToJson(budgetObject);
    QJsonDocument saveDoc(budgetObject);
    saveFile.write(saveDoc.toJson());
//    saveFile.close();
}
