PLUGIN = mms${PLUGIN_SUFFIX}

SRCS = mms.c

include ../../buildsys.mk
include ../../extra.mk

plugindir := ${plugindir}/${TRANSPORT_PLUGIN_DIR}

CPPFLAGS += ${PLUGIN_CPPFLAGS}  ${GTK_CFLAGS} ${GLIB_CFLAGS} ${MMS_CFLAGS} -I../.. -Wall
CFLAGS += ${PLUGIN_CFLAGS}
LIBS += ${GTK_LIBS} ${GLIB_LIBS} ${MMS_LIBS}
