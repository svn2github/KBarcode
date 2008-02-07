/***************************************************************************
                         barcodeprinterdlg.cpp  -  description
                             -------------------
    begin                : Fri Oct 01 2004
    copyright            : (C) 2004 by Dominik Seichter
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

#include "barcodeprinterdlg.h"
#include "printersettings.h"

#include <kcombobox.h>
#include <klocale.h>
#include <kurlrequester.h>

#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvbuttongroup.h> 
//Added by qt3to4:
#include <Q3GridLayout>

BarcodePrinterDlg::BarcodePrinterDlg(QWidget *parent, const char *name)
    : KDialogBase( KDialogBase::Plain, i18n("Barcode Printer"),
      KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, parent,name)
{
    Q3GridLayout* layout = new Q3GridLayout( plainPage(), 6, 6 );
    
    QLabel* label = new QLabel( i18n("&Output Format:"), plainPage() );
    comboFormat = new KComboBox( false, plainPage() );
    label->setBuddy( comboFormat );
    
    checkFile = new QCheckBox( i18n("&Print to File"), plainPage() );
    
    label2 = new QLabel( i18n("&Filename:"), plainPage() );
    requester = new KURLRequester( plainPage() );
    label2->setBuddy( requester );

    label3 = new QLabel( i18n("&Device:"), plainPage() );
    comboDevice = new KComboBox( true, plainPage() );
    label3->setBuddy( comboDevice );
    
    layout->addWidget( label, 0, 0 );
    layout->addMultiCellWidget( comboFormat, 0, 0, 1, 2 );
    layout->addMultiCellWidget( checkFile, 1, 1, 1, 2 );
    layout->addWidget( label2, 2, 0 );
    layout->addWidget( requester, 2, 1 );
    layout->addWidget( label3, 3, 0 );
    layout->addMultiCellWidget( comboDevice, 3, 3, 1, 2 );
    
    comboFormat->insertItem( i18n("TEC Printer (TEC)") );
    comboFormat->insertItem( i18n("Zebra Printer (ZPL)") );
    comboFormat->insertItem( i18n("Intermec Printer (IPL)") );
    comboFormat->insertItem( i18n("EPCL Printer (EPCL)") );
    
    // do not translate unix devicenames...
    comboDevice->insertItem( "/dev/lp0" );
    comboDevice->insertItem( "/dev/lp1" );
    comboDevice->insertItem( "/dev/lp2" );
    comboDevice->insertItem( "/dev/usb/lp0" );
    comboDevice->insertItem( "/dev/usb/lp1" );
    comboDevice->insertItem( "/dev/usb/lp2" );
    
    connect( checkFile, SIGNAL( clicked() ), this, SLOT( enableControls() ) );

//    KFileDialog fd( QString::null, "*.zpl|Zebra Printer Language (*.zpl)\n*.ipl|Intermec Printer Language (*.ipl)", this, "fd", true );
    
    enableControls();
}


BarcodePrinterDlg::~BarcodePrinterDlg()
{
}

void BarcodePrinterDlg::enableControls()
{
    bool b = checkFile->isChecked();
    label2->setEnabled( b );
    requester->setEnabled( b );
    
    label3->setEnabled( !b );
    comboDevice->setEnabled( !b );
}

int BarcodePrinterDlg::outputFormat() const
{
    switch( comboFormat->currentItem() )
    {
        case 0:
            return PrinterSettings::TEC;
        case 1:
            return PrinterSettings::ZEBRA;
        case 2:
            return PrinterSettings::INTERMEC;
        case 3:
            return PrinterSettings::EPCL;
        default:
            return -1;
    }
}

bool BarcodePrinterDlg::printToFile() const
{
    return checkFile->isChecked();
}

const QString BarcodePrinterDlg::deviceName() const
{
    return comboDevice->currentText();
}

const QString BarcodePrinterDlg::fileName() const
{
    return requester->url();
}

#include "barcodeprinterdlg.moc"
