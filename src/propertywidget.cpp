/***************************************************************************
                         propertywidget.h  -  description
                             -------------------
    begin                : Do Sep 10 2004
    copyright            : (C) 2004 by Dominik Seichter
    email                : domseichter@web.de
 ***************************************************************************/

/***************************************************************************
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 2 of the License, or     
    (at your option) any later version.                                   
                                                                          
 ***************************************************************************/

#include "propertywidget.h"
#include "documentitem.h"
#include "rectitem.h"
#include "commands.h"
#include "sqltables.h"
#include "barcodecombo.h"
#include "multilineeditdlg.h"
#include "textitem.h"
#include "imageitem.h"
#include "measurements.h"
#include "mycanvasview.h"
//NY20
#include "textlineitem.h"
#include "textlineedit.h"
//NY20
#include "tokendialog.h"

#include <kcolorbutton.h>
#include <kcombobox.h>
#include <kcommand.h>
#include <kiconloader.h>
#include <kimageio.h>
#include <klineedit.h>
#include <klocale.h>
#include <knuminput.h>
#include <ktextedit.h>
#include <kurlrequester.h>

#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qradiobutton.h>
#include <qsqlquery.h>
#include <q3vbox.h>
#include <qvbuttongroup.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <QPixmap>
#include <kmessagebox.h>

#define IMAGE_INTERNAL i18n("Image Saved in KBarcode")

void FillLineCombo( KComboBox* box )
{
    /* A small helper function to fill
     * a combobox with all by Qt
     * supported pen styles.
     */

    // TODO: Use the enum for Qt line styles here!

    QPainter p;
    for( int i = 1; i < 6; i++ ) {
        QPixmap pixmap( 60, 20 );
        pixmap.fill( Qt::white );
        p.begin( &pixmap );
        p.setPen( QPen( Qt::black, 3, (Qt::PenStyle)i ) );
        p.drawLine( 5, 10, 55, 10 );
        p.end();

        box->insertItem( pixmap );
    }
}

PropertyWidget::PropertyWidget(QWidget* parent )
 : QWidget( parent, 0 )
{
    grid = new Q3GridLayout( this, 2, 2 );
}

PropertyBorder::PropertyBorder( QWidget* parent )
    : PropertyWidget( parent )
{
    checkBorder = new QCheckBox( this );
    checkBorder->setText( i18n("&Border Visible") );
    
    buttonColor = new KColorButton( this );

    spinWidth = new KIntNumInput( this );
    spinWidth->setRange( 1, 100, 1, false );

    comboLine = new KComboBox( false, this );
    FillLineCombo( comboLine );    

    grid->addMultiCellWidget( checkBorder, 0, 0, 0, 1 );
    grid->addWidget( new QLabel( i18n("Color:"), this ), 1, 0 );
    grid->addWidget( buttonColor, 1, 1 );
    grid->addWidget( new QLabel( i18n("Line Width:"), this ), 2, 0 );
    grid->addWidget( spinWidth, 2, 1 );
    grid->addWidget( new QLabel( i18n("Line Style:"), this ), 3, 0 );
    grid->addWidget( comboLine, 3, 1 );
    
    
    connect( checkBorder, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
}

void PropertyBorder::applySettings( DocumentItem* item, KMacroCommand* command )
{
    BorderCommand* bc = new BorderCommand( checkBorder->isChecked(), QPen( buttonColor->color(), spinWidth->value(), (Qt::PenStyle)(comboLine->currentItem() + 1) ), item );
    bc->execute();
    command->addCommand( bc );
}

void PropertyBorder::initSettings( DocumentItem* item )
{
    checkBorder->setChecked( item->border() );
    buttonColor->setColor( item->pen().color() );
    spinWidth->setValue( item->pen().width() );
    comboLine->setCurrentItem( item->pen().style() - 1 );
    
    enableControls();
}

void PropertyBorder::enableControls()
{
    buttonColor->setEnabled( checkBorder->isChecked() );
    spinWidth->setEnabled( checkBorder->isChecked() );
    comboLine->setEnabled( checkBorder->isChecked() );
}


PropertyRotation::PropertyRotation( QWidget* parent )
    : PropertyWidget( parent )
{
    QLabel* label = new QLabel( i18n("&Rotation:"), this );
    comboRotation = new KComboBox( FALSE, this );
    label->setBuddy( comboRotation );

    comboRotation->insertItem( i18n("0") );
    comboRotation->insertItem( i18n("90") );
    comboRotation->insertItem( i18n("180") );
    comboRotation->insertItem( i18n("270") );

    grid->addWidget( label, 0, 0 );
    grid->addWidget( comboRotation, 0, 1  );

}

void PropertyRotation::applySettings( DocumentItem* item, KMacroCommand* command )
{
    TextItem* text = static_cast<TextItem*>(item);
    double rot = 0.0;

    if( comboRotation->currentItem() == 1 )
        rot = 90.0;
    else if( comboRotation->currentItem() == 2 )
        rot = 180.0;
    else if( comboRotation->currentItem() ==  3 )
        rot = 270.0;

    TextRotationCommand* rc = new TextRotationCommand( rot, text );
    rc->execute();
    command->addCommand( rc );
}

void PropertyRotation::initSettings( DocumentItem* item )
{
    TextItem* text = static_cast<TextItem*>(item);

    if( text->rotation() == 0.0 )
        comboRotation->setCurrentItem( 0 );
    else if( text->rotation() == 90.0 )
        comboRotation->setCurrentItem( 1 );
    else if( text->rotation() == 180.0 )
        comboRotation->setCurrentItem( 2 );
    else if( text->rotation() == 270.0 )
        comboRotation->setCurrentItem( 3 );
}

PropertyFill::PropertyFill( QWidget* parent )
    : PropertyWidget( parent )
{
    buttonColor = new KColorButton( this );
    
    grid->addWidget( new QLabel( i18n("Color:"), this ), 0, 0 );
    grid->addWidget( buttonColor, 0, 1 );
}

void PropertyFill::applySettings( DocumentItem* item, KMacroCommand* command )
{
    RectItem* rect = static_cast<RectItem*>(item);
    FillCommand* fc = new FillCommand( buttonColor->color(), rect );
    fc->execute();
    command->addCommand( fc );
}

void PropertyFill::initSettings( DocumentItem* item )
{
    RectItem* rect = static_cast<RectItem*>(item);
    buttonColor->setColor( rect->color() );
}

PropertyBarcode::PropertyBarcode( TokenProvider* token, QWidget* parent )
    : PropertyWidget( parent )
{
    QLabel* TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setText( i18n( "Barcode Settings:" ) );
            
    comboComplex = new KComboBox( FALSE, this );
    if( SqlTables::isConnected() )
    {
        QSqlQuery* query = new QSqlQuery("select encoding_type from barcode_basic group by encoding_type");
        while( query->next() )
            comboComplex->insertItem( "Main:" + query->value( 0 ).toString() );
        
        delete query;
        
        query = new QSqlQuery("select customer_no, encoding_type from customer_text group by customer_no, encoding_type");
        while( query->next() )
            comboComplex->insertItem( query->value( 0 ).toString() + ":" + query->value( 1 ).toString() );
        delete query;
    }
        
    comboComplex->insertItem( "Static" );

    barcode = new BarcodeWidget( this, "barcode" );
    barcode->setTokenProvider( token );

    //connect( this, SIGNAL( defaultClicked() ), this, SLOT( defaults() ) );
    connect( comboComplex, SIGNAL( activated(int) ), this, SLOT(changedCombo() ) );
    
    grid->addWidget( TextLabel1, 0, 0 );
    grid->addWidget( comboComplex, 0, 1 );
    grid->addMultiCellWidget( barcode, 1, 1, 0, 1 );
    
    changedCombo();
}

// void PropertyBarcode::defaults()
// {
//     comboComplex->setCurrentItem( 0 );
//     barcode->defaults();
// }

void PropertyBarcode::changedCombo()
{
    bool v = false;
    if( comboComplex->currentItem() == comboComplex->count() - 1 )
        v = true;

    barcode->setDataEnabled( v );
    barcode->setStandardEnabled( v );
}

void PropertyBarcode::applySettings( DocumentItem* item, KMacroCommand* command )
{
    BarcodeItem* bcode = static_cast<BarcodeItem*>(item);

    Barkode* d = new Barkode();
    barcode->getData( *d );
            
    if( comboComplex->currentItem() != comboComplex->count() - 1 ) {
        d->setType( getTypeFromCaption( comboComplex->currentText() ) );
        d->setValue( SqlTables::getInstance()->getBarcodeMaxLength( d->type() ) );
    }
    d->setDatabaseMode( comboComplex->currentText() );
    
    BarcodeCommand* bc = new BarcodeCommand( bcode, d );
    bc->execute();
    command->addCommand( bc );
}

void PropertyBarcode::initSettings( DocumentItem* item )
{
    BarcodeItem* bcode = static_cast<BarcodeItem*>(item);
    
    barcode->setData( *bcode );
    for( int i = 0; i < comboComplex->count(); i++ )
        if( comboComplex->text( i ).lower() == bcode->databaseMode().lower() )
            comboComplex->setCurrentItem( i );

    changedCombo();
}

PropertyText::PropertyText( TokenProvider* token, QWidget* parent )
    : PropertyWidget( parent )
{
    m_editor = new MultiLineEditor( token, this, "m_editor" );
    grid->addWidget( m_editor, 0, 0 );
}

void PropertyText::applySettings( DocumentItem* item, KMacroCommand* command )
{
    TextItem* text = static_cast<TextItem*>(item);
    
    TextChangeCommand* tc = new TextChangeCommand( text, m_editor->text() );
    tc->execute();
    command->addCommand( tc );
}

void PropertyText::initSettings( DocumentItem* item )
{
    TextItem* text = static_cast<TextItem*>(item);
    m_editor->setText( text->text() );
}

//NY21
PropertyTextLine::PropertyTextLine( TokenProvider* token, QWidget* parent )
    : PropertyWidget( parent )
{
    m_editor = new TextLineEditor( token, this );
    grid->addWidget( m_editor, 0, 0 );
}

void PropertyTextLine::applySettings( DocumentItem* item, KMacroCommand* command )
{
    TextLineItem* text = static_cast<TextLineItem*>(item);
    
    TextLineChangeCommand* tc = new TextLineChangeCommand( text, m_editor->text(), m_editor->getFontType(),m_editor->getVertMag(),m_editor->getHorMag() );
    tc->execute();
    command->addCommand( tc );
}

void PropertyTextLine::initSettings( DocumentItem* item )
{
    TextLineItem* text = static_cast<TextLineItem*>(item);
    m_editor->setText( text->text() );
    m_editor->setFontType(text->getFont());
    m_editor->setHorMag(text->getMagHor());
    m_editor->setVertMag(text->getMagVert());
}
//NY21
        
PropertySize::PropertySize( QWidget* parent )
    : PropertyWidget( parent )
{   
    const double low = -1000.0;
    const double max = 1000.0;
    Q3VBox* box = new Q3VBox( this );

    checkLock = new QCheckBox( i18n("&Protect item from being moved or resized"), box );    
    numTop = new KDoubleNumInput( low, max, 0.0, 0.2, 3, box );
    numLeft = new KDoubleNumInput( numTop, low, max, 0.0, 0.2, 3, box );
    numHeight = new KDoubleNumInput( numLeft, low, max, 0.0, 0.2, 3, box );
    numWidth = new KDoubleNumInput( numHeight, low, max, 0.0, 0.2, 3, box );
    
    numTop->setSuffix( Measurements::system() );
    numLeft->setSuffix( Measurements::system() );
    numHeight->setSuffix( Measurements::system() );
    numWidth->setSuffix( Measurements::system() );
    
    numTop->setLabel( i18n("&Top:"), AlignLeft | AlignCenter );
    numLeft->setLabel( i18n("&Left:"), AlignLeft | AlignCenter );
    numHeight->setLabel( i18n("&Height:"), AlignLeft | AlignCenter );
    numWidth->setLabel( i18n("&Width:"), AlignLeft | AlignCenter );
    
    grid->addWidget( box, 0, 0 );
    
    connect( checkLock, SIGNAL( clicked() ), this, SLOT( enableControls() )); 
}

void PropertySize::enableControls()
{
    numTop->setEnabled( !checkLock->isChecked() );
    numLeft->setEnabled( !checkLock->isChecked() );
    numHeight->setEnabled( !checkLock->isChecked() );
    numWidth->setEnabled( !checkLock->isChecked() );
}
        
void PropertySize::applySettings( DocumentItem* item, KMacroCommand* command )
{
    QRect r = item->rect();
    QPoint translation( 0, 0 );
    
    TCanvasItem* canvasItem = item->canvasItem();
    if( canvasItem )
    {
        MyCanvasView* view = canvasItem->canvasView();
        translation = view->getTranslation();
    }

    if( Measurements::measurementSystem() == Measurements::Metric )
    {
        r.setX( (int)(numLeft->value() * 1000));
        r.setY( (int)(numTop->value()  * 1000));
        r.setWidth( (int)(numWidth->value() * 1000));
        r.setHeight( (int)(numHeight->value() * 1000));
    }
    else
    {
        /* This conversion from int to millimeter should get its own
         * function or maybe even an own class.
         */
        #warning "test inch to mm!"
        r.setX( (int)(numLeft->value() * 1000 * 25.4000508001016 ) );
        r.setY( (int)(numTop->value()  * 1000 * 25.4000508001016 ) );
        r.setWidth( (int)(numWidth->value() * 1000 * 25.4000508001016 ) );
        r.setHeight( (int)(numHeight->value() * 1000 * 25.4000508001016 ) );
    }    
    
    if( canvasItem )
    {   
        if( item->rectMM().x() != r.x() || item->rectMM().y() != r.y() )
        {
            MoveCommand* mc = new MoveCommand( r.x(), r.y(), canvasItem );
            mc->execute();
            command->addCommand( mc );    
        }

        if( item->rectMM() != r )
        {
            ResizeCommand* rc = new ResizeCommand( canvasItem, false );
            rc->setRect( r.x(), r.y(), r.width(), r.height() );
            rc->execute();
            command->addCommand( rc );
        }
                
        if( checkLock->isChecked() != item->locked() )
        {
            LockCommand* lc = new LockCommand( checkLock->isChecked(), canvasItem );
            lc->execute();
            command->addCommand( lc );
        }
    }
}

void PropertySize::initSettings( DocumentItem* item )
{
    QRect r = item->rectMM();
    QPoint translation( 0, 0 );
    
    TCanvasItem* canvasItem = item->canvasItem();
    if( canvasItem )
    {
        MyCanvasView* view = canvasItem->canvasView();
        translation = view->getTranslation();
    }
    
    if( Measurements::measurementSystem() == Measurements::Metric )
    {
        numLeft->setValue( r.x() / 1000.0 );
        numTop->setValue( r.y() / 1000.0 );
        numWidth->setValue( r.width() / 1000.0 );
        numHeight->setValue( r.height() / 1000.0 );
    }
    else // Imperial
    {
        numLeft->setValue( (r.x() / 1000.0) / 25.4000508001016 );
        numTop->setValue( (r.y() / 1000.0) / 25.4000508001016 );
        numWidth->setValue( (r.width() / 1000.0) / 25.4000508001016 );
        numHeight->setValue( (r.height() / 1000.0) / 25.4000508001016 );
    }
    
    checkLock->setChecked( item->locked() );
    enableControls();
}

PropertyImage::PropertyImage( TokenProvider* token, QWidget* parent )
    : PropertyWidget( parent ), m_token( token )
{
    QLabel* label = new QLabel( i18n("&Rotation:"), this );
    comboRotation = new KComboBox( FALSE, this );
    label->setBuddy( comboRotation );

    Q3VButtonGroup* groupFile = new Q3VButtonGroup( i18n("&Image"), this );
    radioImagePath = new QRadioButton( i18n("&Load image from path"), groupFile );
    imgUrl = new KURLRequester( groupFile );
    imgUrl->setFilter( KImageIO::pattern( KImageIO::Reading ) );
    imgUrl->setMode( KFile::File | KFile::ExistingOnly | KFile::LocalOnly );
    radioImageExpression = new QRadioButton( i18n("&Read image path from expression"), groupFile );

    imgHBox = new Q3HBox( groupFile );
    imgHBox->setSpacing( 5 );

    imgExpression = new KLineEdit( imgHBox );
    buttonToken = new KPushButton( i18n("&Insert Data Field..."), imgHBox );
    buttonToken->setIconSet( QIcon( SmallIcon("contents") ) );

    comboRotation->insertItem( i18n("0") );
    comboRotation->insertItem( i18n("90") );
    comboRotation->insertItem( i18n("180") );
    comboRotation->insertItem( i18n("270") );
    
    Q3VButtonGroup* group = new Q3VButtonGroup( i18n("&Size"), this );
    
    radioOriginal = new QRadioButton( i18n("&None"), group );
    radioZoomed = new QRadioButton( i18n("&Zoom"), group );
    radioScaled = new QRadioButton( i18n("S&cale"), group );

    checkMirrorH = new QCheckBox( i18n("Mirror &Horizontaly"), this );
    checkMirrorV = new QCheckBox( i18n("Mirror &Vertically"), this );

    grid->addMultiCellWidget( groupFile, 0, 1, 0, 1 );    
    grid->addMultiCellWidget( group, 2, 3, 0, 1 );
    grid->addWidget( label, 4, 0 );
    grid->addWidget( comboRotation, 4, 1 );
    grid->addMultiCellWidget( checkMirrorH, 5, 5, 0, 1 );
    grid->addMultiCellWidget( checkMirrorV, 6, 6, 0, 1 );

    connect( radioImageExpression, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( radioImagePath, SIGNAL( clicked() ), this, SLOT( enableControls() ) );
    connect( buttonToken, SIGNAL( clicked() ), this, SLOT( slotTokens() ) );
}

void PropertyImage::applySettings( DocumentItem* item, KMacroCommand* command )
{
    ImageItem* img = static_cast<ImageItem*>(item);
    EImageScaling scaling = eImage_Original;
    double rot = 0.0;
    QPixmap pix;

    if( radioImagePath->isChecked() ) 
    {
	if( !imgUrl->url().isEmpty() )
	{
	    if( !pix.load( imgUrl->url() ) )
		KMessageBox::error( NULL, QString( i18n("Image format not supported for file: %1") ).arg( imgUrl->url() ) );
	}
	else
	{
	    pix = img->pixmap();
	}
    }

    if( radioZoomed->isChecked() )
        scaling = eImage_Zoomed;
    else if( radioScaled->isChecked() )
        scaling = eImage_Scaled;
    
    if( comboRotation->currentItem() == 1 )
        rot = 90.0;
    else if( comboRotation->currentItem() == 2 )
        rot = 180.0;
    else if( comboRotation->currentItem() ==  3 )
        rot = 270.0;
        
    PictureCommand* pc = new PictureCommand( rot, checkMirrorH->isChecked(), checkMirrorV->isChecked(), scaling, img );
    pc->setExpression( radioImageExpression->isChecked() ? imgExpression->text() : QString::null );
    pc->setPixmap( pix );
    pc->execute();
    command->addCommand( pc );
}

void PropertyImage::initSettings( DocumentItem* item )
{
    ImageItem* img = static_cast<ImageItem*>(item);
    QString expr = img->expression();

    if( !expr.isEmpty() )
    {
	radioImageExpression->setChecked( true );
	imgExpression->setText( expr );
    }
    else
    {
	// imgUrl->setURL( IMAGE_INTERNAL );
	radioImagePath->setChecked( true );
    }

    switch( img->scaling() )    
    {
        default:
        case eImage_Original:
            radioOriginal->setChecked( true );
            break;
        case eImage_Scaled:
            radioScaled->setChecked( true );
            break;
        case eImage_Zoomed:
            radioZoomed->setChecked( true );
            break;
    }
            
    if( img->rotation() == 0.0 )
        comboRotation->setCurrentItem( 0 );
    else if( img->rotation() == 90.0 )
        comboRotation->setCurrentItem( 1 );
    else if( img->rotation() == 180.0 )
        comboRotation->setCurrentItem( 2 );
    else if( img->rotation() == 270.0 )
        comboRotation->setCurrentItem( 3 );
        
    checkMirrorH->setChecked( img->mirrorHorizontal() );
    checkMirrorV->setChecked( img->mirrorVertical() );


    enableControls();
}

void PropertyImage::enableControls()
{
    imgHBox->setEnabled( radioImageExpression->isChecked() );
    imgUrl->setEnabled( radioImagePath->isChecked() );
}

void PropertyImage::slotTokens()
{
    TokenDialog tokendlg( m_token, this, "tokendlg" );
    if( tokendlg.exec() == QDialog::Accepted )
	imgExpression->insert( tokendlg.token() );
}

PropertyVisible::PropertyVisible( QWidget* parent )
    : PropertyWidget( parent )
{
    QLabel* label = new QLabel( i18n("Evaluate JavsScript code to define the visibility of this item:"), this );
    m_script = new KTextEdit( this );

    grid->addWidget( label, 0, 0 );
    grid->addMultiCellWidget( m_script, 1, 8, 0, 2 );
}

void PropertyVisible::applySettings( DocumentItem* item, KMacroCommand* command )
{
    TCanvasItem* canvasItem = item->canvasItem();
    ScriptCommand* sc = new ScriptCommand( m_script->text(), canvasItem );
    sc->execute();
    command->addCommand( sc);
}

void PropertyVisible::initSettings( DocumentItem* item )
{
    m_script->setText( item->visibilityScript() );
}

#include "propertywidget.moc"
