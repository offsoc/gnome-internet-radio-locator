/*
 * Copyright (C) 2008 Pierre-Luc Beaudoin <pierre-luc@pierlux.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string.h>
#include <gtk/gtk.h>
#include <gst/player/player.h>
#include <champlain/champlain.h>
#include "gnome-internet-radio-locator.h"
#include "gnome-internet-radio-locator-markers.h"
#include "gnome-internet-radio-locator-player.h"

extern GtkWidget *input;
extern GtkEntryCompletion *completion;
extern GNOMEInternetRadioLocatorStationInfo *stationinfo, *localstation;
extern gchar *world_station_xml_filename;
extern GstPlayer *player;
extern ChamplainMarkerLayer *layer;

typedef struct
{
	ChamplainView *view;
	ChamplainMarker *marker;
} LocationCallbackData;

static gboolean
location_callback (LocationCallbackData *data)
{
	/* champlain_view_center_on (data->view, lat, lon); */
	/* champlain_location_set_location (CHAMPLAIN_LOCATION (data->marker), lat, lon); */
	g_print("%s\n", __FUNCTION__);
	return TRUE;
}

void
marker_function (ChamplainMarker *self,
		 gdouble          dx,
		 gdouble          dy,
		 ClutterEvent    *event,
		 gpointer         user_data)
{
	gchar *station;
	station = (gchar *)champlain_label_get_text (CHAMPLAIN_LABEL (self));
	gtk_entry_set_text(GTK_ENTRY(input),(gchar *)station);
	gst_player_stop(player);
	player = gst_player_new (NULL, gst_player_g_main_context_signal_dispatcher_new(NULL));
	stationinfo = gnome_internet_radio_locator_station_load_from_file(localstation, world_station_xml_filename);
	while (stationinfo != NULL) {
		if (strcasecmp(stationinfo->location, station)==0) {
			gst_player_stop(player);
			gnome_internet_radio_locator_player_new(GST_PLAYER(player), stationinfo->stream->uri);
			gst_player_play(player);
		}
		stationinfo = stationinfo->next;
	}
	return;
}


ChamplainMarkerLayer *
create_marker_layer (G_GNUC_UNUSED ChamplainView *view, ChamplainPathLayer **path)
{
  ClutterActor *marker;
  ClutterActor *layer_actor;
  ClutterColor orange = { 0xf3, 0x94, 0x07, 0xbb };
  LocationCallbackData callback_data;

  *path = champlain_path_layer_new ();
  layer = champlain_marker_layer_new_full (CHAMPLAIN_SELECTION_SINGLE);
  layer_actor = CLUTTER_ACTOR (layer);

  /* Create callback that updates the map periodically */
  /* callback_data.view = CHAMPLAIN_VIEW (view); */
  /* callback_data.marker = CHAMPLAIN_MARKER (layer); */
  /* g_timeout_add (1000, (GSourceFunc) location_callback, &callback_data); */

#if 0
  marker = champlain_label_new_with_text ("Norway\n<span size=\"xx-small\">Oslo</span>",
        "Serif 14", NULL, NULL);
  champlain_label_set_use_markup (CHAMPLAIN_LABEL (marker), TRUE);
  champlain_label_set_alignment (CHAMPLAIN_LABEL (marker), PANGO_ALIGN_RIGHT);
  champlain_label_set_color (CHAMPLAIN_LABEL (marker), &orange);

  champlain_location_set_location (CHAMPLAIN_LOCATION (marker),  37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker));
#endif

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Berkeley, California");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);
  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Dublin, Ireland");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 53.3497645,-6.2602732);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Moscow, Russia");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 55.4792046, 37.3273304);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Oslo, Norway");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker),59.9132694,10.7391112);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Reykjavik, Iceland");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 64.145981,-21.9422367);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Cape Town, South Africa");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker),-33.928992,18.417396);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Newcastle, Australia");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker),-32.9272881,151.7812534);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "London, United Kingdom");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker),51.5073219,-0.1276474);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Palo Alto, California");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.442156,-122.1634472);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "San Francisco, California");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.7792808,-122.4192363);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

  marker = champlain_label_new_from_file ("icons/emblem-generic.png", NULL);
  champlain_label_set_text (CHAMPLAIN_LABEL (marker), "Washington, District of Columbia");
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 38.8949549, -77.0366456);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  /* champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker)); */
  g_signal_connect(CHAMPLAIN_LOCATION(marker), "button-press", G_CALLBACK(marker_function), NULL);

#if 0
  marker = champlain_label_new_from_file ("icons/emblem-important.png", NULL);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker));

  marker = champlain_point_new ();
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker));

  marker = champlain_label_new_from_file ("icons/emblem-favorite.png", NULL);
  champlain_label_set_draw_background (CHAMPLAIN_LABEL (marker), FALSE);
  champlain_location_set_location (CHAMPLAIN_LOCATION (marker), 37.873093, -122.303769);
  champlain_marker_layer_add_marker (layer, CHAMPLAIN_MARKER (marker));
  champlain_path_layer_add_node (*path, CHAMPLAIN_LOCATION (marker));
#endif

  champlain_marker_layer_set_all_markers_draggable (layer);

  clutter_actor_show (layer_actor);
  return layer;
}
