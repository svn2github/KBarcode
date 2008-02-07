/***************************************************************************
                         rectitem.h  -  description
                             -------------------
    begin                : Do Sep 2 2004
    copyright            : (C) 2004 by Dominik Seichter
    email                : domseichter@web.de
 ***************************************************************************/

/***************************************************************************
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 2 of the License, or     
    (at your option) any later version.                                   
                                                                          
 ***************************************************************************/

#ifndef RECTITEM_H
#define RECTITEM_H
#include <qstring.h>
#include "documentitem.h"
#include "qcolor.h"

/**
 * Class RectItem
 * A rectangle or elipse.
 */
class RectItem : public DocumentItem {
public:
    RectItem ( );
    
    int rtti() const { return eRtti_Rect; }
    /**
     * 
     * @param c 
     */
    void  setColor (const QColor & c=Qt::black);
    QColor  color () const;
        
    
    /**
     * 
     * @param f 
     */
    void  setFilled (bool f=true);
    bool  filled () const;
    
    /**
     * 
     * @param b 
     */
    void  setCircle (bool b=false);
    bool  circle () const;

    void  loadXML (QDomElement* element);
    void  saveXML (QDomElement* element);
    void  draw (QPainter* painter); 
    void drawZpl( Q3TextStream* stream );
    void drawIpl( Q3TextStream* stream, IPLUtils* utils );
    void drawEPcl( Q3TextStream* stream );
    
private:    
    void init();
    
private:
     bool m_circle;
     bool m_filled;
     QColor m_color;
};
#endif //RECTITEM_H

