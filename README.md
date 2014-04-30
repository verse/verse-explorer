Verse Explorer
==============

[![Coverity Scan Build Status](https://scan.coverity.com/projects/2127/badge.svg)](https://scan.coverity.com/projects/2127)

Verse Explorer is Qt based Verse client used for managing shared data. It can
be used for exploring sharing data. It can be used for basic managing of
shared data. It means that it is possible to create, modify and delete shared
nodes, tag groups, tags, layers, etc.

![Verse Explorer screenshot](/images/screenshot.png "Verse Explorer screenshot")

Compile
-------

This program is tested only at Linux OS, because Verse library is fully supported
only at Linux OS at this time. To compile Verse Explorer you can use QtCreator or
you can compile it using following commands in terminal:

    mkdir build
    qmake-qt4 VerseExplorer.pro -r -spec linux-g++-64 CONFIG+=debug -o ./build/Makefile
    cd ./build
    make

Requirements
------------

This program requires Qt library and Verse library that could be found at:

* http://qt-project.org/
* https://github.com/verse/verse

