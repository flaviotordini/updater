#ifndef UPDATER_IMPL_RUNINSTALLER_H
#define UPDATER_IMPL_RUNINSTALLER_H

#include <QObject>

#include "installer.h"

namespace updater {

/**
 * Installer implementation that simply runs the downloaded update
 */
class RunInstaller : public Installer {
    Q_OBJECT

public:
    RunInstaller();
    void setArguments(const QStringList &value) { arguments = value; };
    void start(const QString &filename);

private:
    QStringList arguments;
};

} // namespace updater

#endif // UPDATER_IMPL_RUNINSTALLER_H
