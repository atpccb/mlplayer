/*
 * icons-stock.c
 * Copyright 2007-2010 Michael Färber and John Lindgren
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

#include <gtk/gtk.h>
#include <mlplayer/misc.h>

#include "config.h"
#include "libmlpgui.h"

static void load_stock_icon (char * id, char * filename,
 GtkIconFactory * iconfactory)
{
    char * path = g_strdup_printf ("%s/images/%s",
     mlp_get_path (AUD_PATH_DATA_DIR), filename);

    GdkPixbuf * pixbuf = gdk_pixbuf_new_from_file (path, NULL);
    if (pixbuf == NULL)
        goto ERR;

    GtkIconSet * iconset = gtk_icon_set_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);

    gtk_icon_factory_add(iconfactory, id, iconset);

ERR:
    g_free (path);
}

EXPORT void
mlpgui_register_stock_icons(void)
{
    GtkIconFactory *iconfactory = gtk_icon_factory_new();

    load_stock_icon(AUD_STOCK_PLAYLIST,
                    "menu_playlist.png", iconfactory);
    load_stock_icon(AUD_STOCK_PLUGIN,
                    "menu_plugin.png", iconfactory);
    load_stock_icon(AUD_STOCK_QUEUETOGGLE,
                    "menu_queue_toggle.png", iconfactory);
    load_stock_icon(AUD_STOCK_RANDOMIZEPL,
                    "menu_randomize_playlist.png", iconfactory);

    gtk_icon_factory_add_default( iconfactory );
    g_object_unref( iconfactory );
}
