/***************************************************************************
                          batchiface.h  -  description
                             -------------------
    begin                : Sat Dec 27 23:54:28 CET 2003
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

#ifndef BATCHIFACE_H
#define BATCHIFACE_H

#include <dcopobject.h>
#include <qstringlist.h>
#include "batchprinter.h"

class BatchIface : virtual public DCOPObject
{
    K_DCOP

    public:
    k_dcop:
        virtual void setFilename( const QString & url ) = 0;
        virtual void setImportCsvFile( const QString & filename ) = 0;
        virtual void setImportSqlQuery( const QString & query ) = 0;
        virtual void setNumLabels( const int n ) = 0;
        virtual void setOutputFormat( const int e ) = 0;
	virtual void setSerialNumber( const QString & val, int inc ) = 0;

        virtual bool addItem( const QString & article, const QString & group, int count = 1, bool msg = true ) = 0;
	virtual bool existsArticle( const QString & article ) = 0;
        virtual void loadFromFile( const QString & url ) = 0;
        virtual void loadFromClipboard() = 0;
	virtual void printNow( const QString & printer, bool bUserInteraction = true ) = 0;
};

#endif /* BATCHIFACE_H */

