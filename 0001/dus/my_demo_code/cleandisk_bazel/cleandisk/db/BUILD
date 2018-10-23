cc_library(
	name = "db_library",
	srcs = [
		"db_interface.c",
		"sqlite3.c"
	],
	hdrs = [
		"db_interface.h",
		"sqlite3.h"
	],
	linkopts = ["-lpthread","-ldl"],
	copts = ["-c","-g","-w"],
	visibility = ["//main:__pkg__"],
)