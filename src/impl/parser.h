#ifndef UPDATER_IMPL_PARSER_H
#define UPDATER_IMPL_PARSER_H

#include <QtCore>

namespace updater {

class DefaultUpdater;

/**
 * Implement this interface to parse your custom manifest format
 */
class Parser {
public:
    /// Parse your manifest and set relevant Updater properties
    virtual void parse(const QByteArray &bytes) = 0;

    void setUpdater(DefaultUpdater *value) { updater = value; }
    bool hasError() const { return error; };
    QString getErrorMessage() const { return errorMessage; };

protected:
    Parser(){};
    DefaultUpdater *updater = nullptr;
    bool error = false;
    QString errorMessage;
};

} // namespace updater

#endif // UPDATER_IMPL_PARSER_H
