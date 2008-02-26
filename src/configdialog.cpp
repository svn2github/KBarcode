/***************************************************************************
                          configdialog.cpp  -  description
                             -------------------
    begin                : Fre Apr 26 2002
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

#include "configdialog.h"
#include "printersettings.h"
#include "sqltables.h"

// Qt includes
#include <q3buttongroup.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <q3paintdevicemetrics.h>
#include <qradiobutton.h>
#include <qsqldatabase.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3Frame>
#include <Q3VBoxLayout>

// KDE includes
#include <kabc/addressee.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kcolorbutton.h>
#include <kiconloader.h>
#include <kimageio.h>
#include <klineedit.h>
#include <klocale.h>
#include <kcombobox.h>
#include <knuminput.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kglobal.h>

const QString cached = I18N_NOOP("There are currently %1 cached barcodes.");
using namespace KABC;

ConfigDialog::ConfigDialog( QWidget* parent )
    : KDialogBase( IconList, i18n("Configure KBarcode"), KDialogBase::Ok|KDialogBase::Cancel,
      KDialogBase::Ok, parent, "", true, true )
{
    setupTab2(); // Printer
    setupTab1(); // SQL
    setupTab4(); // label editor
    setupTab3(); // import
    setupTab5(); // on new
}

ConfigDialog::~ConfigDialog()
{
}

void ConfigDialog::setupTab1( )
{
    QFrame* box = addPage( i18n("SQL Settings"), "", BarIcon("connect_no") );
    Q3VBoxLayout* layout = new Q3VBoxLayout( box, 6, 6 );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );

    sqlwidget = new SqlWidget( false, box, "sqlwidget" );

    layout->addWidget( sqlwidget );
    layout->addItem( spacer );
}

void ConfigDialog::setupTab2()
{
    labelprinterdata* lb = PrinterSettings::getInstance()->getData();
    
    QFrame* box = addPage( i18n("Print Settings"), "", BarIcon("fileprint") );

    Q3VBoxLayout* tabLayout = new Q3VBoxLayout( box, 11, 6 );
    Q3HBoxLayout* Layout0 = new Q3HBoxLayout( 0, 6, 6 );
    Q3HBoxLayout* Layout1 = new Q3HBoxLayout( 0, 6, 6 );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );

    printerQuality = new KComboBox( false, box );
    printerQuality->addItem( i18n("Medium Resolution (300dpi)") );
    printerQuality->addItem( i18n("High Resolution (600dpi)") );
    printerQuality->addItem( i18n("Very High Resolution (1200dpi)") );

    switch( lb->quality ) {
        case PrinterSettings::Middle:
            printerQuality->setCurrentItem( 0 );
            break;
        case PrinterSettings::High:
            printerQuality->setCurrentItem( 1 );
            break;
        case PrinterSettings::VeryHigh:
            printerQuality->setCurrentItem( 2 );
            break;
        default:
            break;
    }

    pageFormat = new KComboBox( false, box );
    PrinterSettings::getInstance()->insertPageFormat( pageFormat );
    pageFormat->setCurrentItem( lb->format );
    
    Layout0->addWidget( new QLabel( i18n("Printer Resolution:"), box ) );
    Layout0->addWidget( printerQuality );
    Layout1->addWidget( new QLabel( i18n("Preview Page Format:"), box ) );
    Layout1->addWidget( pageFormat );
    tabLayout->addLayout( Layout0 );
    tabLayout->addLayout( Layout1 );
    tabLayout->addItem( spacer );
}

void ConfigDialog::setupTab3()
{
    labelprinterdata* lb = PrinterSettings::getInstance()->getData();

    QFrame* box = addPage( i18n("Import"), "", BarIcon("fileimport") );
    Q3GridLayout* grid = new Q3GridLayout( box, 2, 2 );

    QLabel* label = new QLabel( box );
    label->setText( i18n("Comment:") );
    grid->addWidget( label, 0, 0 );

    comment = new KLineEdit( lb->comment, box );
    grid->addWidget( comment, 0, 1 );

    label = new QLabel( box );
    label->setText( i18n( "Separator:" ) );
    grid->addWidget( label, 1, 0 );

    separator = new KLineEdit( lb->separator, box );
    grid->addWidget( separator, 1, 1 );

    label = new QLabel( box );
    label->setText( i18n("Quote Character:") );
    grid->addWidget( label, 2, 0 );

    quote  = new KLineEdit( lb->quote, box );
    grid->addWidget( quote, 2, 1 );

    checkUseCustomNo = new QCheckBox( i18n("&Use customer article no. for import"), box );
    checkUseCustomNo->setChecked( lb->useCustomNo );
    
    grid->addMultiCellWidget( checkUseCustomNo, 3, 3, 0, 2 );
    
    Q3HBoxLayout* Layout1 = new Q3HBoxLayout( 0, 6, 6 );
    Layout1->addWidget( new QLabel( i18n("File Format:"), box ) );

    combo1 = new KComboBox( box );
    combo1->addItem( i18n("Quantity") );
    combo1->addItem( i18n("Article Number") );
    combo1->addItem( i18n("Group") );
    Layout1->addWidget( combo1 );

    combo2 = new KComboBox( box );
    combo2->addItem( i18n("Quantity") );
    combo2->addItem( i18n("Article Number") );
    combo2->addItem( i18n("Group") );
    Layout1->addWidget( combo2 );

    combo3 = new KComboBox( box );
    combo3->addItem( i18n("Quantity") );
    combo3->addItem( i18n("Article Number") );
    combo3->addItem( i18n("Group") );
    Layout1->addWidget( combo3 );

    grid->addMultiCellLayout( Layout1, 4, 4, 0, 2 );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    grid->addItem( spacer, 5, 0 );    

    KConfig* config = KGlobal::config();
    config->setGroup("FileFormat");
    combo1->setCurrentItem( config->readNumEntry("Data0", 0 ) );
    combo2->setCurrentItem( config->readNumEntry("Data1", 1 ) );
    combo3->setCurrentItem( config->readNumEntry("Data2", 2 ) );
}

void ConfigDialog::setupTab4()
{
    QFrame* box = addPage( i18n("Label Editor"), "", BarIcon("kbarcode") );
    Q3GridLayout* tabLayout = new Q3GridLayout( box, 11, 6 );

    checkNewDlg = new QCheckBox( box );
    checkNewDlg->setText( i18n("&Create a new label on startup") );

    date = new KLineEdit( box );
    labelDate = new QLabel( box );
    
    connect( date, SIGNAL( textChanged( const QString & ) ), this, SLOT( updateDatePreview() ) );

    spinGrid = new KIntNumInput( 0, box );
    spinGrid->setLabel( i18n("Grid:" ), Qt::AlignLeft | Qt::AlignVCenter );
    spinGrid->setRange(2, 100, 1, false );

    colorGrid = new KColorButton( box );

    tabLayout->addWidget( checkNewDlg, 0, 0 );
    tabLayout->addMultiCellWidget( spinGrid, 1, 1, 0, 2 );
    tabLayout->addWidget( new QLabel( i18n("Grid Color:"), box ), 2, 0 );
    tabLayout->addWidget( colorGrid, 2, 1 );
    tabLayout->addWidget( new QLabel( i18n("Date Format:"), box ), 3, 0 );
    tabLayout->addWidget( date, 3, 1 );
    tabLayout->addWidget( labelDate, 3, 2 );
}

void ConfigDialog::setupTab5()
{
    labelprinterdata* lb = PrinterSettings::getInstance()->getData();
    QFrame* box = addPage( i18n("On New"), "", BarIcon("filenew") );

    Q3VBoxLayout* tabLayout = new Q3VBoxLayout( box, 11, 6 );

    Q3ButtonGroup* bg = new Q3ButtonGroup( i18n("On New Article"), box );
    bg->setColumnLayout(0, Qt::Vertical );
    bg->layout()->setSpacing( 6 );
    bg->layout()->setMargin( 11 );
    Q3GridLayout* bgLayout = new Q3GridLayout( bg->layout() );

    QStringList alist, glist;
    alist.append( i18n("No Line Break") );
    alist.append( i18n("Line Break") );
    alist.append( i18n("Insert Label X") );
    alist.append( i18n("New Page") );
    alist.append( i18n("Article No.") );

    glist.append( i18n("No Line Break") );
    glist.append( i18n("Line Break") );
    glist.append( i18n("Insert Label X") );
    glist.append( i18n("New Page") );
    glist.append( i18n("Group Name") );

    onNewArticle1 = new KComboBox( false, bg );
    onNewArticle2 = new KComboBox( false, bg );
    onNewArticle3 = new KComboBox( false, bg );
    onNewArticle4 = new KComboBox( false, bg );

    onNewArticle1->insertStringList( alist );
    onNewArticle2->insertStringList( alist );
    onNewArticle3->insertStringList( alist );
    onNewArticle4->insertStringList( alist );

    bgLayout->setColStretch( 1, 3 );

    bgLayout->addWidget( new QLabel( "1.", bg ), 0, 0 );
    bgLayout->addWidget( new QLabel( "2.", bg ), 1, 0 );
    bgLayout->addWidget( new QLabel( "3.", bg ), 2, 0 );
    bgLayout->addWidget( new QLabel( "4.", bg ), 3, 0 );

    bgLayout->addWidget( onNewArticle1, 0, 1 );
    bgLayout->addWidget( onNewArticle2, 1, 1 );
    bgLayout->addWidget( onNewArticle3, 2, 1 );
    bgLayout->addWidget( onNewArticle4, 3, 1 );

    Q3ButtonGroup* bg2 = new Q3ButtonGroup( i18n("On New Group"), box );
    bg2->setColumnLayout(0, Qt::Vertical );
    bg2->layout()->setSpacing( 6 );
    bg2->layout()->setMargin( 11 );
    Q3GridLayout* bg2Layout = new Q3GridLayout( bg2->layout() );

    onNewGroup1 = new KComboBox( false, bg2 );
    onNewGroup2 = new KComboBox( false, bg2 );
    onNewGroup3 = new KComboBox( false, bg2 );
    onNewGroup4 = new KComboBox( false, bg2 );

    onNewGroup1->insertStringList( glist );
    onNewGroup2->insertStringList( glist );
    onNewGroup3->insertStringList( glist );
    onNewGroup4->insertStringList( glist );

    bg2Layout->setColStretch( 1, 3 );

    bg2Layout->addWidget( new QLabel( "1.", bg2 ), 0, 0 );
    bg2Layout->addWidget( new QLabel( "2.", bg2 ), 1, 0 );
    bg2Layout->addWidget( new QLabel( "3.", bg2 ), 2, 0 );
    bg2Layout->addWidget( new QLabel( "4.", bg2 ), 3, 0 );

    bg2Layout->addWidget( onNewGroup1, 0, 1 );
    bg2Layout->addWidget( onNewGroup2, 1, 1 );
    bg2Layout->addWidget( onNewGroup3, 2, 1 );
    bg2Layout->addWidget( onNewGroup4, 3, 1 );

    tabLayout->addWidget( bg );
    tabLayout->addWidget( bg2 );

    onNewArticle1->setCurrentItem( lb->articleEvent1 );
    onNewArticle2->setCurrentItem( lb->articleEvent2 );
    onNewArticle3->setCurrentItem( lb->articleEvent3 );
    onNewArticle4->setCurrentItem( lb->articleEvent4 );

    onNewGroup1->setCurrentItem( lb->groupEvent1 );
    onNewGroup2->setCurrentItem( lb->groupEvent2 );
    onNewGroup3->setCurrentItem( lb->groupEvent3 );
    onNewGroup4->setCurrentItem( lb->groupEvent4 );
}

void ConfigDialog::accept()
{
    KConfig* config = KGlobal::config();
    config->setGroup("FileFormat");
    config->writeEntry("Data0", combo1->currentItem() );
    config->writeEntry("Data1", combo2->currentItem() );
    config->writeEntry("Data2", combo3->currentItem() );

    sqlwidget->save();

    labelprinterdata* lpdata = PrinterSettings::getInstance()->getData();
    lpdata->comment = comment->text();
    lpdata->separator = separator->text();
    lpdata->quote = quote->text();
    lpdata->format = pageFormat->currentItem();

    lpdata->articleEvent1 = onNewArticle1->currentItem();
    lpdata->articleEvent2 = onNewArticle2->currentItem();
    lpdata->articleEvent3 = onNewArticle3->currentItem();
    lpdata->articleEvent4 = onNewArticle4->currentItem();

    lpdata->groupEvent1 = onNewGroup1->currentItem();
    lpdata->groupEvent2 = onNewGroup2->currentItem();
    lpdata->groupEvent3 = onNewGroup3->currentItem();
    lpdata->groupEvent4 = onNewGroup4->currentItem();
    lpdata->useCustomNo = checkUseCustomNo->isChecked();

    switch( printerQuality->currentItem() ) {
        case 0:
            lpdata->quality = PrinterSettings::Middle;
            break;
        case 1:
            lpdata->quality = PrinterSettings::High;
            break;
        case 2:
            lpdata->quality = PrinterSettings::VeryHigh;
            break;
        default:
            break;
    }

    QDialog::accept();
}

void ConfigDialog::updateDatePreview()
{
    labelDate->setText( i18n("Preview: ") + QDateTime::currentDateTime().toString( date->text() ) );
}

#include "configdialog.moc"
