#include "daemon.h"
#include "http.h"
#include "engine.h"

#include <QTcpSocket>
#include <QNetworkSession>

#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QStringList>
#include <QTextStream>
#include <QDateTime>


namespace host {

class DaemonPrivate {
public :
    int port ;
    bool enabled ;
    QNetworkSession* session ;
    Engine* engine ;
    
    DaemonPrivate( int port ) :
        port( port ) ,
        enabled( true ) ,
        session( 0 ),
        engine( new Engine() )
    {
    }

    ~DaemonPrivate() {
        if ( this->session )
            delete this->session ;
        delete this->engine ;
    }

} ;

Daemon::Daemon( int port, QObject* parent ) :
    QTcpServer( parent ) ,
    d_ptr( new DaemonPrivate( port ) )
{
    qDebug() << "construct" ;
    Q_D( Daemon ) ;

    QNetworkConfigurationManager manager ;
    QNetworkConfiguration config = manager.defaultConfiguration() ;
    
    d->session = new QNetworkSession( config, this ) ;
    connect( d->session, SIGNAL(opened()), this, SLOT(opened()) ) ;
    d->session->open() ;

    //this->listen( QHostAddress::Any, port ) ;
}

Daemon::~Daemon() {
    delete this->d_ptr ;
}

void Daemon::opened() {
    qDebug() << "opened" ;
    Q_D( const Daemon ) ;

    this->listen( QHostAddress::Any, d->port ) ;
}

void Daemon::incomingConnection( int socketDescriptor ) {
    qDebug() << "incomingConnection" ;
    QTcpSocket* socket = new QTcpSocket(this);
    connect( socket, SIGNAL(readyRead()), this, SLOT(response()) ) ;
    connect( socket, SIGNAL(disconnected()), this, SLOT(discard()) ) ;
    socket->setSocketDescriptor( socketDescriptor ) ;
}

void Daemon::response() {
    qDebug() << "response" ;
    Q_D( Daemon ) ;

    QTcpSocket* socket = (QTcpSocket*)(this->sender()) ;
    Message message ;
    processRequest( socket, &message ) ;

    if ( message.type == Message::GET ) {
        QTextStream os( socket ) ;
        os.setAutoDetectUnicode( true ) ;
        switch ( message.method ) {
            case Message::getCand :
                os << d->engine->getCandidateString() ;
                break ;
            default :
                break ;
        }
        //QTextStream os( socket ) ;
        //os.setAutoDetectUnicode( true ) ;
        //os << "HTTP/1.0 200 Ok\r\n"
            //"Content-Type: text/html; charset=\"utf-8\"\r\n"
            //"\r\n"
            //"<h1>Nothing to see here</h1>\n"
            //<< QDateTime::currentDateTime().toString() << "\n";
    }
    else if ( message.type == Message::POST ) {
        switch ( message.method ) {
            case Message::appendCode :
                d->engine->appendCode( message.args.code ) ;
                break ;
            case Message::popCode :
                d->engine->popCode() ;
                break ;
            default :
                break ;
        }
        //qDebug() << socket->read( message.contentLength ) ;
    }

    socket->close() ;
    if ( socket->state() == QTcpSocket::UnconnectedState ) 
        delete socket;
}

void Daemon::discard() {
    qDebug() << "discard" ;
    QTcpSocket* socket = (QTcpSocket*)(this->sender()) ;
    socket->deleteLater() ;
}

void Daemon::load( const QString& path ) {
    qDebug() << "load start" ;
    Q_D( Daemon ) ;
    d->engine->load( path ) ;
    qDebug() << "load finish" ;
}

}
