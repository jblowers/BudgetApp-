#include "budgetcalendarwidget.h"

//BudgetCalendarWidget::BudgetCalendarWidget()
//{

//}
void BudgetCalendarWidget::paintCell(QPainter * painter, const QRect & rect, const QDate & date) const{
//        (QCalendarWidget)(this)->paintCell(painter,rect,date);
    painter->save();
    painter->setBrush(date == this->selectedDate() ? Qt::red : Qt::white);
    painter->drawRect(rect);
    painter->drawText(rect,Qt::AlignCenter|Qt::TextSingleLine,date.toString(Qt::DateFormat::SystemLocaleShortDate));
    painter->restore();
}
