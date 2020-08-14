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
#include <QTimer>

static const QUrl m_apiEndpoint("https://api.jsonbin.io/b/5f1ed239c58dc34bf5dbac22");
class DataSource : public QObject
{
    Q_OBJECT

public:
    explicit DataSource(QObject *parent = nullptr);
    ~DataSource();
    Q_INVOKABLE void fetchTroops();
    Q_PROPERTY(QString console READ readConsole WRITE writeConsole NOTIFY consoleChanged);
    Q_PROPERTY(QString chat READ readChat WRITE writeChat NOTIFY chatChanged);
    Q_PROPERTY(bool teamTurn READ teamTurn WRITE setTeamTurn NOTIFY teamTurnChanged);
    Q_PROPERTY(short turn READ turn WRITE setTurn NOTIFY turnChanged);
    QList<Troop*> dataItems(bool team) const;
    Q_INVOKABLE void addTroop(Troop *value,bool team);
    Q_INVOKABLE void removeTroop(bool team, short index);
    Q_INVOKABLE QString getCellColor(int row, int column) const;
    Q_INVOKABLE bool getTokenIn(int row, int column) const;
    Q_INVOKABLE short getCellRowCount() const;
    Q_INVOKABLE short getCellColumnCount() const;
    void initialize();
   QString readConsole() const;
   QString readChat() const;
   bool teamTurn() const;
   short turn() const;
   QString info() const;

signals:
    void preInsertTroopRed();
    void postInsertTroopRed();
    void preInsertTroopBlue();
    void postInsertTroopBlue();
    void preRemoveTroopRed(int index);
    void postRemoveTroopRed();
    void preRemoveTroopBlue(int index);
    void postRemoveTroopBlue();
    Q_INVOKABLE void troopChanged();
    Q_INVOKABLE void consoleChanged(QString console);
    void chatChanged(QString chat);
    void teamTurnChanged(bool teamTurn);
    void turnChanged(short turn);
    void infoChanged(QString info);

private slots:
    void dataReadyRead();
    void readFinished();
    void writeFinished();
public slots:
    Q_INVOKABLE void changeItem();
    Q_INVOKABLE void setTokenIn(int row, int column, bool val);
    Q_INVOKABLE void setCellColor(int row, int column, QString val);
    void writeConsole(QString console);
    void writeChat(QString chat);
    void setTeamTurn(bool teamTurn);
    void setTurn(short turn);
    void setInfo(QString info);

private:
    QNetworkAccessManager * m_netManager;
    QNetworkReply * m_NetReply;
    QByteArray * m_dataBuffer;
    QList<Troop*>  m_troopsRed;
    QList<Troop*>  m_troopsBlue;
    int m_cellRows=15;
    int m_cellColumns=15;
    QVector<bool> *m_tokenIn;
    QVector<QString> *m_cellColor;
    QString m_consoleLine;
    QString m_console;
    int m_dataUpdate=0;
    QTimer* m_refresh;
    QString m_chat;
    QString m_chatLine;
    bool m_teamTurn;
    short m_turn=-1;
};

#endif // DATASOURCE_H
