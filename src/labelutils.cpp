/***************************************************************************
                          labelutils.cpp  -  description
                             -------------------
    begin                : Sam Okt 26 2002
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

#include "labelutils.h"
#include "dsrichtext.h" 

// Qt includes
#include <qbitmap.h>
#include <qimage.h>
#include <q3paintdevicemetrics.h>
#include <qpainter.h>
#include <q3simplerichtext.h>
//Added by qt3to4:
#include <QPixmap>

// KDE includes
#include <kapplication.h>
#include <kglobal.h>
#include <klocale.h>

// own includes
#include "printersettings.h"

#define CONVERSION_FACTOR 25.4000508001016

LabelUtils::LabelUtils()
{
}

LabelUtils::~LabelUtils()
{ }

double LabelUtils::pixelToMm( double pixel, const QPaintDevice* device, int mode )
{
    QPaintDeviceMetrics pdm( device ? device : KApplication::desktop() );
    if( mode == DpiX )
	return (pixel * CONVERSION_FACTOR) / (double)pdm.logicalDpiX();
    else
	return (pixel * CONVERSION_FACTOR) / (double)pdm.logicalDpiY();
}

double LabelUtils::mmToPixel( double mm, const QPaintDevice* device, int mode )
{
    if( !mm )
        return 0;

    // We don't get valid metrics from the printer - and we want a better resolution
    // anyway (it's the PS driver that takes care of the printer resolution).

    QPaintDeviceMetrics pdm( device ? device : KApplication::desktop() );
    
//    qDebug("DpiX=%i", pdm.logicalDpiX());
//    qDebug("DpiY=%i", pdm.logicalDpiY());
    if( mode == DpiX )
	return (mm / CONVERSION_FACTOR) * (double)pdm.logicalDpiX();
    else
	return (mm / CONVERSION_FACTOR) * (double)pdm.logicalDpiY();
}

double LabelUtils::pixelToPixelX( double unit, const QPaintDevice* src, const QPaintDevice* dest )
{
    QPaintDeviceMetrics p1( src );
    QPaintDeviceMetrics p2( dest );

    return ( unit * (double)p2.logicalDpiX() ) / (double)p1.logicalDpiX();
}

double LabelUtils::pixelToPixelY( double unit, const QPaintDevice* src, const QPaintDevice* dest )
{
    QPaintDeviceMetrics p1( src );
    QPaintDeviceMetrics p2( dest );

    //return pixelToPixelX( unit, src, dest );
    return ( unit * (double)p2.logicalDpiY() ) / (double)p1.logicalDpiY();
}

const QString LabelUtils::getTypeFromCaption( const QString & cap )
{
    // TODO: remove this function
    QString search = cap.right( cap.length() - cap.find(":") - 1 ).toLower().trimmed();
    return search;
}

const QString LabelUtils::getModeFromCaption( const QString & cap )
{
    return cap.left( cap.find(":") ).toLower().trimmed();
}

QSize LabelUtils::stringSize( const QString & t )
{
    QSimpleRichText srt( t, KApplication::font() );
    QSize s;
    s.setWidth( srt.widthUsed() );
    s.setHeight( srt.height() );

    return s;
}

void LabelUtils::renderString( QPainter* painter, const QString & t, const QRect & rect, double scalex, double scaley )
{
    // DSRichText cannot calculate the width on its own
    QSimpleRichText srt( t, KApplication::font() );
    int width = (rect.width() > 0) ? rect.width() : srt.widthUsed();
    int height = (rect.height() > 0) ? rect.height() : srt.height();
    
    DSRichText r( t );
    r.setX( rect.x() );
    r.setY( rect.y() );
    r.setWidth( width );
    r.setHeight( height );
    r.setScale( scalex, scaley );
    r.draw( painter );    
}

QPixmap* LabelUtils::drawString( const QString & t, int w, int h, double rot )
{
    QSimpleRichText srt( t, KApplication::font() );

    int width = (w > 0) ? w : srt.widthUsed();
    int height = (h > 0) ? h : srt.height();
    srt.setWidth( width );
    
    QPixmap* pix;
    QPainter painter;
    if( rot == 0.0 ) {
        QBitmap bm( width, height );
        bm.fill( Qt::color0 ); //transparent
        painter.begin( &bm );
    
          painter.save();
            painter.setPen( Qt::color1 );
            QColorGroup cg;
            cg.setColor( QColorGroup::Foreground, Qt::color1 );
              cg.setColor( QColorGroup::Text, Qt::color1 );
              cg.setColor( QColorGroup::Base, Qt::color0 );
    
            srt.draw( &painter, 0, 0, bm.rect(), cg );
          painter.restore();
        painter.end();
        
        pix = new QPixmap( width, height );
        pix->fill( Qt::white );
        pix->setMask( bm );
            
        if( !pix->isNull() ) {
            painter.begin( pix );
            painter.setPen( Qt::black );
            QColorGroup cg;
            srt.draw( &painter, 0, 0, pix->rect(), cg );
            painter.end();
        } 
    } else {
        int w2 = (w > 0) ? w : srt.widthUsed();
        int h2 = (h > 0) ? h : srt.height();

        QMatrix wm;
        wm.rotate( rot );

        QSize s = LabelUtils::stringSize( t );
        QPixmap* tmp = LabelUtils::drawString( t, s.width(), s.height() );

        QPixmap* p = new QPixmap( w2, h2 );
        p->fill( Qt::white );
        painter.begin( p );
        painter.drawPixmap( 0, 0, tmp->transformed( wm ) );
        painter.end();

        p->setMask( p->createHeuristicMask() );

        pix = p;
        delete tmp;
    }
    
    return pix;
}

