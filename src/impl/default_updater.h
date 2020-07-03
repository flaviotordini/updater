#ifndef UPDATER_IMPL_UPDATER_H
#define UPDATER_IMPL_UPDATER_H

#include "updater.h"

namespace updater::impl {

class Downloader;
class Dialog;
class Installer;
class Parser;

class Updater : public ::Updater {
    Q_OBJECT

public:
    Updater();

    QString getAppName() const { return appName; }
    void setAppName(const QString &value) { appName = value; }

    QIcon getIcon() const { return icon; }
    void setIcon(const QIcon &value) { icon = value; }

    QString getLocalVersion() const { return localVersion; }
    void setLocalVersion(const QString &value) { localVersion = value; }

    QUrl getManifestUrl() const { return manifestUrl; }
    void setManifestUrl(const QUrl &value) { manifestUrl = value; }

    void setCheckInterval(const qint64 &value);

    void setDownloadUrl(const QUrl &value) { downloadUrl = value; }

    void setInstaller(Installer *value) { installer = value; }

    Parser *getParser() const { return parser; }
    void setParser(Parser *value) { parser = value; }

    Downloader *downloadUpdate();
    Downloader *getDownloader() const { return downloader; }

    void checkAndShowUI();
    void checkAndMaybeShowUI();
    void checkWithoutUI();

protected slots:
    void update();

private:
    void showDialog();

    QString appName = QGuiApplication::applicationDisplayName();
    QIcon icon = QGuiApplication::windowIcon();
    QString localVersion = QCoreApplication::applicationVersion();

    QUrl manifestUrl;
    qint64 checkInterval = 86400;

    Downloader *downloader = nullptr;
    QUrl downloadUrl;
    QString downloadedFilename;

    Installer *installer = nullptr;
    Parser *parser = nullptr;

    Dialog *dialog = nullptr;

    QTimer *checkTimer;
};

} // namespace updater::impl

#endif // UPDATER_IMPL_UPDATER_H
