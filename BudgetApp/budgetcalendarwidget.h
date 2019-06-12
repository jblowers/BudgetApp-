#ifndef BUDGETCALENDARWIDGET_H
#define BUDGETCALENDARWIDGET_H
#include "QCalendarWidget"
#include "QPainter"
#include "QTextCharFormat"


class BudgetCalendarWidget : public QCalendarWidget
{
public:
    BudgetCalendarWidget(QWidget *parent=Q_NULLPTR):
        QCalendarWidget{parent} {
        QTextCharFormat format;
        format.setFontPointSize(66);
        setHeaderTextFormat(format);

    }



protected:
    void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;
};

#endif // BUDGETCALENDARWIDGET_H
