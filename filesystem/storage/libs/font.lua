local width = 4
local height = 6
local list = {["9"] = 0, ["8"] = 1, [";"] = 2, [":"] = 3, ["="] = 4, ["<"] = 5, ["?"] = 6, [">"] = 7, ["A"] = 8, ["@"] = 9, ["C"] = 10, ["B"] = 11, ["E"] = 12, ["D"] = 13, ["G"] = 14, ["F"] = 15, ["I"] = 16, ["H"] = 17, ["K"] = 18, ["J"] = 19, ["M"] = 20, ["L"] = 21, ["O"] = 22, ["N"] = 23, ["Q"] = 24, ["P"] = 25, ["S"] = 26, ["R"] = 27, ["U"] = 28, ["T"] = 29, ["W"] = 30, ["V"] = 31, ["!"] = 32, [" "] = 33, ["#"] = 34, ["\""] = 35, ["%"] = 36, ["$"] = 37, ["'"] = 38, ["&"] = 39, [")"] = 40, ["("] = 41, ["+"] = 42, ["*"] = 43, ["-"] = 44, [","] = 45, ["/"] = 46, ["."] = 47, ["1"] = 48, ["0"] = 49, ["3"] = 50, ["2"] = 51, ["5"] = 52, ["4"] = 53, ["7"] = 54, ["6"] = 55, ["y"] = 56, ["x"] = 57, ["{"] = 58, ["z"] = 59, ["}"] = 60, ["~"] = 61, ["№"] = 62, ["Y"] = 63, ["X"] = 64, ["["] = 65, ["Z"] = 66, ["]"] = 67, ["\\"] = 68, ["_"] = 69, ["^"] = 70, ["a"] = 71, ["`"] = 72, ["c"] = 73, ["b"] = 74, ["e"] = 75, ["d"] = 76, ["g"] = 77, ["f"] = 78, ["i"] = 79, ["h"] = 80, ["k"] = 81, ["j"] = 82, ["m"] = 83, ["l"] = 84, ["o"] = 85, ["n"] = 86, ["q"] = 87, ["p"] = 88, ["s"] = 89, ["r"] = 90, ["u"] = 91, ["t"] = 92, ["w"] = 93, ["v"] = 94}
local charBinarySize = ((width * height) // 8)
local error = {
	"1111",
	"1..1",
	"1..1",
	"1..1",
	"1111"
}
local file = io.open("/storage/font.bin", "rb")

local function getChar(char)
	local index = list[char]
	if not index or not file then return error end
	file:seek("set", index * charBinarySize)
	local char = file:read(charBinarySize)
	local chartable = {""}
	for i = 1, charBinarySize do
		local byte = char:byte(i)
		for i = 0, 7 do
			local bit = byte % 2 == 1
			byte = byte // 2
			chartable[#chartable] = chartable[#chartable] .. (bit and "1" or ".")
			if #chartable[#chartable] >= width then
				table.insert(chartable, "")
			end
		end
	end
	return chartable
end

return {
	getChar = getChar,
	width = width,
	height = height
}