/****************************************************************************
**
** DCOP Skeleton generated by dcopidl2cpp from batchiface.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#include "/home/dominik/Desktop/Programming/kbarcode/kbarcode/batchiface.h"

#include <kdatastream.h>
#include <q3asciidict.h>
//Added by qt3to4:
#include <Q3CString>


static const int BatchIface_fhash = 13;
static const char* const BatchIface_ftable[12][3] = {
    { "void", "setFilename(QString)", "setFilename(QString url)" },
    { "void", "setImportCsvFile(QString)", "setImportCsvFile(QString filename)" },
    { "void", "setImportSqlQuery(QString)", "setImportSqlQuery(QString query)" },
    { "void", "setNumLabels(int)", "setNumLabels(int n)" },
    { "void", "setOutputFormat(int)", "setOutputFormat(int e)" },
    { "void", "setSerialNumber(QString,int)", "setSerialNumber(QString val,int inc)" },
    { "bool", "addItem(QString,QString,int,bool)", "addItem(QString article,QString group,int count,bool msg)" },
    { "bool", "existsArticle(QString)", "existsArticle(QString article)" },
    { "void", "loadFromFile(QString)", "loadFromFile(QString url)" },
    { "void", "loadFromClipboard()", "loadFromClipboard()" },
    { "void", "printNow(QString,bool)", "printNow(QString printer,bool bUserInteraction)" },
    { 0, 0, 0 }
};
static const int BatchIface_ftable_hiddens[11] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

bool BatchIface::process(const Q3CString &fun, const QByteArray &data, Q3CString& replyType, QByteArray &replyData)
{
    static Q3AsciiDict<int>* fdict = 0;
    if ( !fdict ) {
	fdict = new Q3AsciiDict<int>( BatchIface_fhash, true, false );
	for ( int i = 0; BatchIface_ftable[i][1]; i++ )
	    fdict->insert( BatchIface_ftable[i][1],  new int( i ) );
    }
    int* fp = fdict->find( fun );
    switch ( fp?*fp:-1) {
    case 0: { // void setFilename(QString)
	QString arg0;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = BatchIface_ftable[0][0]; 
	setFilename(arg0 );
    } break;
    case 1: { // void setImportCsvFile(QString)
	QString arg0;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = BatchIface_ftable[1][0]; 
	setImportCsvFile(arg0 );
    } break;
    case 2: { // void setImportSqlQuery(QString)
	QString arg0;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = BatchIface_ftable[2][0]; 
	setImportSqlQuery(arg0 );
    } break;
    case 3: { // void setNumLabels(int)
	int arg0;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = BatchIface_ftable[3][0]; 
	setNumLabels(arg0 );
    } break;
    case 4: { // void setOutputFormat(int)
	int arg0;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = BatchIface_ftable[4][0]; 
	setOutputFormat(arg0 );
    } break;
    case 5: { // void setSerialNumber(QString,int)
	QString arg0;
	int arg1;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	if (arg.atEnd()) return false;
	arg >> arg1;
	replyType = BatchIface_ftable[5][0]; 
	setSerialNumber(arg0, arg1 );
    } break;
    case 6: { // bool addItem(QString,QString,int,bool)
	QString arg0;
	QString arg1;
	int arg2;
	bool arg3;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	if (arg.atEnd()) return false;
	arg >> arg1;
	if (arg.atEnd()) return false;
	arg >> arg2;
	if (arg.atEnd()) return false;
	arg >> arg3;
	replyType = BatchIface_ftable[6][0]; 
	QDataStream _replyStream( replyData, QIODevice::WriteOnly );
	_replyStream << addItem(arg0, arg1, arg2, arg3 );
    } break;
    case 7: { // bool existsArticle(QString)
	QString arg0;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = BatchIface_ftable[7][0]; 
	QDataStream _replyStream( replyData, QIODevice::WriteOnly );
	_replyStream << existsArticle(arg0 );
    } break;
    case 8: { // void loadFromFile(QString)
	QString arg0;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	replyType = BatchIface_ftable[8][0]; 
	loadFromFile(arg0 );
    } break;
    case 9: { // void loadFromClipboard()
	replyType = BatchIface_ftable[9][0]; 
	loadFromClipboard( );
    } break;
    case 10: { // void printNow(QString,bool)
	QString arg0;
	bool arg1;
	QDataStream arg( data, QIODevice::ReadOnly );
	if (arg.atEnd()) return false;
	arg >> arg0;
	if (arg.atEnd()) return false;
	arg >> arg1;
	replyType = BatchIface_ftable[10][0]; 
	printNow(arg0, arg1 );
    } break;
    default: 
	return DCOPObject::process( fun, data, replyType, replyData );
    }
    return true;
}

QCStringList BatchIface::interfaces()
{
    QCStringList ifaces = DCOPObject::interfaces();
    ifaces += "BatchIface";
    return ifaces;
}

QCStringList BatchIface::functions()
{
    QCStringList funcs = DCOPObject::functions();
    for ( int i = 0; BatchIface_ftable[i][2]; i++ ) {
	if (BatchIface_ftable_hiddens[i])
	    continue;
	Q3CString func = BatchIface_ftable[i][0];
	func += ' ';
	func += BatchIface_ftable[i][2];
	funcs << func;
    }
    return funcs;
}


