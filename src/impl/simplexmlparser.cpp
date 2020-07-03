#include "simplexmlparser.h"

#include "default_updater.h"

namespace updater::impl {

SimpleXmlParser::SimpleXmlParser(Updater *parent) : Parser(parent) {}

void SimpleXmlParser::parse(const QByteArray &bytes) {
    error = false;
    errorMessage.clear();

    QXmlStreamReader reader;
    reader.addData(bytes);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            if (reader.name() == rootElementName) {
                while (!reader.atEnd()) {
                    reader.readNext();
                    if (reader.isStartElement() && reader.name() == versionElementName) {
                        updater->setVersion(reader.readElementText());
                    } else if (reader.isStartElement() && reader.name() == urlElementName) {
                        QString url = reader.readElementText();
                        for (const auto &arg : qAsConst(urlArguments)) {
                            url = url.arg(arg);
                        }
                        updater->setDownloadUrl(url);
                    }
                }
            }
        }
    }
    if (reader.hasError()) {
        error = true;
        errorMessage = reader.errorString();
    }
}

} // namespace updater::impl
