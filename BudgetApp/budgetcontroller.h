#ifndef BUDGETCONTROLLER_H
#define BUDGETCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include "budget.h"

class BudgetController : public QObject
{
    Q_OBJECT
public:
    BudgetController(QObject* parent = nullptr);
    virtual ~BudgetController();

signals:
    void updateTransactionGui();
    void LogToGui(QString);

public:

    Budget m_budget;

    QDate getStartDate() { return m_budget.getStartDate(); }
    QDate getEndDate() { return m_budget.getEndDate(); }
    void removeTransaction(QDate date,int nIndex);
    void removeTransaction(Transaction* pTrans) {
        emit LogToGui("Removing Transaction" + pTrans->title());
        qDebug("removeTransaction(ptrans) %d",pTrans);
        m_budget.removeTransaction(pTrans);
        emit updateTransactionGui();
    }

    void addTransaction(Transaction trans) { m_budget.addTransaction(trans); }
    QVector<Transaction*> getTransactionVector(QDate date) { return m_budget.getTransactionsAtDate(date); }

    void saveBudgetToFile(QString strFile);

public slots:
    void onSaveBudgetToJsonFileRequested(QString);
};

#endif // BUDGETCONTROLLER_H
