QT += core gui widgets sql

CONFIG += c++17

TARGET = product_sales
TEMPLATE = app

HEADERS += \
    model/models.h \
    model/database.h \
    controller/authcontroller.h \
    controller/crudcontroller.h \
    view/loginwindow.h \
    view/mainwindow.h \
    view/editdialog.h \
    view/documentdialog.h \
    view/profiledialog.h

SOURCES += \
    main.cpp \
    model/database.cpp \
    controller/authcontroller.cpp \
    controller/crudcontroller.cpp \
    view/loginwindow.cpp \
    view/mainwindow.cpp \
    view/editdialog.cpp \
    view/documentdialog.cpp \
    view/profiledialog.cpp
