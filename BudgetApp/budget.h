#ifndef BUDGET_H
#define BUDGET_H

#include <QObject>
#include <QMap>
#include <QDate>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include "day.h"
#include "transaction.h"


class Budget : public QObject
{
    Q_OBJECT
public:
    explicit Budget(QObject* parent = nullptr);
//    Budget(QDate start, QDate end);

    // meta data members
    QString m_Title;
    QString m_Description;
    QDate m_DateCreated;
    QDate m_DateModified; //saved?

    void setTitle(QString title) { m_Title = title; }
    void setDescription(QString desc) { m_Description = desc; }
    void setStartDate(QDate date) { m_BudgetStartDate = date; }
    void setEndDate(QDate date) { m_BudgetEndDate = date; }
    void setCreatedDate(QDate date) { m_DateCreated = date; }
    void setModifiedDate(QDate date) { m_DateModified = date; }

    QMap<QDate,Day*> m_DayMap;

    //member
    QVector<Transaction> m_TransactionData;
    QDate m_BudgetStartDate;
    QDate m_BudgetEndDate;
    QMap<QDate,QVector<Transaction*>> m_DateMap;

    void addTransaction(Transaction trans, bool bCheckDuplicate = false);
    bool removeTransaction(Transaction* pTrans);
    void removeTransaction(QDate date, int nIndex) { removeTransaction(m_DateMap[date][nIndex]); }
    bool checkAndRemoveDataForTransaction(Transaction* pTrans);

    QVector<Transaction>* getTransactionDataPointer() { return &m_TransactionData; }
    QDate getStartDate() { return m_BudgetStartDate; }
    QDate getEndDate() { return m_BudgetEndDate; }

    QVector<Transaction*> getTransactionsAtDate(QDate date) { return m_DateMap[date]; }
    Day* getDay(QDate date) { return m_DayMap[date]; }
    void updateDateMap();

    bool SaveToJson(QJsonObject &json);
    static Budget* LoadFromJson(QJsonObject &json);

    // debug functions

    void loadDefaultBudget();

};

#endif // BUDGET_H
