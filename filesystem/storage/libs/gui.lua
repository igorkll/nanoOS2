local font = require("font")
local gui = {colors = {}, graphic = {}, x = SCREEN_RESX, y = SCREEN_RESY}
gui.colors.black = tonumber("000000", 16) --обратите внимания что черный на чернобелых экранах это заполненый пиксель!
gui.colors.white = tonumber("ffffff", 16)
gui.colors.red = tonumber("ff0000", 16)

------------- graphic

function gui.graphic.dot(x, y, color)
    if x < 0 or y < 0 or x >= gui.x or y >= gui.y then return end
    screen_set(x, y, color or gui.colors.white)
end

function gui.graphic.clear(color)
    for x = 0, gui.x - 1 do
        for y = 0, gui.y - 1 do
            screen_set(x, y, color or gui.colors.black)
        end
    end
end

function gui.graphic.text(x, y, text, color)
    for i = 1, #text do
        local char = font.getChar(text:sub(i, i))
        for cy, line in ipairs(char) do
            for cx = 1, #line do
                if line:sub(cx, cx) == "1" then
                    gui.graphic.dot(x + (cx - 1) + ((i - 1) * (font.width + 1)), y + (cy - 1), color)
                end
            end
        end
    end
end

function gui.graphic.update()
    screen_update()
end

------------- gui

function gui.menu()
    
end

return gui