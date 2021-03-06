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
 * along with this program.  If not, see <http://www.gnu.org/licenses>.
 *
 * The MLPlayer team does not consider modular code linking to
 * MLPlayer or using our public API to be a derived work.
 */

#ifndef AUDACIOUS_DBUS_H
#define AUDACIOUS_DBUS_H

#include <dbus/dbus-glib.h>

#define AUDACIOUS_DBUS_SERVICE      "org.atheme.mlplayer"
#define AUDACIOUS_DBUS_PATH         "/org/atheme/mlplayer"
#define AUDACIOUS_DBUS_INTERFACE    "org.atheme.mlplayer"
#define AUDACIOUS_DBUS_SERVICE_MPRIS    "org.mpris.mlplayer"
#define AUDACIOUS_DBUS_INTERFACE_MPRIS 	"org.freedesktop.MediaPlayer"
#define AUDACIOUS_DBUS_PATH_MPRIS_ROOT      "/"
#define AUDACIOUS_DBUS_PATH_MPRIS_PLAYER    "/Player"
#define AUDACIOUS_DBUS_PATH_MPRIS_TRACKLIST "/TrackList"

#define NONE                  = 0
#define CAN_GO_NEXT           = 1 << 0
#define CAN_GO_PREV           = 1 << 1
#define CAN_PAUSE             = 1 << 2
#define CAN_PLAY              = 1 << 3
#define CAN_SEEK              = 1 << 4
#define CAN_RESTORE_CONTEXT   = 1 << 5
#define CAN_PROVIDE_METADATA  = 1 << 6
#define PROVIDES_TIMING       = 1 << 7

void init_dbus (void);
void cleanup_dbus (void);
DBusGProxy * mlplayer_get_dbus_proxy (void);

#endif /* AUDACIOUS_DBUS_H */
