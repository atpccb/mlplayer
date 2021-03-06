/*
 * MLPlayer: Cross platform multimedia player
 * Copyright (c) 2013 MLPlayer Team
 * Copyright (c) 2009 Audacious Team
 *
 * Driver for Game_Music_Emu library. See details at:
 * http://www.slack.net/~ant/libs/
 */


#include "config.h"
#include <glib.h>
#include <mlplayer/i18n.h>
#include <mlplayer/plugin.h>
#include <libmlpgui/libmlpgui.h>
#include <libmlpgui/libmlpgui-gtk.h>

#include "configure.h"

Tuple * console_probe_for_tuple(const gchar *filename, VFSFile *fd);
gboolean console_play(InputPlayback *playback, const gchar *filename,
    VFSFile *file, gint start_time, gint stop_time, gboolean pause);
void console_seek(InputPlayback *data, gint time);
void console_stop(InputPlayback *playback);
void console_pause(InputPlayback * playback, gboolean pause);
gboolean console_init (void);
void console_cleanup(void);

static void console_aboutbox (void)
{
    static GtkWidget * aboutbox = NULL;

    mlpgui_simple_message (& aboutbox, GTK_MESSAGE_INFO,
     _("About the Game Console Music Decoder"),
     _("Console music decoder engine based on Game_Music_Emu 0.5.2.\n"
     "Supported formats: AY, GBS, GYM, HES, KSS, NSF, NSFE, SAP, SPC, VGM, VGZ\n"
     "MLPlayer implementation by: William Pitcock <nenolod@dereferenced.org>, \n"
     "        Shay Green <gblargg@gmail.com>\n"));
}

static const gchar *gme_fmts[] = {
    "ay", "gbs", "gym",
    "hes", "kss", "nsf",
    "nsfe", "sap", "spc",
    "vgm", "vgz", NULL
};

AUD_INPUT_PLUGIN
(
    .name = "Game Console Music Decoder",
    .init = console_init,
    .cleanup = console_cleanup,
    .about = console_aboutbox,
    .configure = console_cfg_ui,
    .play = console_play,
    .stop = console_stop,
    .pause = console_pause,
    .mseek = console_seek,
    .extensions = gme_fmts,
    .probe_for_tuple = console_probe_for_tuple,
    .have_subtune = TRUE
)
