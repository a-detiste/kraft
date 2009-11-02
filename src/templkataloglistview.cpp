
/***************************************************************************
             templkataloglistview  - template katalog listview.
                             -------------------
    begin                : 2005-07-09
    copyright            : (C) 2005 by Klaas Freitag
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

#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>

#include "templkataloglistview.h"
#include "portal.h"
#include "kraftglobals.h"
#include "katalog.h"
#include "katalogman.h"
#include "kataloglistview.h"
#include "materialcalcpart.h"
#include "stockmaterial.h"
#include "templkatalog.h"
#include "zeitcalcpart.h"
#include "docposition.h"

TemplKatalogListView::TemplKatalogListView(QWidget *w)
    : KatalogListView(w),
      mShowCalcParts( true )
{
  QStringList labels;
  labels << i18n("Template");
  labels << i18n("Price");
  labels << i18n("Calc. Type");

  setHeaderLabels(labels);

//    setAcceptDrops( true );
//    setDragEnabled( true );
//    setDropVisualizer(true);
}

/*
 * This class adds a complete catalog and fills the view. It gets the
 * catalog from KatalogMan, iterates over the catalog chapters and
 * fills in the templates.
 */
void TemplKatalogListView::addCatalogDisplay( const QString& katName )
{
  KatalogListView::addCatalogDisplay(katName);

  TemplKatalog* catalog = static_cast<TemplKatalog*>(KatalogMan::self()->getKatalog(katName));

  if ( !catalog ) {
    kError() << "Could not load catalog " << katName << endl;
    return;
  }

  setupChapters();

  const QStringList chapters = catalog->getKatalogChapters();
  for ( QStringList::ConstIterator it = chapters.begin(); it != chapters.end(); ++it ) {
    QString chapter = *it;
    QTreeWidgetItem *katItem = chapterItem(chapter);
    kDebug() << "KatItem is " << katItem << " for chapter " << chapter << endl;
    FloskelTemplateList katList = catalog->getFlosTemplates(chapter);
    // kDebug() << "Items in chapter " << chapter << ": " << katList.count() << endl;
    FloskelTemplateListIterator flosIt( katList );

    while( flosIt.hasNext() ) {
      FloskelTemplate *tmpl = flosIt.next();

      /* create a ew item as the child of katalog entry */
      addFlosTemplate( katItem, tmpl );
      if ( mShowCalcParts )
        addCalcParts( tmpl );
    }
  }
}

/*
 * add a single template to the view with setting icon etc.
 */
QTreeWidgetItem* TemplKatalogListView::addFlosTemplate( QTreeWidgetItem *parentItem, FloskelTemplate *tmpl )
{
    if( ! parentItem ) parentItem = m_root;
    QTreeWidgetItem *listItem = new QTreeWidgetItem( parentItem );
    slFreshupItem( listItem, tmpl);
    tmpl->setListViewItem( listItem );
    listItem->setFlags( Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled );
    // listItem->setMultiLinesEnabled(true);

    if( tmpl->calcKind() == CatalogTemplate::ManualPrice )
    {
        listItem->setIcon(0, SmallIcon("roll"));
    }
    else
    {
        listItem->setIcon(0, SmallIcon("kcalc"));
    }
    // store the connection between the listviewitem and the template in a dict.
    m_dataDict.insert( listItem, tmpl );

    return listItem;
}

void TemplKatalogListView::slFreshupItem( QTreeWidgetItem *item, FloskelTemplate *tmpl, bool remChildren )
{
  if( !(item && tmpl) ) return;

  Geld g     = tmpl->unitPrice();
  QString ck = tmpl->calcKindString();
  QString t  = Portal::textWrap(tmpl->getText(), 60);

  item->setText( 0, t );
  QString h;
  h = QString( "%1 / %2" ).arg( g.toString( catalog()->locale() ) )
      .arg( tmpl->einheit().einheitSingular() );
  item->setText( 1,  h );
  item->setText( 2, ck );
  // item->setText( 4, QString::number(tmpl->getTemplID()));

  if( remChildren ) {
    /* remove all children and insert them again afterwards.
        * That updates the view
      */
    for( int i = 0; i < item->childCount(); i++ ) {
      QTreeWidgetItem *it = item->child(i);
      if( it )  {
        item->removeChild( it );
        delete it;
      }
    }

    addCalcParts(tmpl); // Insert to update the view again.
  }
}


void TemplKatalogListView::addCalcParts( FloskelTemplate *tmpl )
{
  QTreeWidgetItem *item = tmpl->getListViewItem();

  if( ! item ) return;

  CalcPartList parts = tmpl->getCalcPartsList();
  CalcPartListIterator it(parts);

  while( it.hasNext() ) {
    CalcPart *cp = it.next();
    QString title = cp->getName();
    QString type = cp->getType();
    kDebug() << "Type is " << type << endl;
    if( type  == KALKPART_TIME ) {
      ZeitCalcPart *zcp = static_cast<ZeitCalcPart*>(cp);
      StdSatz stdsatz = zcp->getStundensatz();
      title = QString( "%1, %2 Min. %3" ).arg( cp->getName() )
              .arg( QString::number( zcp->getMinuten() ) )
              .arg( stdsatz.getName() );
    }

    QStringList list;
    list << title;
    list << cp->kosten().toString( catalog()->locale() );
    list << cp->getType();
    QTreeWidgetItem *cpItem =  new QTreeWidgetItem( item, list );

    /* in case of material, add items for the materials calculated for the
        * template
        */
    if( type == KALKPART_MATERIAL )
    {
      MaterialCalcPart *mcp = static_cast<MaterialCalcPart*>( cp );
      StockMaterialList mats =  mcp->getCalcMaterialList();

      StockMaterialListIterator it( mats );
      while( it.hasNext() ) {
        StockMaterial *mat = it.next();

        Geld g = mcp->getPriceForMaterial(mat);
        QString t = mat->name();
        double usedAmount = mcp->getCalcAmount(mat);
        Einheit e = mat->getUnit();

        t = QString( "%1 %2 of %3 %4 %5" ).arg( usedAmount )
            .arg( e.einheit( usedAmount ) )
            .arg( mat->getAmountPerPack() )
            .arg( e.einheit( mat->getAmountPerPack() ) )
            .arg( t );
        QStringList li;
        li << t;
        li << g.toString( catalog()->locale() );
        (void) new QTreeWidgetItem( cpItem, li );
      }
    }
  }
}

void TemplKatalogListView::setShowCalcParts( bool on )
{
  mShowCalcParts = on;
}

bool TemplKatalogListView::showCalcParts()
{
  return mShowCalcParts;
}

TemplKatalogListView::~TemplKatalogListView()
{
}

DocPosition TemplKatalogListView::itemToDocPosition( QTreeWidgetItem *it )
{
  DocPosition pos;
  if ( ! it ) {
    it = currentItem();
  }

  if ( ! it ) return pos;

  FloskelTemplate *flos = static_cast<FloskelTemplate*>( m_dataDict[ it ] );

  if ( flos ) {
    pos.setText( flos->getText() );
    pos.setUnit( flos->einheit() );
    pos.setUnitPrice( flos->unitPrice() );
  } else {
    kDebug() << "Can not find a template for the item" << endl;
  }

  return pos;
}

CalcPartList TemplKatalogListView::itemsCalcParts( QTreeWidgetItem* it )
{
  CalcPartList cpList;

  if ( ! it ) {
    it = currentItem();
  }

  if ( ! it ) return cpList;

  FloskelTemplate *flos = static_cast<FloskelTemplate*>( m_dataDict[ it ] );
  if ( flos ) {
    kDebug() << "We have calc parts: " << flos->getCalcPartsList().count()<< endl;
    cpList = flos->getCalcPartsList();
  }
  return cpList;
}


