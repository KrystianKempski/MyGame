#include "datasource.h"
#include <QMetaObject>

DataSource::DataSource(QObject *parent) : QObject(parent)
{
    m_netManager= new QNetworkAccessManager(this);
    m_NetReply =nullptr;
    m_dataBuffer = new QByteArray;
    m_console = "";
    m_cellColor = new QVector<QString>(m_cellRows*m_cellColumns,"transparent");
    m_tokenIn = new QVector<bool>(m_cellRows*m_cellColumns,false);
    m_refresh = new QTimer(this);
    connect(m_refresh, &QTimer::timeout,this,&DataSource::fetchTroops);
}

DataSource::~DataSource()
{
    delete m_dataBuffer;
    delete m_cellColor;
    delete m_tokenIn;

}
void DataSource::fetchTroops()
{
    QNetworkRequest request;
    request.setUrl(m_apiEndpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("secret-key","$2b$10$iLVMet4iS5g49NNv/lD57uJp7WLNabTcWRBvQnZFPFJHCcpvFnYv.");
    request.setRawHeader("versioning","false");
    m_NetReply=m_netManager->sendCustomRequest(request,"GET");
    connect(m_NetReply,&QIODevice::readyRead,this,&DataSource::dataReadyRead);
    connect(m_NetReply,&QNetworkReply::finished,this,&DataSource::readFinished);
}
void DataSource::changeItem()
{
    QNetworkRequest request;
    request.setUrl(m_apiEndpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("secret-key","$2b$10$iLVMet4iS5g49NNv/lD57uJp7WLNabTcWRBvQnZFPFJHCcpvFnYv.");
    request.setRawHeader("versioning","false");
    QJsonArray troopsArray;
    QJsonObject troopRedStats;
    for(auto troop : m_troopsRed){
        troop->write(troopRedStats);
        troopsArray.append(troopRedStats);
    }
    QJsonObject troopBlueStats;
    for(auto troop: m_troopsBlue){
        troop->write(troopBlueStats);
        troopsArray.append(troopBlueStats);
    }
    QJsonObject troopObject;
    troopObject.insert("troops",troopsArray);
    troopObject.insert("id",0);
    QJsonArray postArray;
    postArray.append(troopObject);
    QJsonObject data1;
    data1.insert("posts",postArray);
    m_dataUpdate++;
    QJsonValue dataUpdate(m_dataUpdate);
    data1.insert("dataUpdate",dataUpdate);
    QJsonValue consoleLine(m_consoleLine);
    data1.insert("consoleLine",consoleLine);
    QJsonValue chatLine(m_chatLine);
    data1.insert("chatLine",chatLine);
    QJsonValue teamTurn(m_teamTurn);
    data1.insert("teamTurn",teamTurn);
    QJsonValue turn(m_turn);
    data1.insert("turn",turn);
    QJsonDocument doc(data1);
    QByteArray jsonData= doc.toJson();
    m_NetReply=m_netManager->sendCustomRequest(request,"PUT",jsonData);
    if(m_NetReply->error())
        qInfo() <<"błąd w changeItem" +m_NetReply->errorString();
    connect(m_NetReply,&QNetworkReply::finished,this,&DataSource::writeFinished);
}
void DataSource::readFinished()
{
    if(!m_NetReply->error()){
        QJsonDocument doc = QJsonDocument::fromJson(*m_dataBuffer);
        QJsonObject data1 =doc.object();
        QJsonValue dataUpdate = data1["dataUpdate"];
        if(dataUpdate.toInt()!=m_dataUpdate) {                          //sprawdzanie czy zaszły zmiany na serwerze
            m_dataUpdate=dataUpdate.toInt();
            setTeamTurn(data1["teamTurn"].toBool());                   //ustalenie do kogo należy tura
            setTurn(data1["turn"].toInt());                             //ustalenie tury
            if(0!=dataItems(true).size()+dataItems(false).size()) {
                writeConsole(data1["consoleLine"].toString());
                writeChat(data1["chatLine"].toString());
            }
            QJsonArray postsArray = data1["posts"].toArray();
            QJsonObject troopsObject = postsArray.at(0).toObject();
            QJsonArray troopsArray;
            troopsArray = troopsObject["troops"].toArray();
            int troopRedIndex=0;
            int troopBlueIndex=0;
            m_tokenIn->fill(false);
            Troop *troop = new Troop(this);
            for(int i=0;i<troopsArray.size();i++){
                QJsonObject object = troopsArray.at(i).toObject();
                QJsonValue name = object["NAME"];
                QJsonValue type = object["TYPE"];
                QJsonValue maxHp = object["MAX_HP"];
                QJsonValue aVal = object["A_VAL"];
                QJsonValue def = object["DEF"];
                QJsonValue dDice = object["D_DICE"];
                QJsonValue dmg = object["DMG"];
                QJsonValue speed = object["SPEED"];
                QJsonValue range = object["RANGE"] ;
                QJsonValue aCharge = object["A_CHARGE"];
                QJsonValue aCrowd = object["A_CROWD"];
                QJsonValue morale = object["MORALE"];
                QJsonValue aType = object["A_TYPE"];
                QJsonValue str = object["STR"];
                QJsonValue agi = object["AGI"] ;
                QJsonValue end = object["END"];
                QJsonValue will = object["WILL"] ;
                QJsonValue row = object["ROW"] ;
                QJsonValue col = object["COL"] ;
                QJsonValue id = object["T_ID"];
                QJsonValue team = object["TEAM"];
                QJsonValue moved = object["MOVED"];
                QJsonValue attacked = object["ATTACKS"];
                QJsonValue active = object["ACTIVE"];
                QJsonValue hp = object["HP"];
                QJsonValue blank3 = object["BLANK3"];

                if(i>=dataItems(true).size()+dataItems(false).size()){
                    troop = new Troop(this);
                }else{
                    troop= dataItems(team.toBool()).at(team.toBool()?troopRedIndex++:troopBlueIndex++);
                }
                troop->setStatList(0,name);
                troop->setStatList(1,type);
                troop->setStatList(2,maxHp);
                troop->setStatList(3,aVal);
                troop->setStatList(4,def);
                troop->setStatList(5,dDice);
                troop->setStatList(6,dmg);
                troop->setStatList(7,speed);
                troop->setStatList(8,range);
                troop->setStatList(9,aCharge);
                troop->setStatList(10,aCrowd);
                troop->setStatList(11,morale);
                troop->setStatList(12,aType);
                troop->setStatList(13,str);
                troop->setStatList(14,agi);
                troop->setStatList(15,end);
                troop->setStatList(16,will);
                troop->setStatList(17,row);
                troop->setStatList(18,col);
                troop->setStatList(19,id);
                troop->setStatList(20,team);
                troop->setStatList(21,moved);
                troop->setStatList(22,attacked);
                troop->setStatList(23,active);
                troop->setStatList(24,hp);
                troop->setStatList(25,blank3);
                setTokenIn(row.toInt(),col.toInt(),true);

                if(i>=dataItems(true).size()+dataItems(false).size()) addTroop(troop,team.toBool());      //dodawanie oddziału do drużyn

            }
            troop=nullptr;
            delete troop;
            troop=nullptr;
            m_dataBuffer->clear();
            emit troopChanged();
            qInfo() << "readFinnished";
        }
    }else{
        m_dataBuffer->clear();
        qInfo() << "ERROR";
        qInfo() << m_NetReply->errorString();
        writeConsole("Błąd! \r\n"+m_NetReply->errorString());
    }

    m_dataBuffer->clear();
    m_netManager->clearConnectionCache();
    m_NetReply->close();
    m_NetReply->deleteLater();
    m_NetReply = nullptr;
}

void DataSource::writeFinished()
{
    qInfo() << "writeFinished";
    m_netManager->clearConnectionCache();
    m_NetReply->close();
    m_NetReply->deleteLater();
    m_NetReply = nullptr;
}

QString DataSource::getCellColor(int row, int col) const
{
    return m_cellColor->at(row*m_cellRows+col);
}

bool DataSource::getTokenIn(int row, int column) const
{

    return m_tokenIn->at(m_cellRows*row+column);
}

short DataSource::getCellRowCount() const
{
    return m_cellRows;
}

short DataSource::getCellColumnCount() const
{
    return m_cellColumns;
}

QString DataSource::readConsole() const
{
    return m_console;
}

QString DataSource::readChat() const
{
    return m_chat;
}

bool DataSource::teamTurn() const
{
    return m_teamTurn;
}

short DataSource::turn() const
{
    return m_turn;
}


void DataSource::setTokenIn(int row, int col, bool val)
{
    m_tokenIn->replace(row*m_cellRows+col,val);
}

void DataSource::setCellColor(int row, int col, QString val)
{
    m_cellColor->replace(row*m_cellRows+col,val);
}

void DataSource::writeConsole(QString console)
{
    if(m_consoleLine==console) return;
    m_consoleLine = console;
    m_console=m_console+ "\r\n"+console;
    emit consoleChanged(m_console);
}

void DataSource::writeChat(QString chat)
{
    if(m_chatLine==chat) return;
    m_chatLine=chat;
    m_chat = m_chat + "\r\n" +chat;
    emit chatChanged(m_chat);
    changeItem();
}

void DataSource::setTeamTurn(bool teamTurn)
{
    if (m_teamTurn == teamTurn)
        return;

    m_teamTurn = teamTurn;
    emit teamTurnChanged(m_teamTurn);
}

void DataSource::setTurn(short turn)
{
    if (m_turn == turn)
        return;

    m_turn = turn;
    emit turnChanged(m_turn);
}

void DataSource::setInfo(QString info)
{
    m_console=m_console+ "\r\n"+info;
    emit consoleChanged(m_console);

}

QList<Troop*> DataSource::dataItems(bool team) const
{
    return team ? m_troopsRed: m_troopsBlue;
}

void DataSource::dataReadyRead()
{
    m_dataBuffer->append(m_NetReply->readAll());
}

void DataSource::addTroop(Troop *value,bool team)
{
    if(team){
        emit preInsertTroopRed();
        m_troopsRed.append(value) ;
        emit postInsertTroopRed();
    }else{
        emit preInsertTroopBlue();
        m_troopsBlue.append(value) ;
        emit postInsertTroopBlue();
    }
}

void DataSource::removeTroop(bool team, short index)
{
    if(team){
        emit preRemoveTroopRed(index);
        m_troopsRed.removeAt(index) ;
        emit postRemoveTroopRed();
    }else{
        emit preRemoveTroopBlue(index);
        m_troopsBlue.removeAt(index) ;
        emit postRemoveTroopBlue();
    }
}


