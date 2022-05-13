#include "sparkleupdater.h"

#import <SPUStandardUpdaterController.h>
#import <SPUUpdater.h>
#import <SPUUpdaterDelegate.h>
#import <SUAppcastItem.h>

// https://sparkle-project.org/documentation/programmatic-setup/

@interface SparkleDelegate : NSObject <SPUUpdaterDelegate> {
}
@end

@implementation SparkleDelegate {
    Updater *u;
}

- (void)setUpdater:(Updater *)v {
    u = v;
}

- (void)updater:(SPUUpdater *)updater didFindValidUpdate:(SUAppcastItem *)item {
    qDebug() << item.versionString;
    Q_UNUSED(updater)
    u->setVersion(QString::fromNSString(item.displayVersionString));
    u->setStatus(Updater::Status::UpdateAvailable);
    // [updater installUpdatesIfAvailable];
}

- (void)updaterDidNotFindUpdate:(SPUUpdater *)updater {
    Q_UNUSED(updater)
    u->setStatus(Updater::Status::UpToDate);
}

- (void)updater:(SPUUpdater *)updater
        willDownloadUpdate:(SUAppcastItem *)item
               withRequest:(NSMutableURLRequest *)request {
    qDebug() << item.versionString << request;
    Q_UNUSED(updater)
    u->setStatus(Updater::Status::DownloadingUpdate);
}

- (void)updater:(SPUUpdater *)updater didDownloadUpdate:(SUAppcastItem *)item {
    qDebug() << item.versionString;
    Q_UNUSED(updater)
    u->setStatus(Updater::Status::UpdateDownloaded);
}

- (void)userDidCancelDownload:(SPUUpdater *)updater {
    Q_UNUSED(updater)
    u->setStatus(Updater::Status::UpdateAvailable);
}

- (void)updater:(SPUUpdater *)updater
        failedToDownloadUpdate:(SUAppcastItem *)item
                         error:(NSError *)error {
    qDebug() << error;
    u->setStatus(Updater::Status::UpdateDownloadFailed);
}

- (void)updater:(SPUUpdater *)updater
                willInstallUpdateOnQuit:(SUAppcastItem *)item
        immediateInstallationInvocation:(NSInvocation *)invocation {
    Q_UNUSED(updater)
    qDebug() << u->getImmediateInstallAndRelaunch();
    if (u->getImmediateInstallAndRelaunch()) [invocation invoke];
}

- (void)updater:(SPUUpdater *)updater willInstallUpdate:(SUAppcastItem *)item {
    qDebug() << item.versionString;
}

- (void)updaterWillRelaunchApplication:(SPUUpdater *)updater {
    qDebug() << updater;
}

- (void)updater:(SPUUpdater *)updater didAbortWithError:(NSError *)error {
    qDebug() << error;
}

@end

namespace updater {

SparkleUpdater::SparkleUpdater() {
    SparkleDelegate *delegate = [[SparkleDelegate alloc] init];
    [delegate setUpdater:this];

    SPUStandardUpdaterController *updaterController = [SPUStandardUpdaterController alloc];
    [updaterController initWithStartingUpdater:YES updaterDelegate:delegate userDriverDelegate:Nil];

    updater = updaterController.updater;
    updater.automaticallyChecksForUpdates = YES;
    updater.automaticallyDownloadsUpdates = YES;
}

bool SparkleUpdater::getAutomaticDownload() const {
    return updater.automaticallyDownloadsUpdates;
}

void SparkleUpdater::setAutomaticDownload(bool value) {
    updater.automaticallyDownloadsUpdates = value;
}

void SparkleUpdater::checkAndShowUI() {
    [updater checkForUpdates];
}

void SparkleUpdater::checkAndMaybeShowUI() {
    [updater checkForUpdatesInBackground];
}

void SparkleUpdater::checkWithoutUI() {
    [updater checkForUpdateInformation];
}

void SparkleUpdater::update() {
    qDebug() << "Not implemented";
}

} // namespace updater
