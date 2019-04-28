#include "budget.h"

//Budget::Budget()
Budget::Budget(QObject* parent) : QObject(parent)
{

    loadDefaultBudget();

}


//Budget::Budget(QDate start, QDate end)
//{
//    m_BudgetEndDate = end;
//    m_BudgetStartDate = start;
//    loadDefaultBudget();
//}


void Budget::addTransaction(Transaction trans, bool bCheckDuplicate)
{
    if (bCheckDuplicate) {
        // do logic to find if this transaction exists already or not
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
    for(QDate date = m_BudgetStartDate; date < m_BudgetEndDate; date = date.addDays(1))
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
    }


    // printing date map
    if(0) {
        QDate date = m_BudgetStartDate.addDays(16);
        QVector<Transaction*> vec = m_DateMap[date];
        for(int i = 0; i < vec.length(); i++ ) {
            qDebug("date: %s. title: %s.\ndescription: %s\nvalue: $%d",date.toString().toStdString().c_str(),
                   vec[i]->title().toStdString().c_str(),vec[i]->description().toStdString().c_str(),vec[i]->value());

        }


    }
}





