#include "transaction.h"

Transaction::Transaction()
{
//    m_dAmount = 0.0;
}



bool Transaction::SaveToJson(QJsonObject &json)
{
//    QJsonObject TransObject;
    json["transactionDate"] = m_date.toString();
    json["title"] = m_strTitle;
    json["description"] = m_strDescription;
    json["transactionType"] = getTypeAsString();
    json["value"] = m_value;
    json["dateModified"] = m_dateModified.toString(); // not set
    json["dateCreated"] = m_dateCreated.toString(); // not set
    return true;
}
