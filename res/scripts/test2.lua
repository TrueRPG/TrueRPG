local test2 = { }

function test2:init()
	local testNameChange = self.entity:getName()
	print("Test2.lua: "..testNameChange.name.."\n")	
end

return test2

