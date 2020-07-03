#ifndef UPDATER_IMPL_DOWNLOADER_H
#define UPDATER_IMPL_DOWNLOADER_H

#include <QtNetwork>

#include "http.h"

namespace updater::impl {

class Downloader : public QObject {
    Q_OBJECT

public:
    void download(const QUrl &url);
    void stop();

signals:
    void progress(int percent);
    void error(const QString &message);
    void fileReady(const QString &filename);

private:
    QNetworkReply *reply = nullptr;
    QFile file;
};

} // namespace updater::impl

#endif // UPDATER_IMPL_DOWNLOADER_H
