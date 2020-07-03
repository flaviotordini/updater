#include "downloader.h"

#include <QDesktopServices>

namespace updater::impl {

void Downloader::download(const QUrl &url) {
    const QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString filename = tempDir + "/" +
                       QByteArray::number(QRandomGenerator::global()->generate()).toHex() +
                       url.fileName();
    file.setFileName(filename);

    qDebug() << "Downloading" << url << "to" << filename;
    HttpRequest req;
    req.url = url;
    reply = Http::instance().networkReply(req);

    connect(reply, &QNetworkReply::readyRead, this, [this] {
        if (!reply) return;

        if (!file.isOpen()) {
            if (!file.open(QIODevice::ReadWrite)) {
                emit error(QString("Error opening file: %1").arg(file.errorString()));
                reply->disconnect();
                reply->abort();
                reply->deleteLater();
                reply = nullptr;
                return;
            }
        }
        if (-1 == file.write(reply->readAll())) {
            emit error(file.errorString());
            reply->disconnect();
            reply->abort();
            reply->deleteLater();
            reply = nullptr;
        }
    });

    connect(reply,
            static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(
                    &QNetworkReply::error),
            this, [this] {
                emit error(reply->errorString());
                reply->disconnect();
                reply->abort();
                reply->deleteLater();
                reply = nullptr;
            });

    connect(reply, &QNetworkReply::downloadProgress, this,
            [this](qint64 bytesReceived, qint64 bytesTotal) {
                if (bytesTotal <= 0) return;
                int percent = bytesReceived * 100 / bytesTotal;
                emit progress(percent);
            });

    connect(reply, &QNetworkReply::finished, this, [this] {
        if (!reply) return;
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status != 200) {
            QString message =
                    reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
            if (message.isEmpty()) message = reply->errorString();
            emit error(message);
            return;
        }

        file.close();

        reply->deleteLater();
        reply = nullptr;

        emit fileReady(file.fileName());
    });
}

void Downloader::stop() {
    if (!reply) return;
    reply->disconnect();
    reply->abort();
    reply->deleteLater();
    reply = nullptr;
    emit error("Canceled");
}

} // namespace updater::impl
