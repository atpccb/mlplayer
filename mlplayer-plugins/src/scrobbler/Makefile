PLUGIN = scrobbler${PLUGIN_SUFFIX}

SRCS = fmt.c			\
       configure.c		\
       scrobbler.c		\
       plugin.c

include ../../buildsys.mk
include ../../extra.mk

PACKAGE = mlplayer

plugindir := ${plugindir}/${GENERAL_PLUGIN_DIR}

CFLAGS += ${PLUGIN_CFLAGS}
CPPFLAGS += ${PLUGIN_CPPFLAGS}  ${GTK_CFLAGS} ${GLIB_CFLAGS} ${CURL_CFLAGS} -I../..
LIBS += ${GTK_LIBS} ${GLIB_LIBS}  ${CURL_LIBS}
