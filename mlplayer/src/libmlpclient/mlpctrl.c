/*
 * MLPlayer: A cross-platform multimedia player
 * Copyright (c) 2007 Ben Tucker
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; under version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <dbus/dbus-glib.h>
#include "mlplayer/dbus.h"
#include "mlplayer/dbus-client-bindings.h"
#include "mlpctrl.h"
#include "config.h"

static GError *error = NULL; //it must be hidden from outside, otherwise symbol conflict likely to happen.

/**
 * Sends a list of URIs to MLPlayer, either replacing current
 * playlist or enqueuing them.
 *
 * @param[in] proxy DBus proxy for MLPlayer
 * @param[in] list An array of URI strings to add.
 * @param[in] num Number of URIs to add.
 * @param[in] enqueue Whether or not the new playlist should be added on, or replace the current playlist.
 **/
EXPORT void mlplayer_remote_playlist(DBusGProxy *proxy, gchar **list, gint num, gboolean enqueue) {
    GList *glist = NULL;
    gchar **data = list;

    g_return_if_fail(list != NULL);
    g_return_if_fail(num > 0);

    if (!enqueue)
        mlplayer_remote_playlist_clear(proxy);

    // construct a GList
    while(data) {
        glist = g_list_append(glist, (gpointer)data);
        data++;
    }

    org_atheme_mlplayer_playlist_add(proxy, (gpointer)glist, &error);

    g_list_free(glist);
    glist = NULL;

    if (!enqueue)
        mlplayer_remote_play(proxy);
}

/**
 * Queries MLPlayer for its version identifier.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return String describing the version of MLPlayer.
 **/
EXPORT gchar *mlplayer_remote_get_version(DBusGProxy *proxy) {
    char *string = NULL;
    org_atheme_mlplayer_version(proxy, &string, &error);
    g_clear_error(&error);

    return (string ? string : NULL);
}

/**
 * Sends a list of URIs to MLPlayer to add to the playlist.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] list A GList of URIs to add to the playlist.
 **/
EXPORT void mlplayer_remote_playlist_add (DBusGProxy * proxy, GList * list)
{
    const gchar * filenames[g_list_length (list) + 1];
    int count;

    for (count = 0; list != NULL; count ++, list = list->next)
        filenames[count] = list->data;

    filenames[count] = NULL;

    org_atheme_mlplayer_add_list (proxy, filenames, & error);
    g_clear_error (& error);
}

/**
 * Sends a list of URIs for MLPlayer to open.  New in MLPlayer 2.3.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] list A GList of URIs to open
 **/
EXPORT void mlplayer_remote_playlist_open_list (DBusGProxy * proxy, GList * list)
{
    const gchar * filenames[g_list_length (list) + 1];
    int count;

    for (count = 0; list != NULL; count ++, list = list->next)
        filenames[count] = list->data;

    filenames[count] = NULL;

    org_atheme_mlplayer_open_list (proxy, filenames, & error);
    g_clear_error (& error);
}

/**
 * Sends a list of URIs for MLPlayer to open in a temporary playlist.  New in
 * MLPlayer 2.3.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] list A GList of URIs to open
 **/
EXPORT void mlplayer_remote_playlist_open_list_to_temp (DBusGProxy * proxy, GList *
 list)
{
    const gchar * filenames[g_list_length (list) + 1];
    int count;

    for (count = 0; list != NULL; count ++, list = list->next)
        filenames[count] = list->data;

    filenames[count] = NULL;

    org_atheme_mlplayer_open_list_to_temp (proxy, filenames, & error);
    g_clear_error (& error);
}

/**
 * Deletes a playlist entry from current playlist in given position.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos The playlist position to delete.
 **/
EXPORT void mlplayer_remote_playlist_delete(DBusGProxy *proxy, guint pos) {
    org_atheme_mlplayer_delete(proxy, pos, &error);
    g_clear_error(&error);
}

/**
 * Requests mlplayer to begin playback.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_play(DBusGProxy *proxy) {
    org_atheme_mlplayer_play(proxy, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to pause.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_pause(DBusGProxy *proxy) {
    org_atheme_mlplayer_pause(proxy, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to stop.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_stop(DBusGProxy *proxy) {
    org_atheme_mlplayer_stop(proxy, &error);
    g_clear_error(&error);
}

/**
 * Queries mlplayer about whether it is playing or not.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return TRUE if playing, FALSE otherwise.
 **/
EXPORT gboolean mlplayer_remote_is_playing(DBusGProxy *proxy) {
    gboolean is_playing = FALSE;
    org_atheme_mlplayer_playing(proxy, &is_playing, &error);
    g_clear_error(&error);
    return is_playing;
}

/**
 * mlplayer_remote_is_paused:
 * @param[in] proxy DBus proxy for mlplayer
 *
 * Queries mlplayer about whether it is paused or not.
 *
 * @return TRUE if playing, FALSE otherwise.
 **/
EXPORT gboolean mlplayer_remote_is_paused(DBusGProxy *proxy) {
    gboolean is_paused = FALSE;
    org_atheme_mlplayer_paused(proxy, &is_paused, &error);
    g_clear_error(&error);
    return is_paused;
}

/**
 * mlplayer_remote_get_playlist_pos:
 * @param[in] proxy DBus proxy for mlplayer
 *
 * Queries mlplayer about the current playlist position.
 *
 * @return The current playlist position.
 **/
EXPORT gint mlplayer_remote_get_playlist_pos(DBusGProxy *proxy) {
    guint pos = 0;
    org_atheme_mlplayer_position(proxy, &pos, &error);
    g_clear_error(&error);
    return pos;
}

/**
 * mlplayer_remote_set_playlist_pos:
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos Playlist position to jump to.
 *
 * Tells mlplayer to jump to a different playlist position.
 **/
EXPORT void mlplayer_remote_set_playlist_pos(DBusGProxy *proxy, guint pos) {
    org_atheme_mlplayer_jump (proxy, pos, &error);
    g_clear_error(&error);
}

/**
 * mlplayer_remote_get_playlist_length:
 * @param[in] proxy DBus proxy for mlplayer
 *
 * Queries mlplayer about the current playlist length.
 *
 * @return The amount of entries in the playlist.
 **/
EXPORT gint mlplayer_remote_get_playlist_length(DBusGProxy *proxy) {
    gint len = 0;
    org_atheme_mlplayer_length(proxy, &len, &error);
    g_clear_error(&error);
    return len;
}

/**
 * mlplayer_remote_playlist_clear:
 * @param[in] proxy DBus proxy for mlplayer
 *
 * Clears the playlist.
 **/
EXPORT void mlplayer_remote_playlist_clear(DBusGProxy *proxy) {
    org_atheme_mlplayer_clear(proxy, &error);
    g_clear_error(&error);
}

/**
 * mlplayer_remote_get_output_time:
 * @param[in] proxy DBus proxy for mlplayer
 *
 * Queries mlplayer about the current output position.
 *
 * @return The current output position.
 **/
EXPORT gint mlplayer_remote_get_output_time(DBusGProxy *proxy) {
    guint time = 0;
    org_atheme_mlplayer_time(proxy, &time, &error);
    g_clear_error(&error);
    return time;
}

/**
 * mlplayer_remote_jump_to_time:
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos The time (in milliseconds) to jump to.
 *
 * Tells mlplayer to seek to a new time position.
 **/
EXPORT void mlplayer_remote_jump_to_time(DBusGProxy *proxy, guint pos) {
    org_atheme_mlplayer_seek (proxy, pos, &error);
    g_clear_error(&error);
}

/**
 * Queries mlplayer for the current volume settings.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[out] vl Pointer to integer containing the left channel's volume (0-100).
 * @param[out] vr Pointer to integer containing the right channel's volume (0-100).
 **/
EXPORT void mlplayer_remote_get_volume(DBusGProxy *proxy, gint * vl, gint * vr) {
    org_atheme_mlplayer_volume(proxy, vl, vr, &error);
    g_clear_error(&error);
}

/**
 * Queries mlplayer about the current volume.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return The current volume (0-100).
 **/
EXPORT gint mlplayer_remote_get_main_volume(DBusGProxy *proxy) {
    gint vl = 0, vr = 0;

    mlplayer_remote_get_volume(proxy, &vl, &vr);

    return (vl > vr) ? vl : vr;
}

/**
 * Queries mlplayer about the current balance.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return The current balance.
 **/
EXPORT gint mlplayer_remote_get_balance(DBusGProxy *proxy) {
    gint balance = 50;
    org_atheme_mlplayer_balance(proxy, &balance,  &error);
    g_clear_error(&error);
    return balance;
}

/**
 * Sets the volume for the left and right channels in MLPlayer.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] vl The volume for the left channel (0-100).
 * @param[in] vr The volume for the right channel (0-100).
 **/
EXPORT void mlplayer_remote_set_volume(DBusGProxy *proxy, gint vl, gint vr) {
    org_atheme_mlplayer_set_volume(proxy, vl, vr,  &error);
    g_clear_error(&error);
}


/**
 * Sets the volume in MLPlayer.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] v The volume to set (0-100).
 **/
EXPORT void mlplayer_remote_set_main_volume(DBusGProxy *proxy, gint v) {
    gint b = 50, vl = 0, vr = 0;

    b = mlplayer_remote_get_balance(proxy);

    if (b < 0) {
        vl = v;
        vr = (v * (100 - abs(b))) / 100;
    } else if (b > 0) {
        vl = (v * (100 - b)) / 100;
        vr = v;
    } else
        vl = vr = v;
    mlplayer_remote_set_volume(proxy, vl, vr);
}

/**
 * Sets the balance in MLPlayer.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] b The balance to set.
 **/
EXPORT void mlplayer_remote_set_balance(DBusGProxy *proxy, gint b) {
    gint v = 0, vl = 0, vr = 0;

    if (b < -100)
        b = -100;
    if (b > 100)
        b = 100;

    v = mlplayer_remote_get_main_volume(proxy);

    if (b < 0) {
        vl = v;
        vr = (v * (100 - abs(b))) / 100;
    } else if (b > 0) {
        vl = (v * (100 - b)) / 100;
        vr = v;
    } else
        vl = vr = v;
    mlplayer_remote_set_volume(proxy, vl, vr);
}

/**
 * Queries MLPlayer about a playlist entry's file.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos The playlist position to query for.
 * @return A path to the file in the playlist at %pos position.
 **/
EXPORT gchar *mlplayer_remote_get_playlist_file(DBusGProxy *proxy, guint pos) {
    gchar *out = NULL;
    org_atheme_mlplayer_song_filename(proxy, pos, &out, &error);
    g_clear_error(&error);
    return out;
}

/**
 * Queries MLPlayer about a playlist entry's title.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos The playlist position to query for.
 * @return The title for the entry in the playlist at %pos position.
 **/
EXPORT gchar *mlplayer_remote_get_playlist_title(DBusGProxy *proxy, guint pos) {
    gchar *out = NULL;
    org_atheme_mlplayer_song_title(proxy, pos, &out, &error);
    g_clear_error(&error);
    return out;
}

/**
 * Queries MLPlayer about a playlist entry's length.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos The playlist position to query for.
 * @return The length of the entry in the playlist at %pos position.
 **/
EXPORT gint mlplayer_remote_get_playlist_time(DBusGProxy *proxy, guint pos) {
    gint out = 0;
    org_atheme_mlplayer_song_frames(proxy, pos, &out, &error);
    g_clear_error(&error);
    return out;
}

/**
 * Queries MLPlayer about the current audio format.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[out] rate Pointer to an integer containing the bitrate.
 * @param[out] freq Pointer to an integer containing the frequency.
 * @param[out] nch Pointer to an integer containing the number of channels.
 **/
EXPORT void mlplayer_remote_get_info(DBusGProxy *proxy, gint *rate, gint *freq,
                               gint *nch) {
    org_atheme_mlplayer_info(proxy, rate, freq, nch, &error);
    g_clear_error(&error);
}

/**
 * Toggles the main window's visibility.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] show Whether or not to show the main window.
 **/
EXPORT void mlplayer_remote_main_win_toggle(DBusGProxy *proxy, gboolean show) {
    org_atheme_mlplayer_show_main_win(proxy, show, &error);
    g_clear_error(&error);
}

/**
 * Queries MLPlayer about the main window's visibility.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return TRUE if visible, FALSE otherwise.
 **/
EXPORT gboolean mlplayer_remote_is_main_win(DBusGProxy *proxy) {
    gboolean visible = TRUE;
    org_atheme_mlplayer_main_win_visible(proxy, &visible, &error);
    g_clear_error(&error);
    return visible;
}

/**
 * Tells mlplayer to show the preferences pane.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_show_prefs_box(DBusGProxy *proxy) {
    mlplayer_remote_toggle_prefs_box(proxy, TRUE);
}

/**
 * Tells mlplayer to show/hide the preferences pane.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] show shows/hides
 **/
EXPORT void mlplayer_remote_toggle_prefs_box(DBusGProxy *proxy, gboolean show) {
    org_atheme_mlplayer_show_prefs_box(proxy, show, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to show the about box.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_show_about_box(DBusGProxy *proxy) {
    mlplayer_remote_toggle_about_box(proxy, TRUE);
}

/**
 * Tells mlplayer to show/hide the about box.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] show shows/hides
 **/
EXPORT void mlplayer_remote_toggle_about_box(DBusGProxy *proxy, gboolean show) {
    org_atheme_mlplayer_show_about_box(proxy, show, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to set the always-on-top feature.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] ontop Boolean value whether or not MLPlayer should be always-on-top.
 **/
EXPORT void mlplayer_remote_toggle_aot(DBusGProxy *proxy, gboolean ontop) {
    org_atheme_mlplayer_toggle_aot(proxy, ontop, &error);
	g_clear_error(&error);
}

/**
 * Tells mlplayer to display the open files pane.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_eject(DBusGProxy *proxy) {
    org_atheme_mlplayer_eject(proxy, &error);
    g_clear_error(&error);
}

/**
 * mlplayer_remote_playlist_prev:
 * @param[in] proxy DBus proxy for mlplayer
 *
 * Tells mlplayer to move backwards in the playlist.
 **/
EXPORT void mlplayer_remote_playlist_prev(DBusGProxy *proxy) {
    org_atheme_mlplayer_reverse(proxy, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to move forward in the playlist.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_playlist_next(DBusGProxy *proxy) {
    org_atheme_mlplayer_advance(proxy, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to add an URI to the playlist.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] string The URI to add.
 **/
EXPORT void mlplayer_remote_playlist_add_url_string(DBusGProxy *proxy,
                                              gchar *string) {
    org_atheme_mlplayer_add_url(proxy, string, &error);
    g_clear_error(&error);
}

/**
 * Check if an MLPlayer instance is running.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return TRUE if yes, otherwise FALSE.
 **/
EXPORT gboolean mlplayer_remote_is_running(DBusGProxy *proxy) {
    char *string = NULL;
    org_atheme_mlplayer_version(proxy, &string, &error);
    g_clear_error(&error);
    if(string) {
        g_free(string);
        return TRUE;
    }
    else
        return FALSE;
}

/**
 * Tells mlplayer to toggle the repeat feature.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_toggle_repeat(DBusGProxy *proxy) {
    org_atheme_mlplayer_toggle_repeat(proxy, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to toggle the shuffle feature.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_toggle_shuffle(DBusGProxy *proxy) {
    org_atheme_mlplayer_toggle_shuffle (proxy, &error);
    g_clear_error(&error);
}

EXPORT void mlplayer_remote_toggle_stop_after (DBusGProxy * proxy)
{
    org_atheme_mlplayer_toggle_stop_after (proxy, & error);
    g_clear_error (& error);
}

/**
 * Queries mlplayer about whether or not the repeat feature is active.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return TRUE if yes, otherwise FALSE.
 **/
EXPORT gboolean mlplayer_remote_is_repeat(DBusGProxy *proxy) {
    gboolean is_repeat;
    org_atheme_mlplayer_repeat(proxy, &is_repeat, &error);
    g_clear_error(&error);
    return is_repeat;
}

/**
 * Queries mlplayer about whether or not the shuffle feature is active.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return TRUE if yes, otherwise FALSE.
 **/
EXPORT gboolean mlplayer_remote_is_shuffle(DBusGProxy *proxy) {
    gboolean is_shuffle;
    org_atheme_mlplayer_shuffle(proxy, &is_shuffle, &error);
    g_clear_error(&error);
    return is_shuffle;
}

EXPORT gboolean mlplayer_remote_is_stop_after (DBusGProxy * proxy)
{
    gboolean is_stop_after;
    org_atheme_mlplayer_stop_after (proxy, & is_stop_after, & error);
    g_clear_error (& error);
    return is_stop_after;
}

/**
 * Queries mlplayer about the equalizer settings.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[out] preamp Pointer to value for preamp setting.
 * @param[out] bands Pointer to array of band settings.
 **/
EXPORT void mlplayer_remote_get_eq(DBusGProxy *proxy, gdouble *preamp, GArray **bands) {
    org_atheme_mlplayer_get_eq(proxy, preamp, bands, &error);
    g_clear_error(&error);
}

/**
 * Queries mlplayer about the equalizer preamp's setting.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return The equalizer preamp's setting.
 **/
EXPORT gdouble mlplayer_remote_get_eq_preamp(DBusGProxy *proxy) {
    gdouble preamp = 0.0;

    org_atheme_mlplayer_get_eq_preamp(proxy, &preamp, &error);
    g_clear_error(&error);

    return preamp;
}

/**
 * Queries mlplayer about an equalizer band's value.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] band Which band to lookup the value for.
 * @return The equalizer band's value.
 **/
EXPORT gdouble mlplayer_remote_get_eq_band(DBusGProxy *proxy, gint band) {
    gdouble value = 0.0;

    org_atheme_mlplayer_get_eq_band(proxy, band, &value, &error);
    g_clear_error(&error);

    return value;
}

/**
 * Tells mlplayer to set the equalizer up using the provided values.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] preamp Value for preamp setting.
 * @param[in] bands Array of band settings.
 **/
EXPORT void mlplayer_remote_set_eq(DBusGProxy *proxy, gdouble preamp, GArray *bands) {
    org_atheme_mlplayer_set_eq(proxy, preamp, bands, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to set the equalizer's preamp setting.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] preamp Value for preamp setting.
 **/
EXPORT void mlplayer_remote_set_eq_preamp(DBusGProxy *proxy, gdouble preamp) {
    org_atheme_mlplayer_set_eq_preamp(proxy, preamp, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to set an equalizer band's setting.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] band The band to set the value for.
 * @param[in] value The value to set that band to.
 **/
EXPORT void mlplayer_remote_set_eq_band(DBusGProxy *proxy, gint band, gdouble value) {
    org_atheme_mlplayer_set_eq_band(proxy, band, value, &error);
    g_clear_error(&error);
}

/**
 * Requests mlplayer to quit.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_quit(DBusGProxy *proxy) {
    org_atheme_mlplayer_quit(proxy, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to toggle between play and pause.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_play_pause(DBusGProxy *proxy) {
    org_atheme_mlplayer_play_pause(proxy, &error);
}

/**
 * Tells mlplayer to add an URI to the playlist at a specific position.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] string The URI to add.
 * @param[in] pos The position to add the URI at.
 **/
EXPORT void mlplayer_remote_playlist_ins_url_string(DBusGProxy *proxy,
                                              gchar *string, guint pos) {
    org_atheme_mlplayer_playlist_ins_url_string (proxy, string, pos, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to add a playlist entry to the playqueue.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos The playlist position to add to the queue.
 **/
EXPORT void mlplayer_remote_playqueue_add(DBusGProxy *proxy, guint pos) {
    org_atheme_mlplayer_playqueue_add (proxy, pos, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to remove a playlist entry from the playqueue.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos The playlist position to remove from the queue.
 **/
EXPORT void mlplayer_remote_playqueue_remove(DBusGProxy *proxy, guint pos) {
    org_atheme_mlplayer_playqueue_remove (proxy, pos, &error);
    g_clear_error(&error);
}

/**
 * Queries mlplayer about the playqueue's length.
 *
 * @bug This returns the length of the playlist, NOT the length of the playqueue.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return The number of entries in the playqueue.
 **/
EXPORT gint mlplayer_remote_get_playqueue_length(DBusGProxy *proxy) {
    gint len = 0;
    org_atheme_mlplayer_length(proxy, &len, &error);
    g_clear_error(&error);
    return len;
}

/**
 * Tells mlplayer to toggle the no-playlist-advance feature.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_toggle_advance(DBusGProxy *proxy) {
    org_atheme_mlplayer_toggle_auto_advance(proxy, &error);
    g_clear_error(&error);
}

/**
 * mlplayer_remote_is_advance:
 * @param[in] proxy DBus proxy for mlplayer
 *
 * Queries mlplayer about whether or not the no-playlist-advance feature is active.
 *
 * @return TRUE if yes, otherwise FALSE.
 **/
EXPORT gboolean mlplayer_remote_is_advance(DBusGProxy *proxy) {
    gboolean is_advance = FALSE;
    org_atheme_mlplayer_auto_advance(proxy, &is_advance, &error);
    g_clear_error(&error);
    return is_advance;
}

/**
 * Tells mlplayer to show the Jump-to-File pane.
 *
 * @param[in] proxy DBus proxy for mlplayer
 **/
EXPORT void mlplayer_remote_show_jtf_box(DBusGProxy *proxy) {
    mlplayer_remote_toggle_jtf_box(proxy, TRUE);
}

/**
 * Tells mlplayer to show/hide the Jump-to-File pane.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] show shows/hides jtf pane
 **/
EXPORT void mlplayer_remote_toggle_jtf_box(DBusGProxy *proxy, gboolean show) {
    org_atheme_mlplayer_show_jtf_box(proxy, show, &error);
    g_clear_error(&error);
}

/**
 * Tells mlplayer to show the filebrowser dialog.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] show shows/hides filebrowser
 **/
EXPORT void mlplayer_remote_toggle_filebrowser(DBusGProxy *proxy, gboolean show) {
    org_atheme_mlplayer_show_filebrowser(proxy, show, &error);
    g_clear_error(&error);
}

/**
 * mlplayer_remote_playqueue_clear:
 * @param[in] proxy DBus proxy for mlplayer
 *
 * Tells mlplayer to clear the playqueue.
 **/
EXPORT void mlplayer_remote_playqueue_clear(DBusGProxy *proxy) {
    org_atheme_mlplayer_playqueue_clear(proxy, &error);
    g_clear_error(&error);
}

/**
 * Queries mlplayer about whether or not a playlist entry is in the playqueue.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos Position to check queue for.
 * @return TRUE if yes, FALSE otherwise.
 **/
EXPORT gboolean mlplayer_remote_playqueue_is_queued(DBusGProxy *proxy, guint pos) {
    gboolean is_queued;
    org_atheme_mlplayer_playqueue_is_queued (proxy, pos, &is_queued, &error);
    g_clear_error(&error);
    return is_queued;
}

/**
 * Queries mlplayer about what the playqueue position is for a playlist entry.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] pos Position to check queue for.
 * @return the playqueue position for a playlist entry
 **/
EXPORT gint mlplayer_remote_get_playqueue_queue_position(DBusGProxy *proxy, guint pos) {
    guint qpos = 0;
    org_atheme_mlplayer_queue_get_queue_pos (proxy, pos, &qpos, &error);
    g_clear_error(&error);
    return qpos;
}

/**
 * Queries mlplayer what is the playlist position for given a playqueue entry index.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] qpos Position to check queue for.
 *
 * @return the playlist position for a playqueue entry
 **/
EXPORT gint mlplayer_remote_get_playqueue_list_position(DBusGProxy *proxy, guint qpos) {
    guint pos = 0;
    org_atheme_mlplayer_queue_get_list_pos (proxy, qpos, &pos, &error);
    g_clear_error(&error);
    return pos;
}

/**
 * Tells mlplayer to add an URI to a temporary playlist.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] string The URI to enqueue to a temporary playlist.
 **/
EXPORT void mlplayer_remote_playlist_enqueue_to_temp(DBusGProxy *proxy,
                                               gchar *string) {
    org_atheme_mlplayer_playlist_enqueue_to_temp(proxy, string, &error);
	g_clear_error(&error);
}

/**
 * Queries MLPlayer about a playlist entry's tuple information.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] field The name of the tuple field to retrieve.
 * @param[in] pos The playlist position to query for.
 * @return The requested field's data for the entry in the playlist at %pos position.
 **/
EXPORT gchar *mlplayer_get_tuple_field_data(DBusGProxy *proxy, gchar *field,
                                      guint pos) {
    GValue value = {0};
    gchar *s = NULL;

    org_atheme_mlplayer_song_tuple(proxy, pos, field, &value, &error);

    g_clear_error(&error);

    if (G_IS_VALUE(&value) == FALSE)
        return NULL;

    /* I think the original "purpose" of using g_strescape() here
     * has probably been to escape only \n, \t, \r, etc. but the function
     * actually escapes all non-ASCII characters. Which is bad, since we
     * are using UTF-8.  -- ccr
     */
    if (G_VALUE_HOLDS_STRING(&value))
        //s = g_strescape(g_value_get_string(&value), NULL);
        s = g_strdup(g_value_get_string(&value));
    else if (g_value_type_transformable(G_VALUE_TYPE(&value), G_TYPE_STRING))
    {
        GValue tmp_value = { 0, };

        g_value_init(&tmp_value, G_TYPE_STRING);
        g_value_transform(&value, &tmp_value);

        //s = g_strescape(g_value_get_string(&tmp_value), NULL);
        s = g_strdup(g_value_get_string(&tmp_value));

        g_value_unset(&tmp_value);
    }
    else
        s = g_strdup("<unknown type>");

    g_value_unset(&value);
    return s;
}

/**
 * Toggles the equalizer.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @param[in] active Whether or not to activate the equalizer.
 **/
EXPORT void mlplayer_remote_eq_activate(DBusGProxy *proxy, gboolean active) {
    org_atheme_mlplayer_equalizer_activate (proxy, active, &error);
    g_clear_error(&error);
}

/**
 * Returns a array of strings with available tuple field names.
 *
 * @param[in] proxy DBus proxy for mlplayer
 * @return Array of strings.
 **/
EXPORT gchar **mlplayer_remote_get_tuple_fields(DBusGProxy *proxy) {
    gchar **res = NULL;
    org_atheme_mlplayer_get_tuple_fields (proxy, &res, &error);
    g_clear_error(&error);
    return res;
}

/**
 * Returns the active playlist name.
 */
EXPORT gchar *mlplayer_remote_playlist_get_active_name(DBusGProxy *proxy) {
    char *string = NULL;
    org_atheme_mlplayer_get_active_playlist_name (proxy, &string, &error);
    g_clear_error(&error);

    return (string ? string : NULL);
}
