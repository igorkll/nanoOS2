local fs = require("fs")
local pkg = {}

function pkg.find(name)
    if fs.exists(name) then --если name указывает на файл
        if not fs.isDir(name) then
            return name
        elseif fs.exists(name .. "/main.lua") then
            return name .. "/main.lua"
        end
    end
    
    local function do_storage(basepath)
        local path = basepath .. "/apps/" .. name .. ".lua"
        if fs.exists(path) and not fs.isDir(path) then
            return path
        end
        
        path = basepath .. "/apps/" .. name
        if fs.exists(path) and fs.isDir(path) and fs.exists(path .. "/main.lua") and not fs.isDir(path .. "/main.lua") then
            return path .. "/main.lua"
        end
    end
    
    local paths = {"/rom", "/storage"}
    for _, path in ipairs(paths) do
        path = do_storage(path)
        if path then
            return path
        end
    end
end

function pkg.load(name)
    local path = pkg.find(name)
    if path then
        return loadfile(path)
    end
end

function pkg.run(name, ...)
    local code = pkg.load(name)
    if code then
        return code(...)
    end
    return nil, "application is not installed"
end

return pkg