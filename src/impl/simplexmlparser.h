#ifndef SIMPLEXMLPARSER_H
#define SIMPLEXMLPARSER_H

#include <QtCore>

#include "parser.h"

namespace updater::impl {

class Updater;

/**
 * Format example:
 *
 * <release><version>3.4.2</version></release>
 *
 * Or
 *
 * <release>
 * <version>3.4.2</version>
 * <url>https://mydomain/mypayload.%1</url>
 * </release>
 */
class SimpleXmlParser : public Parser {
    Q_OBJECT

public:
    explicit SimpleXmlParser(Updater *parent);
    void parse(const QByteArray &bytes);

    void setRootElementName(const QString &value) { rootElementName = value; }
    void setVersionElementName(const QString &value) { versionElementName = value; }
    void setUrlElementName(const QString &value) { urlElementName = value; }
    void setUrlArguments(const QStringList &value) { urlArguments = value; }

private:
    QString rootElementName = "release";
    QString versionElementName = "version";
    QString urlElementName = "url";
    QStringList urlArguments;
};

} // namespace updater::impl

#endif // SIMPLEXMLPARSER_H
