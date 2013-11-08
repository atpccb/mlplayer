/*
 * ui_dock.h
 * Copyright 2011 John Lindgren
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

#ifndef SKINS_UI_DOCK_H
#define SKINS_UI_DOCK_H

#include <gtk/gtk.h>

void dock_add_window (GtkWidget * window, gint * x, gint * y, gint w, gint h,
 gboolean main);
void dock_remove_window (GtkWidget * window);

void dock_set_size (GtkWidget * window, gint w, gint h);

void dock_move_start (GtkWidget * window, gint x, gint y);
void dock_move (gint x, gint y);

#endif
