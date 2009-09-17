CONFIG += ordered
TEMPLATE = subdirs

SUBDIRS += KDWallApp

!macx-xcode:addExclusiveBuilds(debug, Debug, release, Release)

