#include "runinstaller.h"

#include <QDesktopServices>

#include "defaultupdater.h"

namespace updater {

RunInstaller::RunInstaller() : Installer() {}

void RunInstaller::start(const QString &filename) {
    if (arguments.isEmpty()) {
        QDesktopServices::openUrl(QUrl("file:///" + filename));
    } else {
        QProcess *process = new QProcess(this);
        process->startDetached(filename, arguments);
    }
}

} // namespace updater
