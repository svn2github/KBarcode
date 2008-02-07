/***************************************************************************
                          gnubarcode.cpp  -  description
                             -------------------
    begin                : 
    copyright            : (C) 2005 by Dominik Seichter
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

#include "gnubarcode.h"

#ifdef _ENABLE_NATIVE_GNU_BARCODE

#include <barcode.h>
#include "barkode.h"
#include <string.h>
#include <ctype.h>

#include <qfont.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>

#include <klocale.h>

#define FONT_SPACE (2*m_scaley)
#define SHRINK_AMOUNT 0.15
#define MUL_FACTOR 10

GnuBarcode::GnuBarcode()
    : BarkodeEngine()
{
    m_item = NULL;
    m_size = QSize( 0, 0 );
    
    m_scalex = 1.0;
    m_scaley = 1.0;
    
    m_font_size = 10;
}

GnuBarcode::~GnuBarcode()
{
    if( m_item )
        Barcode_Delete( m_item );
}

void GnuBarcode::update( const QPaintDevice* device )
{
    QString val = barkode->parsedValue();
    m_valid = false;
        
    if( m_item )
    {
        qDebug("Deleting");
        Barcode_Delete( m_item );
        m_item = NULL;
    }

    if( val.isEmpty() )
        return;

    char* value = new char[val.length()+1];
    strncpy( value, val.latin1(), val.length()+1 );
    value[val.length()] = '\0';
    
    m_item = Barcode_Create( value );
    if( !m_item )
    {
        Barcode_Delete( m_item );
        m_item = NULL;
        delete [] value;
        return;
    }
    
    if( Barcode_Encode( m_item, Barkode::internalType( barkode->type() ) ) == -1 )
    {
        Barcode_Delete( m_item );
        m_item = NULL;
        
        delete [] value;
        return;
    }

    delete [] value;
 
    Q3PaintDeviceMetrics metrics( device );
    m_scalex = (double)metrics.logicalDpiX() / (double)QPaintDevice::x11AppDpiX();
    m_scaley = (double)metrics.logicalDpiY() / (double)QPaintDevice::x11AppDpiY();
   
    // 72.0 is the postscript resolution generated by GNU barcode
    m_scale_ps_x = (double)metrics.logicalDpiX() / 72.0;
    m_scale_ps_y = (double)metrics.logicalDpiY() / 72.0;
    
/*    m_scalex *= m_scale_ps_x;
    m_scaley *= m_scale_ps_y;*/
    
    m_font_size = (int)(barkode->fontsize() * barkode->scaling() * m_scaley);
    setupSize();
    m_valid = true;
}

const QSize GnuBarcode::size() const
{
    if( m_size.isNull() )
        return QSize( 100, 80 );
    else
        return m_size;
}

void GnuBarcode::drawBarcode( QPainter & painter, int x, int y )
{
    int dx = x + barkode->quietZone();
    int dy = y + barkode->quietZone();
    
    if( !m_item )
    {
        barkode->drawInvalid( painter, x, y );
        return;
    }
    
    painter.save();
    painter.scale( m_scalex, m_scaley );

    // based on David J. Humphreys barcode-svg
    // Draw the bars
    drawBars( &painter, (int)(dx/m_scalex), (int)(dy/m_scaley) );

    // Only scale for the bars, text should be scaled 
    // correctly by Qt on Qt >= 3.3.2    
    painter.restore();
    
    // draw the text
    if( barkode->textVisible() )
        drawText( &painter, dx, dy );
}

void GnuBarcode::setupSize()
{
    m_bar_height = (int)(barkode->barHeight() * barkode->scaling() * barkode->cut());
    m_barcode_height = m_bar_height;
    if( barkode->textVisible() )
        m_barcode_height += (int)(m_font_size/2 + FONT_SPACE*2);
    
    unsigned int width = drawBars( 0 , 0, 0 );
    unsigned int twidth = drawText( 0, 0, 0 );
    
    //if(m_item->height == 0) 
    m_item->height = m_barcode_height + (2 * barkode->quietZone());
    
    width = ( width > twidth ? width : twidth );
    width += 2* barkode->quietZone();

    //if(m_item->width == 0)  
    m_item->width = width;

    m_size.setWidth( (int)(m_item->width * m_scalex) );
    m_size.setHeight( (int)(m_item->height * m_scaley) );
}

int GnuBarcode::drawBars( QPainter* painter, int x, int y )
{
#if 0
    int height = 0; // height of the bar to draw
    int xpos = 0;   // Where the current box is drawn
    int current = 0;
    unsigned int i = 0;

    qDebug("Partial=%s\n", m_item->partial );
    for( i = 0; i < strlen(m_item->partial); i++)
    {
        current = (int)m_item->partial[i] - ASCII_ZERO;
        /* Guide bar */
        if(current > 9)
        {
            height = m_barcode_height; 
            current = (int)m_item->partial[++i] - ASCII_ZERO;
            i++;   /* Skip the following 'a' */
        }
        else
            height = m_bar_height;

        current *= barkode->scaling();
        
        if( current < 0 )
        {
            qDebug("current < 0: %i", current );
            current = 0;
            break;
        }
           
        if( i % 2)
        {
            painter.fillRect( x + xpos, y, current, height, barkode->foreground() );
        }
        
        xpos += current;
    }
#endif
    int xpos;
    int mode = '-'; // text below bars
    char *ptr;
    int i, j;
    int x0, yr, y0;
    
    xpos = m_item->partial[0]-'0';
    
    for (ptr = m_item->partial+1, i=1; *ptr; ptr++, i++) 
    {
        /* special cases: '+' and '-' */
        if (*ptr == '+' || *ptr == '-') 
        {
            mode = *ptr; /* don't count it */ i++; continue;
        }
        /* j is the width of this bar/space */
        if (isdigit (*ptr))   
            j = *ptr-'0';
        else
            j = *ptr-'a'+1;
        
        j = (int)( j * barkode->scaling() );
        if (i%2) 
        { 
            x0 = xpos + j/2;
            y0 = y;
            yr = m_barcode_height;
            
            if( barkode->textVisible() ) 
            { 
                // leave space for text
                if (mode == '-') 
                {
                    // text below bars
                    if( isdigit( *ptr ) )
                        yr = m_bar_height;
                    else
                        yr = m_barcode_height;
                } else { /* '+' */
                    // text above bars
                    // TODO: this needs testing and a GUI
                    if( isdigit( *ptr ) )
                        yr = m_bar_height;
                    else
                    {
                        yr = m_barcode_height;
                        y0 += (m_barcode_height - m_bar_height);
                    }
                }
            }
            
            if( painter )
            {
                painter->fillRect( x + x0, y0, j, yr, barkode->foreground() ); 
            }
            //qDebug("Bar  = %i", j );
        }
        xpos += j;
    }
    
    return xpos;
}

int GnuBarcode::drawText( QPainter* painter, int x, int y )
{
#if 0
    unsigned int infosz = strlen( m_item->textinfo );
    unsigned int correction = 0;    // This correction seems to be needed to align text properly
    unsigned int j = 0;
    unsigned int i = 0;
    int prev_x = 0;
    int xpos = 0;

    double dub1, fsize;
    char printer;
    char *temp_info = new char[infosz+1];

    painter.setFont( QFont( "Helvetica", 12 ) );
    while(i < infosz)
    {
        for(j = 0; m_item->textinfo[i + j + 1] != ' ' &&
            m_item->textinfo[i + j + 1] != '\0';j++);   /* Empty loop, just increment j */
        j ++;
        strncpy(temp_info, (m_item->textinfo + i),j);
        sscanf(temp_info, "%lf:%lf:%c", &dub1, &fsize, &printer);
        i += j;

        xpos = (int)dub1;
        //if((xpos - prev_x) >= 10)
        //correction += 2;

        prev_x = xpos;
        painter.drawText( x + ((xpos-correction) * m_scalex), 
                          y + ((m_bar_height + (unsigned int)(barkode->fontsize()/ 2) + FONT_SPACE) * m_scaley), 
                          QChar( printer ) );
    }
    delete [] temp_info;
#endif

    double f1, f2;
    int y0;
    char* ptr;
    char c;
  
    int mode = '-';
    for (ptr = m_item->textinfo; ptr; ptr = strchr(ptr, ' ')) 
    {
        while (*ptr == ' ') 
            ptr++;
        if (!*ptr) 
            break;
        if (*ptr == '+' || *ptr == '-') 
        {
            mode = *ptr; continue;
        }
        
        if (sscanf(ptr, "%lf:%lf:%c", &f1, &f2, &c) != 3) 
        {
            fprintf(stderr, "barcode: impossible data: %s\n", ptr);
            continue;
        }

        if( mode == '-' )
        {
            // text below bars
            y0 = y + (int)(m_bar_height * m_scaley + m_font_size - FONT_SPACE);
        }
        else
            y0 = (int)(y + FONT_SPACE);
            
        if( painter )
        {
            painter->setFont( QFont( "Helvetica", m_font_size ) );
            painter->drawText( x + (int)(f1 * m_scalex * barkode->scaling() ), y0, QChar( c ) );
        }
    }
    
    return x;
}

#endif // _ENABLE_NATIVE_GNU_BARCODE

