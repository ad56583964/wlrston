#define _POSIX_C_SOURCE 200112L
#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wayland-server-core.h>
#include <wlr/util/log.h>
#include <xkbcommon/xkbcommon.h>
#include <signal.h>

#include <wlrston.h>

static int on_term_signal(int signal_number, void *data)
{
	struct wl_display *display = data;

	wlr_log(WLR_ERROR, "caught signal %d", signal_number);
	wl_display_terminate(display);

	return 1;
}

static void sigint_helper(int sig)
{
	raise(SIGUSR2);
}

int main(int argc, char *argv[]) {
	wlr_log_init(WLR_DEBUG, NULL);
	char *startup_cmd = NULL;
	struct wlrston_server server;
	struct wl_event_source *signals[2];
	struct wl_event_loop *loop;
	struct sigaction action;
	int i;

	int c;
	while ((c = getopt(argc, argv, "s:h")) != -1) {
		switch (c) {
		case 's':
			startup_cmd = optarg;
			break;
		default:
			printf("Usage: %s [-s startup command]\n", argv[0]);
			return 0;
		}
	}
	if (optind < argc) {
		printf("Usage: %s [-s startup command]\n", argv[0]);
		return 0;
	}

	/* The Wayland display is managed by libwayland. It handles accepting
	 * clients from the Unix socket, manging Wayland globals, and so on. */
	server.wl_display = wl_display_create();
	loop = wl_display_get_event_loop(server.wl_display);

	signals[0] = wl_event_loop_add_signal(loop, SIGTERM, on_term_signal,
					      server.wl_display);
	signals[1] = wl_event_loop_add_signal(loop, SIGUSR2, on_term_signal,
					      server.wl_display);
	action.sa_handler = sigint_helper;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	if (!signals[0] || !signals[1])
		goto out_signals;

	server_init(&server);

	/* Add a Unix socket to the Wayland display. */
	const char *socket = wl_display_add_socket_auto(server.wl_display);
	if (!socket) {
		wlr_backend_destroy(server.backend);
		return 1;
	}

	/* Start the backend. This will enumerate outputs and inputs, become the DRM
	 * master, etc */
	if (!wlr_backend_start(server.backend)) {
		wlr_backend_destroy(server.backend);
		wl_display_destroy(server.wl_display);
		return 1;
	}

	/* Set the WAYLAND_DISPLAY environment variable to our socket and run the
	 * startup command if requested. */
	setenv("WAYLAND_DISPLAY", socket, true);
	if (startup_cmd) {
		if (fork() == 0) {
			execl("/bin/sh", "/bin/sh", "-c", startup_cmd, (void *)NULL);
		}
	}
	/* Run the Wayland event loop. This does not return until you exit the
	 * compositor. Starting the backend rigged up all of the necessary event
	 * loop configuration to listen to libinput events, DRM events, generate
	 * frame events at the refresh rate, and so on. */
	wlr_log(WLR_INFO, "Running Wayland compositor on WAYLAND_DISPLAY=%s",
			socket);
	wl_display_run(server.wl_display);

	server_finish(&server);

out_signals:
	for (i = 1; i >= 0; i--)
		if (signals[i])
			wl_event_source_remove(signals[i]);

	return 0;
}
