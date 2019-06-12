#include "budget.h"



//Budget::Budget()
Budget::Budget(QObject* parent) : QObject(parent)
{

//    loadDefaultBudget();

}


//Budget::Budget(QDate start, QDate end)
//{
//    m_BudgetEndDate = end;
//    m_BudgetStartDate = start;
//    loadDefaultBudget();
//}

Budget* Budget::LoadFromJson(QJsonObject &json)
{
    QString title = json[JSON_TITLE].toString();
    QString desc = json[JSON_DESC].toString();
    Budget* b = this;/*new Budget();*/
    QJsonValue val;

    val = json[JSON_TITLE];
    if( val != QJsonValue::Undefined)
        b->setTitle(val.toString());

    val = json[JSON_DESC];
    if( val != QJsonValue::Undefined) {
        b->setDescription(val.toString());
    }
    val = json[JSON_STARTDATE];
    if( val != QJsonValue::Undefined){
        b->setStartDate(QDate::fromString(val.toString()));
    }
    val = json[JSON_ENDDATE];
    if( val != QJsonValue::Undefined){
        b->setEndDate(QDate::fromString(val.toString()));
    }
    val = json[JSON_DATECREATED];
    if( val != QJsonValue::Undefined){
        b->setCreatedDate(QDate::fromString(val.toString()));
    }
    val = json[JSON_DATEMODIFIED];
    if( val != QJsonValue::Undefined){
        b->setModifiedDate(QDate::fromString(val.toString()));
    }

    val = json[JSON_TRANSACTIONARRAY];
    if ( val != QJsonValue::Undefined) {
        auto arr = val.toArray();
        QJsonObject transObject;
        foreach(auto trans,arr){
            transObject = trans.toObject();
            Transaction t;
            if( t.LoadFromJson(trans.toObject()) ) {
                b->addTransaction(t);
            }
        }
    }
    return b;
}

bool Budget::SaveToJson(QJsonObject &json)
{
    json[JSON_TITLE] = "Test Budget";
    json[JSON_DESC] = "Test test 123Abc";
    json[JSON_DATECREATED] = m_DateCreated.toString();
    json[JSON_DATEMODIFIED] = m_DateModified.toString();
    json[JSON_STARTDATE] = m_BudgetStartDate.toString();
    json[JSON_ENDDATE] = m_BudgetEndDate.toString();
    QJsonArray transArray;
    foreach(Transaction trans,m_TransactionData)
    {
        QJsonObject transObj;
        trans.SaveToJson(transObj);
        transArray.append(transObj);
    }
    json[JSON_TRANSACTIONARRAY] = transArray;

    return true;
}

void Budget::addTransaction(Transaction trans, bool bCheckDuplicate)
{
    if (bCheckDuplicate) {
        // do logic to find if this transaction exists already or not
        bool bDuplicate = false;
        QVector<Transaction*> vec = m_DateMap[trans.date()];
        int len = vec.length();
        for(int i = 0; i < len; i++) {
            if ( trans.title() == vec[i]->title() && trans.value() == vec[i]->value() && trans.description() == vec[i]->description()){
                bDuplicate = true;
            }
        }
        if (!bDuplicate) {
            m_TransactionData.append(trans);
        }
    } else {
        m_TransactionData.append(trans);
    }
    updateDateMap();
}

bool Budget::removeTransaction(Transaction* pTrans)
{
    qDebug("removeTransaction(pTrans)");
    bool ret = true;
    if(!pTrans) {
        qDebug("Can't remove transaction, doesn't exist");
        ret = false;
    } else {
         checkAndRemoveDataForTransaction(pTrans);
    }

    qDebug("return: %d", ret);
    updateDateMap();
    return ret;
}

bool Budget::checkAndRemoveDataForTransaction(Transaction* pTrans)
{
    if(!pTrans) {
        return false;
    }
    else {
        QMutableVectorIterator<Transaction> it(m_TransactionData);
        while(it.hasNext()) {
            Transaction* pT = &it.next();
            if(pT == pTrans) {
                qDebug("Removed transaction. %d =?= %d  %s",pT,pTrans,it.peekPrevious().title().toStdString().c_str());
                it.remove(); // does this remove the current or the next?
                return true;
            }
        }
    }
    return false;
}

void Budget::loadDefaultBudget()
{
    m_BudgetStartDate = QDate::currentDate().addDays(-100);
    m_BudgetEndDate = QDate::currentDate().addDays(100);

    int numTransactions = 3;
    for(QDate date = QDate::currentDate().addDays(-20)/*m_BudgetStartDate*/; date < QDate::currentDate().addDays(20); date = date.addDays(1))
    {
        numTransactions = 4;//date.toJulianDay() % 10;
        qDebug("Date: %s\tnumTrans: %d\n",date.toString().toStdString().c_str(),numTransactions);
        for(int i = 0; i < numTransactions; i++) {
            Transaction trans;
            trans.setTitle("JD: " + QString::number(2458600-date.toJulianDay()) + " it: " + QString::number(i));
            qDebug("%s",trans.title().toStdString().c_str());
            trans.setDescription("Iteration: " + QString::number(i));
            trans.setDate(date);
            if ( date.toJulianDay()%2 == 0)
                trans.setValue(i);
            else {
                trans.setValue(-i);
            }
            addTransaction(trans);
//            m_TransactionData.append(trans);
        }
    }
    updateDateMap();
}


void Budget::updateDateMap()
{
    // iterate through transactionData
    // store pointers for each date
//    QVectorIterator<Transaction> it(m_TransactionData);
//    while(it.hasNext()) {
//        m_DateMap[it.next().m_date].append(it.next());
//    }
    m_DateMap.clear();
    Transaction trans;
    for(int i = 0; i < m_TransactionData.length(); i++) {
        trans = m_TransactionData[i];
        m_DateMap[trans.date()].append(&m_TransactionData[i]);
//        m_DayMap[trans.date()]->addTransaction(&m_TransactionData[i]);
    }


//    // printing date map
//    if(0) {
//        QDate date = m_BudgetStartDate.addDays(16);
//        QVector<Transaction*> vec = m_DateMap[date];
//        for(int i = 0; i < vec.length(); i++ ) {
//            qDebug("date: %s. title: %s.\ndescription: %s\nvalue: $%d",date.toString().toStdString().c_str(),
//                   vec[i]->title().toStdString().c_str(),vec[i]->description().toStdString().c_str(),vec[i]->value());
//        }
//    }
}





