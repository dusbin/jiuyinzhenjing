cc_library(
	name = "ux_comm_lib_library",
	srcs = [
		"rbtree.c",
		"uxmd5.c"
	],
	hdrs = [
		"rbtree.h",
		"uxmd5.h",
		"rbtree_augmented.h",
		"simple_list.h"
	],
	linkopts = ["-lpthread","-ldl"],
	copts = ["-c","-g","-w"],
	visibility = ["//main:__pkg__"],
)