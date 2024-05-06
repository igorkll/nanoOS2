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
        if arg:sub(#arg, #arg) == ")" or (startwith(arg, "struct") and charsCount(arg, " ") ~= 2) then
            
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

local function convertType(arg, isReturn)
    local argtype = isReturn and "RET" or "ARG"
    if ints[arg] then
        return "LUA_" .. argtype .."_INT"
    elseif nums[arg] then
        return "LUA_" .. argtype .. "_NUM"
    end
end

local function getFunctionArgs(line)
    local rawArgs = getRawFunctionArgs(line)
    local args = {}
    for _, arg in ipairs(rawArgs) do
        local argtype = convertType(arg)
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
    local recursion = 0;
    for i = #line, 1, -1 do
        local chr = line:sub(i, i)
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
        if flag1 and flag2 then
            table.insert(retType, 1, chr)
        end
    end
    return table.concat(retType)
end

local function parse(line, blacklist)
    if not startwith(line, "#") and endwith(line, ");") then
        local retType = getReturnType(line)
        local funcName = getFunctionName(line)
        local argsStr = getFunctionArgs(line)
        print(retType, argsStr)
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

local function parseHeaders(output, blacklist, path)
    local headersFolder = dirOpen(path)
    local bindings = {}
    local bindingsCount = 0
    for filename in headersFolder:lines() do
        if filename:sub(#filename - 1, #filename) == ".h" then
            bindings[filename] = {}
            bindingsCount = bindingsCount + 1
            local file = io.open(path .. "/" .. filename, "r")
            for line in file:lines() do
                local bind = parse(line, blacklist)
                if bind then
                    table.insert(bindings[filename], bind)
                end
            end
            file:close()
        end
    end

    local index = 0
    local realIndex = 0
    for filename, bindings in pairs(bindings) do
        index = index + 1
        if #bindings > 0 then
            realIndex = realIndex + 1
            if realIndex > 1 then
                output:write("\n")
            end
            output:write("    // -------- " .. filename .. "\n")
            for i, bind in ipairs(bindings) do
                output:write("    " .. bind)
                if i ~= #bindings or index ~= bindingsCount then
                    output:write("\n")
                end
            end
        end
    end
    headersFolder:close()
end

local output = io.open("../../main/service/lua_binds.h", "wb")
output:write("//THIS FILE WAS GENERATED AUTOMATICALLY BY THE '/tools/generate_binds' UTILITY\n{\n")

---- default bind
local blacklist = {}
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
parseHeaders(output, blacklist, "../../main")

output:write("\n}")
output:close()