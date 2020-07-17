#include "runinstaller.h"

#include <QDesktopServices>

namespace updater {

RunInstaller::RunInstaller() : Installer() {}

void RunInstaller::start(const QString &filename) {
    if (arguments.isEmpty()) {
        QDesktopServices::openUrl(QUrl("file:///" + filename));
    } else {
        QProcess *process = new QProcess(this);
        QObject::connect(process, &QProcess::errorOccurred,
                         [](auto error) { qWarning() << "Update error:" << error; });
        process->startDetached(filename, arguments);
    }
}

} // namespace updater
