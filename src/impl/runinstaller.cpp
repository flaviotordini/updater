#include "runinstaller.h"

#include <QDesktopServices>

#include "defaultupdater.h"

namespace updater {

RunInstaller::RunInstaller() : Installer() {}

void RunInstaller::start(const QString &filename) {
    auto processedArguments = arguments;

    if (command.isEmpty()) {
        command = filename;
    } else {
        // replace markers
        for (auto &arg : processedArguments)
            arg.replace("%filename%", filename);
#ifdef Q_OS_LINUX
            if (runAsAdmin) {
                processedArguments.prepend(command);
                command = "pkexec";
            }
#endif
    }

    QProcess *process = new QProcess();
    QObject::connect(process, &QProcess::errorOccurred, this, [this](auto error) {
        this->emit error("Update error: " + QVariant::fromValue(error).toString());
    });

    if (!fork) {
        auto thread = new QThread;
        process->moveToThread(thread);
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
                [this](int exitCode, QProcess::ExitStatus exitStatus) {
                    qDebug() << "finished" << exitCode << exitStatus;
                    if (autoRestart) {
                        qDebug() << "Restarting" << qApp->applicationFilePath();
                        QProcess *restartProcess = new QProcess(this);
                        restartProcess->startDetached(qApp->applicationFilePath(), {});
                    }
                    emit finished();
                });
        qDebug() << "Executing" << command << processedArguments;
        process->start(command, processedArguments);
        process->waitForFinished(60000);
    } else {
        qDebug() << "Forking" << command << processedArguments;
        if (!process->startDetached(command, processedArguments)) {
            qWarning() << "Cannot execute" << command << processedArguments
                       << process->errorString();
            // Fallback to opening the downloaded payload
            QDesktopServices::openUrl(QUrl("file:///" + filename));
        }
        emit finished();
    }
}

} // namespace updater
