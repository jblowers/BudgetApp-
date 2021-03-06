#ifndef DAY_H
#define DAY_H

#include <QObject>
#include "transaction.h"

class Day
{
public:
    Day();

    QDate m_date;
    QList<Transaction*> m_pTransactions;

    void setDate(QDate date) { m_date = date; }
    QList<Transaction*> getTransactionList() { return m_pTransactions; }
    Transaction* getTransaction(int nIndex) { return m_pTransactions.at(nIndex); }
    int getTransactionCount() { return m_pTransactions.length(); }

    void addTransaction(Transaction* trans) { if(!m_pTransactions.contains(trans)) { m_pTransactions.append(trans); } }
//    bool deleteTransaction(Transaction trans);

    QString print() {
        QString str = "------\nDate: " + m_date.toString() + "\n";
        for(int i = 0; i< m_pTransactions.length(); i++) {
            str += "Transaction " + QString::number(i) + "\n\t";
            str += m_pTransactions[i]->print() + "\n\n";
        }
        return str + "\n------\n";
    }

};

#endif // DAY_H
