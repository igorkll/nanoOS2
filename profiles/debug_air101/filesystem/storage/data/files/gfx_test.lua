graphic_clear(0x0000ee)
graphic_setCrop(1)
while true do
    for i = 1, 100 do
        graphic_drawPixel(math.random(0, graphic_x() - 1), math.random(0, graphic_y() - 1), math.random(0, 0xffffff))
    end
    graphic_update()
    wait(100)
end