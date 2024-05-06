local function dirOpen(path)
    return io.popen("chcp 1251|dir /a /b \"" .. path .."\"")
end

local function startwith(str, startCheck)
    return string.sub(str, 1, string.len(startCheck)) == startCheck
end

local function endwith(str, startCheck)
    local len = string.len(str)
    return string.sub(str, len - (string.len(startCheck) - 1), len) == startCheck
end

local function getFunctionName(line)
    local chrs
    for i = 1, #line do
        local chr = line:sub(i, i)
        if chrs then
            if chr == "(" then
                break
            end
            table.insert(chrs, chr)
        elseif chr == " " then
            chrs = {}
        end
    end
    if chrs then
        return table.concat(chrs)
    end
end

local function parse(line)
    if not startwith(line, "#") and endwith(line, ");") then
        local retType
        local funcName = getFunctionName(line)
        if startwith(line, "void") then
            
        elseif startwith(line, "bool") then
        elseif startwith(line, "int") then
        else
            return
        end
        if retType then
            return "LUA_BIND_RETR(" .. funcName .. ")"
        else
            return 
        end
    end
end

local function parseHeaders(path)
    local headersFolder = dirOpen(path)
    local bindings = {}
    for filename in headersFolder:lines() do
        if filename:sub(#filename - 1, #filename) == ".h" then
            local file = io.open(path .. "/" .. filename, "r")
            for line in file:lines() do
                local bind = parse(line)
                if bind then
                    table.insert(bindings, bind)
                end
            end
            file:close()
        end
    end
    headersFolder:close()
    return table.concat(bindings, "\n")
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

---- parse headers
parseHeaders("../../main")

output:write("\n}")
output:close()