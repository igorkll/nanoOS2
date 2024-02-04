local fs = {}

function fs.format(path)
    if path:sub(#path, #path) == "/" then
        path = path:sub(1, #path - 1)
    end
    return path
end

function fs.exp(path)
    return path:match("^.+(%..+)$")
end

------------------------

function fs.remove(path)
    path = fs.format(path)
    return os.remove(path)
end

function fs.rename(path, newpath)
    path = fs.format(path)
    newpath = fs.format(newpath)
    return os.rename(path, newpath)
end

function fs.exists(path)
    path = fs.format(path)
    return not not os.rename(path, path)
end
 
function fs.isDir(path)
    path = fs.format(path)
    local file = io.open(path, "rb")
    if file then file:close() end
    return not file
end

return fs