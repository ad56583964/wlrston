srcs_wlrston = [
	'main.c',
	xdg_shell_protocol_h,
	xdg_shell_protocol_c,
]

deps_wlrston = [
	dep_wayland_server,
	dep_wlroots,
	dep_xkbcommon,
	dep_pixman,
]

executable(
	'wlrston',
	sources: srcs_wlrston,
	include_directories: public_inc,
	dependencies: deps_wlrston,
)
