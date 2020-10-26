#ifndef OPENINSTALLER_H
#define OPENINSTALLER_H

#include <QtCore>

#include "installer.h"

namespace updater {

/// Installer implementation that opens the downloaded update
class OpenInstaller : public Installer {
    Q_OBJECT

public:
    OpenInstaller();
    void start(const QString &filename);
};

} // namespace updater
#endif // OPENINSTALLER_H
