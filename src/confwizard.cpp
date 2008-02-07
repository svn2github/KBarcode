/***************************************************************************
                          confwizard.cpp  -  description
                             -------------------
    begin                : Son Jun 16 2002
    copyright            : (C) 2002 by Dominik Seichter
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

#include "confwizard.h"
#include "sqltables.h"
#include "printersettings.h"
#include "dsmainwindow.h"

// Qt includes
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcursor.h>
#include <qlayout.h>
#include <qsqldatabase.h>
#include <qradiobutton.h>
#include <qprinter.h>
#include <qtextbrowser.h>

// KDE includes
#include <kapplication.h>
#include <kcombobox.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kurllabel.h>
#include <kstandarddirs.h>


const char* description = I18N_NOOP(
        "KBarcode is a barcode and label printing application for KDE 3. It can "
        "be used to print every thing from simple business cards up to complex "
        "labels with several barcodes (e.g. article descriptions). KBarcode "
        "comes with an easy to use WYSIWYG label designer, a setup wizard, "
        "batch import of labels (directly from the delivery note), thousands "
        "of predefined labels, database management tools and translations "
        "in many languages. Even printing more than 10.000 labels in one go is "
        "no problem for KBarcode. Additionally it is a simply xbarcode "
        "replacement for the creation of barcodes. All major types of barcodes "
        "like EAN, UPC, CODE39 and ISBN are supported." );

ConfWizard::ConfWizard( QWidget* parent,  const char* name, bool modal )
    : KWizard( parent, name, modal )
{
    setCaption( i18n( "Configure KBarcode" ) );

    setupPage1();
    setupPage0();
    setupPage2();
    setupPage3();

    setNextEnabled( page_2, false );
    helpButton()->hide();
    
    connect( buttonCreate, SIGNAL( clicked() ), this, SLOT( create() ) );
    connect( buttonExample, SIGNAL( clicked() ), this, SLOT( example() ) );
    connect( checkDatabase, SIGNAL( clicked() ), this, SLOT( useDatabase() ) );
}

ConfWizard::~ConfWizard()
{ }

void ConfWizard::accept()
{
    sqlwidget->save( checkDatabase->isChecked() );

    KWizard::accept();
}

void ConfWizard::setupPage1()
{
    page = new QWidget( this, "page" );
    pageLayout = new QVBoxLayout( page, 11, 6, "pageLayout");

    Layout8 = new QHBoxLayout( 0, 0, 6, "Layout8");

    Layout7 = new QVBoxLayout( 0, 0, 6, "Layout7");

    logo = new QLabel( page, "logo" );
    logo->setPixmap( locate("data", "kbarcode/logo.png") );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout->addWidget( logo );
    Layout7->addItem( spacer );
    Layout8->addLayout( Layout7 );

    TextLabel2_2 = new QLabel( page, "TextLabel2_2" );
    TextLabel2_2->setText( i18n( "<qt><h1>Welcome to KBarcode</h1><br><br>") +
        i18n( description ) + "</qt>" );
    Layout8->addWidget( TextLabel2_2 );

    pageLayout->addLayout( Layout8 );

    KURLLabel1 = new KURLLabel( page, "KURLLabel1" );
    KURLLabel1->setText( "http://www.kbarcode.net" );
    KURLLabel1->setURL("http://www.kbarcode.net");
    pageLayout->addWidget( KURLLabel1 );
    addPage( page, i18n( "Welcome" ) );
}

void ConfWizard::setupPage0()
{
    QWidget* page_0 = new QWidget( this, "page_0" );
    QVBoxLayout* pageLayout = new QVBoxLayout( page_0, 11, 6, "pageLayout");

    QTextBrowser* b = new QTextBrowser( page_0, "b" );
    b->setText( DSMainWindow::systemCheck() );

    pageLayout->addWidget( b );
    
    addPage( page_0, i18n("System Check") );
}

void ConfWizard::setupPage2()
{
    page_2 = new QWidget( this, "page_2" );
    pageLayout_2 = new QVBoxLayout( page_2, 11, 6, "pageLayout_2");

    checkDatabase = new QCheckBox( page_2 );
    checkDatabase->setText( i18n("&Use database with KBarcode") );
    checkDatabase->setChecked( true );

    sqlwidget = new SqlWidget( true, page_2, "sqlwidget" );
    connect( sqlwidget, SIGNAL( databaseWorking( bool ) ), this, SLOT( testSettings( bool ) ) );
        
    QSpacerItem* spacer_5 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    pageLayout_2->addWidget( checkDatabase );
    pageLayout_2->addWidget( sqlwidget );
    pageLayout_2->addItem( spacer_5 );

    addPage( page_2, i18n( "Database" ) );
}

void ConfWizard::setupPage3()
{
    page_3 = new QWidget( this, "page_3" );
    pageLayout_3 = new QVBoxLayout( page_3, 11, 6, "pageLayout_3");

    TextLabel1_2 = new QLabel( page_3, "TextLabel1_2" );
    TextLabel1_2->setText( i18n( "KBarcode can create the required SQL tables for you.<br>KBarcode will add also some Label Definitions to the tables.<br>After that you can fill the tables with some example data." ) );
    TextLabel1_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    pageLayout_3->addWidget( TextLabel1_2 );

    Layout5_2 = new QVBoxLayout( 0, 0, 6, "Layout5_2");

    buttonCreate = new KPushButton( page_3, "buttonCreate" );
    buttonCreate->setText( i18n( "&Create Tables" ) );
    Layout5_2->addWidget( buttonCreate );

    buttonExample = new KPushButton( page_3, "buttonExample" );
    buttonExample->setEnabled( FALSE );
    buttonExample->setText( i18n( "&Add Example Data" ) );
    Layout5_2->addWidget( buttonExample );
    QSpacerItem* spacer_6 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout5_2->addItem( spacer_6 );
    pageLayout_3->addLayout( Layout5_2 );
    addPage( page_3, i18n( "Create Tables" ) );
}

void ConfWizard::testSettings( bool b )
{
    setNextEnabled( page_2, b );
}

void ConfWizard::create()
{
    KApplication::setOverrideCursor( Qt::WaitCursor );
    if(!SqlTables::getInstance()->newTables( sqlwidget->username(), sqlwidget->password(), sqlwidget->hostname(), sqlwidget->database(), sqlwidget->driver() ) )
    {
        KApplication::restoreOverrideCursor();
        return;
    }
    else
        KApplication::restoreOverrideCursor();

    QSqlDatabase* db = QSqlDatabase::addDatabase( sqlwidget->driver() );
    db->setDatabaseName( sqlwidget->database() );
    db->setUserName( sqlwidget->username() );
    db->setPassword( sqlwidget->password() );
    db->setHostName( sqlwidget->hostname() );

    if( !db->open() )
        KMessageBox::error( this, i18n("<qt>Connection failed:<br>") + sqlwidget->database(),
              db->lastError().databaseText() + "</qt>" );

    if( db->open() ) {
        KApplication::setOverrideCursor( Qt::WaitCursor );
        SqlTables::getInstance()->importData(
            locate("appdata", "labeldefinitions.sql"), db );
        buttonExample->setEnabled( true );
        KApplication::restoreOverrideCursor();
    }

    db->close();
}

void ConfWizard::example()
{
    QSqlDatabase* db = QSqlDatabase::addDatabase( sqlwidget->driver() );
    db->setDatabaseName( sqlwidget->database() );
    db->setUserName( sqlwidget->username() );
    db->setPassword( sqlwidget->password() );
    db->setHostName( sqlwidget->hostname() );

    if( !db->open() )
        KMessageBox::error( this, i18n("<qt>Connection failed:<br>") + sqlwidget->database(),
              db->lastError().databaseText() + "</qt>" );


    SqlTables::getInstance()->importData(
        locate("appdata", "exampledata.sql"), db );
    KMessageBox::information( this, i18n("Example data has been imported.") );

    db->close();
}

void ConfWizard::showPage( QWidget* page )
{
    QWizard::showPage(page);

    if( page == page_2 && !sqlwidget->driverCount() ) {
        KMessageBox::information( this, i18n(
            "There are no Qt SQL drivers installed. "
            "KBarcode needs those drivers to access the different SQL databases. "
            "This drivers are part of the Qt Source distribution and should also be part of "
            "your distribution. Please install them first.") );
    }
    
    if ( page == page_3 )
        finishButton()->setEnabled( true );
}

void ConfWizard::useDatabase()
{
    setFinishEnabled( page_2, !checkDatabase->isChecked() );
    setNextEnabled( page_2, false );
    setFinishEnabled( page_3, checkDatabase->isChecked() );
    sqlwidget->setEnabled( checkDatabase->isChecked() );
}


#include "confwizard.moc"
