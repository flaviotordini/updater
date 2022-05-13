#ifndef SPARKLE_UPDATER_H
#define SPARKLE_UPDATER_H

#include "updater.h"

Q_FORWARD_DECLARE_OBJC_CLASS(SPUUpdater);

namespace updater {

class SparkleUpdater : public ::Updater {
    Q_OBJECT

public:
    SparkleUpdater();

    bool getAutomaticDownload() const;
    void setAutomaticDownload(bool value);

    void checkAndShowUI();
    void checkAndMaybeShowUI();
    void checkWithoutUI();

protected slots:
    void update();

private:
    SPUUpdater *updater = NULL;
};

} // namespace updater

#endif // SPARKLE_UPDATER_H
