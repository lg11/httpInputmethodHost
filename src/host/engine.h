#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

namespace host {

class EnginePrivate ;

class Engine : public QObject {
    Q_OBJECT
public :
    explicit Engine( QObject* parent = 0 ) ;
    ~Engine() ;
public slots :
    void load( const QString& path ) ;
    void appendCode( QChar code ) ;
    void appendCode( const QString& code ) ;
    void popCode() ;
    QString getCandidateString() ;
private :
    Q_DISABLE_COPY( Engine ) ;
    Q_DECLARE_PRIVATE( Engine ) ;
    EnginePrivate* d_ptr ;
} ;

}

#endif // ENGINE_H
