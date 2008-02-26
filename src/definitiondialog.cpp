/***************************************************************************
                      definitiondialog.cpp  -  description
                             -------------------
    begin                : Don Apr 18 12:34:56 CEST 2002
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

#include "definitiondialog.h"
#include "printersettings.h"
#include "labelutils.h"
#include "definition.h"

#include <kcombobox.h>
#include <kpushbutton.h>
#include <krestrictedline.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <QPaintEvent>
#include <QPixmap>
#include <Q3VBoxLayout>

// KDE includes
#include <klocale.h>

LabelPreview::LabelPreview( QWidget* parent, const char* name )
    : QWidget( parent, name)
{
    // For old DIN A4 preview
    PrinterSettings* ps = PrinterSettings::getInstance();
    setFixedSize( QSize(  ((int)ps->pageWidth() + 20)/2, ((int)ps->pageHeight() + 20)/2 ) );
    m_prv_enabled = true;
}

LabelPreview::~LabelPreview()
{ }

void LabelPreview::paintEvent( QPaintEvent* )
{
    QPainter p( this );
    p.fillRect( 0, 0, width(), height(), QBrush( Qt::white ) );
    p.setPen( QPen( Qt::black ) );
    p.translate( 10, 10 );

    p.fillRect( 0, 0, 210/2, 297/2, QBrush( Qt::gray ) );
    p.drawRect( 0, 0, 210/2, 297/2 );

    if(!m_prv_enabled)
        return;
        
//    int numv = measure.num_v > 0 ? measure.num_v : 1;
    for( int v = 0; v < measure.numV(); v++ ) {
        for( int h = 0; h < measure.numH(); h++ ) {
            if( !v && !h )
                p.setPen( QPen( Qt::red ) );
            else
                p.setPen( QPen( Qt::black ) );

            p.fillRect( int(measure.gapLeftMM() + (measure.gapHMM() * h)) / 2,
                        int(measure.gapTopMM() + (measure.gapVMM() * v)) / 2 ,
                        (int)measure.widthMM() / 2 , (int)measure.heightMM() / 2, QBrush( Qt::white ) );

            p.drawRect( int(measure.gapLeftMM() + (measure.gapHMM() * h)) / 2,
                        int(measure.gapTopMM() + (measure.gapVMM() * v)) / 2,
                        (int)measure.widthMM() / 2, (int)measure.heightMM() / 2 );
        }
    }
}

/*****************************************/
DefinitionDialog::DefinitionDialog( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    resize( 465, 345 );
    setCaption( i18n( "Add Label Definition" ) );
    Q3HBoxLayout* layout = new Q3HBoxLayout( this, 6, 6 );
    DefinitionDialogLayout = new Q3VBoxLayout( 0, 11, 6, "DefinitionDialogLayout");

    Layout17 = new Q3HBoxLayout( 0, 0, 6, "Layout17"); 

    Layout13 = new Q3VBoxLayout( 0, 0, 6, "Layout13"); 

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( i18n( "Producer:" ) );
    Layout13->addWidget( TextLabel1 );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setText( i18n( "Type:" ) );
    Layout13->addWidget( TextLabel2 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setText( QString( i18n( "Width (in %1):" ) ).arg( Measurements::system() ) );
    Layout13->addWidget( TextLabel3 );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setText( QString( i18n( "Height (in %1):" ) ).arg( Measurements::system() ) );
    Layout13->addWidget( TextLabel4 );

    TextLabel5 = new QLabel( this, "TextLabel5" );
    TextLabel5->setText( i18n( "Horizontal Gap:" ) );
    Layout13->addWidget( TextLabel5 );

    TextLabel6 = new QLabel( this, "TextLabel6" );
    TextLabel6->setText( i18n( "Vertical Gap:" ) );
    Layout13->addWidget( TextLabel6 );

    TextLabel7 = new QLabel( this, "TextLabel7" );
    TextLabel7->setText( i18n( "Top Gap:" ) );
    Layout13->addWidget( TextLabel7 );

    TextLabel8 = new QLabel( this, "TextLabel8" );
    TextLabel8->setText( i18n( "Left Gap:" ) );
    Layout13->addWidget( TextLabel8 );

    TextLabel9 = new QLabel( this, "TextLabel9" );
    TextLabel9->setText( i18n("Number Horizontal:") );
    Layout13->addWidget( TextLabel9 );

    TextLabel10 = new QLabel( this, "TextLabel10" );
    TextLabel10->setText( i18n("Number Vertical:") );
    Layout13->addWidget( TextLabel10 );
     
    Layout17->addLayout( Layout13 );
    Layout14 = new Q3VBoxLayout( 0, 0, 6, "Layout14"); 

    comboProducer = new KComboBox( FALSE, this );
    comboProducer->setObjectName( "comboProducer" );
    comboProducer->setEditable( TRUE );
    Layout14->addWidget( comboProducer );

    comboType = new KComboBox( FALSE, this );
    comboType->setObjectName( "type" );
    comboType->setEditable( TRUE );
    Layout14->addWidget( comboType );

    editWidth = new KRestrictedLine( this, "editWidth" );
    editWidth->setText( i18n( "0" ) );
    editWidth->setValidChars( i18n( "0123456789." ) );
    Layout14->addWidget( editWidth );

    editHeight = new KRestrictedLine( this, "editHeight" );
    editHeight->setText( i18n( "0" ) );
    editHeight->setValidChars( i18n( "0123456789." ) );
    Layout14->addWidget( editHeight );

    editHGap = new KRestrictedLine( this, "editHGap" );
    editHGap->setText( i18n( "0" ) );
    editHGap->setValidChars( i18n( "0123456789." ) );
    Layout14->addWidget( editHGap );

    editVGap = new KRestrictedLine( this, "editVGap" );
    editVGap->setText( i18n( "0" ) );
    editVGap->setValidChars( i18n( "0123456789." ) );
    Layout14->addWidget( editVGap );

    editTGap = new KRestrictedLine( this, "editTGap" );
    editTGap->setText( i18n( "0" ) );
    editTGap->setValidChars( i18n( "0123456789." ) );
    Layout14->addWidget( editTGap );

    editLGap = new KRestrictedLine( this, "editLGap" );
    editLGap->setText( i18n( "0" ) );
    editLGap->setValidChars( i18n( "0123456789." ) );
    Layout14->addWidget( editLGap );

    editNumH = new KRestrictedLine( this, "editNumH" );
    editNumH->setText( i18n( "0" ) );
    editNumH->setValidChars( "0123456789" );
    Layout14->addWidget( editNumH );
    
    editNumV = new KRestrictedLine( this, "editNumV" );
    editNumV->setText( i18n( "0" ) );
    editNumV->setValidChars( "0123456789" );
    Layout14->addWidget( editNumV );

    Layout17->addLayout( Layout14 );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout17->addItem( spacer );
    DefinitionDialogLayout->addLayout( Layout17 );
    QSpacerItem* spacer_2 = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    DefinitionDialogLayout->addItem( spacer_2 );

    Layout16 = new Q3HBoxLayout( 0, 0, 6, "Layout16");
    buttonInfo = new KPushButton( i18n("More &Information"), this );
    Layout16->addWidget( buttonInfo );

    QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout16->addItem( spacer_3 );

    buttonAdd = new KPushButton( this, "buttonAdd" );
    buttonAdd->setText( i18n( "&Add" ) );
    Layout16->addWidget( buttonAdd );

    buttonCancel = new KPushButton( this, "buttonCancel" );
    buttonCancel->setText( i18n( "&Cancel" ) );
    Layout16->addWidget( buttonCancel );
    DefinitionDialogLayout->addLayout( Layout16 );

    preview = new LabelPreview( this );
    layout->addLayout( DefinitionDialogLayout );
    layout->addWidget( preview );

    l = new QLabel( this );
    setExtension( l );
    setOrientation( Vertical );

    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonAdd, SIGNAL( clicked() ), this, SLOT( add() ) );

    connect( editWidth, SIGNAL( textChanged(const QString &) ), this, SLOT( updatePreview() ) );
    connect( editHeight, SIGNAL( textChanged(const QString &) ), this, SLOT( updatePreview() ) );
    connect( editHGap, SIGNAL( textChanged(const QString &) ), this, SLOT( updatePreview() ) );
    connect( editVGap, SIGNAL( textChanged(const QString &) ), this, SLOT( updatePreview() ) );
    connect( editTGap, SIGNAL( textChanged(const QString &) ), this, SLOT( updatePreview() ) );
    connect( editLGap, SIGNAL( textChanged(const QString &) ), this, SLOT( updatePreview() ) );
    connect( editNumH, SIGNAL( textChanged(const QString &) ), this, SLOT( updatePreview() ) );
    connect( editNumV, SIGNAL( textChanged(const QString &) ), this, SLOT( updatePreview() ) );

    comboProducer->setInsertPolicy( QComboBox::NoInsert );
    comboType->setInsertPolicy( QComboBox::NoInsert );

    comboProducer->clear();
    QStringList list = Definition::getProducers();
    comboProducer->insertStringList( list );

    types = new QStringList[list.count()];
    for( unsigned int i = 0; i < list.count(); i++ )
        types[i] = Definition::getTypes( list[i] );

    connect( comboProducer, SIGNAL( textChanged(const QString &) ), this, SLOT( updateType() ) );
    connect( comboType, SIGNAL( textChanged(const QString &) ), this, SLOT( updateText() ) );
    connect( buttonInfo, SIGNAL( clicked() ), this, SLOT( toggleExtension() ) );

    drawGraphic();
    updateType();

    showExtension( false );
}

DefinitionDialog::~DefinitionDialog()
{
    delete [] types;
}
                                            
void DefinitionDialog::add()
{
    Definition::write( getCurrentMeasure(), comboType->currentText(), comboProducer->currentText() );

    accept();
}

void DefinitionDialog::updateType()
{
    comboType->clear();
    if( comboProducer->currentText() == comboProducer->text( comboProducer->currentItem() ) )
        if( comboProducer->currentItem() < comboProducer->count())
            comboType->insertStringList( types[ comboProducer->currentItem() ] );
    comboType->addItem( i18n( "Custom" ), 0 );
}

const Measurements DefinitionDialog::getCurrentMeasure()
{
    Measurements m;
    m.setGapLeft( editLGap->text().toDouble() );
    m.setGapTop( editTGap->text().toDouble() );
    m.setWidth( editWidth->text().toDouble() );
    m.setHeight( editHeight->text().toDouble() );
    m.setGapV( editVGap->text().toDouble() );
    m.setGapH( editHGap->text().toDouble() );
    m.setNumH( editNumH->text().toInt() );
    m.setNumV( editNumV->text().toInt() );

    return m;
}

void DefinitionDialog::updateText()
{
    Definition   d( comboProducer->currentText(), comboType->currentText() );
    Measurements m = d.getMeasurements();

    editTGap->setText( I2S( m.gapTop() ) );
    editLGap->setText( I2S( m.gapLeft() ) );
    editWidth->setText( I2S( m.width() ) );
    editHeight->setText( I2S( m.height() ) );
    editVGap->setText( I2S( m.gapV() ) );
    editHGap->setText( I2S( m.gapH() ) );
    editNumH->setText( I2S( m.numH() ) );
    editNumV->setText( I2S( m.numV() ) );

    updatePreview();
}

void DefinitionDialog::updatePreview()
{
    Measurements m = getCurrentMeasure();

    editHeight->setPaletteForegroundColor( Qt::black );
    editWidth->setPaletteForegroundColor( Qt::black );
    editVGap->setPaletteForegroundColor( Qt::black );
    editHGap->setPaletteForegroundColor( Qt::black );
    editTGap->setPaletteForegroundColor( Qt::black );
    editLGap->setPaletteForegroundColor( Qt::black );
    editNumH->setPaletteForegroundColor( Qt::black );
    editNumV->setPaletteForegroundColor( Qt::black );
    
    // Mark errors in Red
    if( m.heightMM() > PrinterSettings::getInstance()->pageHeight() )
        editHeight->setPaletteForegroundColor( Qt::red );

    if( m.widthMM() > PrinterSettings::getInstance()->pageWidth() )
        editWidth->setPaletteForegroundColor( Qt::red );

    if( m.gapVMM() < m.heightMM() )
        editVGap->setPaletteForegroundColor( Qt::red );

    if( m.gapHMM() < m.widthMM() )
        editHGap->setPaletteForegroundColor( Qt::red );

    if( m.gapTopMM() >  PrinterSettings::getInstance()->pageHeight() - m.heightMM() )
        editTGap->setPaletteForegroundColor( Qt::red );

    if( m.gapLeftMM() > PrinterSettings::getInstance()->pageWidth() - m.widthMM() )
        editLGap->setPaletteForegroundColor( Qt::red );

    if( m.gapLeftMM() + m.numH() * m.gapHMM() > PrinterSettings::getInstance()->pageWidth() )
        editNumH->setPaletteForegroundColor( Qt::red );

    if( m.gapTopMM() + m.numV() * m.gapVMM() > PrinterSettings::getInstance()->pageHeight() )
        editNumV->setPaletteForegroundColor( Qt::red );

    preview->setRect( QRect( (int)m.gapLeftMM(), (int)m.gapTopMM(), (int)m.widthMM(), (int)m.heightMM() ) );
    preview->setMeasurements( m );
    preview->repaint();
}

void DefinitionDialog::drawGraphic()
{
    QMatrix wm;
    wm.rotate( 90 );

    QPixmap pic( 450, 330 );
    pic.fill( Qt::gray );

    QPainter p( &pic );
    p.setPen( Qt::black );

    p.fillRect( 60, 60, 450, 330, QColor( 255, 254, 217 ) );
    p.drawRect( 60, 60, 450, 330 );
    p.setBrush( Qt::white );

    p.setPen( Qt::DotLine );
    p.drawRoundRect( 90, 90, 150, 90 );
    p.drawRoundRect( 270, 90, 150, 90 );

    p.drawRoundRect( 90, 210, 150, 90 );
    p.drawRoundRect( 270, 210, 150, 90 );

    // Draw width
    p.setPen( QPen( Qt::red, 2 ) );
    p.drawLine( 90, 150, 240, 150 );

    // Draw numh
    p.drawLine( 90, 270, 450, 270 );

    // Draw Gap Top
    p.setPen( QPen( Qt::red, 2, Qt::DotLine ) );
    p.drawLine( 0, 60, 90, 60 );
    p.drawLine( 0, 90, 90, 90 );

    // draw Gap V
    p.drawLine( 0, 210, 90, 210 );

    // Draw height
    p.setPen( QPen( Qt::green, 2 ) );
    p.drawLine( 150, 90, 150, 180 );


    // Draw numv
    p.drawLine( 330, 90, 330, 330 );

    // Draw Gap Left
    p.setPen( QPen( Qt::green, 2, Qt::DotLine ) );
    p.drawLine( 60, 0, 60, 90 );
    p.drawLine( 90, 0, 90, 90 );

    // draw Gap H
    p.drawLine( 270, 0, 270, 90 );

    // draw Texts:
    p.setPen( Qt::black );
    p.drawText( 100, 140, i18n("Width") );
    p.drawText( 160, 120, i18n("Height") );
    p.drawText( 100, 260, i18n("Number of horizontal Labels") );

    QPixmap* pix = LabelUtils::drawString( i18n("Number of vertical Labels") );
    p.drawPixmap( 340, 110, pix->transformed( wm ) );
    delete pix;

    p.drawText( 5, 80, i18n("Gap Top") );

    pix = LabelUtils::drawString( i18n("Gap Left") );

    p.drawPixmap( 60, 5, pix->transformed( wm ) );
    delete pix;

    p.drawText( 5, 150, i18n("Vertical Gap") );
    p.drawText( 150, 50, i18n("Horizontal Gap") );

    p.end();

    l->setPixmap( pic );
}

void DefinitionDialog::toggleExtension()
{
    if( l->isVisible() ) {
        showExtension( false );
    } else
        showExtension( true );
}


#include "definitiondialog.moc"
