/***************************************************************************
                          kbarcodeiface.h  -  description
                             -------------------
    begin                : Sat Dec 27 23:53:28 CET 2003
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

#ifndef KBARCODEIFACE_H
#define KBARCODEIFACE_H

#include <dcopobject.h>

class KBarcodeIface : virtual public DCOPObject
{
    K_DCOP

    public:
    k_dcop:
        virtual void startBarcode() = 0;
        virtual void startLabelEditor() = 0;
        virtual void startBatchPrint() = 0;

        virtual void editArticles() = 0;
        virtual void editCustomers() = 0;
        virtual void editCustomerText() = 0;
        virtual void editLabelDef() = 0;

        virtual void importCSV() = 0;

        virtual bool isSQLConnected() const = 0;
        virtual bool connectSQL() = 0;
        virtual void showWizard() = 0;
        virtual void showConfigure() = 0;
};

#endif /* KBARCODEIFACE_H */
