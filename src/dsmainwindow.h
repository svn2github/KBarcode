/***************************************************************************
                          dsmainwindow.h  -  description
                             -------------------
    begin                : Fre Jan 17 2003
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

#ifndef DSMAINWINDOW_H
#define DSMAINWINDOW_H

#include <qwidget.h>
#include <kmainwindow.h>

class KAction;
class KPopupMenu;
/** The base class of all main windows of KBarcode. It provides standard menu items
  * for database connections, preferences, help, the first start wizard and donations.
  * It checks also if this is the first start of KBarcode and if the configuration wizard
  * should be started.
  *
  * @author Dominik Seichter
  */
class DSMainWindow : public KMainWindow {
    Q_OBJECT
    public:
        DSMainWindow(QWidget *parent=0, const char *name=0, WFlags f = WType_TopLevel | WDestructiveClose );
        ~DSMainWindow();

        /** Is this the first start of KBarcode ?
          * If true, the configuration wizard will
          * be started automatically.
          */
        bool isFirstStart() const {
            return first;
        }


        /** Do a system check and return a text
          * informing the user about missing, programs
          * and drivers for barcode support and database
          * access.
          */
        static QString systemCheck();

    public slots:
        void wizard();

    signals:
        void connectedSQL();

    private slots:
        void connectMySQL();
        void appHelpActivated();
        void startInfo();
        bool newTables();
        void donations();
        void showCheck();
        void slotFunctionMap();

    protected slots:
        void loadConfig();
        void saveConfig();

    protected:
        void setupActions();

        static bool autoconnect;
        static bool startwizard;

        bool first;
        
        KAction* connectAct;
        KAction* importLabelDefAct;
        KAction* importExampleAct;
        
        KPopupMenu* file;
        KPopupMenu* settings;
        KPopupMenu* hlpMenu;
};
#endif
