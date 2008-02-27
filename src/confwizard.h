/***************************************************************************
                          confwizard.h  -  description
                             -------------------
    begin                : Son Jun 16 2002
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

#ifndef CONFWIZARD_H
#define CONFWIZARD_H

#include <kwizard.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

class Q3ButtonGroup;
class QCheckBox;
class Q3VBoxLayout;
class Q3HBoxLayout; 
class Q3GridLayout;
class QRadioButton;
class KComboBox;
class KLineEdit;
class KPushButton;
class KUrlLabel;
class QLabel;
class QWidget;
class SqlWidget;
/** KBarcodes configuration wizard.
  */
class ConfWizard : public KWizard
{ 
    Q_OBJECT

    public:
        ConfWizard( QWidget* parent = 0, bool modal = true );
        ~ConfWizard();

        void showPage( QWidget* page );

        // used in mainwindow.cpp
        // not a clean API, but I am lazy :-(
        
        // TODO: rewrite it cleanly
        QCheckBox* checkDatabase;

    private slots:
        void testSettings( bool b );
        void create();
        void example();
        void useDatabase();

    protected slots:
        void accept();
        
    private:
        void setupPage1();
        void setupPage0();
        void setupPage2();
        void setupPage3();

        SqlWidget* sqlwidget;
        
        QWidget* page;
        QLabel* logo;
        QLabel* TextLabel2_2;
        KUrlLabel* KUrlLabel1;
        QWidget* page_2;
        QLabel* TextLabel1;
        QLabel* TextLabel2;
        QLabel* TextLabel3;
        QLabel* TextLabel4;
        QLabel* TextLabel5;
        QLabel* TextLabel6;
        KPushButton* buttonTest;
        QWidget* page_3;
        QLabel* TextLabel1_2;
        KPushButton* buttonCreate;
        KPushButton* buttonExample;
        Q3ButtonGroup* groupDatabase;

    protected:
        Q3VBoxLayout* pageLayout;
        Q3HBoxLayout* Layout8;
        Q3VBoxLayout* Layout7;
        Q3VBoxLayout* pageLayout_2;
        Q3VBoxLayout* pageLayout_4;
        Q3HBoxLayout* Layout5;
        Q3VBoxLayout* Layout3;
        Q3VBoxLayout* Layout4;
        Q3VBoxLayout* Layout6;
        Q3VBoxLayout* pageLayout_3;
        Q3VBoxLayout* Layout5_2;
};

#endif // CONFWIZARD_H
