/****************************************************************************
** Klasa Troop reprezentuje pojedynczy oddział - jednostkę, która jest podsta-
** wą gry. Informacja o wszystkich statystykach jednostki jest przechowywana
** w JsonArray, którą łatwo pobrać i wysłać na serwer z bazą danych. Klasa
** Troop ma metody umożliwiające pobranie i ustawienie statystyk jednostki.
****************************************************************************/

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

    template<typename T>
    void setStatList(int index, T &&value){
        if constexpr(!std::is_same_v<T, QJsonValue>){
            QJsonValue jValue(value);
            m_statList->replace(index,jValue);
            emit statListChanged();
        }else{
            m_statList->replace(index,value);
            emit statListChanged();
        }
    }

    void write(QJsonObject &json) const;
signals:

    void statListChanged();

private:

    QJsonArray *m_statList;
};

#endif // TROOP_H
