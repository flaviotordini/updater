#include "dialog.h"

#include "downloader.h"

namespace {
static const int padding = 15;
} // namespace

namespace updater {

DownloadWidget::DownloadWidget(const QString &message, QDialog *parent)
    : QWidget(parent) {
    QBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(padding);

    QLabel *msgLabel = new QLabel(message);
    QFont boldFont = msgLabel->font();
    boldFont.setBold(true);
    msgLabel->setFont(boldFont);
    layout->addWidget(msgLabel);

    progressBar = new QProgressBar();
    layout->addWidget(progressBar);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

    QPushButton *cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked, this, [parent] { parent->reject(); });

    QPushButton *closeButton = buttonBox->addButton(QDialogButtonBox::Ok);
    closeButton->setDefault(true);
    closeButton->setFocus();
    closeButton->connect(closeButton, &QPushButton::clicked, this, [parent] { parent->accept(); });

    layout->addWidget(buttonBox);

    errorLabel = new QLabel();
    errorLabel->hide();
    layout->addWidget(errorLabel);
}

void DownloadWidget::setErrorMessage(const QString &message) {
    errorLabel->setText(message);
    errorLabel->show();
}

Dialog::Dialog(DefaultUpdater *updater, QWidget *parent)
    : QDialog(parent), updater(updater), downloadWidget(nullptr) {
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setSizeConstraint(QLayout::SetFixedSize);
    hLayout->setMargin(padding * 2);
    hLayout->setSpacing(padding);

    if (!updater->getIcon().isNull()) {
        QLabel *logo = new QLabel();
        logo->setPixmap(updater->getIcon().pixmap(64, 64));
        hLayout->addWidget(logo, 0, Qt::AlignTop);
    }

    stackedLayout = new QStackedLayout();
    hLayout->addLayout(stackedLayout, 1);

    auto onStatusChange = [this](Updater::Status status) {
        QWidget *w = nullptr;
        switch (status) {
        case Updater::Status::UpToDate:
            w = new QLabel(tr("You already have the latest version"));
            break;
        case Updater::Status::UpdateAvailable:
            w = messageWidget();
            break;

        case Updater::Status::DownloadingUpdate:
        case Updater::Status::UpdateDownloaded:
        case Updater::Status::UpdateDownloadFailed:
            w = setupDownloadWidget();
            break;
        }
        if (w) showWidget(w);
    };
    connect(updater, &Updater::statusChanged, this, onStatusChange);
    onStatusChange(updater->getStatus());
}

QWidget *Dialog::setupDownloadWidget() {
    if (!downloadWidget) {
        QString message =
                tr("Downloading %1 %2...").arg(updater->getAppName(), updater->getVersion());
        downloadWidget = new DownloadWidget(message, this);
    }

    Downloader *downloader = updater->getDownloader();
    if (downloader) {
        connect(downloader, &Downloader::progress, downloadWidget, &DownloadWidget::setProgress);
        connect(downloader, &Downloader::error, downloadWidget, &DownloadWidget::setErrorMessage);
        // connect(downloader, &Downloader::fileReady, downloadWidget,
        // &DownloadWidget::setProgress);
        connect(this, &QDialog::rejected, downloader, &Downloader::stop);
    }

    return downloadWidget;
}

void Dialog::showWidget(QWidget *widget) {
    QWidget *currentWidget = stackedLayout->currentWidget();
    if (currentWidget) {
        stackedLayout->removeWidget(currentWidget);
        currentWidget->deleteLater();
    }
    stackedLayout->addWidget(widget);
}

QWidget *Dialog::messageWidget() {
    QWidget *w = new QWidget();

    QFormLayout *layout = new QFormLayout(w);
    layout->setMargin(0);
    layout->setSpacing(padding);

    QLabel *titleLabel =
            new QLabel(tr("A new version of %1 is available!").arg(updater->getAppName()));
    QFont boldFont = titleLabel->font();
    boldFont.setBold(true);
    titleLabel->setFont(boldFont);
    layout->addWidget(titleLabel);

    QLabel *label = new QLabel(
            tr("%1 %2 is now available. You have %3.")
                    .arg(updater->getAppName(), updater->getVersion(), updater->getLocalVersion()));
    layout->addWidget(label);

    label = new QLabel(tr("Would you like to download it now?"));
    layout->addWidget(label);

    QDialogButtonBox *buttonBox = new QDialogButtonBox();

    QPushButton *laterButton =
            buttonBox->addButton(tr("Remind Me Later"), QDialogButtonBox::RejectRole);
    connect(laterButton, &QPushButton::clicked, this, &QDialog::close);

    QPushButton *updateButton =
            buttonBox->addButton(tr("Download Update"), QDialogButtonBox::AcceptRole);
    connect(updateButton, &QPushButton::clicked, updater, &DefaultUpdater::downloadUpdate);

    layout->addWidget(buttonBox);

    return w;
}

} // namespace updater
