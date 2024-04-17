local inputFile
local outputFile
while true do
    io.write("font name: ")
    local ok, input = pcall(io.read)
    if not ok then
        return
    end
    local err
    if not input or input == "" then
        print("invalid input")
    else
        inputFile, err = io.open("fonts/" .. input .. ".lua", "r")
        if inputFile then
            outputFile, err = io.open("build/" .. input .. ".fnt", "wb")
            if outputFile then
                local inputData = inputFile:read("*a")
                local inputEnv = {}
                local inputCode, err = load(inputData, input, "=t", inputEnv)
                if inputCode then
                    local ok, err = pcall(inputCode)
                    if ok then
                        
                    else
                        print("failed to execute font: ", err)
                    end
                else
                    print("failed to load(compile) font: ", err)
                end
                inputFile:close()
                outputFile:close()
            else
                inputFile:close()
                print("failed to create build: ", err)
            end
        else
            print("failed to load font: ", err)
        end
    end
end

local function write(name, data)
    local file = io.open(name, "wb")
    file:write(data)
    file:close()
end

local function bool_table_to_string(bool_table)
    local str = ''
    for i = 1, #bool_table, 8 do
        local byte = 0
        for j = i, i+7 do
            if bool_table[j] then
                byte = byte + 2^(j-i)
            end
        end
        str = str .. string.char(byte)
    end
    return str
end

local function toChar(str1, str2)
    local byte = 0
    for i = 1, 4 do
        if str1:sub(i, i) == "1" then
            byte = byte + (2 ^ (i - 1))
        end
    end
    for i = 1, 4 do
        if str2:sub(i, i) == "1" then
            byte = byte + (2 ^ (i + 3))
        end
    end
    return string.char(byte)
end

------------------------------------

for key, char in pairs(font.chars) do --штоб каждый символ занимал 3 байта
    table.insert(font.chars[key], string.rep(".", font.width))
end

local data = ""
for i = 0, 255 do
    local chardata = font.chars[string.char(i)]
    if chardata then
        data = data .. toChar(chardata[1], chardata[2])
        data = data .. toChar(chardata[3], chardata[4])
        data = data .. toChar(chardata[5], chardata[6])
    else
        data = data .. (string.char(0)):rep(3)
    end
end
write("bin.txt", data)