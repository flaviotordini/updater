#include "appcastparser.h"

#include "defaultupdater.h"

namespace updater {

AppcastParser::AppcastParser() {}

void AppcastParser::parse(const QByteArray &bytes) {
    error = false;
    errorMessage.clear();

    const QLatin1String sparkleNS("http://www.andymatuschak.org/xml-namespaces/sparkle");

    QXmlStreamReader reader(bytes);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name() == QLatin1String("item")) {
            while (reader.readNextStartElement()) {
                if (reader.name() == QLatin1String("enclosure")) {
                    auto attrs = reader.attributes();
                    QString url = attrs.value(QLatin1String("url")).toString();
                    updater->setDownloadUrl(url);

                    QString version = attrs.value(sparkleNS, QLatin1String("version")).toString();
                    updater->setVersion(version);

                    QString shortVersionString =
                            attrs.value(sparkleNS, QLatin1String("shortVersionString")).toString();
                    updater->setDisplayVersion(shortVersionString);
                }
                reader.skipCurrentElement();
            }
        }
    }

    if (reader.hasError()) {
        error = true;
        errorMessage = reader.errorString();
    }
}

} // namespace updater
