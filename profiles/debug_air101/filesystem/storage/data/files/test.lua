print(154, -213)
print("hello, lua!")

local i = 0
while true do
    wait(1000)
    print("tick", i)
    i = i + 1

    control_begin()
    if control_getState(CONTROL_UP) then
        break
    end
end

print("end!!")