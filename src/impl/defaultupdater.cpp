#include "defaultupdater.h"

#include "checker.h"
#include "dialog.h"
#include "downloader.h"
#include "installer.h"
#include "openinstaller.h"
#include "parser.h"

namespace updater {

DefaultUpdater::DefaultUpdater() {
    checkTimer = new QTimer(this);
    checkTimer->setInterval(checkInterval);
    checkTimer->setTimerType(Qt::VeryCoarseTimer);
    auto autoCheck = [this] {
        // auto check after interval
        qint64 lastCheck = Checker::getLastCheck();
        int secondsSinceLastCheck = QDateTime::currentSecsSinceEpoch() - lastCheck;
        if (secondsSinceLastCheck >= checkInterval) {
            auto checker = new Checker(this);
            connect(checker, &Checker::done, this, [this, checker] {
                if (!getAutomaticDownload() && getStatus() != Updater::Status::UpToDate) {
                    showDialog();
                }
                checker->deleteLater();
            });
            checker->check();
        }
    };
    connect(checkTimer, &QTimer::timeout, this, autoCheck);
    QTimer::singleShot(5000, autoCheck);
    checkTimer->start();
}

void DefaultUpdater::setCheckInterval(const qint64 &value) {
    checkInterval = value;
    checkTimer->setInterval(checkInterval);
}

void DefaultUpdater::setInstaller(Installer *value) {
    installer = value;
    installer->setUpdater(this);
}

void DefaultUpdater::setParser(Parser *value) {
    parser = value;
    parser->setUpdater(this);
}

void DefaultUpdater::checkAndShowUI() {
    auto checker = new Checker(this);
    connect(checker, &Checker::done, this, [this, checker] {
        if (getStatus() == Updater::Status::UpToDate) {
            QMessageBox msgBox(qApp->activeWindow());
            msgBox.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            msgBox.setWindowModality(Qt::WindowModal);
            msgBox.setIconPixmap(getIcon().pixmap(64, 64));
            msgBox.setText(tr("There are currently no updates available."));
            msgBox.exec();
        } else {
            if (getStatus() != Updater::Status::UpdateAvailable) {
                downloadUpdate();
            }
            showDialog();
        }

        checker->deleteLater();
    });
    checker->check();
}

void DefaultUpdater::checkAndMaybeShowUI() {
    auto checker = new Checker(this);
    connect(checker, &Checker::done, this, [this, checker] {
        if (getStatus() != Updater::Status::UpToDate) {
            showDialog();
        }
        checker->deleteLater();
    });
    checker->check();
}

void DefaultUpdater::checkWithoutUI() {
    auto checker = new Checker(this);
    connect(checker, &Checker::done, this, [checker] { checker->deleteLater(); });
    checker->check();
}

void DefaultUpdater::update() {
    if (!installer) {
        installer = new OpenInstaller();
        installer->setUpdater(this);
    }
    connect(installer, &Installer::error, this, [](auto message) { qWarning() << message; });
    connect(installer, &Installer::finished, this,
            [this]() { setStatus(Updater::Status::UpToDate); });
    installer->start(downloadedFilename);
}

Downloader *DefaultUpdater::downloadUpdate() {
    if (downloader) return downloader;
    if (!downloadedFilename.isEmpty()) {
        qDebug() << "Update already downloded";
    }

    setStatus(Updater::Status::DownloadingUpdate);

    downloader = new Downloader();
    connect(downloader, &Downloader::fileReady, [this](auto filename) {
        downloader->deleteLater();
        downloader = nullptr;
        downloadedFilename = filename;
        setStatus(Updater::Status::UpdateDownloaded);

        if (getImmediateInstallWithoutRelaunch()) {
            update();
        } else if (getImmediateInstallAndRelaunch()) {
            update();
            qApp->quit();
        } else {
            connect(qApp, &QCoreApplication::aboutToQuit, this, [this] { update(); });
        }

        setStatus(Updater::Status::UpdateDownloaded);
    });

    connect(downloader, &Downloader::error, [this](auto message) {
        qWarning() << message;
        downloader->deleteLater();
        downloader = nullptr;
        downloadedFilename.clear();
        setStatus(Updater::Status::UpdateDownloadFailed);
    });

    connect(downloader, &Downloader::progress,
            [](int percent) { qDebug() << QString("Downloading update %1%").arg(percent); });

    downloader->download(downloadUrl);

    return downloader;
}

void DefaultUpdater::showDialog() {
    if (!dialog) {
        dialog = new Dialog(this, qApp->activeWindow());
        connect(dialog, &QWidget::destroyed, this, [this] { dialog = nullptr; });
    }
    dialog->show();
}

} // namespace updater
