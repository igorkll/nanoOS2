print("hello, lua!")

while not control_getState(CONTROL_ENTER) do
    wait(1)
end

print("end!!")