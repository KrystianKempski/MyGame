#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "troop.h"
static const QUrl m_apiEndpoint("https://api.jsonbin.io/b/5f195325c1edc466175cb957");
class DataSource : public QObject
{
    Q_OBJECT

public:
    explicit DataSource(QObject *parent = nullptr);
    Q_INVOKABLE void fetchTroops();;
    Q_INVOKABLE void addTroopRow();
    // Q_INVOKABLE void removeLastTroop();
    QList<Troop*> dataItems(bool team) const;
    // Q_INVOKABLE void addTroop();
    Q_INVOKABLE void addTroop(Troop *value,bool team);
   // Q_INVOKABLE void addTroop(const QString &trName);
   // Q_INVOKABLE void removeTroop(int index);
    Q_INVOKABLE QString getCellColor(int row, int column) const;
    Q_INVOKABLE bool getTokenIn(int row, int column) const;
    int getCellRowCount() const;
    int getCellColumnCount() const;
    void initialize();

signals:
    void preInsertTroopRed();
    void postInsertTroopRed();
    void preInsertTroopBlue();
    void postInsertTroopBlue();
    void preRemoveTroop(int index);
    void postRemoveTroop();
    void tokenIn(int row,int col,bool val);

private slots:
    void dataReadyRead();
    void readFinished();

    void writeFinished();;
public slots:
    Q_INVOKABLE void changeItem();
    Q_INVOKABLE void setTokenIn(int row, int column, bool val);
    Q_INVOKABLE void setCellColor(int row, int column, QString val);



private:
    QByteArray updateData();

    QNetworkAccessManager * m_netManager;
    QNetworkReply * m_NetReply;
    QByteArray * m_dataBuffer;
    QList<Troop*>  m_troopsRed;
    QList<Troop*>  m_troopsBlue;
    QHash<int,QString> statNames;
    int m_cellRows=10;
    int m_cellColumns=10;
    // QVector<QVector<bool>>* m_cells = new QVector<QVector<bool>>;
    bool m_cells[10][10];
    QVector<bool> *m_tokenIn;
    QVector<QString> *m_cellColor;

};

#endif // DATASOURCE_H
