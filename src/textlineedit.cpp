//
// C++ Implementation: textlineedit
//
// Description: 
//
//
// Author: Dominik Seichter <domseichter@web.de>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "textlineedit.h"
#include "tokendialog.h"

// KDE includes
#include <knuminput.h>
#include <kaction.h>
#include <kdeversion.h>
#include <kcolordialog.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kspell.h>
#if QT_VERSION >= 0x030100
    #include <klineedit.h>
#else
    #include <qlineedit.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#endif
#include <ktoolbar.h>
#include <kcombobox.h>

// Qt includes
#include <q3dockarea.h>
#include <qregexp.h>
#include <qlabel.h>
#include <qlayout.h>

TextLineEditor::TextLineEditor( TokenProvider* token, QWidget *parent )
    : QWidget( parent ), m_token( token )
{
    Q3VBoxLayout* layout = new Q3VBoxLayout( this, 6, 6 );



#if QT_VERSION >= 0x030100
    editor = new KLineEdit( this );
#else
    editor = new QLineEdit( this );
#endif        

    editor->setFocus();

    Q3DockArea* area = new Q3DockArea( Qt::Horizontal, Q3DockArea::Normal, this );
    toolBar = new KToolBar( area );
    tool2Bar = new KToolBar( area );
    tool3Bar = new KToolBar( area );
    
    setupActions();

    layout->addWidget( area );
    layout->addWidget( editor );

    

    
    

}

TextLineEditor::~TextLineEditor()
{
}


void TextLineEditor::setupActions()
{
    ac = new KActionCollection( this );

   
    KAction *action_undo = KStandardAction::undo( editor, SLOT( undo() ), ac );
    action_undo->setEnabled( false );
    connect( editor, SIGNAL( undoAvailable(bool) ), action_undo, SLOT( setEnabled(bool) ) );

    
    KAction *action_redo = KStandardAction::redo( editor, SLOT( redo() ), ac );
    action_redo->setEnabled( false );
    connect( editor, SIGNAL( redoAvailable(bool) ), action_redo, SLOT( setEnabled(bool) ) );

    KAction *action_cut = KStandardAction::cut( editor, SLOT( cut() ), ac );
    action_cut->setEnabled( false );
    connect( editor, SIGNAL( copyAvailable(bool) ), action_cut, SLOT( setEnabled(bool) ) );

    KAction *action_copy = KStandardAction::copy( editor, SLOT( copy() ), ac );
    action_copy->setEnabled( false );
    connect( editor, SIGNAL( copyAvailable(bool) ), action_copy, SLOT( setEnabled(bool) ) );

    KAction* action_paste = KStandardAction::paste( editor, SLOT( paste() ), ac );

    KAction* textDataAct = new KAction( i18n("Insert &Data Field"), "contents", 0, this, SLOT( insertNewField() ), ac, "text_data_act");
   
    action_undo->plug( toolBar );
    action_redo->plug( toolBar );
    toolBar->insertSeparator();
    action_cut->plug( toolBar );
    action_copy->plug( toolBar );
    action_paste->plug( toolBar );
 

    QStringList fuentes;
    fuentes += "Times Roman (Medium) 8 point";
    fuentes += "Times Roman (Medium) 10 point";
    fuentes += "Times Roman (Bold) 10 point";
    fuentes += "Times Roman (Bold) 12 point";
    fuentes += "Times Roman (Bold) 14 point";
    fuentes += "Times Roman (Italic) 12 point";
    fuentes += "Helvetica (Medium) 6 point";
    fuentes += "Helvetica (Medium) 10 point";
    fuentes += "Helvetica (Medium) 12 point";
    fuentes += "Helvetica (Bold) 12 point";
    fuentes += "Helvetica (Bold) 14 point";
    fuentes += "Helvetica (Italic) 12 point";
    fuentes += "Presentation (Bold) 18 point";
    fuentes += "Letter Gothic (Medium) 9.5 point";
    fuentes += "Prestige Elite (Medium) 7 point";
    fuentes += "Prestige Elite (Bold) 10 point";
    fuentes += "Courier (Medium) 10 point";
    fuentes += "Courier (Bold) 12 point";
    fuentes += "OCR-A 12 point";
    fuentes += "OCR-B 12 point";

    textDataAct->plug( tool2Bar );

    action_font_type = new KComboBox(tool2Bar,"font_type") ;
    connect( action_font_type, SIGNAL( activated(int) ), this, SLOT( setFontType(int) ) );
    action_font_type->insertStringList(fuentes) ;
    
    
    
    QLabel* labelv = new QLabel( i18n("&Mag. Vert.:"), tool3Bar );
    mag_vert = new KIntNumInput( tool3Bar,"magvert" );
    QLabel* labelh = new QLabel( i18n("&Mag. Hor.:"), tool3Bar );
    mag_hor = new KIntNumInput( tool3Bar,"maghor" );
    connect( mag_vert, SIGNAL( activated(int) ), this, SLOT( setVerMag(int) ) );
    connect( mag_hor, SIGNAL( activated(int) ), this, SLOT( setHorMag(int) ) );  
    mag_vert->setRange( 1, 9, 1, false );
    mag_hor->setRange( 1, 9, 1, false );
        
       
    labelv->setBuddy( mag_vert );
    labelh->setBuddy( mag_hor );

    
    
    updateActions();


}

QString TextLineEditor::text()
{
    return editor->text();
}

void TextLineEditor::setText( const QString & t )
{
    editor->setText( t );
}


void TextLineEditor::updateActions()
{
  
}


void TextLineEditor::insertNewField()
{
    TokenDialog dlg( m_token, this, "dlg" );
    if( dlg.exec() == QDialog::Accepted )
        editor->insert( dlg.token() ) ;
}

void TextLineEditor::setFontType( int index )
{    
    action_font_type->setCurrentItem(index);
}

int TextLineEditor::getFontType()
{    
    return action_font_type->currentItem();
}
void TextLineEditor::setVertMag( int index )
{    
    mag_vert->setValue(index);
}

int TextLineEditor::getVertMag()
{    
    return mag_vert->value();
}

void TextLineEditor::setHorMag( int index )
{    
    mag_hor->setValue(index);
}

int TextLineEditor::getHorMag()
{    
    return mag_hor->value();
}
#include "textlineedit.moc"
