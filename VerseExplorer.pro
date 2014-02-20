#-------------------------------------------------
#
# Project created by QtCreator 2012-02-20T13:29:47
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = VerseExplorer
TEMPLATE = app

# Verse staff
unix:!macx:!symbian: LIBS += -lssl -lverse

SOURCES += src/main.cpp\
    src/verse_node.cpp \
    src/verse_client.cpp \
    src/verse_model.cpp \
    src/login_dialog.cpp \
    src/nodepermdialog.cpp \
    src/nodeownerdialog.cpp \
    src/verse_taggroup.cpp \
    src/verse_tag.cpp \
    src/verse_data.cpp \
    src/verse_node_data_model.cpp \
    src/taggroupdialog.cpp \
    src/tagdialog.cpp \
    src/verse_data_model.cpp \
    src/fpsdialog.cpp \
    src/tagvaluedialog.cpp \
    src/nodedialog.cpp \
    src/verse_layer.cpp \
    src/layerdialog.cpp

INCLUDEPATH += $$PWD/include

HEADERS  += \
    include/verse_node.h \
    include/verse_client.h \
    include/verse_model.h \
    include/login_dialog.h \
    include/nodepermdialog.h \
    include/nodeownerdialog.h \
    include/verse_taggroup.h \
    include/verse_tag.h \
    include/verse_data.h \
    include/verse_node_data_model.h \
    include/taggroupdialog.h \
    include/tagdialog.h \
    include/verse_data_model.h \
    include/fpsdialog.h \
    include/tagvaluedialog.h \
    include/nodedialog.h \
    include/verse_layer.h \
    include/layerdialog.h

FORMS    += ui/verseclient.ui \
    ui/logindialog.ui \
    ui/nodepermdialog.ui \
    ui/nodeownerdialog.ui \
    ui/taggroupdialog.ui \
    ui/tagdialog.ui \
    ui/fpsdialog.ui \
    ui/tagvaluedialog.ui \
    ui/nodedialog.ui \
    ui/layerdialog.ui

RESOURCES += \
    res/icons.qrc

OTHER_FILES += \
    icons/folder.png \
    icons/list-remove.png \
    icons/list-add.png \
    icons/user-home.png \
    icons/user-desktop.png \
    icons/system-users.png \
    icons/folder-visiting.png \
    icons/face-smile.png \
    icons/face-monkey.png \
    icons/folder-open.png \
    icons/tab-new.png \
    icons/emblem-unreadable.png \
    icons/emblem-readonly.png \
    icons/folder-new.png \
    icons/face-monkey_unsubscribed.png \
    icons/face-smile_unsubscribed.png \
    icons/folder-open_unsubscribed.png \
    icons/folder_unsubscribed.png \
    icons/system-users_unsubscribed.png \
    icons/user-home_unsubscribed.png \
    icons/user-desktop_unsubscribed.png \
    icons/emblem-unknown.png \
    icons/emblem-locked.png \
    icons/emblem-default.png \
    icons/emblem-locked-me.png \
    icons/unlock.png \
    icons/network-server_unsubscribed.png \
    icons/network-server.png \
    icons/lock.png \
    icons/tag-group.png \
    icons/emblem-system.png
