# please include this file after other config

unix {
	CONFIG *= debug_and_release
}

OS = unknown
win32 { OS = win32 }
unix  { OS = unix  }
mac   { OS = mac   }

CONFIG(debug, debug|release) {
	win32 {
		TARGET = ../$$join(TARGET,,,d)
	} else {
		TARGET = $$join(TARGET,,,d)
	}
	BUILD		= debug
	CONFIG		*= debug
	CONFIG		-= release
	DEFINES		*= DEBUG
	DEFINES		-= NDEBUG
	DEFINES		-= QT_NO_DEBUG_OUTPUT
} else {
	win32 {
		TARGET = ../$${TARGET}
	} else {
		TARGET = $${TARGET}
	}
	BUILD		= release
	CONFIG		-= debug
	CONFIG		*= release
	DEFINES		-= DEBUG
	DEFINES		*= NDEBUG
	DEFINES		*= QT_NO_DEBUG_OUTPUT
}

isEmpty(OBJECTS_DIR)	{ OBJECTS_DIR	= tmp/$${OS}/$${BUILD} }
isEmpty(MOC_DIR)		{ MOC_DIR		= tmp/$${OS}/$${BUILD} }
isEmpty(RCC_DIR)		{ RCC_DIR		= tmp/$${OS}/$${BUILD} }
