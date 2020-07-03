#include "sparkle_updater.h"

#import <SUAppcastItem.h>
#import <SUUpdater.h>
#import <SUUpdaterDelegate.h>

@interface SparkleDelegate : NSObject <SUUpdaterDelegate> {
}
@end

@implementation SparkleDelegate {
    Updater *u;
}

- (void)setUpdater:(Updater *)v {
    u = v;
}

- (void)updater:(SUUpdater *)updater didFindValidUpdate:(SUAppcastItem *)item {
    qDebug() << item.versionString;
    Q_UNUSED(updater)
    u->setVersion(QString::fromNSString(item.displayVersionString));
    u->setStatus(Updater::Status::UpdateAvailable);
    // [updater installUpdatesIfAvailable];
}

- (void)updaterDidNotFindUpdate:(SUUpdater *)updater {
    Q_UNUSED(updater)
    u->setStatus(Updater::Status::UpToDate);
}

- (void)updater:(SUUpdater *)updater
        willDownloadUpdate:(SUAppcastItem *)item
               withRequest:(NSMutableURLRequest *)request {
    qDebug() << item.versionString << request;
    Q_UNUSED(updater)
    u->setStatus(Updater::Status::DownloadingUpdate);
}

- (void)updater:(SUUpdater *)updater didDownloadUpdate:(SUAppcastItem *)item {
    qDebug() << item.versionString;
    Q_UNUSED(updater)
    u->setStatus(Updater::Status::UpdateDownloaded);
}

- (void)userDidCancelDownload:(SUUpdater *)updater {
    Q_UNUSED(updater)
    u->setStatus(Updater::Status::UpdateAvailable);
}

- (void)updater:(SUUpdater *)updater
        failedToDownloadUpdate:(SUAppcastItem *)item
                         error:(NSError *)error {
    qDebug() << error;
    u->setStatus(Updater::Status::UpdateDownloadFailed);
}

- (void)updater:(SUUpdater *)updater
                willInstallUpdateOnQuit:(SUAppcastItem *)item
        immediateInstallationInvocation:(NSInvocation *)invocation {
    Q_UNUSED(updater)
    qDebug() << u->getImmediateInstallAndRelaunch();
    if (u->getImmediateInstallAndRelaunch()) [invocation invoke];
}

- (void)updater:(SUUpdater *)updater willInstallUpdate:(SUAppcastItem *)item {
    qDebug() << item.versionString;
}

- (void)updaterWillRelaunchApplication:(SUUpdater *)updater {
    qDebug() << updater;
}

- (void)updater:(SUUpdater *)updater didAbortWithError:(NSError *)error {
    qDebug() << error;
}

@end

namespace updater::sparkle {

Updater::Updater() {
    SparkleDelegate *delegate = [[SparkleDelegate alloc] init];
    [delegate setUpdater:this];
    [[SUUpdater sharedUpdater] setDelegate:delegate];
    [SUUpdater sharedUpdater].automaticallyChecksForUpdates = YES;
    [SUUpdater sharedUpdater].automaticallyDownloadsUpdates = YES;
}

bool Updater::getAutomaticDownload() const {
    return [SUUpdater sharedUpdater].automaticallyDownloadsUpdates;
}

void Updater::setAutomaticDownload(bool value) {
    [SUUpdater sharedUpdater].automaticallyDownloadsUpdates = value;
}

void Updater::checkAndShowUI() {
    [[SUUpdater sharedUpdater] checkForUpdates:nil];
}

void Updater::checkAndMaybeShowUI() {
    [[SUUpdater sharedUpdater] checkForUpdatesInBackground];
}

void Updater::checkWithoutUI() {
    [[SUUpdater sharedUpdater] checkForUpdateInformation];
}

void Updater::update() {
    [[SUUpdater sharedUpdater] installUpdatesIfAvailable];
}

} // namespace updater::sparkle
