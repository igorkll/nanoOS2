local function dirOpen(path)
    return io.popen("chcp 1251|dir /a /b \"" .. path .."\"")
end

local output = io.open("../../main/service/lua_binds.h", "wb")
output:write("//THIS FILE WAS GENERATED AUTOMATICALLY BY THE '/tools/generate_binds' UTILITY\n{\n")

---- default bind
local defaultBinds = dirOpen("binds")
for bindname in defaultBinds:lines() do
    output:write("    // -------- " .. bindname .. "\n")
    local file = io.open("binds/" .. bindname, "rb")
    for line in file:lines() do
        output:write("    " .. line)
    end
    output:write("\n\n")
    file:close()
end
defaultBinds:close()

output:write("\n}")
output:close()