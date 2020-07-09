#ifndef UPDATER_IMPL_INSTALLER_H
#define UPDATER_IMPL_INSTALLER_H

#include <QtCore>

namespace updater {

class DefaultUpdater;

/**
 * Implement this interface to create your own installer mechanism
 * E.g. Unzip and copy files, payload integrity checking, validation, etc.
 */
class Installer : public QObject {
    Q_OBJECT

public:
    Installer(){};
    void setUpdater(DefaultUpdater *value) { updater = value; }

    virtual void start(const QString &filename) = 0;

signals:
    void error(const QString &message);

protected:
    DefaultUpdater *updater;
};

} // namespace updater

#endif // UPDATER_IMPL_INSTALLER_H
