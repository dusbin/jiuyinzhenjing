cc_binary(
    name = "cleandisk",
    srcs = ["main.c"],
	linkopts = ["-lpthread","-ldl"],
	copts = ["-c","-g","-w"],
	deps = [
		"//db:db_library",
		"//ux_comm_lib:ux_comm_lib_library",
	],
)