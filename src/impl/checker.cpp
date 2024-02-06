#include "checker.h"

#include "appcastparser.h"
#include "defaultupdater.h"
#include "parser.h"

#ifdef HTTP
#include "http.h"
#endif

namespace {
const QString updateCheckKey = "updateCheck";
}

namespace updater {

Checker::Checker(DefaultUpdater *updater, QObject *parent) : QObject(parent), updater(updater) {}

void Checker::check() {
    qDebug() << "Checking for update" << updater->getManifestUrl();
    error = false;
    errorMessage.clear();

#ifdef HTTP
    HttpReply *reply = Http::instance().get(updater->getManifestUrl());
    connect(reply, &HttpReply::finished, this, [this](const HttpReply &reply) {
        if (reply.isSuccessful()) {
            invokeParser(reply.body());
        } else {
            qWarning() << "Update check failed" << reply.statusCode() << reply.reasonPhrase();
            error = true;
            errorMessage = reply.reasonPhrase();
            updater->setStatus(Updater::Status::UpToDate);
            emit done();
        }
    });
#else
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(updater->getManifestUrl());
    request.setRawHeader("User-Agent",
                         (updater->getAppName() + ' ' + updater->getLocalVersion()).toUtf8());
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply) {
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status >= 200 && status < 300) {
            invokeParser(reply->readAll());
        } else {
            QString msg = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
            qWarning() << "Update check failed" << status << msg;
            error = true;
            errorMessage = msg;
            updater->setStatus(Updater::Status::UpToDate);
            emit done();
        }
        reply->deleteLater();
    });
    manager->get(request);
#endif
} // namespace updater

qint64 Checker::getLastCheck() {
    QSettings settings;
    return settings.value(updateCheckKey).toInt();
}

void Checker::invokeParser(const QByteArray &bytes) {
    auto parser = updater->getParser();
    if (!parser) {
        AppcastParser defaultParser;
        defaultParser.setUpdater(updater);
        defaultParser.parse(bytes);
    } else {
        parser->parse(bytes);
    }

    auto compareVersions = [](QString localVersion, QString remoteVersion) {
        qDebug() << "Comparing versions" << localVersion << remoteVersion;
        auto local = QVersionNumber::fromString(localVersion);
        auto remote = QVersionNumber::fromString(remoteVersion);
        return QVersionNumber::compare(local, remote) < 0;
    };

    bool remoteVersionIsNewer = compareVersions(updater->getLocalVersion(), updater->getVersion());
    if (remoteVersionIsNewer && updater->getStatus() != Updater::Status::UpdateDownloaded) {
        if (updater->getAutomaticDownload()) {
            updater->downloadUpdate();
        } else {
            updater->setStatus(Updater::Status::UpdateAvailable);
        }
    } else {
        updater->setStatus(Updater::Status::UpToDate);
    }

    QSettings settings;
    settings.setValue(updateCheckKey, QDateTime::currentSecsSinceEpoch());
    emit done();
}

} // namespace updater
