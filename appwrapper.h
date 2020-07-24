#ifndef APPWRAPPER_H
#define APPWRAPPER_H

#include <QObject>

class AppWrapper : public QObject
{
    Q_OBJECT
public:
    explicit AppWrapper(QObject *parent = nullptr);

signals:

};

#endif // APPWRAPPER_H
