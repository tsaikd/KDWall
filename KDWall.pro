CONFIG += ordered
TEMPLATE = subdirs

QT_BUILD_PARTS += KDWallApp
SUBDIRS += KDWallApp

!macx-xcode:addExclusiveBuilds(debug, Debug, release, Release)

