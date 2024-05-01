print(154, -213)
print("hello, lua!")

local i = 0
while not control_getState(CONTROL_UP) do
    wait(1000)
    print("tick", i)
    i = i + 1
end

print("end!!")