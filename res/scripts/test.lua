local test = {
	-- properties & fields contains here
	simplexNoise = OpenSimplexNoise.new()
}

function test:onCreate()
	print("Test1.lua: "..self.entity:getName().name)
end

function test:onUpdate(deltaTime)

end

function test:onDestroy()
	print("This is destroy from lua")
end

return test

