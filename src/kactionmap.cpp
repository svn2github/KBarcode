/***************************************************************************
                          kactionmap.cpp  -  description
                             -------------------
    begin                : Fri Mai 19 2006
    copyright            : (C) 2006 by Dominik Seichter
    email                : domseichter@web.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kactionmap.h"

#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmenubar.h>
#include <qmenudata.h>
#include <qpixmap.h>
#include <q3popupmenu.h>
#include <qregexp.h>
#include <q3vbox.h>
//Added by qt3to4:
#include <QVBoxLayout>

#include <kaction.h>
#include <kapplication.h>
#include <k3listview.h>
#if KDE_VERSION >= 0x030500
#include <k3listviewsearchline.h>
#endif
#include <klocale.h>

class KListViewActionItem : public KListViewItem {
public:
    KListViewActionItem( KListView* parent, KAction* action )
        : KListViewItem( parent ), m_action( action )
        {
            QPixmap  pix;
            QSize    size    = QIcon::iconSize( QIcon::Large );
            QIcon iconset = m_action->iconSet( KIconLoader::Panel, KIconLoader::SizeLarge );
            QRegExp  regtag( "<[^>]*>" );

            pix = iconset.pixmap( QIcon::Large, QIcon::Normal ); // m_action->isEnabled() ? QIconSet::Normal : QIconSet::Disabled );
            if( pix.isNull() )
            {
                pix.resize( size );
                pix.fill( backgroundColor() );
            }
            else
            {
                if( pix.size() != size )
                {
                    pix = pix.convertToImage().smoothScale( size );
                }
            }

            setText( 0, m_action->plainText() );
            setText( 1, m_action->shortcutText() );
            // replace HTML tags in What's this help
            setText( 2, m_action->whatsThis().replace( regtag, "" ) );
            setPixmap( 0, pix );
        }

    void paintCell( QPainter *p, const QColorGroup &cg,
                                int column, int width, int alignment )
        {
            QColorGroup _cg( cg );
            QColor c = _cg.text();
            if( m_action && !m_action->isEnabled() )
                _cg.setColor( QColorGroup::Text, Qt::gray );
            
            KListViewItem::paintCell( p, _cg, column, width, alignment );
            _cg.setColor( QColorGroup::Text, c );
        }

    inline KAction* action() const 
        {
            return m_action;
        }

private:
    KAction* m_action;
};

KActionMapDlg::KActionMapDlg( KActionCollection* actions, QWidget* parent )
    : KDialogBase( parent, false, i18n("Action Map"), KDialogBase::Close, KDialogBase::Close )
{
    KVBox *page = makeVBoxMainWidget();

    new QLabel( i18n("Find and execute actions."), page );
    m_map = new KActionMap( actions, page );

    show();
}

void KActionMapDlg::updateEnabledState()
{
    m_map->updateEnabledState();
}

KActionMap::KActionMap( KActionCollection* actions, QWidget* parent )
    : QWidget( parent ), m_actions( actions ), m_showMenuTree( true ), m_grayOutItems( false )
{
    QVBoxLayout* layout = new QVBoxLayout( this );

    m_listView   = new KListView( this );
#if KDE_VERSION >= 0x030500
    m_searchLine = new KListViewSearchLineWidget( m_listView, this );
#endif

    m_listView->addColumn( i18n("Action") );
    m_listView->addColumn( i18n("Shortcut") );
    m_listView->addColumn( i18n("Description") );
    m_listView->setColumnWidthMode( 0, QListView::Maximum );
    m_listView->setColumnWidthMode( 1, QListView::Maximum );
    m_listView->setColumnWidthMode( 2, QListView::Manual );
    m_listView->setSorting( 0 );
    m_listView->setAllColumnsShowFocus( true );

#if KDE_VERSION >= 0x030500
    layout->addWidget( m_searchLine );
#endif 
    layout->addWidget( m_listView );

    connect( m_listView, SIGNAL( executed( QListViewItem* ) ), this, SLOT( slotExecuteAction( QListViewItem* ) ) );
    connect( actions, SIGNAL( inserted( KAction* ) ), this, SLOT( slotActionCollectionChanged() ) );
    connect( actions, SIGNAL( removed( KAction* ) ), this, SLOT( slotActionCollectionChanged() ) );
    slotActionCollectionChanged();
}

KActionMap::~KActionMap()
{

}

void KActionMap::slotActionCollectionChanged()
{
    KActionPtrList                 actions;
    KActionPtrList::const_iterator it;

    m_listView->clear();
    
    if( !m_actions )
        return;

    actions = m_actions->actions();
    it = actions.begin();

    while( it != actions.end() )
    {
        /*
        if( m_showMenuTree ) 
        {
        }
        */

        new KListViewActionItem( m_listView, (*it) );

        connect( *it, SIGNAL( enabled(bool) ), this, SLOT( updateEnabledState() ) );

        ++it;
    }
    
}

void KActionMap::slotExecuteAction( QListViewItem* item )
{
    KListViewActionItem* action = dynamic_cast<KListViewActionItem*>(item);
    if( !action )
        return;

    if( !action->action()->isEnabled() )
        return;

    action->action()->activate();
}

void KActionMap::updateEnabledState()
{
    m_listView->repaintContents();
}

#include "kactionmap.moc"
