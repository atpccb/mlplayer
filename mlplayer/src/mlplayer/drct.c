/*
 * drct.c
 * Copyright 2009-2011 John Lindgren
 *
 * This file is part of MLPlayer.
 *
 * MLPlayer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 2 or version 3 of the License.
 *
 * MLPlayer is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * MLPlayer. If not, see <http://www.gnu.org/licenses/>.
 *
 * The MLPlayer team does not consider modular code linking to MLPlayer or
 * using our public API to be a derived work.
 */

#include <glib.h>
#include <libmlpcore/hook.h>
#include <libmlpcore/vfs.h>

#include "config.h"
#include "drct.h"
#include "i18n.h"
#include "misc.h"
#include "playback.h"
#include "playlist.h"

/* --- PROGRAM CONTROL --- */

void drct_quit (void)
{
    hook_call ("quit", NULL);
}

/* --- PLAYBACK CONTROL --- */

void drct_play (void)
{
    if (playback_get_playing ())
    {
        if (playback_get_paused ())
            playback_pause ();
        else
            playback_seek (0);
    }
    else
    {
        playlist_set_playing (playlist_get_active ());
        playback_play (0, FALSE);
    }
}

void drct_pause (void)
{
    if (playback_get_playing ())
        playback_pause ();
}

void drct_stop (void)
{
    if (playback_get_playing ())
        playback_stop ();
}

bool_t drct_get_playing (void)
{
    return playback_get_playing ();
}

bool_t drct_get_ready (void)
{
    return playback_get_ready ();
}

bool_t drct_get_paused (void)
{
    return playback_get_paused ();
}

char * drct_get_filename (void)
{
    return playback_get_filename ();
}

char * drct_get_title (void)
{
    return playback_get_title ();
}

void drct_get_info (int * bitrate, int * samplerate, int * channels)
{
    playback_get_info (bitrate, samplerate, channels);
}

int drct_get_time (void)
{
    return playback_get_time ();
}

int drct_get_length (void)
{
    return playback_get_length ();
}

void drct_seek (int time)
{
    playback_seek (time);
}

/* --- VOLUME CONTROL --- */

void drct_get_volume (int * left, int * right)
{
    playback_get_volume (left, right);
    * left = CLAMP (* left, 0, 100);
    * right = CLAMP (* right, 0, 100);
}

void drct_set_volume (int left, int right)
{
    playback_set_volume (CLAMP (left, 0, 100), CLAMP (right, 0, 100));
}

void drct_get_volume_main (int * volume)
{
    int left, right;
    drct_get_volume (& left, & right);
    * volume = MAX (left, right);
}

void drct_set_volume_main (int volume)
{
    int left, right, current;
    drct_get_volume (& left, & right);
    current = MAX (left, right);

    if (current > 0)
        drct_set_volume (volume * left / current, volume * right / current);
    else
        drct_set_volume (volume, volume);
}

void drct_get_volume_balance (int * balance)
{
    int left, right;
    drct_get_volume (& left, & right);

    if (left == right)
        * balance = 0;
    else if (left > right)
        * balance = -100 + right * 100 / left;
    else
        * balance = 100 - left * 100 / right;
}

void drct_set_volume_balance (int balance)
{
    int left, right;
    drct_get_volume_main (& left);

    if (balance < 0)
        right = left * (100 + balance) / 100;
    else
    {
        right = left;
        left = right * (100 - balance) / 100;
    }

    drct_set_volume (left, right);
}

/* --- PLAYLIST CONTROL --- */

void drct_pl_next (void)
{
    bool_t play = playback_get_playing ();
    if (playlist_get_playing () < 0)
        playlist_set_playing (playlist_get_active ());
    if (playlist_next_song (playlist_get_playing (), get_bool (NULL, "repeat")) && play)
        playback_play (0, FALSE);
}

void drct_pl_prev (void)
{
    bool_t play = playback_get_playing ();
    if (playlist_get_playing () < 0)
        playlist_set_playing (playlist_get_active ());
    if (playlist_prev_song (playlist_get_playing ()) && play)
        playback_play (0, FALSE);
}

static void add_list (Index * filenames, int at, bool_t to_temp, bool_t play)
{
    if (to_temp)
        playlist_set_active (playlist_get_temporary ());

    int playlist = playlist_get_active ();

    if (play)
    {
        if (get_bool (NULL, "clear_playlist"))
            playlist_entry_delete (playlist, 0, playlist_entry_count (playlist));
        else
            playlist_queue_delete (playlist, 0, playlist_queue_count (playlist));
    }

    playlist_entry_insert_batch (playlist, at, filenames, NULL, play);
}

void drct_pl_add (const char * filename, int at)
{
    Index * filenames = index_new ();
    index_append (filenames, str_get (filename));
    add_list (filenames, at, FALSE, FALSE);
}

void drct_pl_add_list (Index * filenames, int at)
{
    add_list (filenames, at, FALSE, FALSE);
}

void drct_pl_open (const char * filename)
{
    Index * filenames = index_new ();
    index_append (filenames, str_get (filename));
    add_list (filenames, -1, get_bool (NULL, "open_to_temporary"), TRUE);
}

void drct_pl_open_list (Index * filenames)
{
    add_list (filenames, -1, get_bool (NULL, "open_to_temporary"), TRUE);
}

void drct_pl_open_temp (const char * filename)
{
    Index * filenames = index_new ();
    index_append (filenames, str_get (filename));
    add_list (filenames, -1, TRUE, TRUE);
}

void drct_pl_open_temp_list (Index * filenames)
{
    add_list (filenames, -1, TRUE, TRUE);
}

/* Advancing to the next song when the current one is deleted is tricky.  First,
 * we delete all the selected songs except the current one.  We can then advance
 * to a new song without worrying about picking one that is also selected.
 * Finally, we can delete the former current song without stopping playback. */

void drct_pl_delete_selected (int list)
{
    int pos = playlist_get_position (list);

    if (get_bool (NULL, "advance_on_delete")
     && ! get_bool (NULL, "no_playlist_advance")
     && playback_get_playing () && list == playlist_get_playing ()
     && pos >= 0 && playlist_entry_get_selected (list, pos))
    {
        playlist_entry_set_selected (list, pos, FALSE);
        playlist_delete_selected (list);
        pos = playlist_get_position (list); /* it may have moved */

        if (playlist_next_song (list, get_bool (NULL, "repeat"))
         && playlist_get_position (list) != pos)
            playback_play (0, FALSE);

        playlist_entry_delete (list, pos, 1);
    }
    else
        playlist_delete_selected (list);
}
