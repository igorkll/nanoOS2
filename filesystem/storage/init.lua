print("lua version:", _VERSION)
require("package").path = "/storage/libs/?.lua;/storage/libs/?/init.lua;/rom/libs/?.lua;/rom/libs/?/init.lua"
assert(require("pkg").run("main"))