/***************************************************************************
                          barcodedialog.cpp  -  description
                             -------------------
    begin                : Son Dez 29 2002
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

#include "barcodedialog.h"
#include "barcodeitem.h"
#include "printersettings.h"
#include "barcodecombo.h"
#include "mimesources.h"
#include "tokenprovider.h"

// KDE includes
#include <kapplication.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kimageio.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qprinter.h>
#include <kpushbutton.h>

// Qt includes
#include <qclipboard.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <q3picture.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

BarCodeDialog::BarCodeDialog( QWidget* parent,  const char* name )
    : QDialog( parent, name, false)
{
    setCaption( i18n( "Barcode Generator" ) );

    BarCodeDialogLayout = new Q3HBoxLayout( this, 11, 6, "BarCodeDialogLayout");
    Layout5 = new Q3VBoxLayout( 0, 0, 6, "Layout5");
    Layout6 = new Q3VBoxLayout( 0, 0, 6, "Layout2");
    widget = new BarcodeWidget( this, "widget" );

    m_token = new TokenProvider( KApplication::desktop() );
    widget->setTokenProvider( m_token );

    buttonGenerate = new KPushButton( this, "buttonGenerate" );
    buttonGenerate->setText( i18n( "&Generate" ) );
    buttonGenerate->setEnabled( Barkode::haveBarcode() );
    buttonGenerate->setIconSet( SmallIconSet("barcode") );
    
    buttonSave = new KPushButton( this, "buttonSave" );
    buttonSave->setText( i18n( "&Save" ) );
    buttonSave->setEnabled( false );
    buttonSave->setIconSet( SmallIconSet("filesave") );
        
    buttonCopy = new KPushButton( this, "buttonCopy" );
    buttonCopy->setText( i18n("&Copy") );
    buttonCopy->setEnabled( false );
    buttonCopy->setIconSet( SmallIconSet("editcopy") );
    
    buttonPrint = new KPushButton( this );
    buttonPrint->setText( i18n("&Print") );
    buttonPrint->setEnabled( false );
    buttonPrint->setIconSet( SmallIconSet("fileprint") );
    
    buttonClose = new KPushButton( this );
    buttonClose->setText( i18n("&Close" ) );
    buttonClose->setIconSet( SmallIconSet("fileclose") );
    

    Q3ScrollView* sv = new Q3ScrollView( this );
    
    barcode = new QLabel( sv->viewport(), "barcode" );
    sv->addChild( barcode );
    connect( buttonGenerate, SIGNAL( clicked() ), this, SLOT( generate() ) );
    connect( buttonSave, SIGNAL( clicked() ), this, SLOT( save() ) );
    connect( buttonPrint, SIGNAL( clicked() ), this, SLOT( print() ) );
    connect( buttonCopy, SIGNAL( clicked() ), this, SLOT( copy() ) );

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding );

    Layout5->addWidget( buttonGenerate );
    Layout5->addWidget( buttonSave );
    Layout5->addWidget( buttonPrint );
    Layout5->addWidget( buttonCopy );
    Layout5->addItem( spacer );
    Layout5->addWidget( buttonClose );
    
    Layout6->addWidget( widget );
    Layout6->addWidget( sv );
    
    BarCodeDialogLayout->addLayout( Layout6 );
    BarCodeDialogLayout->addLayout( Layout5 );
    BarCodeDialogLayout->setStretchFactor( Layout6, 2 );

    connect( buttonClose, SIGNAL( clicked() ), this, SLOT( close() ) );
    
    buttonGenerate->setDefault( true );
    
    show();
}

BarCodeDialog::~BarCodeDialog()
{
    delete m_token;
}

void BarCodeDialog::generate()
{        
    Barkode d;
    widget->getData( d );
    d.setTokenProvider( m_token );
    d.update( KApplication::desktop() );

    barcode->setPixmap( d.pixmap() );

    buttonSave->setEnabled( !barcode->pixmap()->isNull() );
    buttonPrint->setEnabled( !barcode->pixmap()->isNull() );
    buttonCopy->setEnabled( !barcode->pixmap()->isNull() );
}

void BarCodeDialog::save()
{
    Barkode bc;
    widget->getData( bc );

    if(!bc.isValid()) 
    {
        KFileDialog fd( ":save_image", KImageIO::pattern( KImageIO::Writing ), this, "fd", true );
        fd.setMode( KFile::File );
        fd.setOperationMode( KFileDialog::Saving );       
        if( fd.exec() == QDialog::Accepted ) 
        {
            QString path = fd.selectedURL().path();
            QString extension = KImageIO::type( path );
        
            if( extension.isNull() )
                extension = KImageIO::type( fd.currentFilter() ); 

        
            bc.setTokenProvider( m_token );
            bc.update( KApplication::desktop() );

            if(!bc.pixmap().save( path, extension, 0 ))
                KMessageBox::error( this, i18n("An error occurred during saving the image") );
        }
    }        
}

void BarCodeDialog::print()
{
    Barkode d;
    widget->getData( d );
    
    if( d.isValid() )
        return;

    QPrinter* printer = PrinterSettings::getInstance()->setupPrinter( "kbarcode", this );
    if( !printer )
        return;

    // unless we can center the barcode
    printer->setFullPage( false );
    
    Q3PaintDeviceMetrics metrics( printer );
    
    double scalex = (double)metrics.logicalDpiX() / (double)QPaintDevice::x11AppDpiX();
    double scaley = (double)metrics.logicalDpiY() / (double)QPaintDevice::x11AppDpiY();
    
    Q3Picture picture;
    QPainter p( printer );
    p.scale( scalex, scaley );
    // TODO: center barcode

    TokenProvider tp( printer );
    
    d.setTokenProvider( &tp );
    d.update( printer );

    picture = d.picture();
    p.drawPicture( QPoint( 0, 0 ), picture );
    p.end();
    
    delete printer;
}

void BarCodeDialog::copy()
{
    if( barcode->pixmap()->isNull() )
        return;
        
    Barkode bc;
    widget->getData( bc );

    BarcodeItem* item = new BarcodeItem( bc );
    DocumentItemList list;
    list.append( item );
    DocumentItemDrag* drag = new DocumentItemDrag();
    drag->setDocumentItem( &list );

#if QT_VERSION >= 0x030100
    kapp->clipboard()->setData( drag, QClipboard::Clipboard );
#else
    kapp->clipboard()->setData( drag );
#endif
}

#include "barcodedialog.moc"
