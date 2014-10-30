HEADERS       = androidscreencast.h \
    controlthread.h
SOURCES       = androidscreencast.cpp \
                main.cpp \
    controlthread.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/androidscreencast
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS androidscreencast.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/widgets/androidscreencast
INSTALLS += target sources

symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)

#Symbian has built-in component named androidscreencast so we use different target
symbian: TARGET = androidscreencastexample

wince*: {
   DEPLOYMENT_PLUGIN += qjpeg qmng qgif
}
maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)

symbian: warning(This example might not fully work on Symbian platform)
maemo5: warning(This example might not fully work on Maemo platform)
simulator: warning(This example might not fully work on Simulator platform)
