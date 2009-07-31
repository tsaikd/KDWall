# please include this file after other config

unix {
	CONFIG += debug_and_release
}

CONFIG(debug, debug|release) {
	win32 {
		TARGET = ../$$join(TARGET,,,d)
	} else {
		TARGET = $$join(TARGET,,,d)
	}
	CONFIG -= release
	CONFIG += debug
	DEFINES += DEBUG
	DEFINES -= NDEBUG
	DEFINES -= QT_NO_DEBUG_OUTPUT
} else {
	win32 {
		TARGET = ../$${TARGET}
	} else {
		TARGET = $${TARGET}
	}
	CONFIG -= debug
	CONFIG += release
	DEFINES -= DEBUG
	DEFINES += NDEBUG
	DEFINES += QT_NO_DEBUG_OUTPUT
}
