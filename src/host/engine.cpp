#include "engine.h"
#include "../lookup/lookup.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

namespace host {

class EnginePrivate {
public :
    lookup::Lookup* lookup ;
    int pageStartIndex ;
    int pageLength ;
    EnginePrivate() :
        lookup( new lookup::Lookup() ),
        pageStartIndex( 0 ) , 
        pageLength( 5 )
    {
    }
    ~EnginePrivate() {
        delete this->lookup ;
    }
} ;

Engine::Engine( QObject* parent ) :
    QObject( parent ) ,
    d_ptr( new EnginePrivate() )
{
}

Engine::~Engine() {
    delete this->d_ptr ;
}

void Engine::load( const QString& path ) {
    Q_D( Engine ) ;

    QFile file( path ) ;
    if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        QSet<QString> newKeySet ;
        QTextStream in( &file ) ;
        in.setCodec( "utf-8" ) ;
        while( !in.atEnd() ) {
            QString line( in.readLine() ) ;
            QStringList list( line.split( QChar(' ') ) ) ;
            if ( list.length() == 3 ) {
                QString key = list.at(0) ;
                QString word = list.at(1) ;
                qreal freq = list.at(2).toDouble() ;
                //qDebug() << key << word << freq ;
                if ( !d->lookup->dict.hash.contains( key ) )
                    newKeySet.insert( key ) ;
                d->lookup->dict.insert( key, word, freq ) ;
            }
        }
        foreach( const QString& key, newKeySet ) {
            if ( key.count( '\'' ) <= 0 )
                split::add_key( &(d->lookup->spliter.keySet), key ) ;
            fit::add_key( &(d->lookup->keyMap), key ) ;
        }
    }
}

void Engine::appendCode( QChar code ) {
    qDebug() << "appendCode" << code ;
    if ( code >= QChar('a') && code <= QChar('z') ) {
        Q_D( Engine ) ;
        d->lookup->appendCode( code ) ;
    }
}

void Engine::appendCode( const QString& code ) {
    if ( code.length() > 0 ) {
        this->appendCode( code[0] ) ;
    }
}

void Engine::popCode() {
    qDebug() << "popCode" ;
    Q_D( Engine ) ;
    d->lookup->popCode() ;
}

QString Engine::getCandidateString() {
    qDebug() << "getCand" ;
    Q_D( Engine ) ;
    QString str ;
    const lookup::Candidate* candidate ;
    qDebug() << "1" ;
    for ( int i = 0 ; i < d->pageLength ; i++ ) {
        qDebug() << "2" << i ;
        candidate = d->lookup->getCandidate( d->pageStartIndex + i ) ;
        qDebug() << "2" << i ;
        if ( candidate ) 
            str.append( lookup::get_word( candidate ) ) ;
        qDebug() << "2" << i ;
        str.append( QChar(',') ) ;
    }

    return str ;
}

}
