#include <QtGui>
#include "utils/uninstall-helpers.h"

#include "uninstall-helper-dialog.h"


UninstallHelperDialog::UninstallHelperDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    setWindowIcon(QIcon(":/images/seafile.png"));
    setWindowTitle(tr("Uninstall %1").arg(SEAFILE_CLIENT_BRAND));

    mText->setText(tr("Do you want to remove the %1 account information?").arg(SEAFILE_CLIENT_BRAND));

    loadQss("qt.css") || loadQss(":/qt.css");
#if defined(Q_WS_WIN)
    loadQss("qt-win.css") || loadQss(":/qt-win.css");
#elif defined(Q_WS_X11)
    loadQss("qt-linux.css") || loadQss(":/qt-linux.css");
#else
    loadQss("qt-mac.css") || loadQss(":/qt-mac.css");
#endif

    const QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.center() - this->rect().center());

    connect(mYesBtn, SIGNAL(clicked()),
            this, SLOT(onYesClicked()));

    connect(mNoBtn, SIGNAL(clicked()),
            this, SLOT(onNoClicked()));
}

void UninstallHelperDialog::onYesClicked()
{
    removeSeafileData();
    ::exit(0);
}

void UninstallHelperDialog::removeSeafileData()
{
    mYesBtn->setEnabled(false);
    mNoBtn->setEnabled(false);

    mText->setText(tr("Removing account information..."));

    qApp->processEvents();

    QString ccnet_dir;
    QString seafile_data_dir;

    if (get_ccnet_dir(&ccnet_dir) < 0) {
        fprintf (stderr, "ccnet dir not found");
        return;
    }
    if (get_seafile_data_dir(ccnet_dir, &seafile_data_dir) < 0) {
        delete_dir_recursively(ccnet_dir);
        return;
    }

    delete_dir_recursively(ccnet_dir);
    delete_dir_recursively(seafile_data_dir);
}

void UninstallHelperDialog::onNoClicked()
{
    ::exit(0);
}

bool UninstallHelperDialog::loadQss(const QString& path)
{
    QFile file(path);
    if (!QFileInfo(file).exists()) {
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream input(&file);
    style_ += "\n";
    style_ += input.readAll();
    qApp->setStyleSheet(style_);

    return true;
}
