/***************************************************************************
             katalogman  -
                             -------------------
    begin                : 2004-12-09
    copyright            : (C) 2004 by Klaas Freitag
    email                : freitag@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// include files for Qt
#include <qstringlist.h>
#include <qsqlcursor.h>

// include files for KDE
#include <klocale.h>
#include <kdebug.h>

#include "kraftdb.h"
#include "katalogman.h"
#include "katalog.h"

KatalogMan::KatalogMan( )
{
    if( m_katalogDict == 0 )
        m_katalogDict = new QDict<Katalog>;
}


KatalogMan::~KatalogMan( )
{
    // delete m_katalogDict;
}

QStringList KatalogMan::allKatalogs()
{
    
    QStringList list;
 
    QSqlCursor cur( "CatalogSet" );
    cur.select( );
    while( cur.next() ) {
        list << cur.value("name").toString();
    }

    return list;
}

void KatalogMan::registerKatalog( Katalog *k )
{
    Katalog* kat = (*m_katalogDict)[k->getName()];

    if( kat ) {
        kdWarning() << "Katalog with same name already here -> deleting!" << endl;
        delete kat;
    } else {
        // not found, try to open it
        kdDebug() << "Katalog " << k->getName() << " registered and loading..." << endl;
        m_katalogDict->insert( k->getName(), k );
        k->load ();
    }
}

Katalog *KatalogMan::getKatalog(const QString& name)
{
    if( m_katalogDict == 0 )
        m_katalogDict = new QDict<Katalog>;
    
    Katalog* kat = (*m_katalogDict)[name];

    if( !kat ) {
        kdDebug() << "No katalog " << name << " found" << endl;
    } else {
        kdDebug() << "Returning existing katalog " << name << endl;
    }
    return kat;
}

QDict<Katalog> *KatalogMan::m_katalogDict = 0;

/* END */

