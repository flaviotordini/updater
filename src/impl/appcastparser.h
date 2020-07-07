#ifndef APPCASTPARSER_H
#define APPCASTPARSER_H

#include <QtCore>

#include "parser.h"

namespace updater {

class DefaultUpdater;

class AppcastParser : public Parser {
public:
    AppcastParser();
    void parse(const QByteArray &bytes);
};

} // namespace updater

#endif // APPCASTPARSER_H
