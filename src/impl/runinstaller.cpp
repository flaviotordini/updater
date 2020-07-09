#include "runinstaller.h"

#include <QDesktopServices>

#include "defaultupdater.h"

namespace updater {

RunInstaller::RunInstaller() : Installer() {}

void RunInstaller::start(const QString &filename) {
    // TODO use updaterArgs for silent install on Windows
    QDesktopServices::openUrl(QUrl("file:///" + filename));
}

} // namespace updater
