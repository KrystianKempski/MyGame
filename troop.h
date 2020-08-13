#ifndef TROOP_H
#define TROOP_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

class Troop : public QObject
{
    Q_OBJECT

public:
    explicit Troop(QObject *parent = nullptr);
    ~Troop();
    Q_INVOKABLE QJsonArray statList() const;
    void setStatList(int index,QJsonValue &value);
    void write(QJsonObject &json) const;
signals:

    void statListChanged();

private:

    QJsonArray *m_statList;
};

#endif // TROOP_H
