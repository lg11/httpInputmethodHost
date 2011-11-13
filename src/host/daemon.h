#ifndef DAEMON_H
#define DAEMON_H

#include <QTcpServer>

namespace host {

class DaemonPrivate ;

class Daemon : public QTcpServer {
    Q_OBJECT
public :
    explicit Daemon( int port, QObject* parent = 0 ) ;
    ~Daemon() ;
public slots :
    void opened() ;
    void incomingConnection( int socketDescriptor ) ;
    void response() ;
    void discard() ;

    void load( const QString& path ) ;
private :
    Q_DISABLE_COPY( Daemon ) ;
    Q_DECLARE_PRIVATE( Daemon ) ;
    DaemonPrivate* d_ptr ;
} ;

}
#endif // DAEMON_H
