#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QObject>
#include <QDate>

class Transaction
{
public:
    Transaction();

    QString m_strTitle;
    QString m_strDescription;

    enum TransactionType {
        Transaction_Unknown,
        Transaction_Expense,
        Transaction_Income
    };
    QDate m_date;

    TransactionType m_type;
    double m_value;

    QDate date() { return m_date; }
    void setDate(QDate date) { m_date = date; }
    QString title() { return m_strTitle; }
    void setTitle(QString title) { m_strTitle = title; }
    QString description() { return m_strDescription; }
    void setDescription(QString desc) { m_strDescription = desc; }
    TransactionType type() { return m_type; }
    void setTransactionType(TransactionType type) { m_type = type; }
    double value() { return m_value; }
    void setValue(double val) { m_value = val; }
    QString getTypeAsString() {
        switch(m_type) {
        case Transaction_Income:
            return "Income";
        case Transaction_Expense:
            return "Expense";
//        case Transaction_Unknown:
        default:
            return "Unknown";
        }

    }


    QString print() { QString str = "Title: " + m_strTitle;
                    str += "\nDesc: " + m_strDescription;
                    str += "\nType: " + getTypeAsString();
                    str += "\nValue: " + QString::number(m_value);
                    return str;
                    }

};

#endif // TRANSACTION_H
