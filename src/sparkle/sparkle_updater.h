#ifndef SPARKLE_UPDATER_H
#define SPARKLE_UPDATER_H

#include "updater.h"

namespace updater::sparkle {

class Updater : public ::Updater {
    Q_OBJECT

public:
    Updater();

    bool getAutomaticDownload() const;
    void setAutomaticDownload(bool value);

    void checkAndShowUI();
    void checkAndMaybeShowUI();
    void checkWithoutUI();

protected slots:
    void update();
};

} // namespace updater::sparkle

#endif // SPARKLE_UPDATER_H
