#include "budget.h"

//Budget::Budget()
Budget::Budget(QObject* parent)
{

    loadDefaultBudget();

}

Budget::Budget(QDate start, QDate end)
{
    m_BudgetEndDate = end;
    m_BudgetStartDate = start;
    loadDefaultBudget();
}



//Day Budget::getDay(QDate date)
//{
//    return m_dayMap[date];
//}

//void Budget::addTransactionToDate(QDate date, Transaction trans)
//{
////    if(m_dayMap.contains(date)) {
////        m_dayMap[date].addTransaction(trans);
//////        m_dayMap.insert(date,trans);
////    }
//}
int nInc = 0;

void Budget::loadDefaultBudget()
{
    m_BudgetStartDate = QDate::currentDate().addDays(-100);
    m_BudgetEndDate = QDate::currentDate().addDays(100);

    int numTransactions = 3;
    for(QDate date = m_BudgetStartDate; date < m_BudgetEndDate; date = date.addDays(1))
    {
        numTransactions = date.toJulianDay() % 10;
        qDebug("Date: %s\tnumTrans: %d\n",date.toString().toStdString().c_str(),numTransactions);
        for(int i = 0; i < numTransactions; i++) {
            nInc++;
            Transaction trans;
            trans.setTitle("JD: " + QString::number(date.toJulianDay()) + " it: " + QString::number(i));
            trans.setDescription("Iteration: " + QString::number(nInc));
            trans.setDate(date);
            if ( date.toJulianDay()%2 == 0)
                trans.setValue(nInc*numTransactions);
            else {
                trans.setValue(-1*nInc*numTransactions);
            }
            m_TransactionData.append(trans);
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
    if(1) {
        QDate date = m_BudgetStartDate.addDays(16);
        QVector<Transaction*> vec = m_DateMap[date];
        for(int i = 0; i < vec.length(); i++ ) {
            qDebug("date: %s. title: %s.\ndescription: %s\nvalue: $%d",date.toString().toStdString().c_str(),
                   vec[i]->title().toStdString().c_str(),vec[i]->description().toStdString().c_str(),vec[i]->value());

        }


    }
}





