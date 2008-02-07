/***************************************************************************
                          tokendialog.h  -  description
                             -------------------
    begin                : Sat Oct 23 2004
    copyright            : (C) 2004 by Dominik Seichter
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

#include "tokendialog.h"
#include "tokenprovider.h"
#include "sqltables.h"
#include "dstextedit.h"

#include <klineedit.h>
#include <klistbox.h>
#include <klistview.h>
#include <klocale.h>

#include <qhbox.h> 
#include <qlabel.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qtooltip.h>
#include <qvaluelist.h>
#include <qvbox.h>
#include <qvbuttongroup.h>
#include <qwidgetstack.h>
#include <qradiobutton.h>
#include <kcombobox.h>
#include <qtextbrowser.h>
#include <kpushbutton.h>

TokenDialog::TokenDialog(TokenProvider* token ,QWidget *parent, const char *name)
    : KWizard( parent, name ), m_token( token )
{
    m_custom_tokens = m_token->listUserVars();

    setupPage1();
    setupPage2();
    setupPage3();

    setupStackPage1();
    setupStackPage2();
 
    setupStack2Page1();
    setupStack2Page2();
    setupStack2Page3();
    setupStack2Page4();
    setupStack2Page5();

    enableControls();
}

void TokenDialog::setupPage1()
{
    QVButtonGroup* page = new QVButtonGroup( i18n("What do you want to insert?") );

    radioFixed = new QRadioButton( i18n("Insert a &fixed data field"), page );
    radioCustom = new QRadioButton( i18n("Insert a &custom SQL query, variable or JavaScript function"), page );
    
    radioFixed->setChecked( true );

    addPage( page, i18n("Step 1 of 3") );
}

void TokenDialog::setupPage2()
{
    page2 = new QWidgetStack();

    addPage( page2, i18n("Step 2 of 3") );
}

void TokenDialog::setupPage3()
{
    page3 = new QWidgetStack();

    addPage( page3, i18n("Step 3 of 3") );
}

void TokenDialog::setupStackPage1()
{
    stackPage1 = new QVBox();

    QVButtonGroup* group = new QVButtonGroup( i18n("What do you want to insert?"), stackPage1 );
    radioAll = new QRadioButton( i18n("&Select from a list of all tokens"), group );
    radioLabel = new QRadioButton( i18n("Insert printing &informations"), group );
    radioSQL = new QRadioButton( i18n("&Insert a database field"), group );
    radioDate = new QRadioButton( i18n("Insert a &date/time function"), group );
    radioAddress = new QRadioButton( i18n("Insert an &addressbook field"), group );

    radioAll->setChecked( true );

    page2->addWidget( stackPage1 );
}

void TokenDialog::setupStackPage2()
{
    stackPage2 = new QVBox();

    QVButtonGroup* group = new QVButtonGroup( i18n("What do you want to insert?"), stackPage2 );

    radioVariable = new QRadioButton( i18n("Insert a custom &variable"), group );
    radioSQLQuery = new QRadioButton( i18n("Insert a &SQL query"), group );
    radioJavaScript = new QRadioButton( i18n("Insert a &JavaScript function"), group );

    radioVariable->setChecked( true );

    connect( radioVariable, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( radioSQLQuery, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( radioJavaScript, SIGNAL( clicked() ), this, SLOT( enableControls() ) );

    page2->addWidget( stackPage2 );
}

void TokenDialog::setupStack2Page1()
{
    stack2Page1 = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout( stack2Page1 );
    QSplitter* splitter = new QSplitter( stack2Page1 );
    layout->addWidget( splitter );
    
    QVBox* left = new QVBox( splitter );
    QVBox* right = new QVBox( splitter );
    
    QLabel* label = new QLabel( i18n("&Category:"), left );
    category = new KListBox( left );
    label->setBuddy( category );

    label = new QLabel( i18n("&Token:"), right );
    allList = new KListView( right );
    allList->addColumn( i18n("Token"), 0 );
    allList->addColumn( i18n("Description"), 1 );
    allList->setColumnWidthMode( 0, QListView::Maximum );
    allList->setColumnWidthMode( 1, QListView::Maximum );
    label->setBuddy( allList );
    label = new QLabel( i18n("&Custom Expression to be inserted in the token."), right );
    lineEdit = new KLineEdit( right );
    lineEdit->setEnabled( false );
    label->setBuddy( lineEdit );

    QToolTip::add( lineEdit, i18n("<qt>Certain tokens, like for exaple the sqlquery token need arguments. "
				  "In the case of the sqlquery token, the sure has to enter a sql query in "
				  "this text field.</qt>" ) );

    QValueList<int> sizes;
    int w = (width() / 4);
    sizes << w << w * 3;
    
    left->setStretchFactor( category, 2 );
    right->setStretchFactor( allList, 2 );
    splitter->setSizes( sizes );

    connect( category, SIGNAL( executed( QListBoxItem* ) ), this, SLOT( categoryChanged( QListBoxItem* ) ) );
    connect( allList, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( itemChanged( QListViewItem* ) ) );

    initAll();

    page3->addWidget( stack2Page1 );
}

void TokenDialog::setupStack2Page2()
{
    stack2Page2 = new QVBox();

    labelList = new KListView( stack2Page2 );
    labelList->addColumn( i18n("Token"), 0 );
    labelList->addColumn( i18n("Description"), 1 );
    labelList->setColumnWidthMode( 0, QListView::Maximum );
    labelList->setColumnWidthMode( 1, QListView::Maximum );

    connect( labelList, SIGNAL( selectionChanged() ), this, SLOT( enableControls() ) );
    connect( labelList, SIGNAL( doubleClicked( QListViewItem *, const QPoint &, int ) ), this, SLOT( accept() ) );

    page3->addWidget( stack2Page2 );
}

void TokenDialog::setupStack2Page3() 
{
    stack2Page3 = new QVButtonGroup();

    radioVariableNew = new QRadioButton( i18n("&Create a new custom variable"), stack2Page3 );
    editVariable = new KLineEdit( stack2Page3 );

    radioVariableExisting = new QRadioButton( i18n("&Insert an existing custom variable"), stack2Page3 );
    listVariable = new KListBox( stack2Page3 );

    radioVariableNew->setChecked( true );

    if( m_token )
        listVariable->insertStringList( m_token->listUserVars() );

    if( !listVariable->count() )
        radioVariableExisting->setEnabled( false );

    connect( radioVariableNew, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( radioVariableExisting, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( editVariable, SIGNAL( textChanged( const QString & ) ), this, SLOT( enableControls() ) );
    connect( listVariable, SIGNAL( highlighted( int ) ), this, SLOT( enableControls() ) );
    connect( listVariable, SIGNAL( doubleClicked( QListBoxItem*, const QPoint & ) ), this, SLOT( accept() ) );

    page3->addWidget( stack2Page3 );
}

void TokenDialog::setupStack2Page4() 
{
    stack2Page4 = new QVBox();

    if( !SqlTables::isConnected() )
        new QLabel( i18n("<qt><b>No SQL connection found!</b><br>You can build a query, "
                         "but you will not be able to execute or test it right now.<br></qt>"), stack2Page4 );
    
    QHBox* hbox = new QHBox( stack2Page4 );

    QLabel* label = new QLabel( i18n("&SQL Query:"), hbox );
    editQuery = new KLineEdit( hbox );
    buttonQuery = new KPushButton( i18n("&Test"), hbox );
    label->setBuddy( editQuery );

    hbox->setStretchFactor( editQuery, 2 );

    new QLabel( i18n("Query test results:"), stack2Page4 );
    textQueryResults = new QTextBrowser( stack2Page4 );
    textQueryResults->setReadOnly( true );

    connect( buttonQuery, SIGNAL( clicked() ), this, SLOT( testQuery() ) );
    connect( editQuery, SIGNAL( textChanged( const QString & ) ), this, SLOT( enableControls() ) );
        
    page3->addWidget( stack2Page4 );
}

void TokenDialog::setupStack2Page5() 
{
    stack2Page5 = new QVBox();

    editJavaScript = new DSTextEdit( stack2Page5 );
    editJavaScript->setText( i18n("/* Place your JavaScript code into this text field. */\n") );

    connect( editJavaScript, SIGNAL( textChanged() ), SLOT( enableControls() ) );

    page3->addWidget( stack2Page5 );
}

void TokenDialog::accept()
{
    if( radioCustom->isChecked() )
    {
        if( radioVariable->isChecked() ) 
        {
            if( radioVariableNew->isChecked() ) 
            {
                m_result = editVariable->text();
                if( !m_result.startsWith( "$" ) )
                    m_result.prepend( '$' );
            }
            else if( radioVariableExisting->isChecked() )
                m_result = listVariable->currentText();
        }
        else if( radioSQLQuery->isChecked() )
            m_result = QString( "sqlquery:%2").arg( editQuery->text() );
        else if( radioJavaScript->isChecked() )
            m_result = QString( "js:%2").arg( editJavaScript->text() );

        m_result = "[" + m_result + "]";
    }
    else
    {
        QListViewItem* item = ( radioAll->isChecked() ? allList->selectedItem() : labelList->selectedItem() );

        if( item )
        {
            for( unsigned int i = 0; i < m_tokens.count(); i++ )
                if( QString( "[%1]").arg( m_tokens[i].token ) == item->text( 0 ) )
                {
                    if( m_tokens[i].appendix )
                        m_result =  QString( "[%1%2]").arg( m_tokens[i].token ).arg( lineEdit->text() );
                    else
                        m_result = item->text( 0 );
                    break;
                }
        }
    }

    KWizard::accept();
}

void TokenDialog::showPage( QWidget* w )
{
    if( w == page3 ) 
    {
        if( radioCustom->isChecked() ) 
        {
            if( radioVariable->isChecked() )
                page3->raiseWidget( stack2Page3 );
            else if( radioSQLQuery->isChecked() )
                page3->raiseWidget( stack2Page4 );
            else if( radioJavaScript->isChecked() ) 
            {
                page3->raiseWidget( stack2Page5 );
                editJavaScript->setFocus();
            }
        }
        else
        {
            if( radioAll->isChecked() )
                page3->raiseWidget( stack2Page1 );
            else 
            {
                initStackPage2();
                page3->raiseWidget( stack2Page2 );
            }
        }
    }
    else if( w == page2 ) 
    {
        if( radioFixed->isChecked() )
            page2->raiseWidget( stackPage1 );
        else if( radioCustom->isChecked() )
            page2->raiseWidget( stackPage2 );
    }

    KWizard::showPage( w );
}

void TokenDialog::initAll()
{
    unsigned int i, z;
    QValueList<tCategories>* categories = TokenProvider::getTokens();

    category->insertItem( i18n("All") );
    
    for( i = 0; i < categories->count(); i++ )
        category->insertItem( TokenProvider::captionForCategory( (TokenProvider::ECategories)(*categories)[i].category ) );

    for( i = 0; i < categories->count(); i++ )
	for( z = 0; z < (*categories)[i].tokens.count(); z++ )
	    m_tokens.append( (*categories)[i].tokens[z] );

    if( m_token )
    {
        QStringList custom_tokens = m_token->listUserVars();
        for( i = 0; i < custom_tokens.count(); i++ )
            m_tokens.append( tToken(  custom_tokens[i], i18n("Variable defined by the user for this label.") ) );
    }

    category->setCurrentItem( 0 );
    categoryChanged( category->item( 0 ) );
}

void TokenDialog::initStackPage2()
{
    TokenProvider::ECategories cat;
    labelList->clear();

    if( radioLabel->isChecked() )
        cat = TokenProvider::CAT_LABEL;
    else if( radioSQL->isChecked() )
        cat = TokenProvider::CAT_DATABASE;
    else if( radioDate->isChecked() )
        cat = TokenProvider::CAT_DATE;
    else if( radioAddress->isChecked() )
        cat = TokenProvider::CAT_ADDRESS;
    else
        return;

    QValueList<tCategories>* categories = TokenProvider::getTokens();
    for( int i = 0; i < (int)categories->count(); i++ )
    {
        if( (*categories)[i].category == cat )
        {
            for( unsigned int z = 0; z < (*categories)[i].tokens.count(); z++ )
                labelList->insertItem( new KListViewItem( labelList, QString( "[%1]").arg( (*categories)[i].tokens[z].token ),
                                                     (*categories)[i].tokens[z].description ) );
            
            break;
        }
    }
}

void TokenDialog::categoryChanged( QListBoxItem* item )
{
    unsigned int i;
    QValueList<tCategories>* categories = TokenProvider::getTokens();
    allList->clear();
    lineEdit->setEnabled( false );

    if( item->prev() == 0 )
    {
        for( i = 0; i < m_tokens.count(); i++ )
	    allList->insertItem( new KListViewItem( allList, QString( "[%1]").arg( m_tokens[i].token ),
						 m_tokens[i].description ) );
    } 
    else
    {
        for( i = 0; i < categories->count(); i++ )
        {
            if( TokenProvider::captionForCategory( (TokenProvider::ECategories)(*categories)[i].category ) == item->text() )
            {
                for( unsigned int z = 0; z < (*categories)[i].tokens.count(); z++ )
                    allList->insertItem( new KListViewItem( allList, QString( "[%1]").arg( (*categories)[i].tokens[z].token ),
                                      (*categories)[i].tokens[z].description ) );
                
                break;
            }
        }

	// TODO: comparing by a user visible string cries for bugs!!!
	if( item->text() == i18n("Custom Values") )
	    for( i=0;i<m_custom_tokens.count();i++ )
		allList->insertItem( new KListViewItem( allList, QString( "[%1]").arg( m_custom_tokens[i] ), 
						     i18n("Variable defined by the user for this label.") ) );
    }
}

void TokenDialog::itemChanged( QListViewItem* item )
{
    for( unsigned int i = 0; i < m_tokens.count(); i++ )
    {
	if( QString( "[%1]").arg( m_tokens[i].token ) == item->text( 0 ) )
	{
	    lineEdit->setEnabled( m_tokens[i].appendix );
	    if(  m_tokens[i].appendix )
		lineEdit->setFocus();
	    break;
	}
    }

    enableControls();
}

void TokenDialog::enableControls()
{
    setFinishEnabled( page3, false );

    listVariable->setEnabled( radioVariableExisting->isChecked() );
    editVariable->setEnabled( radioVariableNew->isChecked() );    

    if( editVariable->isEnabled() && !editVariable->text().isEmpty() ||
        listVariable->isEnabled() && listVariable->currentItem() != -1 ) 
        setFinishEnabled( page3, true );

    buttonQuery->setEnabled( radioSQLQuery->isChecked() && !editQuery->text().isEmpty() && SqlTables::isConnected() );
    if( radioSQLQuery->isChecked() && !editQuery->text().isEmpty() ) 
        setFinishEnabled( page3, true );

    if( radioJavaScript->isChecked() && !editJavaScript->text().isEmpty() )
        setFinishEnabled( page3, true );

    if( !radioCustom->isChecked() )
    {
        if( !radioAll->isChecked() && labelList->selectedItem() )
            setFinishEnabled( page3, true );
        
        if( radioAll->isChecked() && allList->selectedItem() )
            setFinishEnabled( page3, true );
    }
}

void TokenDialog::testQuery()
{
    QString ret = "[sqlquery:" + editQuery->text() + "]";
    if( m_token )
        ret = m_token->parse( ret );
    textQueryResults->setText( ret );
}

#include "tokendialog.moc"

