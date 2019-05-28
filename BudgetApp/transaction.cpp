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

bool Transaction::LoadFromJson(QJsonObject json)
{
    QJsonValue val;
    val = json["transactionDate"];
    if ( val != QJsonValue::Undefined) {
        setDate(QDate::fromString(val.toString()));
    }
    val = json["title"];
    if ( val != QJsonValue::Undefined) {
        setTitle(val.toString());
    }
    val = json["description"];
    if ( val != QJsonValue::Undefined) {
        setDescription(val.toString());
    }
    val = json["transactionType"];
    if ( val != QJsonValue::Undefined) {
        setTransactionType(getTypeFromString(val.toString()));
    }
    val = json["value"];
    if ( val != QJsonValue::Undefined) {
        setValue(val.toDouble());
    }
    val = json["dateModified"];
    if ( val != QJsonValue::Undefined) {
        setDateModified(QDate::fromString(val.toString()));
    }
    val = json["dateCreated"];
    if ( val != QJsonValue::Undefined) {
        setDateCreated(QDate::fromString(val.toString()));
    }
    // if _this_ is empty, return false...?
    return true;
}
