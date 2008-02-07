/***************************************************************************
                          barcodedialog.h  -  description
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

#ifndef BARCODEDIALOG_H
#define BARCODEDIALOG_H

#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <QPixmap>
#include <Q3HBoxLayout>
#include <QLabel>

class BarcodeCombo;
class BarcodeWidget;
class KAction;
class KColorButton;
class KMenuBar;
class KToolBar;
class KIntNumInput;
class KLineEdit;
class KPushButton;
class KPopupMenu;
class QCheckBox;
class QLabel;
class QPixmap;
class Q3VBoxLayout;
class Q3HBoxLayout; 
class Q3GridLayout;
class TokenProvider;

/** This class provides a dialog, where the user can create a single barcode
  * and print it, copy it to the clipboard or save it in various image formats.
  * This dialog can also be seen as a powerful replacement for e.g. xbarcode.
  *
  * All barcoding features are available for the user. This dialog allows also
  * to just experiment a little bit with barcodes.
  */
class BarCodeDialog : public QDialog
{ 
    Q_OBJECT

    public:
        BarCodeDialog( QWidget* parent = 0, const char* name = 0 );
        ~BarCodeDialog();

    private:
        BarcodeWidget* widget;
        TokenProvider* m_token;

        KPushButton* buttonGenerate;
        KPushButton* buttonPrint;
        KPushButton* buttonSave;
        KPushButton* buttonCopy;
        KPushButton* buttonClose;
        QLabel* barcode;

    private slots:
        void generate();
        void save();
        void print();
        void copy();

    protected:
        Q3HBoxLayout* BarCodeDialogLayout;
        Q3VBoxLayout* Layout6;
        Q3VBoxLayout* Layout5;
};

#endif // BARCODEDIALOG_H
