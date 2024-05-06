local function dirOpen(path)
    return io.popen("chcp 1251|dir /a /b \"" .. path .."\"")
end

local function exists(path)
    local ok, err, code = os.rename(path, path)
    if not ok then
        if code == 13 then
            -- Permission denied, but it exists
            return true
        end
    end
    return not not ok
end

local function isFile(path)
    if not exists(path) then return false end
    local f = io.open(path)
    if f then
        f:close()
        return true
    end
    return false
end

local function isDir(path)
    return exists(path) and not isFile(path)
end





local function startwith(str, startCheck)
    return string.sub(str, 1, string.len(startCheck)) == startCheck
end

local function endwith(str, startCheck)
    local len = string.len(str)
    return string.sub(str, len - (string.len(startCheck) - 1), len) == startCheck
end

local function charsCount(str, char)
    local chars = 0
    for i = 1, #str do
        local chr = str:sub(i, i)
        if chr == char then
            chars = chars + 1
        end
    end
    return chars
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

local function getRawFunctionArgs(line)
    local args
    local recursion = 0
    for i = 1, #line do
        local chr = line:sub(i, i)
        if chr == ")" and recursion == 0 then
            break
        elseif args then
            if #args == 0 or (chr == "," and recursion == 0) then
                table.insert(args, "")
            end
            if chr ~= "," and (chr ~= " " or #args[#args] > 0) then
                args[#args] = args[#args] .. chr
            end
            if chr == "(" then
                recursion = recursion + 1
            elseif chr == ")" then
                recursion = recursion - 1
            end
        elseif chr == "(" then
            args = {}
        end
    end
    for i, arg in ipairs(args) do
        local spaces = charsCount(arg, " ")
        if spaces == 0 or arg:sub(#arg, #arg) == ")" or (startwith(arg, "struct") and spaces ~= 2) then
            
        else
            local newarg = {}
            local firstSpace = false
            for i = #arg, 1, -1 do
                local chr = arg:sub(i, i)
                if firstSpace then
                    table.insert(newarg, 1, chr)
                elseif chr == " " then
                    firstSpace = true
                end
            end
            args[i] = table.concat(newarg)
        end
    end
    return args
end

local ints = {
    ["int"] = true,
    ["short"] = true,
    ["long"] = true,
    ["long long"] = true,
    ["unsigned int"] = true,
    ["unsigned short"] = true,
    ["unsigned long"] = true,
    ["unsigned long long"] = true,
    ["int8_t"] = true,
    ["int16_t"] = true,
    ["int32_t"] = true,
    ["int64_t"] = true,
    ["uint8_t"] = true,
    ["uint16_t"] = true,
    ["uint32_t"] = true,
    ["uint64_t"] = true
}

local nums = {
    ["float"] = true,
    ["double"] = true
}

local bools = {
    ["bool"] = true
}

local function typeChange(typeChangers, ttype)
    if typeChangers[ttype] then return typeChangers[ttype] end
    return ttype
end

local function convertType(arg, isReturn)
    local argtype = isReturn and "RET" or "ARG"
    if ints[arg] then
        return "LUA_" .. argtype .."_INT"
    elseif nums[arg] then
        return "LUA_" .. argtype .. "_NUM"
    elseif bools[arg] then
        return "LUA_" .. argtype .. "_BOOL"
    end
end

local function getFunctionArgs(line, typeChangers)
    local rawArgs = getRawFunctionArgs(line)
    local args = {}
    for _, arg in ipairs(rawArgs) do
        local argtype = convertType(typeChange(typeChangers, arg))
        if argtype then
            table.insert(args, argtype)
        else
            return
        end
    end
    return "(" .. table.concat(args, ", ") .. ")"
end

local function getReturnType(line)
    local retType = {}
    local flag1 = false
    local flag2 = false
    local activate = false
    local recursion = 0
    for i = #line, 1, -1 do
        local chr = line:sub(i, i)
        if not flag2 then
            if chr == ")" then
                recursion = recursion + 1
            elseif chr == "(" then
                recursion = recursion - 1
                if recursion == 0 then
                    flag1 = true
                end
            elseif flag1 and chr == " " then
                flag2 = true
            end
        end
        if activate then
            table.insert(retType, 1, chr)
        elseif flag1 and flag2 then
            activate = true
        end
    end
    return table.concat(retType)
end

local function parse(line, blacklist, typeChangers)
    if not startwith(line, "#") and endwith(line, ");") then
        local retType = typeChange(typeChangers, getReturnType(line))
        local funcName = getFunctionName(line)
        local argsStr = getFunctionArgs(line, typeChangers)
        if blacklist[funcName] or not argsStr then return end
        if retType == "void" then
            retType = nil
        else
            retType = convertType(retType, true)
            if not retType then return end
        end
        if retType then
            return "LUA_BIND_RETR(" .. funcName .. ", " .. argsStr .. ", " .. retType .. ");"
        else
            return "LUA_BIND_VOID(" .. funcName .. ", " .. argsStr .. ");"
        end
    elseif startwith(line, "#define") then

    end
end

local function parseHeaders(output, blacklist, path, typeChangers)
    local headersFolder = dirOpen(path)
    local bindings = {}
    for filename in headersFolder:lines() do
        if filename:sub(#filename - 1, #filename) == ".h" then
            local file = io.open(path .. "/" .. filename, "r")
            local added = false
            for line in file:lines() do
                local bind = parse(line, blacklist, typeChangers)
                if bind then
                    if not added then
                        table.insert(bindings, {filename = filename})
                        added = true
                    end
                    table.insert(bindings[#bindings], bind)
                end
            end
            file:close()
        end
    end

    local index = 1
    for _, fileBindings in ipairs(bindings) do
        output:write("\n    // -------- " .. fileBindings.filename .. "\n")
        for i, bind in ipairs(fileBindings) do
            output:write("    " .. bind .. "\n")
        end
        index = index + 1
    end
    headersFolder:close()
end

local function parseRecursion(output, blacklist, path, typeChangers, folderBlacklist)
    parseHeaders(output, blacklist, path, typeChangers)
    local dir = dirOpen(path)
    for foldername in dir:lines() do
        local folderpath = path .. "/" .. foldername
        if not folderBlacklist[folderpath] and isDir(folderpath) then
            parseRecursion(output, blacklist, folderpath, typeChangers, folderBlacklist)
        end
    end
    dir:close()
end

local output = io.open("../../main/service/lua_binds.h", "wb")
output:write("//THIS FILE WAS GENERATED AUTOMATICALLY BY THE '/tools/generate_binds' UTILITY\n{\n")

---- default bind
local blacklist = {
    ["customshell_run"] = true,
    ["shell_run"] = true,
}
local defaultBinds = dirOpen("binds")
for bindname in defaultBinds:lines() do
    blacklist[bindname] = true
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
parseRecursion(output, blacklist, "../../main", {["tcolor"] = "uint32_t"}, {["../../main/service"] = true})

output:write("\n}")
output:close()