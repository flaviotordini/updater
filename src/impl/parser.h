#ifndef UPDATER_IMPL_PARSER_H
#define UPDATER_IMPL_PARSER_H

#include <QObject>

namespace updater::impl {

class Updater;

/**
 * Implement this interface to parse your custom manifest format
 */
class Parser : public QObject {
    Q_OBJECT

public:
    /// Parse your manifest and set relevant Updater properties
    virtual void parse(const QByteArray &bytes) = 0;
    bool hasError() const { return error; };
    QString getErrorMessage() const { return errorMessage; };

protected:
    explicit Parser(Updater *parent = nullptr) : updater(parent){};
    Updater *updater;
    bool error = false;
    QString errorMessage;
};

} // namespace updater::impl

#endif // UPDATER_IMPL_PARSER_H
