#include "openinstaller.h"

#include <QDesktopServices>

namespace updater {

OpenInstaller::OpenInstaller() {}

void updater::OpenInstaller::start(const QString &filename) {
    if (!QDesktopServices::openUrl(QUrl("file:///" + filename))) emit error("Cannot start update");
}

} // namespace updater
