/***************************************************************************
                          kactionmap.h  -  description
                             -------------------
    begin                : Fri Mai 19 2006
    copyright            : (C) 2006 by Dominik Seichter
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

#ifndef _KACTION_MAP_H_
#define _KACTION_MAP_H_

#include <qwidget.h>
#include <kdeversion.h>
#include <kdialogbase.h>

class KActionCollection;
class KActionMap;
#if KDE_VERSION >= 0x030500
class KListViewSearchLineWidget;
#endif 
class KListView;
class Q3ListViewItem;

/** Open a simple dialog containing an action map.
 */
class KActionMapDlg : public KDialogBase {
 public:
    KActionMapDlg( KActionCollection* actions, QWidget* parent = NULL, const char* name = NULL );

    /** Call this function whenever you changed the enabled/disalbed state of an action
     *  in your application.
     */
    void updateEnabledState();

 private:
    KActionMap* m_map;
};

/**
 *
 * This class is an implementation of an Action Map for KDE.
 *
 *
 * What is an Action Map?
 *
 * Almost every web page has a site map. According to Wikipedia, 
 * a site map "helps visitors, and search engine robots, to find
 * pages on the site". Similar an Action Map should help the
 * user to find actions (i.e. functions or menu items) in an application.
 *
 * More concrete an Action Map is a dialog with a list of all 
 * actions (i.e. make text bold, save file or insert image) 
 * that can be performed in the application at its current state. 
 * Additionally there is a search box at the top of the list so
 * that the user can search the list easily for a special action. 
 * The action is executed by clicking on it. Disabled actions are
 * grayed out and cannot be clicked. The dialog is modeless so that 
 * it can always be open and the normal usage of the application is not disturbed.
 */
class KActionMap : public QWidget {
    Q_OBJECT
 public:
    /** Creates a new KActionMap widget
     *  @param action all actions from this KActionCollection are displayed.
     *  @param parent the parent
     *  @param name   the name
     */
    KActionMap( KActionCollection* actions, QWidget* parent = NULL, const char* name = NULL );
    ~KActionMap();

 public slots:
    /** Call this function whenever you changed the enabled/disalbed state of an action
     *  in your application.
     */
    void updateEnabledState();

 private slots:
    void slotActionCollectionChanged();
    void slotExecuteAction( Q3ListViewItem* item );

 private:
    KActionCollection*         m_actions;
    KListView*                 m_listView;
#if KDE_VERSION >= 0x030500
    KListViewSearchLineWidget* m_searchLine;
#endif

    bool                       m_showMenuTree;
    bool                       m_grayOutItems;
};

#endif // _KACTION_MAP_H_
