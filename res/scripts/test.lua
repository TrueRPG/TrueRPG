local test = {}

function test:onCreate()
	print("Test1.lua: "..self.entity:getName().name)
	print(addChild)
end

function test:onUpdate(deltaTime)

end

function test:onDestroy()
	print("This is destroy from lua")
end

return test

