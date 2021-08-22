local test = {}

function test:init()
	print("Test1.lua: "..self.entity:getName().name)
	print(addChild)
end

function test:update(deltaTime)

end

function test:destroy()
	print("This is destroy from lua")
end

return test

