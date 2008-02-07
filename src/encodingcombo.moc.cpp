/****************************************************************************
** EncodingCombo meta object code from reading C++ file 'encodingcombo.h'
**
** Created: Sat Sep 29 10:38:06 2007
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "encodingcombo.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *EncodingCombo::className() const
{
    return "EncodingCombo";
}

QMetaObject *EncodingCombo::metaObj = 0;
static QMetaObjectCleanUp cleanUp_EncodingCombo( "EncodingCombo", &EncodingCombo::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString EncodingCombo::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EncodingCombo", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString EncodingCombo::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EncodingCombo", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* EncodingCombo::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = KComboBox::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"EncodingCombo", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_EncodingCombo.setMetaObject( metaObj );
    return metaObj;
}

void* EncodingCombo::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "EncodingCombo" ) )
	return this;
    return KComboBox::qt_cast( clname );
}

bool EncodingCombo::qt_invoke( int _id, QUObject* _o )
{
    return KComboBox::qt_invoke(_id,_o);
}

bool EncodingCombo::qt_emit( int _id, QUObject* _o )
{
    return KComboBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool EncodingCombo::qt_property( int id, int f, QVariant* v)
{
    return KComboBox::qt_property( id, f, v);
}

bool EncodingCombo::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
