#include "runinstaller.h"

#include <QDesktopServices>

#include "default_updater.h"

namespace updater::impl {

RunInstaller::RunInstaller(Updater *updater) : Installer(updater) {}

void RunInstaller::start(const QString &filename) {
    // TODO use updaterArgs for silent install on Windows
    QDesktopServices::openUrl(QUrl("file:///" + filename));
}

} // namespace updater::impl
