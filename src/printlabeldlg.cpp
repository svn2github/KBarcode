/***************************************************************************
                          printlabeldlg.cpp  -  description
                             -------------------
    begin                : Mon Jun 30 2003
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

#include "printlabeldlg.h"
#include "printersettings.h"

// Qt includes
#include <qcheckbox.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3VBoxLayout>

// KDE includes
#include <knuminput.h>
#include <klocale.h>

PrintLabelDlg::PrintLabelDlg(QWidget *parent, const char *name )
    : KDialogBase( KDialogBase::Plain, i18n("Print"),
      KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Ok, parent,name)
{
    Q3VBoxLayout* box = new Q3VBoxLayout( plainPage() );

    numLabels  = new KIntNumInput( plainPage() );
    numLabels->setLabel( i18n("Number of labels:" ), KNumInput::AlignLeft | KNumInput::AlignVCenter );
    numLabels->setRange( 1, 100000, 1, false );
    
    numPos = new KIntNumInput( numLabels, 1, plainPage() );
    numPos->setLabel( i18n("Start at label position:" ), KNumInput::AlignLeft | KNumInput::AlignVCenter );
    numPos->setRange( 1, 100, 1, false );

    checkBorder = new QCheckBox( i18n("&Print crop marks (borders)"), plainPage() );
    checkBorder->setChecked( PrinterSettings::getInstance()->getData()->border );
    
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding );
    
    box->addWidget( numLabels );
    box->addWidget( numPos );
    box->addWidget( checkBorder );
    box->addItem( spacer );
}

PrintLabelDlg::~PrintLabelDlg()
{
}

void PrintLabelDlg::setLabelsEnabled( bool b )
{
    numLabels->setEnabled( b );
}

int PrintLabelDlg::labels() const
{
    return numLabels->value();
}

int PrintLabelDlg::position() const
{
    return numPos->value();
}

bool PrintLabelDlg::border() const
{
    return checkBorder->isChecked();
}

#include "printlabeldlg.moc"
