local function boolsToStr(bool_table)
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

local function writeByte(file, byte)
    file:write(string.char(byte))
end

local function writeBits(file, bits)
    file:write(boolsToStr(bits))
end

local function compileFont(file, font)
    writeByte(file, font.width)
    writeByte(file, font.height)
    for char, charData in pairs(font.chars) do
        file:write(char)
        local bits = {}
        for posy, line in ipairs(charData) do
            for posx = 1, font.width do
                table.insert(bits, line:byte(posx) ~= ".")
            end
        end
        writeBits(file, bits)
    end
end

os.execute("mkdir build")
local inputFile
local outputFile
while true do
    io.write("font name: ")
    local input = io.read()
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
                        print("font compilation...")
                        compileFont(outputFile, inputEnv.font)
                        print("done!")
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