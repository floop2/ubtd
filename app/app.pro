TEMPLATE = app
TARGET = app

load(ubuntu-click)

QT += qml quick dbus bluetooth
CONFIG += c++11

SOURCES += main.cpp \
    adapter.cpp \
    obexd.cpp \
    obexagent.cpp \
    obexagentadaptor.cpp \
    transfer.cpp

RESOURCES += app.qrc

OTHER_FILES += app.apparmor \
               app.desktop \
               icon.svg

#specify where the config files are installed to
config_files.path = /app
config_files.files += $${OTHER_FILES}
message($$config_files.files)
INSTALLS+=config_files

# Default rules for deployment.
target.path = $${UBUNTU_CLICK_BINARY_PATH}
INSTALLS+=target

HEADERS += \
    adapter.h \
    obexd.h \
    obexagent.h \
    obexagentadaptor.h \
    transfer.h


