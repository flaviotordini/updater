#include "runinstaller.h"

#include <QDesktopServices>

namespace updater {

RunInstaller::RunInstaller() : Installer() {}

void RunInstaller::start(const QString &filename) {
    if (arguments.isEmpty()) {
        if (!QDesktopServices::openUrl(QUrl("file:///" + filename)))
            emit error("Cannot start update");
    } else {
        QProcess *process = new QProcess(this);
        QObject::connect(process, &QProcess::errorOccurred, this, [this](auto error) {
            this->emit error("Update error: " + QVariant::fromValue(error).toString());
        });
        process->startDetached(filename, arguments);
    }
}

} // namespace updater
