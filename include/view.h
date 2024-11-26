// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2024 He Yong <hyyoxhk@163.com>
 */

#ifndef VIEW_H
#define VIEW_H

#include <wayland-server-core.h>

struct wlr_surface;

struct wlrston_view {
	struct wl_list link;
	struct wlrston_server *server;
	struct wlr_xdg_toplevel *xdg_toplevel;
	struct wlr_scene_tree *scene_tree;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener destroy;
	struct wl_listener request_move;
	struct wl_listener request_resize;
	struct wl_listener request_maximize;
	struct wl_listener request_fullscreen;
	int x, y;
};

void focus_view(struct wlrston_view *view, struct wlr_surface *surface);

#endif
