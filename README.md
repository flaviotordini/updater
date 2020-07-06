# An updater for Qt apps

This is an extensible updater for Qt apps. It can wrap Sparkle on macOS and use its own implementation on Windows and Linux. I use it in my apps at https://flavio.tordini.org .

## Design

The main interface is [Updater](https://github.com/flaviotordini/updater/blob/master/src/updater.h). A shared Updater subclass instance should be set on startup using `Updater::setInstance()`. Available implementations are:

- [`updater::DefaultUpdater`](https://github.com/flaviotordini/updater/blob/master/src/impl/defaultupdater.h), the default Qt-based implementation.

- [`updater::SparkleUpdater`](https://github.com/flaviotordini/updater/blob/master/src/sparkle/sparkleupdater.h), a Sparkle-based implementation for macOS

## User Interface

### Built-in Widgets

Updater provides ready-to-use widgets:

- `Updater::getAction()` returns a QAction suitable to be inserted in a QMenu.
- `Updater::getLabel()` returns a QLabel that automatically changes its message. Typically used in the about box.
- `Updater::getButton()` returns a QPushButton that autohides or automatically changes its function depending on the Updater status.

When the user triggers the action or pushes the button a dialog will show which is dependent on the Updater implementation.

## Entension Points

[updater::DefaultUpdater](https://github.com/flaviotordini/updater/blob/master/src/impl/defaultupdater.h) has a number of extension points so it can be adapted to different release manifest formats and update mechanisms.

### Parser

Implement [updater::Parser](https://github.com/flaviotordini/updater/blob/master/src/impl/parser.h) to parse your own manifest format. There two ready-to-use Parsers:

- TODO [updater::AppCastParser](https://github.com/flaviotordini/updater/blob/master/src/impl/appcastparser.h). This the AppCast format also used by Sparkle. It's a RSS feed with Sparkle extensions.
- [updater::SimpleXmlParser](https://github.com/flaviotordini/updater/blob/master/src/impl/simplexmlparser.h). This is a very simple XML format

### Installer

[updater::Installer](https://github.com/flaviotordini/updater/blob/master/src/impl/installer.h) is the abstraction responsible for preparing and running the update process. Currently the only available Installer implementation is [updater::RunInstaller](https://github.com/flaviotordini/updater/blob/master/src/impl/runinstaller.h). It just runs an executable update payload, optionally with arguments.

Installer can be implemented in other ways, for example an Installer that unzips a payload and moves files. Or one that invokes an update helper. Another idea is signature validation.

## Build Instructions

### qmake
```
mkdir build
cd build
qmake ..
make
```

## Integration

You can use this library as a git submodule. For example, add it to your project inside a lib subdirectory:

```
git submodule add -b master https://github.com/flaviotordini/updater lib/updater
```

Then you can update your git submodules like this:

```
git submodule update --init --recursive --remote
```

To integrate the library in your qmake based project just add this to your .pro file:

```
include(lib/updater/updater.pri)
```

qmake builds all object files in the same directory. In order to avoid filename clashes use:

```
CONFIG += object_parallel_to_source
```

## Examples

Example setup of the shared Updater instance:

```
#include "updater.h"
#ifdef UPDATER_SPARKLE
#include "sparkleupdater.h"
#else
#include "defaultupdater.h"
#endif

void setupUpdater() {
    #ifdef UPDATER_SPARKLE
        Updater::setInstance(new updater::SparkleUpdater());
    #else
        auto updater = new updater::DefaultUpdater();
        updater->setManifestUrl(myAppCastUrl);
        Updater::setInstance(updater);
    #endif
}
```

Updater provides a QAction instance ready to be used in a menu.

```
myMenu->addAction(Updater::instance().getAction());
```

In the About box you can use the standard widgets provided by Updater. A QLabel and a QPushButton.

```
myLayout->addWidget(Updater::instance().getLabel());
myLayout->addWidget(Updater::instance().getButton());
```

## Security

Always serve your updates via HTTPS.

## License

You can use this library under the GPLv3 license terms. If you do, you're welcome contributing your changes and fixes. Donations are welcome at https://flavio.tordini.org/donate

For commercial projects I ask for a one-time license fee, contact me at flavio.tordini@gmail.com

## TODO
- Appcast parser
- Finish README