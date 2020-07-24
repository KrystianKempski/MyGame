#ifndef TROOP_H
#define TROOP_H

#include <QObject>

class Troop : public QObject
{
    Q_OBJECT
public:
    explicit Troop(QObject *parent = nullptr);

signals:

};

#endif // TROOP_H
