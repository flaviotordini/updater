#ifndef UPDATER_H
#define UPDATER_H

#include <QtWidgets>


class Updater : public QObject {
    Q_OBJECT

public:
    /// Set a shared Updater instance
    static void setInstance(Updater *value);

    /// Get a shared updater instance, null by default
    static Updater &instance();

    Updater(QObject *parent = nullptr);

    QAction *getAction();
    QPushButton *getButton();
    QLabel *getLabel();

    enum class Status {
        Unknown,
        UpToDate,
        UpdateAvailable,
        DownloadingUpdate,
        UpdateDownloaded,
        UpdateDownloadFailed
    };
    Q_ENUM(Status)
    Status getStatus() const { return status; }
    // This should be protected
    void setStatus(Status v);

    QString getVersion() const { return version; }
    // This should be protected
    void setVersion(const QString &value) { version = value; }

    QString getDisplayVersion() const { return displayVersion; }
    // This should be protected
    void setDisplayVersion(const QString &value) { displayVersion = value; }

    /// true if the app can be stopped and relaunched without user interaction
    virtual bool getImmediateInstallAndRelaunch() const { return immediateInstallAndRelaunch; }
    virtual void setImmediateInstallAndRelaunch(bool value) { immediateInstallAndRelaunch = value; }

    /// true if the app has to be relaunched after the update has completed
    virtual bool getRelaunchAfterInstall() const { return relaunchAfterInstall; }
    virtual void setRelaunchAfterInstall(bool value) { relaunchAfterInstall = value; }

    /// when true an available update will be downloaded without user interaction
    virtual bool getAutomaticDownload() const { return automaticDownload; }
    virtual void setAutomaticDownload(bool value) { automaticDownload = value; }

    virtual void checkAndShowUI() = 0;
    virtual void checkAndMaybeShowUI() = 0;
    virtual void checkWithoutUI() = 0;

protected slots:
    virtual void update() = 0;
    virtual void onUserAction();

signals:
    void statusChanged(Status status);
    void actionTextChanged(const QString &actionText);

private:
    QAction *action = nullptr;
    QPushButton *button = nullptr;
    QLabel *label = nullptr;

    bool automaticDownload = true;
    bool immediateInstallAndRelaunch = false;
    bool relaunchAfterInstall = false;
    Status status = Status::Unknown;
    QString version;
    QString displayVersion;
};

#endif // UPDATER_H
