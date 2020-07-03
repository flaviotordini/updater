#ifndef UPDATER_IMPL_CHECKER_H
#define UPDATER_IMPL_CHECKER_H

#include <QtNetwork>

namespace updater {

class Updater;

class Checker : public QObject {
    Q_OBJECT

public:
    explicit Checker(Updater *updater, QObject *parent = nullptr);

    void check();
    bool hasError() const { return error; };
    QString getErrorMessage() const { return errorMessage; };

    static qint64 getLastCheck();

signals:
    void done();

private:
    Updater *updater;

    bool error = false;
    QString errorMessage;
};

} // namespace updater

#endif // UPDATER_IMPL_CHECKER_H
