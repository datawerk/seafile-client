#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QWidget>
#include <QDir>

#include <glib-object.h>
#include <stdio.h>

#include "utils/utils.h"
#include "utils/process.h"
#include "seafile-applet.h"
#include "QtAwesome.h"
#ifdef Q_WS_MAC
#include "Application.h"
#endif

#define APPNAME "seafile-applet"

int main(int argc, char *argv[])
{
#ifdef Q_WS_MAC
    if ( QSysInfo::MacintoshVersion > QSysInfo::MV_10_8 ) {
        // fix Mac OS X 10.9 (mavericks) font issue
        // https://bugreports.qt-project.org/browse/QTBUG-32789
        QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
    }
#endif

#ifdef Q_WS_MAC
    Application app(argc, argv);
#else
    QApplication app(argc, argv);
#endif

    if (count_process(APPNAME) > 1) {
        QMessageBox::warning(NULL, getBrand(),
                             QObject::tr("%1 is already running").arg(getBrand()),
                             QMessageBox::Ok);
        return -1;
    }

    QDir::setCurrent(QApplication::applicationDirPath());

    app.setQuitOnLastWindowClosed(false);

    // see QSettings documentation
    QCoreApplication::setOrganizationName(getBrand());
    QCoreApplication::setOrganizationDomain("seafile.com");
    QCoreApplication::setApplicationName(QString("%1 Client").arg(getBrand()));

    // initialize i18n
    QTranslator qtTranslator;
#if defined(Q_WS_WIN)
    qtTranslator.load("qt_" + QLocale::system().name());
#else
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    app.installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load(QString(":/i18n/seafile_%1.qm").arg(QLocale::system().name()));
    app.installTranslator(&myappTranslator);


#if !GLIB_CHECK_VERSION(2, 35, 0)
    g_type_init();
#endif
#if !GLIB_CHECK_VERSION(2, 31, 0)
    g_thread_init(NULL);
#endif

    awesome = new QtAwesome(qApp);
    awesome->initFontAwesome();

    seafApplet = new SeafileApplet;
    seafApplet->start();

    return app.exec();
}
