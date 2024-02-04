local gui = require("gui")

--print("wifi_accesspoint from LUA:", wifi_accesspoint("nanoOS2", ""))
while true do
    gui.graphic.clear()
    gui.graphic.text(1, 1, "12345678!!!")
    if keyboard_get then
        for x = 0, 3 do
            for y = 0, 3 do
                if keyboard_get(x, y) then
                    gui.graphic.text(x * 8, (y * 8) + 10, "@")
                end
            end
        end
    end

    if keyboard_isMoveButton then
        for i = 0, 3 do
            if keyboard_isMoveButton(i) then
                gui.graphic.text(i * 8, 40, "=")
            end
        end
    end
    
    for i = 1, 4 do
        gui.graphic.dot(math.random(0, gui.x - 1), math.random(0, gui.y - 1))
    end
    gui.graphic.update()

    sleep(0)
end