# An updater for Qt apps

This is an extensible updater for Qt apps. It can wrap Sparkle on macOS and use its own implementation on Windows and Linux. I use it in my apps at https://flavio.tordini.org .

## Design

The main interface is [Updater](https://github.com/flaviotordini/updater/blob/master/src/updater.h). A shared Updater subclass instance should be set on startup using `Updater::setInstance()`. Available implementations are:

- [`updater::impl::Updater`](https://github.com/flaviotordini/updater/blob/master/src/impl/default_updater.h), the default Qt-based implementation.

- [`updater::sparkle::Updater`](https://github.com/flaviotordini/updater/blob/master/src/sparkle/sparkle_updater.h), a Sparkle-based implementation for macOS

## User Interface

### Dialog

### Built-in Widgets

- QAction
- QLabel
- QPushButton

## Entension Points

[updater::impl::Updater](https://github.com/flaviotordini/updater/blob/master/src/impl/default_updater.h) has a number of extension points so it can be adapted to different release manifest formats and update mechanisms.

### Parser

### Installer

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

A basic example:

```

```

This is a real-world example of setting up the shared Updater instance:

```
#include "updater.h"
#ifdef UPDATER_SPARKLE
#include "sparkle_updater.h"
#else
#include "default_updater.h"
#endif

#ifdef UPDATER_SPARKLE
    Updater::setInstance(new updater::sparkle::Updater());
#else
    auto updater = new updater::impl::Updater();
    updater->setManifestUrl(myAppCastUrl);
    Updater::setInstance(updater);
#endif

```

In the About box you can use the standard widgets provided by Updater. A QLabel and a QPushButton.

```

```

## License

You can use this library under the GPLv3 license terms. If you do, you're welcome contributing your changes and fixes. Donations are welcome at https://flavio.tordini.org/donate

For commercial projects I ask for a one-time license fee, contact me at flavio.tordini@gmail.com

## TODO
- RunInstaller: run process with arguments
- Appcast parser
- Finish README
- Finish Dialog UI