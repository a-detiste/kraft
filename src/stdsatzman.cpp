/***************************************************************************
             stdsatzman  -
                             -------------------
    begin                : 2004-13-09
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
#include <qstring.h>
// include files for KDE
#include <klocale.h>
#include <kdebug.h>

#include "stdsatzman.h"
#include "kraftdb.h"
#include <qsqlquery.h>
#include <qsqlcursor.h>

StdSatz::StdSatz():
    m_dbId(0)
{

}

StdSatz::StdSatz( int id ):
    m_dbId(id)
{

}

StdSatz::StdSatz( int id, const QString& name, Geld g ):
    m_dbId(id),
    m_name(name),
    m_value(g)
{

}

/*
 * ********** Stundensatz Manager **********
 */
StdSatzMan::StdSatzMan( )
{
    load();
}

QStringList StdSatzMan::allStdSaetze()
{
    QStringList list;
    load();

    StdSatzValueVector::iterator it;
    for( it = m_stdSaetze->begin(); it != m_stdSaetze->end(); ++it )
    {
        QString n = (*it).getName();
        if( !n.isEmpty())
            list << n;
    }
    return list;

}

StdSatz  StdSatzMan::getStdSatz( const QString& name )
{
    load();
    StdSatzValueVector::iterator it;
    for( it = m_stdSaetze->begin(); it != m_stdSaetze->end(); ++it )
    {
        if( (*it).getName() == name ) return (*it);
    }
    return StdSatz();
}

StdSatz StdSatzMan::getStdSatz( dbID id )
{
    load();
    StdSatzValueVector::iterator it;
    for( it = m_stdSaetze->begin(); it != m_stdSaetze->end(); ++it )
    {
        dbID dbid = (*it).getId();
        if( dbid == id ) return (*it);
    }
    return StdSatz();
}

StdSatzMan::~StdSatzMan( )
{

}

void StdSatzMan::load()
{
    if( ! KraftDB::getDB() )
    {
        kdDebug() << "Have a problem: No database!" << endl;
        return;
    }

    if( m_stdSaetze )
    {
        /* Die Daten sind bereits geladen, Methode verlassen */
        return;
    }
    else
    {
        /* noetige Groesse rausfinden */
        int max = -1;

        QSqlQuery q("SELECT count(*) from stdSaetze;");
        if( q.isActive())
        {
            q.next();
            max = q.value(0).toInt();
        }
        kdDebug() << "Groesse fuer Stundensatzliste: " << max << endl;

        m_stdSaetze = new StdSatzValueVector(max);
    }


    /* Daten laden */
    if( m_stdSaetze )
    {
        QSqlCursor cur("stdSaetze");
        cur.setMode( QSqlCursor::ReadOnly );

        // Create an index that sorts from high values for einheitID down.
        // that makes at least on resize of the vector.
        QSqlIndex indx = cur.index( "sortKey" );
        // indx.setDescending ( 0, true );

        cur.select(indx);
        while( cur.next())
        {
            int satzID = cur.value("stdSaetzeID").toInt();
            kdDebug() << "Neue StdSatz ID " << satzID << endl;
            // resize if index is to big.
            StdSatz ss( satzID, QString::fromUtf8(cur.value("name").toCString()),
                        Geld( cur.value("price").toDouble()));

            m_stdSaetze->append(ss);
        }
    }
}

StdSatzValueVector *StdSatzMan::m_stdSaetze = 0;


/* END */

