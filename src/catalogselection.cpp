/***************************************************************************
        katalogselection  - widget to select catalog entries from
                             -------------------
    begin                : 2006-08-30
    copyright            : (C) 2006 by Klaas Freitag
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
#include "catalogselection.h"
#include "katalogman.h"
#include "templkatalog.h"
#include "templkataloglistview.h"
#include "matkatalog.h"

#include <klocale.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kaction.h>
#include <kactioncollection.h>

#include <qsizepolicy.h>
#include <qcombobox.h>
#include <qwidgetstack.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qpopupmenu.h>

CatalogSelection::CatalogSelection( QWidget *parent )
  :QVBox( parent ),
   mCatalogSelector( 0 ),
   mWidgets( 0 ),
   mActions( 0 ),
   mAcAddToDoc( 0 )
{
  setMargin( KDialog::marginHint() );
  setSpacing( KDialog::spacingHint() );

  QHBox *hb = new QHBox( this );
  QWidget *spaceEater = new QWidget( hb );
  spaceEater->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
  QLabel *l = new QLabel( i18n( "Selected &Catalog:" ), hb );
  mCatalogSelector = new QComboBox( hb );
  connect( mCatalogSelector, SIGNAL( activated( const QString& ) ),
           this,  SLOT( slotSelectCatalog( const QString& ) ) );
  l->setBuddy( mCatalogSelector );
  mWidgets  = new QWidgetStack( this );
  mWidgets->setSizePolicy( QSizePolicy( QSizePolicy::Expanding,  QSizePolicy::Expanding ) );

  initActions();
  setupCatalogList();
}

void CatalogSelection::setupCatalogList()
{
  QStringList katalogNames = KatalogMan::self()->allKatalogs();
  mCatalogSelector->insertStringList( katalogNames );
  slotSelectCatalog( katalogNames[0] );
}

void CatalogSelection::initActions()
{
  mActions     = new KActionCollection( this );
  mAcAddToDoc  = new KAction( i18n("&Add to document"), "back", 0, this,
                              SLOT( slotAppendToDoc() ), mActions, "appendToDoc");

}

void CatalogSelection::slotAppendToDoc()
{
  const QString currentCat = mCatalogSelector->currentText();

  kdDebug() << "Insert a template from " << currentCat << " to document " << endl;

  Katalog *kat = KatalogMan::self()->getKatalog( currentCat );

  if ( ! kat ) {
    kdError() << "Could not find catalog " << currentCat << endl;
  }

  if ( kat->type() == TemplateKatalog ) {

    TemplKatalogListView *listview = static_cast<TemplKatalogListView*> ( mWidgetDict[currentCat] );
    FloskelTemplate *currTempl = static_cast<FloskelTemplate*> (listview->currentItemData());
  }

}

void CatalogSelection::slotSelectCatalog( const QString& katName )
{
  Katalog *kat = KatalogMan::self()->getKatalog( katName );

  if ( ! kat ) {
    const QString type = KatalogMan::self()->catalogTypeString( katName );

    kdDebug() << "Catalog type for cat " << katName << " is " << type << endl;
    if ( type == "TemplCatalog" ) {
      kat = new TemplKatalog( katName );
    } else if ( type == "MaterialCatalog"  ) {
      kat = new MatKatalog( katName );
    }

    if ( kat ) {
      KatalogMan::self()->registerKatalog( kat );
    } else {
      kdError() << "Could not find a catalog type for catname " << katName << endl;
    }
  }

  if ( kat ) {
    if ( mWidgetDict[katName] ) {
      mWidgets->raiseWidget( mWidgetDict[katName] );
    } else {
      if ( kat->type() == TemplateKatalog ) {
        TemplKatalogListView *tmpllistview = new TemplKatalogListView( this );

        tmpllistview->setShowCalcParts( false );
        tmpllistview->addCatalogDisplay( katName );
        mAcAddToDoc->plug( tmpllistview->contextMenu() );

        mWidgets->addWidget( tmpllistview );
        mWidgetDict.insert(  katName, tmpllistview );
        kdDebug() << "Creating a selection list for catalog " << katName << endl;
      }
    }
  }
}

#include "catalogselection.moc"
