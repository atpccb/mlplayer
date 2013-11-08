/*
 * MLPlayer - a cross-platform multimedia player
 * Copyright (c) 2008 Tomasz Moń
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

#ifndef PLUGIN_SKINS_H
#define PLUGIN_SKINS_H

#include <glib.h>

enum {
    SKINS_PATH_USER_SKIN_DIR,
    SKINS_PATH_SKIN_THUMB_DIR,
    SKINS_PATH_COUNT
};

extern gchar *skins_paths[];

#endif
