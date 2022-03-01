#ifndef UPDATER_IMPL_RUNINSTALLER_H
#define UPDATER_IMPL_RUNINSTALLER_H

#include <QtCore>

#include "installer.h"

namespace updater {

/**
 * Installer implementation that executes a command with arguments
 */
class RunInstaller : public Installer {
    Q_OBJECT

public:
    RunInstaller();
    void setCommand(const QString &value) { command = value; }
    void setArguments(const QStringList &value) { arguments = value; };

    /**
     * Currently supported on Linux only using pkexec. Does not work together with fork.
     */
    void setRunAsAdmin(bool value) { runAsAdmin = value; }

    /**
     * This will cause the update process to run while the app is still running. Then the app will
     * self-restart. Does not work together with fork.
     */
    void setAutoRestart(bool value) { autoRestart = value; }

    /**
     * When true the update process will be detached from the app process (i.e. the app can quit
     * without killing it). Does not work together with autoRestart.
     */
    void setFork(bool value) { fork = value; }

    void start(const QString &filename);

private:
    QString command;
    QStringList arguments;
    bool runAsAdmin = false;
    bool autoRestart = false;
    bool fork = false;
};

} // namespace updater

#endif // UPDATER_IMPL_RUNINSTALLER_H
