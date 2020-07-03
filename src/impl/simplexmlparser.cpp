#include "simplexmlparser.h"

#include "default_updater.h"

namespace updater::impl {

SimpleXmlParser::SimpleXmlParser(Updater *parent) : Parser(parent) {}

} // namespace updater::impl

void updater::impl::SimpleXmlParser::parse(const QByteArray &bytes) {
    error = false;
    errorMessage.clear();

    QXmlStreamReader reader;
    reader.addData(bytes);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("release")) {
                while (!reader.atEnd()) {
                    reader.readNext();
                    if (reader.isStartElement() && reader.name() == QLatin1String("version")) {
                        updater->setVersion(reader.readElementText());
                        break;
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
