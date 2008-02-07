/***************************************************************************
                          editoriface.h  -  description
                             -------------------
    begin                : Sat Dec 27 09:53:28 CET 2003
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

#ifndef EDITORIFACE_H
#define EDITORIFACE_H

#include <dcopobject.h>

class EditorIface : virtual public DCOPObject
{
    K_DCOP

    public:
    k_dcop:

    virtual const QString fileName() const = 0;
    virtual bool isChanged() = 0;
    virtual void print() = 0;
    virtual bool newLabel() = 0;

    virtual void insertBarcode() = 0;
    virtual void insertPicture() = 0;
    virtual void insertText() = 0;
    virtual void insertRect() = 0;
    virtual void insertCircle() = 0;
    virtual void insertLine() = 0;

    virtual void changeDes() = 0;
    virtual void changeSize() = 0;

    virtual void preview() = 0;
    virtual void toggleGrid() = 0;

    virtual void closeLabel() = 0;

    virtual bool openUrl( const QString & url ) = 0;
    virtual bool save( QString url ) = 0;

};

#endif /* EDITORIFACE_H */
