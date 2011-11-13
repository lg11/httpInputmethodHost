#ifndef HTTP_H
#define HTTP_H

#include <QTcpSocket>

#include <QStringList>

namespace host {

struct Message {
    enum Type {
        UNDEFINED ,
        GET ,
        POST
    } ;
    enum Type type ;
    enum Method {
        foo ,
        getCand ,
        appendCode ,
        popCode ,
        clearCode ,
        setCode
    } ;
    enum Method method ;
    union Args {
        char code ;
        int index ;
    } ;
    union Args args ;
    //int contentLength ;
} ;

inline void processRequest( QTcpSocket* socket, Message* message ) {
    message->type = Message::UNDEFINED ;
    message->method = Message::foo ;
    message->args.index = 0 ;
    
    char buffer[16] ;
    int count = 0 ;

    count = socket->read( buffer, 4 ) ;
    if ( count > 0 ) {
        if ( buffer[0] == 'G' ) 
            message->type = Message::GET ;
        else if ( buffer[0] == 'P' )
            message->type = Message::POST ;
    }

    if ( message->type == Message::GET ) {
        count = socket->read( buffer, 8 ) ;
        if ( count > 4 ) {
            switch ( buffer[4] ) {
            case 'C' :
                message->method = Message::getCand ;
                break ;
            }
        }
    }
    else if ( message->type == Message::POST ) {
        count = socket->read( buffer, 4 ) ;
        if ( count > 2 ) {
            switch ( buffer[2] ) {
            case 'a' :
                message->method = Message::appendCode ;
                count = socket->read( buffer, 16 ) ;
                if ( count > 14 ) 
                    message->args.code = buffer[14] ;
                break ;
            case 'p' :
                message->method = Message::popCode ;
                break ;
            case 'c' :
                message->method = Message::clearCode ;
                break ;
            case 's' :
                message->method = Message::setCode ;
                break ;
            }
        }
    }
}

}

#endif // HTTP_H
