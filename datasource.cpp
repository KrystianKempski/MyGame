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
    //m_refresh->start(2000);
}
void DataSource::fetchTroops()
{
    QNetworkRequest request;
    request.setUrl(m_apiEndpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("secret-key","$2b$10$iLVMet4iS5g49NNv/lD57uJp7WLNabTcWRBvQnZFPFJHCcpvFnYv.");
    request.setRawHeader("versioning","false");
  //  request.setRawHeader("key","AIzaSyBNOuhCUeadaiSwvWCvtdGQ-vziKqNSavM");
    m_NetReply=m_netManager->sendCustomRequest(request,"GET");
  //    qInfo() << m_NetReply->readAll();
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
   // request.setRawHeader("key","AIzaSyBNOuhCUeadaiSwvWCvtdGQ-vziKqNSavM");
    //  QJsonArray array;
    QJsonArray troopsArray;
    for(int i=0;i<m_troopsRed.size();i++){
        QJsonObject troopRedStats;
        Troop * troop = m_troopsRed.at(i);
        troop->write(troopRedStats);
        troopsArray.append(troopRedStats);
    }
    for(int i=0;i<m_troopsBlue.size();i++){
        QJsonObject troopBlueStats;
        Troop * troop = m_troopsBlue.at(i);
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
    QJsonDocument doc(data1);
    QByteArray jsonData= doc.toJson();
    //m_NetReply=m_netManager->put(request,jsonData);
    m_NetReply=m_netManager->sendCustomRequest(request,"PUT",jsonData);
   // qInfo() << m_NetReply;
    if(m_NetReply->error())
        qInfo() <<"błąd w changeItem" +m_NetReply->errorString();
    connect(m_NetReply,&QNetworkReply::finished,this,&DataSource::writeFinished);
}
void DataSource::readFinished()
{
    if(!m_NetReply->error()){
        //turn data into troop stats values
        QJsonDocument doc = QJsonDocument::fromJson(*m_dataBuffer);
       // qInfo() << *m_dataBuffer;
        QJsonObject data1 =doc.object();
        QJsonValue dataUpdate = data1["dataUpdate"];
        if(dataUpdate.toInt()!=m_dataUpdate) {                          //sprawdzanie czy zaszły zmiany na serwerze
            m_dataUpdate=dataUpdate.toInt();
            QJsonValue consoleLine = data1["consoleLine"];
            QJsonValue chatLine = data1["chatLine"];
            if(0!=dataItems(true).size()+dataItems(false).size()) {
                writeConsole(consoleLine.toString());
                writeChat(chatLine.toString());
            }
            QJsonArray postsArray = data1["posts"].toArray();
            QJsonObject troopsObject = postsArray.at(0).toObject();
            QJsonArray troopsArray;
            troopsArray = troopsObject["troops"].toArray();
            int troopRedIndex=0;
            int troopBlueIndex=0;
            m_tokenIn->fill(false); //
            for(int i=0;i<troopsArray.size();i++){
                Troop *troop;
                QJsonObject object = troopsArray.at(i).toObject();
                QJsonValue name = object["NAME"];
                QJsonValue type = object["TYPE"];
                QJsonValue hp = object["HP"];
                QJsonValue aVal = object["A_VAL"];
                QJsonValue def = object["DEF"];
                QJsonValue dDice = object["D_DICE"];
                QJsonValue dmg = object["DMG"];
                QJsonValue aType = object["A_TYPE"];
                QJsonValue speed = object["SPEED"];
                QJsonValue aCharge = object["A_CHARGE"];
                QJsonValue aCrowd = object["A_CROWD"];
                QJsonValue morale = object["MORALE"];
                QJsonValue range = object["RANGE"] ;
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
                QJsonValue maxHp = object["MAX_HP"];
                QJsonValue blank3 = object["BLANK3"];
                if(i>=dataItems(true).size()+dataItems(false).size()){
                    troop = new Troop(this);
                }else{
                    troop= dataItems(team.toBool()).at(team.toBool()?troopRedIndex++:troopBlueIndex++);
                }
                troop->setStatList(0,name);
                troop->setStatList(1,type);
                troop->setStatList(2,hp);
                troop->setStatList(3,aVal);
                troop->setStatList(4,def);
                troop->setStatList(5,dDice);
                troop->setStatList(6,dmg);
                troop->setStatList(7,aType);
                troop->setStatList(8,speed);
                troop->setStatList(9,aCharge);
                troop->setStatList(10,aCrowd);
                troop->setStatList(11,morale);
                troop->setStatList(12,range);
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
                troop->setStatList(24,maxHp);
                troop->setStatList(25,blank3);
                setTokenIn(row.toInt(),col.toInt(),true);
                if(i>=dataItems(true).size()+dataItems(false).size()) addTroop(troop,team.toBool());      //dodawanie oddziału do drużyn
            }
            m_dataBuffer->clear();
            //  writeConsole("pobrano jednostki");
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


QList<Troop*> DataSource::dataItems(bool team) const
{
    return team ? m_troopsRed: m_troopsBlue;
}

void DataSource::dataReadyRead()
{
    m_dataBuffer->append(m_NetReply->readAll());
}

void DataSource::addTroopRow()
{
    //    const QUrl API_ENDPOINT("http://localhost:3000/troops");
    //    qInfo() << API_ENDPOINT;
    //    QNetworkRequest request;
    //    request.setUrl(API_ENDPOINT);

    //    QJsonArray troopsArray;

    //    //  QJsonArray array;
    //    for(int i=0;i<m_troops.size();i++){

    //        QJsonObject staty1;
    //        Troop * troop = m_troops.at(i);
    //        troop->write(staty1);

    //        qInfo() << "name: " << troop->name();
    //        qInfo() << "staty1: " <<staty1;
    //        qInfo() << "statlist" <<troop->statList();
    //        troopsArray.append(staty1);
    //    }
    //    qInfo() << troopsArray;
    //    //doc.setArray(array);
    //    // QJsonObject data;
    //    // data.insert("troops",array);
    //    QJsonObject json;
    //    json.insert("troops",troopsArray);
    //    //        json.insert("title","xxxx");
    //    //        json.insert("","xxxx");
    //    //        QJsonDocument jsonDoc(json);
    //    QJsonDocument doc(json);
    //    qInfo() << "doc : " << doc;
    //    QByteArray jsonData= doc.toJson();
    //    // QByteArray data= doc.toJson();
    //    qInfo() <<"data: "<< jsonData
    //              ;
    //    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json; charset=UTF-8");
    //    //    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(jsonData.size()));
    //    m_NetReply=m_netManager->post(request,jsonData);
    //    qInfo() <<m_NetReply->errorString();

    //    connect(m_NetReply,&QNetworkReply::finished,this,&DataSource::writeFinished);
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

//void DataSource::removeTroop(int index)
//{
//    emit preRemoveTroop(index);
//    m_troops.removeAt(index);
//    emit postRemoveTroop();
//}

//void DataSource::removeLastTroop()
//{
//    if(!m_troops.isEmpty()){
//        m_troops.removeLast();
//    }
//}


