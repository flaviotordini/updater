#ifndef SIMPLEXMLPARSER_H
#define SIMPLEXMLPARSER_H

#include <QtCore>

#include "parser.h"

namespace updater::impl {

class Updater;

class SimpleXmlParser : public Parser {
    Q_OBJECT

public:
    explicit SimpleXmlParser(Updater *parent);
    void parse(const QByteArray &bytes);
};

} // namespace updater::impl

#endif // SIMPLEXMLPARSER_H
