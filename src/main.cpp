/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Don Apr 18 12:34:56 CEST 2002
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

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kimageio.h>
#include <klocale.h>
#include <kstandarddirs.h>

#include "kbarcode.h"
#include "labeleditor.h"
#include "barcodedialog.h"

#ifndef VERSION
    #define VERSION "Unknown Version"
#endif

static KCmdLineOptions options[] =
{
    { "+[file]", I18N_NOOP("Load the file in KBarcode (if --batch is specified, the file will be used in batchprinting mode)"), 0},
    { "print", I18N_NOOP("Print any loaded files immediately on the default printer "
                         "or on the printer specified by the --printer commandline option and exit afterwards"), 0 },
    { "printer <printer>", I18N_NOOP("Printer/destination to print on"), 0},
    { ":", I18N_NOOP("KBarcode Modes:"), 0 },
    { "barcode", I18N_NOOP("Start KBarcode as xbarcode replacement"), 0 },
    { "label", I18N_NOOP("Open the label editor"), 0 },
    { "batch", I18N_NOOP("Start directly in batchprinting mode"), 0 },
    { ":", I18N_NOOP("Additional options for batchprinting (--batch):"), 0 },
    { "output <mode>", I18N_NOOP("Output format where mode is one of POSTSCRIPT|IMAGE|BARCODE"), "POSTSCRIPT" },
    { "serialnumber <value>", I18N_NOOP("Set the serial number to be used"), 0 },
    { "serialinc <value>", I18N_NOOP("Increase a previously specified serial number using this value for each printed label"), 0 },
    { "numlabels <value>", I18N_NOOP("Print <value> labels without any data"), 0 },
    { "importsql <query>", I18N_NOOP("Import variable data from a specified sql query"), 0 },
    { "importcsv <csvfile>", I18N_NOOP("Import variable data from a specified csv file"), 0 },
    KCmdLineLastOption
};

void setupDirs()
{
    if( locate("data", "kbarcode/logo.png").isEmpty() ) {
        // Maybe KBarcode was not installed into $KDEDIR
        KStandardDirs* dirs = KGlobal::dirs();
        QString kdedir = dirs->kfsstnd_prefixes();

        if( !kdedir.contains( "/usr/local/" ) )
            dirs->addPrefix( "/usr/local/" );

        if( !kdedir.contains( "/usr/local/kde/" ) )
            dirs->addPrefix( "/usr/local/kde/" );

        if( !kdedir.contains( "/usr/" ) )
            dirs->addPrefix( "/usr/" );

        qDebug("WARNING: Prefix changed: %s", dirs->kfsstnd_prefixes().latin1() );
    }
}

int main(int argc, char *argv[])
{
    KAboutData aboutData( "kbarcode", I18N_NOOP("KBarcode"), VERSION, I18N_NOOP(
        "KBarcode is a barcode and label printing application for KDE 3."
        ), KAboutData::License_GPL, "KBarcode Build:\n" __DATE__ " " __TIME__
        , "(c) 2001-2006, Dominik Seichter\n\n",
        "http://www.kbarcode.net", "kbarcode-users@lists.sourceforge.net" );

    aboutData.addAuthor("Dominik Seichter", I18N_NOOP("Programmer"), "domseichter@web.de");
    aboutData.addAuthor("Stefan \"Stonki\" Onken",
        I18N_NOOP("Project Manager"),
        "support@stonki.de", "http://www.stonki.de" );

    aboutData.addCredit("Alessandro Rubini", I18N_NOOP("Wrote GNU Barcode on which kbarcode is based."),
        "rubini@prosa.it", "http://arcana.linux.it/" );
    aboutData.addCredit("Terry Burton", I18N_NOOP("Author of Barcode Writer in Pure Postscript"),
                        "tez@terryburton.co.uk", "http://www.terryburton.co.uk/" );
    aboutData.addCredit("Daniele Medri", I18N_NOOP("Italian translation"), "madrid@linuxmeeting.net" );
    aboutData.addCredit("Anton Vaaranmaa",
         I18N_NOOP("Finnish and Swedish translation, created the KBarcode icon"), "antonv@postikaista.net" );
    aboutData.addCredit("Miguel Revilla Rodr�uez", I18N_NOOP("Spanish translation"), "yo@miguelrevilla.com" );
    aboutData.addCredit("Sandor Jager", I18N_NOOP("Hungarian translation"), "jager@puskas.hu" );
    aboutData.addCredit("Daniel Etzold",
        I18N_NOOP("Tab icon code stolen from his excellent app qtella."), "detzold@qtella.net" );
    aboutData.addCredit("Richard J. Moore", I18N_NOOP("Wrote the RichText KPart"), "rich@kde.org", "http://xmelegance.org/" );
    aboutData.addCredit("Yann Bouan", I18N_NOOP("French Translation"), "yann@bouan.net" );
    aboutData.addCredit("Erich Kitzmüller", I18N_NOOP("Help with ZPL and IPL code"), "kitzmueller@metasyst.at" );
    aboutData.addCredit("Dag Nygren", I18N_NOOP("Wrote many patches to improve KBarcode"), "dag@newtech.fi" );
    aboutData.addCredit("Frank Schoolmeesters", I18N_NOOP("Made the Netherlands translation"), "frank_schoolmeesters@fastmail.fm" );
    aboutData.addCredit("John Volpe", I18N_NOOP("Added lot's of useful data fields to kbarcode"), "jtvolpe@cape.com" );
    aboutData.addCredit("Nyssa s.r.l.", I18N_NOOP("Added TEC barcode printer support"), "imorrison@nyssa.com.ar", "http://www.nyssa.com.ar" );
    aboutData.addCredit("Brian Glass", I18N_NOOP("Added EPCL barcode printer support"), "brian@glassbrian.com", "http://www.glassbrian.com" );
    KCmdLineArgs::init( argc, argv, &aboutData );
    KCmdLineArgs::addCmdLineOptions( options );

    KApplication a;
    // setup standard dirs
    setupDirs();

    KImageIO::registerFormats();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

    KBarcode* kbc = new KBarcode( 0, "KBarcodeMainWindow" );
    
    if( kbc->parseCmdLine() )
        return 0;
    else
        return a.exec();
}
