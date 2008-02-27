/***************************************************************************
                          sqltables.cpp  -  description
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

#include "sqltables.h"
#include "definition.h"

// Qt includes
#include <qcheckbox.h>
#include <qfile.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmap.h>
#include <q3progressdialog.h>
#include <qsqldatabase.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <QSqlQuery>
#include <Q3VBoxLayout>

// KDE includes
#include <kapplication.h>
#include <kcombobox.h>
#include <kconfig.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <kstandarddirs.h>
#include <kglobal.h>

QMap<QString,SqlDescription*> drivers;

class MySqlDescription : public SqlDescription {
    public:
        virtual const QString initdb( QString ) const { return "mysql"; }
        virtual const QString autoIncrement() const { return "int(11) NOT NULL auto_increment"; }
        virtual const QString showColumns( const QString & table ) const { return "SHOW COLUMNS FROM " + table; }
};

class PostgreSQLDescription : public SqlDescription {
    public:
        virtual const QString initdb( QString ) const { return "template1"; }
        virtual const QString autoIncrement() const { return "serial NOT NULL"; }
        virtual const QString showColumns( const QString & table ) const {
            return "select a.attname, pg_catalog.format_type(a.atttypid, a.atttypmod), "
                   "a.attnotnull, a.atthasdef, a.attnum, substring(d.adsrc for 128) "
                   "from pg_catalog.pg_attribute a "
                   "inner join pg_catalog.pg_class c on c.oid = a.attrelid "
                   "left join pg_catalog.pg_attrdef d on d.adrelid = c.oid and a.attnum = "
                   "d.adnum "
                   "where a.attnum > 0 and not a.attisdropped and c.relname ~ '^" + table + "$';";
            }
};

/** should work for ODBC, too */
class SQLiteDescription : public SqlDescription {
    public:
        virtual const QString initdb( QString db = QString::null ) const { return db; }
        virtual const QString autoIncrement() const { return "serial NOT NULL"; }
        virtual const QString showColumns( const QString & table ) const { return "SHOW COLUMNS FROM " + table; }
};

SqlTables::SqlTables( QObject* parent )
    : QObject( parent, "sqltables" )
{
    drivers.insert( "QMYSQL3", new MySqlDescription() );
    drivers.insert( "QPSQL7", new PostgreSQLDescription() );
    /* The same driver plugin is used for QDBC and SQLite */
    drivers.insert( "QSQLITE", new SQLiteDescription() );
    drivers.insert( "QODBC3", new SQLiteDescription() );

    db = NULL;
    connected = false;
    loadConfig();

    connect( kapp, SIGNAL( aboutToQuit() ), this, SLOT( saveConfig() ) );
}

SqlTables::~SqlTables()
{
}

SqlTables* SqlTables::instance = NULL;

SqlTables* SqlTables::getInstance()
{
    if( !instance )
        instance = new SqlTables();

    return instance;
}

const SqlDescription* SqlTables::driver() const
{
    return drivers[sqldata.driver];
}
bool SqlTables::connectMySQL()
{
    db = QSqlDatabase::addDatabase( sqldata.driver );

    db->setDatabaseName( sqldata.database );
    db->setUserName( sqldata.username );
    db->setPassword( sqldata.password );
    db->setHostName( sqldata.hostname );

    if( !db->open() )
        KMessageBox::error( 0, i18n("<qt>Unable to open database: ") + sqldata.database + "<br>" +
              db->lastError().databaseText() + "</qt>");

    connected = db->open();
    if( connected ) {
        updateTables();
        emit connectedSQL();
        // be sure that we have a clean list of producers
        Definition::updateProducer();
    }

    return connected;
}

bool SqlTables::newTables()
{
    return newTables( sqldata.username, sqldata.password, sqldata.hostname, sqldata.database, sqldata.driver );
}

bool SqlTables::newTables( const QString & username, const QString & password, const QString & hostname, const QString & database, const QString & driver )
{
    if( KMessageBox::warningContinueCancel( 0,
                     i18n("We are going to re-create the tables '") +
                          QString( TABLE_BASIC "', '" TABLE_CUSTOMER "', '"
                          TABLE_CUSTOMER_TEXT) +i18n("' and '") + QString(TABLE_LABEL_DEF "'")) ==  KMessageBox::Cancel )
        return false;

    if( !drivers[driver] )
        return false;

    QSqlDatabase*dbase = QSqlDatabase::addDatabase(driver, drivers[driver]->initdb( database ) );
    dbase->setDatabaseName( drivers[driver]->initdb( database ) );
    dbase->setUserName( username );
    dbase->setPassword( password );
    dbase->setHostName( hostname );

    if(dbase->open()) {

        if (driver != "QSQLITE")
        {
            bool found = false;
            QSqlQuery existing("SHOW DATABASES LIKE '" + database + "';");
            while( existing.next() )
                found = true;

            QSqlQuery firstquery( NULL, dbase );
            if( !found && !firstquery.exec("CREATE DATABASE " + database + ";")) {
                if( KMessageBox::warningContinueCancel( 0, i18n("<qt>Can't create database ")+ database + i18n("<br>You can continue if the database exists already.</qt>")
                    + firstquery.lastError().databaseText() ) == KMessageBox::Cancel ) {
                    dbase->close();
                    QSqlDatabase::removeDatabase(drivers[driver]->initdb( database ));
                    return false;
                }
            }
        }
        dbase->close();
        QSqlDatabase::removeDatabase(drivers[driver]->initdb( database ));

        // The database is created, now connect to the one specified by the user
        dbase = QSqlDatabase::addDatabase(driver, database );
        dbase->setDatabaseName( database );
        dbase->setUserName( username );
        dbase->setPassword( password );
        dbase->setHostName( hostname );
        if(!dbase->open() || !dbase->isOpen()) {
            KMessageBox::error( 0, i18n("KBarcode could not create the required database. Please create it manually.") + dbase->lastError().databaseText() );
            QSqlDatabase::removeDatabase( database );
            return false;
        }


        QSqlQuery query( NULL, dbase );

        // barcode_basic
        query.exec("DROP TABLE " TABLE_BASIC );
        exec( &query, "CREATE TABLE " TABLE_BASIC " ("
                   "    uid " + drivers[driver]->autoIncrement() + ","
                   "    article_no varchar(50) DEFAULT NULL,"
                   "    article_desc varchar(50) DEFAULT NULL,"
                   "    barcode_no TEXT DEFAULT NULL,"
                   "    encoding_type varchar(50) DEFAULT NULL,"
                   "    field0 varchar(50) DEFAULT NULL,"
                   "    field1 varchar(50) DEFAULT NULL,"
                   "    field2 varchar(50) DEFAULT NULL,"
                   "    field3 varchar(50) DEFAULT NULL,"
                   "    field4 varchar(50) DEFAULT NULL,"
                   "    field5 varchar(50) DEFAULT NULL,"
                   "    field6 varchar(50) DEFAULT NULL,"
                   "    field7 varchar(50) DEFAULT NULL,"
                   "    field8 varchar(50) DEFAULT NULL,"
                   "    field9 varchar(50) DEFAULT NULL,"
                   "    PRIMARY KEY  (uid)"
                   ");" );

        // customer
        query.exec("DROP TABLE " TABLE_CUSTOMER );
        exec( &query, "CREATE TABLE " TABLE_CUSTOMER " ("
                   "    uid " + drivers[driver]->autoIncrement() + " ,"
                   "    customer_no varchar(20) DEFAULT NULL,"
                   "    customer_name varchar(20) DEFAULT NULL,"
                   "    PRIMARY KEY  (uid)"
                   ");" );

        // customer_text
        query.exec("DROP TABLE " TABLE_CUSTOMER_TEXT );
        exec( &query, "CREATE TABLE " TABLE_CUSTOMER_TEXT " ("
                   "    uid " + drivers[driver]->autoIncrement() + ","
                   "    customer_no varchar(20) DEFAULT NULL,"
                   "    encoding_type varchar(50) DEFAULT NULL,"
                   "    article_no varchar(50) DEFAULT NULL,"
                   "    article_no_customer varchar(50) NULL,"
                   "    barcode_no TEXT DEFAULT NULL,"
                   "    line0 varchar(50) DEFAULT NULL,"
                   "    line1 varchar(50) DEFAULT NULL,"
                   "    line2 varchar(50) DEFAULT NULL,"
                   "    line3 varchar(50) DEFAULT NULL,"
                   "    line4 varchar(50) DEFAULT NULL,"
                   "    line5 varchar(50) DEFAULT NULL,"
                   "    line6 varchar(50) DEFAULT NULL,"
                   "    line7 varchar(50) DEFAULT NULL,"
                   "    line8 varchar(50) DEFAULT NULL,"
                   "    line9 varchar(50) DEFAULT NULL,"
                   "    PRIMARY KEY  (uid)"
                   ");" );

        // label_def
        query.exec("DROP TABLE " TABLE_LABEL_DEF );
        exec( &query, "CREATE TABLE " TABLE_LABEL_DEF " ("
                   "    label_no " + drivers[driver]->autoIncrement() + ","
                   "    manufacture varchar(255) DEFAULT NULL,"
                   "    type varchar(255) DEFAULT NULL,"
                   "    paper char(1) DEFAULT NULL,"
                   "    gap_top NUMERIC(10,4) NULL,"
                   "    gap_left NUMERIC(10,4) NULL,"
                   "    height NUMERIC(10,4) NULL,"
                   "    width NUMERIC(10,4) NULL,"
                   "    gap_v NUMERIC(10,4) NULL,"
                   "    gap_h NUMERIC(10,4) NULL,"
                   "    number_h int DEFAULT NULL," //smalint(6)
                   "    number_v int DEFAULT NULL," //smalint(6)
                   "    paper_type varchar(30) DEFAULT NULL,"
                   "    compatibility varchar(10) DEFAULT NULL," // keep compatibility with older versions, was "remark text"
                   "    PRIMARY KEY  (label_no)"
                   ");" );

        dbase->close();
        QSqlDatabase::removeDatabase( database );
        KMessageBox::information( 0, i18n("Created table ")+database+i18n(" successfully!") );
    } else {
        dbase->close();
        QSqlDatabase::removeDatabase(drivers[driver]->initdb( database ));
        KMessageBox::sorry( 0, i18n("Can't connect to database.") );
        return false;
    }

    return true;
}

void SqlTables::importLabelDef()
{
    if( KMessageBox::warningContinueCancel( 0, i18n("We are going to delete the complete table: " ) + TABLE_LABEL_DEF ) ==
        KMessageBox::Cancel )
        return;

    QSqlQuery query( QString::null, db );
    exec( &query, "delete from " TABLE_LABEL_DEF );

    QString f = KStandardDirs::locateLocal( "data", "kbarcode/labeldefinitions.sql" );
    if( !QFile::exists( f ) ) {
        KConfig* config = KGlobal::config();
        config->setGroup( "Definitions" );
        f = config->readEntry( "defpath", locate( "data", "kbarcode/labeldefinitions.sql" ) );
    }

    importData( f, db );

    Definition::updateProducer();
}

void SqlTables::importExampleData()
{
    if( KMessageBox::warningContinueCancel( 0,
        i18n("We are going to delete the complete tables: " ) +  QString( TABLE_BASIC ", " TABLE_CUSTOMER ", " TABLE_CUSTOMER_TEXT ) ) ==
        KMessageBox::Cancel )
        return;

    importData( locate("appdata", "exampledata.sql"), db );
}

void SqlTables::importData( const QString & filename, QSqlDatabase* db )
{
    if( !db ) {
        qDebug("Can't import data, dabase not open!");
        return;
    }

    if( filename.isEmpty() || !db->isOpen() ) // quick escape
    {
        KMessageBox::error( NULL, i18n("Data file for import not found. Continuing without importing data. Please check your KBarcode installation.") );
        return;
    }

    QFile data( filename);
    Q3ProgressDialog* dlg = new Q3ProgressDialog( i18n("SQL import progress:"),  QString::null, data.size(), 0, "dlg", true );

    if( data.open( QIODevice::ReadOnly ) ) {
        QString s;
        QSqlQuery query( QString::null, db );
        while( data.readLine( s, 1024 ) != -1 )
            if( !s.isEmpty() ) {
                dlg->setProgress( dlg->progress() + s.length() );
                exec( &query, s );
            }
    } else
        KMessageBox::sorry( 0, i18n("Can't open the data file containing the label definitions.") );

    dlg->close( true );
    data.close();
}

void SqlTables::exec( QSqlQuery* query, const QString & text )
{
    if( !query->exec( text ) )
        KMessageBox::sorry( 0, i18n("<qt>Can't execute command:<br><b>") + text + "</b><br></qt>" + query->lastError().databaseText(), "sqlexecerror" );
}

void SqlTables::loadConfig()
{
    KConfig* config = KGlobal::config();

    config->setGroup("SQL");
    sqldata.username = config->readEntry("username", "root");
    sqldata.password = config->readEntry("password", "" );
    sqldata.hostname = config->readEntry("hostname", "localhost" );
    sqldata.database = config->readEntry("database", "kbarcode" );
    sqldata.driver = config->readEntry("driver", "QMYSQL3" );
    sqldata.autoconnect = config->readBoolEntry("autoconnect", false );
}

void SqlTables::saveConfig()
{
    KConfig* config = KGlobal::config();

    config->setGroup("SQL");
    config->writeEntry("username", sqldata.username );
    config->writeEntry("password", sqldata.password );
    config->writeEntry("hostname", sqldata.hostname );
    config->writeEntry("database", sqldata.database );
    config->writeEntry("driver", sqldata.driver );
    config->writeEntry("autoconnect", sqldata.autoconnect );

    config->sync();
}

void SqlTables::updateTables()
{
    /* Older versions of KBarcode did only
     * support MySQL. As a reason, databases
     * do not need to be updated for other DBs
     * as they have already been created with the most
     * recent database structures.
     */
    if( sqldata.driver != "QMYSQL3" )
        return;

    bool changed = false;
    // check for field0 - field9 (1.4.0)
    QStringList fields;
    for( unsigned int i = 0; i < 10; i++ )
        fields.append( QString("field%1").arg( i )  );

    QSqlQuery query("SHOW FIELDS FROM " TABLE_BASIC );
    while ( query.next() )
        if( fields.grep( query.value( 0 ).toString(), false ).count() ) {
            fields.remove( query.value( 0 ).toString() );
        }

    if( fields.count() ) {
        QSqlQuery q;
        for( unsigned int i = 0; i < 10; i++ )
            q.exec("ALTER TABLE " TABLE_BASIC " ADD " + fields[i] + " varchar(50)");

        qDebug("changing fields");
        changed = true;
    }

    // change barcode_no from varchar to TEXT (1.5.0)
    QSqlQuery query2("SHOW FIELDS FROM " TABLE_BASIC );
    while( query2.next() )
        if( query2.value( 0 ).toString() == "barcode_no" && query2.value(1) == "varchar(50)" ) {
            query2.exec( "ALTER TABLE " TABLE_BASIC " CHANGE barcode_no barcode_no TEXT" );
            changed = true;
        }

    // change barcode_no from varchar to TEXT (1.5.0)
    QSqlQuery query3("SHOW FIELDS FROM " TABLE_CUSTOMER_TEXT );
    while( query3.next() )
        if( query3.value( 0 ).toString() == "barcode_no" && query3.value(1) == "varchar(50)" ) {
            query3.exec( "ALTER TABLE " TABLE_CUSTOMER_TEXT " CHANGE barcode_no barcode_no TEXT" );
            changed = true;
        }

    // change NUMERIC DEFAULT TO NUMERIC(10,4) (1.6.1)
    QStringList update;
    update << "gap_top" << "gap_left" << "height" << "width" << "gap_v" << "gap_h";
    QSqlQuery query4("SHOW FIELDS FROM " TABLE_LABEL_DEF );
    while( query4.next() )
        if( update.contains( query4.value( 0 ).toString() ) && query4.value(1).toString() == "decimal(10,0)" ) {
            QSqlQuery q;
            q.exec( QString("ALTER TABLE " TABLE_LABEL_DEF " CHANGE ") +
                    query4.value( 0 ).toString() + " " + query4.value( 0 ).toString() + " NUMERIC(10,4)" );
            changed = true;
        }

    if( changed )
        KMessageBox::information( 0, i18n("The SQL tables of KBarcode have changed since the last version. "
                                          "KBarcode updated them without any loss of data."  ) );
}

bool SqlTables::testSettings( const QString & username, const QString & password, const QString & hostname, const QString & database, const QString & driver )
{
    QSqlDatabase* db = QSqlDatabase::addDatabase( driver );
    if( !drivers[driver] )
      return false;

    db->setDatabaseName( database );
    db->setUserName( username );
    db->setPassword( password );
    db->setHostName( hostname );

    if( !db->open() ) 
    {
        QSqlDatabase::removeDatabase( database );
    }
    else 
    {
        KMessageBox::information( 0, i18n("Connected successfully to your database") );
        db->close();
        QSqlDatabase::removeDatabase( database );
        return true;
    }

    db = QSqlDatabase::addDatabase( driver );
    
    db->setDatabaseName( drivers[driver]->initdb( database ) );

    db->setUserName( username );
    db->setPassword( password );
    db->setHostName( hostname );

    if( !db->open() ) {
        KMessageBox::error( 0, i18n("<qt>Connection failed:<br>") + database + "<br>" +
              db->lastError().databaseText() + "</qt>" );
        QSqlDatabase::removeDatabase(drivers[driver]->initdb( database ));
        return false;
    } else {
        KMessageBox::information( 0, i18n("Connected successfully to your database") );
        db->close();
        QSqlDatabase::removeDatabase(drivers[driver]->initdb( database ));
        return true;
    }

}

const QString SqlTables::getBarcodeMaxLength( const QString & name )
{
    if( SqlTables::isConnected() )
    {
        QSqlQuery query("select uid, (length(barcode_no)) as LEN from " TABLE_BASIC
            " where encoding_type = '" + name +"' ORDER by LEN DESC LIMIT 1" );
        
        while( query.next() ) {
            QSqlQuery queryuid("select barcode_no from barcode_basic where uid = '"
                + query.value( 0 ).toString() + "'" );
            while( queryuid.next() )
                if(!queryuid.value( 0 ).toString().isEmpty())
                    return queryuid.value( 0 ).toString();
        }
        
        QSqlQuery query1("select uid, (length(barcode_no)) as LEN from " TABLE_CUSTOMER_TEXT 
                        " where encoding_type = '" + name +"' ORDER by LEN DESC LIMIT 1" );
        
        while( query1.next() ) {
            QSqlQuery queryuid("select barcode_no from customer_text where uid = '"
                + query1.value( 0 ).toString() + "'" );
            while( queryuid.next() )
                if(!queryuid.value( 0 ).toString().isEmpty())
                    return queryuid.value( 0 ).toString();
        }
    }
    
    return "1234567";        
}

SqlWidget::SqlWidget( bool showlabel, QWidget* parent, const char* name )
    : QWidget( parent, name )
{
    Q3VBoxLayout* layout = new Q3VBoxLayout( this );

    Q3GroupBox* groupDatabase = new Q3GroupBox( this );
    groupDatabase->setTitle( i18n( "Database Settings" ) );
    groupDatabase->setColumnLayout(0, Qt::Vertical );
    groupDatabase->layout()->setSpacing( 6 );
    groupDatabase->layout()->setMargin( 11 );
    Q3VBoxLayout* groupDatabaseLayout = new Q3VBoxLayout( groupDatabase->layout() );
    groupDatabaseLayout->setAlignment( Qt::AlignTop );

    Q3GridLayout* grid = new Q3GridLayout( 2, 2 );

    QLabel* label = new QLabel( groupDatabase );
    label->setText( i18n("Username :") );
    grid->addWidget( label, 0, 0 );

    m_username = new KLineEdit( groupDatabase );
    grid->addWidget( m_username, 0, 1 );

    label = new QLabel( groupDatabase );
    label->setText( i18n("Password :") );
    grid->addWidget( label, 1, 0 );

    m_password = new KLineEdit( groupDatabase );
    m_password->setEchoMode( KLineEdit::Password );
    grid->addWidget( m_password, 1, 1 );

    label = new QLabel( groupDatabase );
    label->setText( i18n("Database :") );
    grid->addWidget( label, 2, 0 );

    m_database = new KLineEdit( "kbarcode", groupDatabase );
    grid->addWidget( m_database, 2, 1 );

    label = new QLabel( groupDatabase );
    label->setText( i18n("Host :") );
    grid->addWidget( label, 3, 0 );

    m_hostname = new KLineEdit( "localhost", groupDatabase );
    grid->addWidget( m_hostname, 3, 1 );

    label = new QLabel( groupDatabase );
    label->setText( i18n("Driver :") );
    grid->addWidget( label, 4, 0 );

    m_driver = new KComboBox( false, groupDatabase );
    QStringList drList = QSqlDatabase::drivers();
    QStringList::Iterator it = drList.begin();
    while( it != drList.end() ) {
        m_driver->addItem( *it );
        ++it;
    }
    grid->addWidget( m_driver, 4, 1 );
    groupDatabaseLayout->addLayout( grid );

    m_autoconnect = new QCheckBox( i18n("&Autoconnect on program start"), groupDatabase );
    groupDatabaseLayout->addWidget( m_autoconnect );

    buttonTest = new KPushButton( groupDatabase );
    buttonTest->setText( i18n( "&Test Settings" ) );

    groupDatabaseLayout->addWidget( buttonTest );
    if( showlabel )
        groupDatabaseLayout->addWidget( new QLabel( i18n("<b>You have to test your database settings before you can procede.</b>"), groupDatabase ) );

    layout->add( groupDatabase );

    connect( buttonTest, SIGNAL( clicked() ), this, SLOT( testSettings() ) );

    const mysqldata & sqldata = SqlTables::getInstance()->getData();
    m_username->setText( sqldata.username );
    m_password->setText( sqldata.password );
    m_hostname->setText( sqldata.hostname );
    m_database->setText( sqldata.database );
    m_autoconnect->setChecked( sqldata.autoconnect );
    for( int i = 0; i < m_driver->count(); i++ )
        if( m_driver->text(i) == sqldata.driver )
            m_driver->setCurrentItem( i );
}

SqlWidget::~SqlWidget()
{
}

void SqlWidget::save( bool usedb )
{
    mysqldata sqldata = SqlTables::getInstance()->getData();
    sqldata.username = m_username->text();
    sqldata.password = m_password->text();
    sqldata.hostname = m_hostname ->text();
    sqldata.database = m_database->text();
    sqldata.driver = m_driver->currentText();
    sqldata.autoconnect = ( usedb ? m_autoconnect->isChecked() : false );
    SqlTables::getInstance()->setData( sqldata );
}

void SqlWidget::testSettings()
{
    emit databaseWorking(
    SqlTables::getInstance()->testSettings( m_username->text(), m_password->text(),
                                            m_hostname->text(), m_database->text(),
                                            m_driver->currentText() ) );
}

const QString SqlWidget::username() const
{
    return m_username->text();
}

const QString SqlWidget::driver() const
{
    return m_driver->currentText();
}

const QString SqlWidget::database() const
{
    return m_database->text();
}

const QString SqlWidget::hostname() const
{
    return m_hostname->text();
}

const QString SqlWidget::password() const
{
    return m_password->text();
}

bool SqlWidget::autoconnect() const
{
    return m_autoconnect->isChecked();
}

int SqlWidget::driverCount() const
{
    return m_driver->count();
}


#include "sqltables.moc"
