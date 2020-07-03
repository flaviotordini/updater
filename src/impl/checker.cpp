#include "checker.h"

#include "default_updater.h"
#include "http.h"
#include "parser.h"
#include "simplexmlparser.h"

namespace {
const QString updateCheckKey = "updateCheck";
}

namespace updater {

Checker::Checker(Updater *updater, QObject *parent) : QObject(parent), updater(updater) {}

void Checker::check() {
    error = false;
    errorMessage.clear();

    HttpReply *reply = Http::instance().get(updater->getManifestUrl());
    connect(reply, &HttpReply::finished, this, [this](const HttpReply &reply) {
        if (!reply.isSuccessful()) {
            qWarning() << "Update check failed" << reply.statusCode() << reply.reasonPhrase();
            error = true;
            errorMessage = reply.reasonPhrase();
            emit done();
            return;
        }

        auto parser = updater->getParser();
        if (!parser) {
            // default to SimpleXmlParser
            SimpleXmlParser simpleParser(updater);
            simpleParser.parse(reply.body());
        } else {
            parser->parse(reply.body());
        }

        bool shouldUpdate = !updater->getVersion().isEmpty() &&
                            updater->getVersion() != updater->getLocalVersion();
        if (shouldUpdate) {
            if (updater->getAutomaticDownload()) {
                updater->downloadUpdate();
            } else {
                updater->setStatus(Updater::Status::UpdateAvailable);
            }
        }

        QSettings settings;
        settings.setValue(updateCheckKey, QDateTime::currentSecsSinceEpoch());
        emit done();
    });
}

qint64 Checker::getLastCheck() {
    QSettings settings;
    return settings.value(updateCheckKey).toInt();
}

} // namespace updater
