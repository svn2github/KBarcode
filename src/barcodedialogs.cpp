/***************************************************************************
                          barcodedialogs.cpp  -  description
                             -------------------
    begin                : Fre Sep 5 2003
    copyright            : (C) 2003 by Dominik Seichter
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

#include "barcodedialogs.h"
#include "barkode.h"
#include "purepostscript.h"
#include "tbarcode2.h"

// Qt includes
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmap.h>
#include <qradiobutton.h>
#include <qtooltip.h>
#include <qvbox.h>
#include <qvgroupbox.h>

// KDE includes
#include <kcombobox.h>
#include <knuminput.h>
#include <klocale.h>
#include <kcolorbutton.h>


AdvancedBarcodeDialog::AdvancedBarcodeDialog( QString type, QWidget* parent, const char* name )
    : KDialogBase( KDialogBase::Tabbed, i18n("Barcode Settings"),
      KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, parent,name)
{
    list.setAutoDelete( false );
    
    if( Barkode::hasFeature( type, PDF417BARCODE ) ) {
        QVBox* box = addVBoxPage( i18n("PDF417") );
        PDF417BarcodeDlg* dlg = new PDF417BarcodeDlg( box );
        list.append( (BarcodeDlgBase*)dlg );
    }

    if( Barkode::hasFeature( type, DATAMATRIX ) ) {
        QVBox* box = addVBoxPage( i18n("DataMatrix") );
        DataMatrixDlg* dlg = new DataMatrixDlg( box );
        list.append( (BarcodeDlgBase*)dlg );        
    }

    if( Barkode::hasFeature( type, TBARCODEADV ) ) {
        QVBox* box = addVBoxPage( i18n("TBarcode") );
        TBarcodeDlg* dlg = new TBarcodeDlg( box );
        list.append( (BarcodeDlgBase*)dlg );        
    }

    if( Barkode::hasFeature( type, PUREADV ) ) {
        QVBox* box = addVBoxPage( i18n("Barcode Writer in Pure Postscript") );
        PurePostscriptDlg* dlg = new PurePostscriptDlg( box );
        list.append( (BarcodeDlgBase*)dlg );        
    }

    if( Barkode::hasFeature( type, COLORED ) ) {
        QVBox* box = addVBoxPage( i18n("Colors") );
        ColorDlg* dlg = new ColorDlg( box );
        list.append( (BarcodeDlgBase*)dlg );        
    }

    QVBox* box = addVBoxPage( i18n("Sequence") );
    SequenceDlg* dlg = new SequenceDlg( box );
    list.append( (BarcodeDlgBase*)dlg );
}

AdvancedBarcodeDialog::~AdvancedBarcodeDialog()
{
}

void AdvancedBarcodeDialog::setData( Barkode* b )
{
    for( unsigned int i = 0; i < list.count(); i++ ) {
        BarcodeDlgBase* bdb = list.at( i );
        bdb->setData( b );
    }
}

void AdvancedBarcodeDialog::getData( Barkode* b ) 
{
    for( unsigned int i = 0; i < list.count(); i++ ) {
        BarcodeDlgBase* bdb = list.at( i );
        bdb->getData( b );
    }
}

TBarcodeDlg::TBarcodeDlg(QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    QVBoxLayout* layout = new QVBoxLayout( this, 6, 6 );

    QGroupBox* gb  = new QGroupBox( i18n("TBarcode"), this );
    gb->setColumnLayout(0, Qt::Vertical );
    gb->layout()->setSpacing( 6 );
    gb->layout()->setMargin( 11 );
    gb->setEnabled( Barkode::haveTBarcode() || Barkode::haveTBarcode2() );
    QVBoxLayout* gbLayout = new QVBoxLayout( gb->layout() );

    spinModule = new KDoubleNumInput( gb );
    spinModule->setLabel( i18n("Module width (mm):"), AlignLeft | AlignVCenter );
    spinModule->setRange( 0.190, 1.500, 0.001, true );

    spinHeight = new KIntNumInput( gb );
    spinHeight->setLabel( i18n("Barcode Height (mm):"), AlignLeft | AlignVCenter );
    spinHeight->setRange( 1, 1000, 10, false );

    checkEscape = new QCheckBox( i18n("&Translate escape sequences"), gb );
    checkAbove = new QCheckBox( i18n("&Text above barcode"), gb );
    checkAutoCorrect = new QCheckBox( i18n("&Auto correction"), gb );
    
    comboCheckSum = new KComboBox( false, gb );
    
    QLabel* label = new QLabel( i18n("&Checksum calculation method:"), gb );
    label->setBuddy( comboCheckSum );

    QHBoxLayout * hbox = new QHBoxLayout( 0, 6, 6 );
    hbox->addWidget( label );
    hbox->addWidget( comboCheckSum );
            
    gbLayout->addWidget( spinModule );
    gbLayout->addWidget( spinHeight );
    gbLayout->addWidget( checkEscape );
    gbLayout->addWidget( checkAbove );
    gbLayout->addWidget( checkAutoCorrect );
    gbLayout->addLayout( hbox );
    
    QToolTip::add( spinModule, i18n("<qt>Change the module with used by tbarcode. Take a look into the "
                                    "tbarcode documentation for details. Normaly you do not want to change "
                                    "this value.</qt>") );
    
    layout->addWidget( gb );
}

void TBarcodeDlg::setData( Barkode* b )
{
    TBarcodeOptions* options = dynamic_cast<TBarcodeOptions*>( b->engine()->options() );

    if( options ) 
    {
        spinModule->setValue( options->moduleWidth() );
        spinHeight->setValue( options->height() );
        checkEscape->setChecked( options->escape() );
        checkAbove->setChecked( options->above() );
        checkAutoCorrect->setChecked( options->autocorrect() );
    
        map.insert( i18n("No Checksum"), 0 );
        map.insert( i18n("Default Checksum Method"), 1 );

        if( Barkode::hasFeature( b->type(), MODULOALLCHECK ) ||
            Barkode::hasFeature( b->type(), MODULO10CHECK ) )
            map.insert( i18n("Modulo 10 Checksum"), 2 );
    
        if( Barkode::hasFeature( b->type(), MODULOALLCHECK ) ||
            b->type() == "b13" || // EAN 13
            b->type() == "b14" || // EAN 13
            b->type() == "b15" || // EAN 13
            b->type() == "b18" ) // CodaBar (2 width)
            map.insert( i18n("Module 43 (suggested for Code39 and Logmars, 1 digit)"), 3 );
        
        if( Barkode::hasFeature( b->type(), MODULOALLCHECK ) )
            map.insert( i18n("Modula 47 (2 digits)"), 4 );
        
        if( b->type() == "b21" ) // Deutsche Post Leitcode
            map.insert( i18n("Deutsche Post Leitcode"), 5 );
        
        if( b->type() == "b22") // Deutsche Post Identcode
            map.insert( i18n("Deutsche Post Identcode"), 6 );

        if( b->type() == "b1" ) { // Code11
        map.insert( i18n("Code 11 (1 digit)"), 7 );
        map.insert( i18n("Code 11 (2 digits)"), 8 );
        }

        if( Barkode::hasFeature( b->type(), POSTNETCHECK ) )
            map.insert( i18n("USPS Postnet"), 9 );

        if( b->type() == "b47" ) { // MSI
            map.insert( i18n("MSI (1 digit)"), 10 );
            map.insert( i18n("MSI (2 digits)"), 11 );
        }

        if( b->type() == "b46" ) // Plessey
            map.insert( i18n("Plessey"), 12 );

        if( Barkode::hasFeature( b->type(), EAN8CHECK ) )
            map.insert( i18n("EAN 8"), 13 );

        if( Barkode::hasFeature( b->type(), EAN13CHECK ) )    
            map.insert( i18n("EAN 13"), 14 );

        if( Barkode::hasFeature( b->type(), UPCACHECK ) )
            map.insert( i18n("UPC A"), 15 );
        
        if( Barkode::hasFeature( b->type(), UPCECHECK ) )
            map.insert( i18n("UPC E"), 16 );

        if( b->type() == "b16" )  // EAN 128
            map.insert( i18n("EAN 128"), 17 );

        if( Barkode::hasFeature( b->type(),  CODE128CHECK ) )
            map.insert( i18n("Code 128"), 18 );
        
        if( b->type() == "b70" ) // Royal Mail 4 State
            map.insert( i18n("Royal Mail 4 State"), 19 );

        comboCheckSum->insertStringList( map.keys() );
        
        QMap<QString,int>::Iterator it;
        for ( it = map.begin(); it != map.end(); ++it ) {
            if( it.data() == options->checksum() ) {
                for( int i = 0; i < comboCheckSum->count(); i++ )
                    if( comboCheckSum->text( i ) == it.key() ) {
                        comboCheckSum->setCurrentItem( i );
                        break;
                    }
                break;
            }
        }
    }
}

void TBarcodeDlg::getData( Barkode* b ) const
{
    TBarcodeOptions* options = dynamic_cast<TBarcodeOptions*>( b->engine()->options() );

    if( options ) 
    {
        options->setModuleWidth( spinModule->value() );
        options->setEscape( checkEscape->isChecked() );
        options->setAbove( checkAbove->isChecked() );
        options->setAutocorrect( checkAutoCorrect->isChecked() );
        options->setCheckSum( map[comboCheckSum->currentText()] );
        options->setHeight( spinHeight->value() );
    }
}

PDF417BarcodeDlg::PDF417BarcodeDlg(QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    QVBoxLayout* layout = new QVBoxLayout( this, 6, 6 );

    QGroupBox* gpdf  = new QGroupBox( i18n("PDF417"), this );
    gpdf->setColumnLayout(0, Qt::Vertical );
    gpdf->layout()->setSpacing( 6 );
    gpdf->layout()->setMargin( 11 );
    gpdf->setEnabled( Barkode::haveTBarcode() || Barkode::havePDFBarcode() );
    QVBoxLayout* gpdfLayout = new QVBoxLayout( gpdf->layout() );

    spinRow = new KIntNumInput( gpdf );
    spinRow->setLabel( i18n("Rows:"),  AlignLeft | AlignVCenter );
    spinRow->setRange( 0, 90, 1, true );

    spinCol = new KIntNumInput( spinRow, 0, gpdf );
    spinCol->setLabel( i18n("Columns:"),  AlignLeft | AlignVCenter );
    spinCol->setRange( 0, 30, 1, true );

    spinErr = new KIntNumInput( spinCol, 0, gpdf );
    spinErr->setLabel( i18n("Error correction level:"),  AlignLeft | AlignVCenter );
    spinErr->setRange( 1, 8, 1, true );

    gpdfLayout->addWidget( spinRow );
    gpdfLayout->addWidget( spinCol );
    gpdfLayout->addWidget( spinErr );
    layout->addWidget( gpdf );
}

void PDF417BarcodeDlg::setData( Barkode* b )
{
    PDF417Options* options = dynamic_cast<PDF417Options*>( b->engine()->options() );

    if( options ) 
    {
        spinRow->setValue( options->row() );
        spinCol->setValue( options->col() );
        spinErr->setValue( options->err() );
    }
}

void PDF417BarcodeDlg::getData( Barkode* b ) const
{
    PDF417Options* options = dynamic_cast<PDF417Options*>( b->engine()->options() );

    if( options ) 
    {
        options->setRow( spinRow->value() );
        options->setCol( spinCol->value() );
        options->setErr( spinErr->value() );
    }
}

DataMatrixDlg::DataMatrixDlg(QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    QHBoxLayout* datamLayout = new QHBoxLayout( this, 6, 6 );

    comboDataMatrix = new KComboBox( false, this );

    datamLayout->addWidget( new QLabel( i18n("Data Matrix symbol sizes (rows x cols):"), this ) );
    datamLayout->addWidget( comboDataMatrix );
    datamLayout->addItem( new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    comboDataMatrix->insertItem( i18n("Automatic calculation" ) );
    comboDataMatrix->insertItem( "10 x 10" );
    comboDataMatrix->insertItem( "12 x 12" );
    comboDataMatrix->insertItem( "14 x 14" );
    comboDataMatrix->insertItem( "16 x 16" );
    comboDataMatrix->insertItem( "18 x 18" );
    comboDataMatrix->insertItem( "20 x 20" );
    comboDataMatrix->insertItem( "22 x 22" );
    comboDataMatrix->insertItem( "24 x 24" );
    comboDataMatrix->insertItem( "26 x 26" );
    comboDataMatrix->insertItem( "32 x 32" );
    comboDataMatrix->insertItem( "36 x 36" );
    comboDataMatrix->insertItem( "40 x 40" );
    comboDataMatrix->insertItem( "44 x 44" );
    comboDataMatrix->insertItem( "48 x 48" );
    comboDataMatrix->insertItem( "52 x 52" );
    comboDataMatrix->insertItem( "64 x 64" );
    comboDataMatrix->insertItem( "72 x 72" );
    comboDataMatrix->insertItem( "80 x 80" );
    comboDataMatrix->insertItem( "88 x 88" );
    comboDataMatrix->insertItem( "96 x 96" );
    comboDataMatrix->insertItem( "104 x 104" );
    comboDataMatrix->insertItem( "120 x 120" );
    comboDataMatrix->insertItem( "132 x 132" );
    comboDataMatrix->insertItem( "144 x 144" );
    comboDataMatrix->insertItem( "8 x 18" );
    comboDataMatrix->insertItem( "8 x 32" );
    comboDataMatrix->insertItem( "12 x 26" );
    comboDataMatrix->insertItem( "12 x 36" );
    comboDataMatrix->insertItem( "16 x 36" );
    comboDataMatrix->insertItem( "16 x 48" );
}

void DataMatrixDlg::setData( Barkode* b )
{
    comboDataMatrix->setCurrentItem( b->datamatrixSize());
}

void DataMatrixDlg::getData( Barkode* b ) const
{
    b->setDatamatrixSize( comboDataMatrix->currentItem() );
}

SequenceDlg::SequenceDlg( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
    QVBoxLayout* main = new QVBoxLayout( this, 6, 6 );

    QButtonGroup* group = new QButtonGroup( i18n("Sequence"), this );
    group->setColumnLayout(0, Qt::Vertical );
    group->layout()->setSpacing( 6 );
    group->layout()->setMargin( 11 );
    QVBoxLayout* layout = new QVBoxLayout( group->layout() );

    checkSequence = new QCheckBox( i18n("&Enable sequence"), group );
                                                                                                        
    radioNumbers = new QRadioButton( i18n("Iterate over numbers 0-9"), group );
    radioAlpha = new QRadioButton( i18n("Iterate over characters A-Z"), group );
    radioAlphaNum = new QRadioButton( i18n("Iterate over A-Z, 0-9"), group );

    spinStep = new KIntNumInput( group );
    spinStep->setLabel( i18n("Step:"), AlignLeft | AlignVCenter );
    spinStep->setRange( -100, 100, 1, false );

    spinStart = new KIntNumInput( spinStep, 1, group );
    spinStart->setLabel( i18n("Start:"), AlignLeft | AlignVCenter );
    spinStart->setRange( -100000, 100000, 1, false );
    
    layout->addWidget( checkSequence );
    layout->addWidget( radioNumbers );
    layout->addWidget( radioAlpha );
    layout->addWidget( radioAlphaNum );
    layout->addWidget( spinStep );
    layout->addWidget( spinStart );
    
    main->addWidget( group );

    connect( checkSequence, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( radioNumbers, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( radioAlpha, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( radioAlphaNum, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
}

void SequenceDlg::setData( Barkode* b )
{
    checkSequence->setChecked( b->sequenceEnabled() );
    spinStep->setValue( b->sequenceStep() );
    spinStart->setValue( b->sequenceStart() );
    
    if( b->sequenceMode() == NUM )
        radioNumbers->setChecked( true );
    else if( b->sequenceMode() == ALPHA )
        radioAlpha->setChecked( true );
    else if( b->sequenceMode() == ALPHANUM )
        radioAlphaNum->setChecked( true );
        
    enableControls();
}

void SequenceDlg::getData( Barkode* b ) const
{
    b->setSequenceEnabled( checkSequence->isChecked() );
    
    b->setSequenceStep( spinStep->value() );
    b->setSequenceStart( spinStart->value() );
    
    if( radioNumbers->isChecked() )
        b->setSequenceMode( NUM );
    else if( radioAlpha->isChecked() )
        b->setSequenceMode( ALPHA );
    else if( radioAlphaNum->isChecked() )
        b->setSequenceMode( ALPHANUM );
}

void SequenceDlg::enableControls()
{
    radioNumbers->setEnabled( checkSequence->isChecked() );
    radioAlpha->setEnabled( checkSequence->isChecked() );
    radioAlphaNum->setEnabled( false ); //checkSequence->isChecked() );
    spinStep->setEnabled( checkSequence->isChecked() );
    spinStart->setEnabled( checkSequence->isChecked() && radioNumbers->isChecked() );
}

ColorDlg::ColorDlg(QWidget *parent, const char *name)
    : QVBox( parent, name )
{
    QGroupBox* gb  = new QGroupBox( i18n("Colors"), this );
    gb->setColumnLayout(0, Qt::Vertical );
    gb->layout()->setSpacing( 6 );
    gb->layout()->setMargin( 11 );
    gb->setEnabled( Barkode::havePurePostscriptBarcode() );
    QGridLayout* gbLayout = new QGridLayout( gb->layout() );

    buttonBarColor = new KColorButton( gb );
    buttonBackColor = new KColorButton( gb );
    buttonTextColor = new KColorButton( gb );

    gbLayout->addWidget( new QLabel( i18n("Bar Color:"), gb ), 0, 0 );
    gbLayout->addWidget( new QLabel( i18n("Background Color:"), gb ), 1, 0 );
    gbLayout->addWidget( new QLabel( i18n("Text Color:"), gb ), 2, 0 );
    gbLayout->addWidget( buttonBarColor, 0, 1 );
    gbLayout->addWidget( buttonBackColor, 1, 1 );
    gbLayout->addWidget( buttonTextColor, 2, 1 );
}

void ColorDlg::setData( Barkode* b )
{
    buttonBarColor->setColor( b->foreground() );
    buttonBackColor->setColor( b->background() );
    buttonTextColor->setColor( b->textColor() );
}

void ColorDlg::getData( Barkode* b ) const
{
    b->setForeground( buttonBarColor->color() );
    b->setBackground( buttonBackColor->color() );
    b->setTextColor( buttonTextColor->color() );
}

PurePostscriptDlg::PurePostscriptDlg(QWidget *parent, const char *name)
    : QVBox( parent, name )
{
    QVGroupBox* gb  = new QVGroupBox( i18n("Barcode Writer in Pure Postscript"), this );
    gb->setEnabled( Barkode::havePurePostscriptBarcode() );
    
    checkChecksum = new QCheckBox( i18n("Enable &Checksum"), gb );
}

void PurePostscriptDlg::setData( Barkode* b )
{
    PurePostscriptOptions* options = dynamic_cast<PurePostscriptOptions*>( b->engine()->options() );

    if( options ) 
    {
        checkChecksum->setChecked( options->checksum() );
        checkChecksum->setEnabled( Barkode::hasFeature( b->type(), MODULOALLCHECK ) );
    }
}

void PurePostscriptDlg::getData( Barkode* b ) const
{
    PurePostscriptOptions* options = dynamic_cast<PurePostscriptOptions*>( b->engine()->options() );
    
    if( options ) 
        options->setChecksum( checkChecksum->isChecked() );
}


#include "barcodedialogs.moc"
