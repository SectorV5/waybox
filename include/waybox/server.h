#ifndef _WB_SERVER_H
#define _WB_SERVER_H

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))
#define TITLEBAR_HEIGHT 8 /* TODO: Get this from the theme */
#include <wlr/version.h>
#define WLR_CHECK_VERSION(major, minor, micro) (WLR_VERSION_NUM >= ((major << 16) | (minor << 8) | (micro)))

#include <wlr/backend.h>
#include <wlr/render/allocator.h>
#include <wlr/render/wlr_renderer.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_ext_foreign_toplevel_list_v1.h>
#include <wlr/types/wlr_gamma_control_v1.h>
#include <wlr/types/wlr_idle_notify_v1.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_screencopy_v1.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_xdg_output_v1.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/util/log.h>

#include <stdlib.h>

#ifdef USE_NLS
#	include <libintl.h>
#	include <locale.h>
#	define _ gettext
#else
#	define _(s) (s)
#endif

#include "config.h"
#include "waybox/cursor.h"
#include "decoration.h"
#include "layer_shell.h"
#include "waybox/xdg_shell.h"
#include "waybox/seat.h"

struct wb_server {
	struct wl_display *wl_display;
	struct wl_event_loop *wl_event_loop;

	struct wlr_allocator *allocator;
	struct wlr_backend *backend;
	struct wlr_compositor *compositor;
	struct wlr_gamma_control_manager_v1 *gamma_control_manager;
	struct wlr_idle_inhibit_manager_v1 *idle_inhibit_manager;
	struct wlr_idle_notifier_v1 *idle_notifier;
	struct wlr_output_layout *output_layout;
	struct wlr_xdg_output_manager_v1 *output_manager;
	struct wlr_renderer *renderer;
	struct wlr_scene *scene;
	struct wlr_scene_output_layout *scene_layout;
	struct wlr_session *session;
	struct wlr_subcompositor *subcompositor;
	struct wlr_output_manager_v1 *wlr_output_manager;

	struct wb_config *config;
	char *config_file;

	struct wb_cursor *cursor;
	struct wb_seat *seat;

	struct wb_toplevel *grabbed_toplevel;
	struct wlr_box grab_geo_box;
	double grab_x, grab_y;
	uint32_t resize_edges;
	struct wlr_ext_foreign_toplevel_list_v1 *foreign_toplevel_list;
	struct wl_list toplevels;

	struct wlr_layer_shell_v1 *layer_shell;
	struct wlr_xdg_shell *xdg_shell;

	struct wl_listener gamma_control_set_gamma;
	struct wl_listener new_layer_surface;
	struct wl_listener new_xdg_decoration;
	struct wl_listener new_xdg_popup;
	struct wl_listener new_xdg_toplevel;

	struct wl_listener destroy_inhibit_manager;
	struct wl_listener destroy_inhibitor;
	struct wl_listener new_inhibitor;
	struct wl_list inhibitors;

	struct wl_listener new_input;
	struct wl_listener new_output;
	struct wl_listener output_configuration_applied;
	struct wl_listener output_configuration_tested;
	struct wl_list outputs; /* wb_output::link */
};

bool wb_create_backend(struct wb_server *server);
bool wb_start_server(struct wb_server *server);
bool wb_terminate(struct wb_server *server);

#endif /* server.h */
