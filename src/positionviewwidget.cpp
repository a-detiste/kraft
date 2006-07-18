/***************************************************************************
        positionviewwidget - inherited class for doc position views.
                             -------------------
    begin                : 2006-02-20
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

#include <qlabel.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qcolor.h>
#include <qlayout.h>
#include <kglobal.h>
#include <klocale.h>
#include <knuminput.h>
#include <ktextedit.h>
#include <kpopupmenu.h>
#include <kiconloader.h>

#include "docposition.h"
#include "positionviewwidget.h"
#include "unitmanager.h"

PositionViewWidget::PositionViewWidget()
 : positionWidget(),
 mModified( false ),
 m_skipModifiedSignal( false ),
 mToDelete(false),
 mOrdNumber(0),
 mExecPopup( new KPopupMenu( this ) ) ,
 mState( Active )
{
  const QString currSymbol = KGlobal().locale()->currencySymbol();
  m_sbUnitPrice->setPrefix( currSymbol + " " );
  pbExec->setToggleButton( false );
  connect( m_sbAmount, SIGNAL( valueChanged( double )),
             this, SLOT( slotRefreshPrice( ) ) );
  connect( m_sbUnitPrice, SIGNAL( valueChanged( double )),
             this, SLOT( slotRefreshPrice( ) ) );  
  connect( pbExec, SIGNAL( pressed() ), 
             this,  SLOT( slotExecButtonPressed() ) );
  
  /* modified signals */
  connect( m_cbUnit,      SIGNAL( activated(int) ), this,      SLOT( slotModified() ) );
  connect( m_teFloskel,   SIGNAL( textChanged() ), this,       SLOT( slotModified() ) );
  connect( m_sbAmount,    SIGNAL( valueChanged(double)), this, SLOT( slotModified() ) );
  connect( m_sbUnitPrice, SIGNAL( valueChanged(double)), this, SLOT( slotModified() ) );
  
  mExecPopup->insertTitle( i18n("Position Actions") );
  mExecPopup->insertItem(  SmallIconSet("up"), 
                           i18n("Move Up"),         this, SIGNAL( moveUp() ) );
  mExecPopup->insertItem(  SmallIconSet("down"), 
                           i18n("Move Down"),       this, SIGNAL( moveDown() ) );
  mLockId = mExecPopup->insertItem(  SmallIconSet("encrypted"),
                           i18n("Lock Position"),   this, SIGNAL( lockPosition() ) );
  mUnlockId = mExecPopup->insertItem(  SmallIconSet("decrypted"),
                           i18n("Unlock Position"), this, SIGNAL( unlockPosition() ) );
  mDeleteId = mExecPopup->insertItem(  SmallIconSet("remove"), 
                           i18n("Delete Position"), this, SIGNAL( deletePosition() ) );
  
  connect( this, SIGNAL( lockPosition() ),   this, SLOT( slotLockPosition() ) );
  connect( this, SIGNAL( unlockPosition() ), this, SLOT( slotUnlockPosition() ) );
  
  connect( mExecPopup, SIGNAL( aboutToShow() ), this, SLOT( slotMenuAboutToShow() ) );
  connect( mExecPopup, SIGNAL( aboutToHide() ), this, SLOT( slotMenuAboutToHide() ) );
             
  mExecPopup->setItemEnabled( mUnlockId, false );
  lStatus->setPixmap( QPixmap() );
}

void PositionViewWidget::setPosition( DocPositionBase *dp )
{
  if( ! dp ) return;
  if( dp->type() == DocPositionBase::Position ) {
    DocPosition *pos = static_cast<DocPosition*>(dp);
    m_skipModifiedSignal = true;
    // m_labelPosition->setText( QString("%1.").arg( mPositionPtr->position() ) );
  
    m_teFloskel->setText( pos->text() );
  
    m_sbAmount->setValue( pos->amount() );
    m_cbUnit->setCurrentText( pos->unit().einheitSingular() );
    m_sbUnitPrice->setValue( pos->unitPrice().toDouble() );
    lStatus->hide();
    slotSetOverallPrice( pos->overallPrice().toDouble() );
    mPositionPtr = dp;
    m_skipModifiedSignal = false;
  }
}

void PositionViewWidget::slotExecButtonPressed()
{
  kdDebug() << "Opening Context Menu over exec button" << endl;
  
  // set bg-color
  mExecPopup->popup( QWidget::mapToGlobal( pbExec->pos() ) );
  
}

void PositionViewWidget::slotMenuAboutToShow()
{
  // setPaletteBackground( QColor( "blue" ) );
  setBackgroundMode( Qt::PaletteMid );
}

void PositionViewWidget::slotMenuAboutToHide()
{
  kdDebug() << "Set normal again" << endl;
  setBackgroundMode( Qt::PaletteBackground );
}

void PositionViewWidget::slotLockPosition( ) 
{
  slotSetState( Locked );
}

void PositionViewWidget::slotUnlockPosition( ) 
{
  slotSetState( Active );
}

QString PositionViewWidget::stateString( const State& state ) const
{
  QString str;
  
  if( state == Active ) {
    str = i18n( "Active" );
  } else if( state == New ) {
    str = i18n( "New" );
  } else if( state == Deleted ) {
    str = i18n( "Deleted" );
  } else if( state == Locked ) {
    str = i18n( "Locked" );
  } else {
    str = i18n( "Unknown" );
  }
  return str;  
}

void PositionViewWidget::slotSetState( State state )
{
  mState = state;
  kdDebug() << "Setting new widget state " << stateString( state ) << endl;
  if( state == Active ) {
    mExecPopup->setItemEnabled( mLockId, true);
    mExecPopup->setItemEnabled( mUnlockId, false );
    
    lStatus->hide();
    lStatus->setPixmap( QPixmap() );
    mToDelete = false;
    slotSetEnabled( true );
  } else if( state == New ) {
    lStatus->setPixmap( SmallIcon( "filenew" ) );
    lStatus->show();    
  } else if( state == Deleted ) {
    lStatus->setPixmap( SmallIcon( "remove" ) );
    lStatus->show();
    mToDelete = true;
    slotSetEnabled( false );
  } else if( state == Locked ) {
    mExecPopup->setItemEnabled( mLockId, false );
    mExecPopup->setItemEnabled( mUnlockId, true );  
    slotSetEnabled( false );
    lStatus->setPixmap( SmallIcon( "encrypted" ) );
    lStatus->show();
  }
}

void PositionViewWidget::setOrdNumber( int o )
{
  mOrdNumber = o;
  m_labelPosition->setText( QString("%1.").arg( mOrdNumber ) );
}

void PositionViewWidget::slotSetEnabled( bool doit )
{
  if( !doit ) {
    m_sbAmount->setEnabled( false );
    m_sbUnitPrice->setEnabled( false );
    m_labelPosition->setEnabled( false );
    m_teFloskel->setEnabled( false );
    m_sumLabel->setEnabled( false );
    m_cbUnit->setEnabled( false );
  } else {
    m_sbAmount->setEnabled( true );
    m_sbUnitPrice->setEnabled( true );
    m_labelPosition->setEnabled( true );
    m_teFloskel->setEnabled( true );
    m_sumLabel->setEnabled( true );
    m_cbUnit->setEnabled( true );
  }
}

void PositionViewWidget::slotRefreshPrice()
{
    double amount = m_sbAmount->value();
    double price = m_sbUnitPrice->value();
    
    slotSetOverallPrice( amount * price );
}

void PositionViewWidget::slotSetOverallPrice( double p )
{
    const QString moneyStr = KGlobal().locale()->formatMoney( p );
    m_sumLabel->setText( moneyStr );
}

void PositionViewWidget::slotModified() 
{
    if( mModified ) return;
    if( m_skipModifiedSignal ) return;
    kdDebug() << "Modified Position!" << endl;

    mModified = true;
    emit positionModified();
}

PositionViewWidget::~PositionViewWidget()
{
}

PositionViewWidgetList::PositionViewWidgetList()
  : QPtrList<PositionViewWidget>()
{
  
}

PositionViewWidget* PositionViewWidgetList::widgetFromPosition( DocPositionGuardedPtr ptr)
{
  PositionViewWidget *pvw = 0;
  
  for( pvw = first(); pvw; pvw = next() ) {
    if( pvw->position() == ptr ) return pvw;
  }
  return 0;
}

#include "positionviewwidget.moc"

