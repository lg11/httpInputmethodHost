#include "daemon.h"
#include "engine.h"

#include <QCoreApplication>
#include <QSettings>
#include <QDir>

#include <QDebug>

int main( int argc, char** argv ) {
    QCoreApplication* app = new QCoreApplication( argc, argv ) ;
    host::Daemon* daemon = new host::Daemon( 8011 ) ;
    daemon->load( argv[1] ) ;
    return app->exec() ;
}

