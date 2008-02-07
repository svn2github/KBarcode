/***************************************************************************
                          labeleditor.h  -  description
                             -------------------
    begin                : Die Apr 23 2002
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

#ifndef LABELEDITOR_H
#define LABELEDITOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "sqltables.h"
#include "labelutils.h"
#include "definition.h"
#include "dsmainwindow.h"
#include "xmlutils.h"
#include "editoriface.h"

#include <kdialogbase.h>
#include <qdialog.h>
#include <qmap.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <QCloseEvent>
#include <QLabel>
#include <Q3VBoxLayout>

class BatchPrinter;
class KUrl;
class KAction;
class KActionMenu;
class KActionSeparator;
class KToggleAction;
class KRecentFilesAction;
class KToolBarPopupAction;
class K3ListBox;
class KMenuBar;
class KMenu;
class KRuler;
class KToolBar;
class K3CommandHistory;
class MyCanvasText;
class MyCanvasRectangle;
class MyCanvasView;
class BarcodeItem;
class MyCanvas;
class Q3CanvasItemList;
class QCheckBox;
class KComboBox;
class Q3DockArea;
class Q3DockWindow;
class Q3GroupBox;
class QLabel;
class QImage;
class Q3HBoxLayout;
class Q3VBoxLayout;
class QPoint;
class QRect;
class QPaintDevice;
class QString;
class KPushButton;
class KPrinter;
class Q3CanvasItem;
class TCanvasItem;
class RectItem;
class LineItem;

class TokenProvider;
/** The LabelEditor is a graphical, easy to use label designer.
  * It provides all the function necessary to create colorful labels
  * and save them. Labels can also be printed directly, but without database
  * support. Use Batchprinting (LabelPrinter) for this.
  */
class LabelEditor : public DSMainWindow, public EditorIface, private LabelUtils, private XMLUtils {
    Q_OBJECT
    public:
        enum ELabelEditorMode {
            eCreateNewLabel,
            eLoadLabel
        };

        LabelEditor( QWidget *parent=0, QString _filename = QString::null,
            const char *name="labeleditor", Qt::WFlags f = Qt::WType_TopLevel | Qt::WDestructiveClose );
        ~LabelEditor();
        bool startupDlg( ELabelEditorMode mode, QString f = QString::null );

        /**
          * returns the filename of the currently loaded label.
          */
        const QString fileName() const;
        
    public slots:
        void print();
        void printBCP();
        void printImage();
        
    private:

	/** Creates a K3CommandHistory object which will be 
	 *  pointed to by history.
	 *  Make sure to delete history by yourself when needed.
	 */
	void createCommandHistory();
	/** Create undo and redo actions and plug them into
	 *  menus and toolbars. They have to be regenerated
	 *  when ever createCommandHistory was called.
	 */
	void createCommandHistoryActions();

        QString getCustomerName( int mode = 0, QString text = "" );
        void clearLabel();
        //void setupBarcode( BarcodeItem* bcode );
        void insertText( QString caption );
        //NY35
        void insertTextLine( QString caption );
        //NY35
        void setupActions();
        void enableActions();
        bool isChanged();   // if the current document has been changed
        void batchPrint( BatchPrinter* batch, int copies, int mode );
        void setupContextMenu();
        
    private slots:
        void startEditor();
        bool open();
        bool openUrl( const QString & url );
        bool save();
        bool saveas();
        bool save( QString url );
        void save( QIODevice* device );
        bool newLabel();
        void insertBarcode();
        void insertPicture();
        void insertText();
        void insertDataText();
        //NY36
        void insertTextLine();
        //NY36
        void insertRect();
        void insertCircle();
        void insertLine();

        void changeDes();
        void changeSize();
        void updateInfo();
        void doubleClickedItem( TCanvasItem* item );
        void showContextMenu( QPoint pos );
        void doubleClickedCurrent();
        void spellCheck();
        void centerVertical();
        void centerHorizontal();
        void raiseCurrent();
        void lowerCurrent();
        void onTopCurrent();
        void backCurrent();
        void preview();
        void toggleGrid();
        void cut();
        void copy();
        void paste();
        void startBarcodeGen();
        void saveConfig();
        void loadConfig();
        void startLoadRecentEditor( const KUrl& url );
        void startLoadEditor();
        void batchPrint();
        void closeLabel();
        void setEdited();
        void launchAddressBook();
        void lockItem();
        
    protected:
        void closeEvent( QCloseEvent* e );

        K3CommandHistory* history;

        KAction* saveAct;
        KAction* saveAsAct;
        KAction* exportAct;
        KAction* descriptionAct;
        KAction* printAct;
        KAction* bcpAct;
        KAction* imgAct;
        KAction* changeSizeAct;
        KAction* deleteAct;
        KAction* editPropAct;
        KAction* barcodeAct;
        KAction* rectAct;
        KAction* circleAct;
        KAction* lineAct;
        KAction* pictureAct;
        KAction* previewAct;
        KAction* textAct;
        KAction* textDataAct;
        KAction* textLineAct;
        KAction* spellAct;
        KAction* cutAct;
        KAction* copyAct;
        KAction* pasteAct;
        KAction* undoAct;
        KAction* redoAct;
        KAction* closeLabelAct;
        KAction* addressBookAct;
        KAction* selectAllAct;
        KAction* deSelectAllAct;

	/**
	 * Undo/Redo actins get pluged into this menu
	 */
	KMenu* editMenu;        
        KMenu* exportMenu;        
        KMenu* m_mnuContext;
        
        KToolBar* tools;
        
        KToggleAction* gridAct;
        
        KComboBox* comboScale;

        KRecentFilesAction* recentAct;
        KActionSeparator* sep;
        KToolBarPopupAction* listAct;
        MyCanvas* c;
        MyCanvasView* cv;

        Definition* d;
	TokenProvider* m_token;

        // Label info's:
        QString description;    // description
        QString filename;       // current file

        K3ListBox* listFields;
        KPushButton* buttonText;

        Q3VBoxLayout* pageLayout;

        QMap<QString,QString> fields;

        bool m_edited;
};

#endif
