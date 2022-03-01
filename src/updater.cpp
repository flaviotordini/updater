#include "updater.h"

namespace {
Updater *sharedInstance = nullptr;
}

void Updater::setInstance(Updater *value) {
    sharedInstance = value;
}

Updater &Updater::instance() {
    return *sharedInstance;
}

Updater::Updater(QObject *parent) : QObject(parent) {}

QAction *Updater::getAction() {
    if (!action) {
        action = new QAction(this);
        connect(action, &QWidget::destroyed, this, [this] { action = nullptr; });
        action->setMenuRole(QAction::ApplicationSpecificRole);
        connect(action, &QAction::triggered, this, &Updater::onUserAction);
        auto onStatusChange = [this](Updater::Status status) {
            QString v = displayVersion.isEmpty() ? version : displayVersion;
            QString t;
            switch (status) {
            case Updater::Status::UpToDate:
                t = tr("Check for Updates...");
                break;
            case Updater::Status::UpdateAvailable:
                t = tr("Version %1 is available...").arg(v);
                break;
            case Updater::Status::DownloadingUpdate:
                t = tr("Downloading version %1...").arg(v);
                break;
            case Updater::Status::UpdateDownloaded:
                t = tr("Restart to Update");
                break;
            case Updater::Status::UpdateDownloadFailed:
                t = tr("Version %1 download failed").arg(v);
                break;
            }
            action->setText(t);
        };
        connect(this, &Updater::statusChanged, this, onStatusChange);
        onStatusChange(status);
    }
    return action;
}

QPushButton *Updater::getButton() {
    if (!button) {
        button = new QPushButton();
        connect(button, &QWidget::destroyed, this, [this] { button = nullptr; });
        connect(button, &QPushButton::clicked, this, &Updater::onUserAction);
        auto onStatusChange = [this](Updater::Status status) {
            QString t;
            bool visible = true;
            switch (status) {
            case Updater::Status::Unknown:
            case Updater::Status::UpToDate:
                t = tr("Check for Updates");
                break;
            case Updater::Status::UpdateAvailable:
                t = tr("Download Update");
                break;
            case Updater::Status::DownloadingUpdate:
                t = tr("Downloading update...");
                visible = false;
                break;
            case Updater::Status::UpdateDownloaded:
                t = tr("Restart to Update");
                break;
            case Updater::Status::UpdateDownloadFailed:
                t = tr("Retry Update Download");
                break;
            }
            button->setText(t);
            button->setVisible(visible);
        };
        connect(this, &Updater::statusChanged, button, onStatusChange);
        onStatusChange(status);
    }
    return button;
}

QLabel *Updater::getLabel() {
    if (!label) {
        label = new QLabel();
        connect(label, &QWidget::destroyed, this, [this] { label = nullptr; });
        auto onStatusChange = [this](Updater::Status status) {
            QString v = displayVersion.isEmpty() ? version : displayVersion;
            QString t;
            switch (status) {
            case Updater::Status::Unknown:
                t = tr("Didn't check for updates.");
                break;
            case Updater::Status::UpToDate:
                t = tr("You have the latest version.");
                break;
            case Updater::Status::UpdateAvailable:
                t = tr("Version %1 is available.").arg(v);
                break;
            case Updater::Status::DownloadingUpdate:
                t = tr("Downloading update...");
                break;
            case Updater::Status::UpdateDownloaded:
                t = tr("An update has been downloaded and is ready to be installed.");
                break;
            case Updater::Status::UpdateDownloadFailed:
                t = tr("Version %1 download failed.").arg(v);
                break;
            }
            label->setText(t);
        };
        connect(this, &Updater::statusChanged, label, onStatusChange);
        onStatusChange(status);
    }
    return label;
}

void Updater::setStatus(Status v) {
    if (status != v) {
        status = v;
        emit statusChanged(status);
    }
}

void Updater::onUserAction() {
    if (status == Updater::Status::UpdateDownloaded) {
        // update will be installed on quit
        qApp->quit();
        return;
    } else {
        checkAndShowUI();
    }
}
