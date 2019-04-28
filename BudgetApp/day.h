#ifndef DAY_H
#define DAY_H

#include <QObject>
#include "transaction.h"

class Day
{
public:
    Day();

    QDate m_date;
    QList<Transaction> m_transactions;

    void setDate(QDate date) { m_date = date; }
    QList<Transaction> getTransactionList() { return m_transactions; }
    Transaction getTransaction(int nIndex) { return m_transactions.at(nIndex); }
    int getTransactionCount() { return m_transactions.length(); }

    void addTransaction(Transaction trans) { m_transactions.append(trans); }
//    bool deleteTransaction(Transaction trans);

    QString print() {
        QString str = "------\nDate: " + m_date.toString() + "\n";
        for(int i = 0; i< m_transactions.length(); i++) {
            str += "Transaction " + QString::number(i) + "\n\t";
            str += m_transactions[i].print() + "\n\n";
        }
        return str + "\n------\n";
    }

};

#endif // DAY_H
