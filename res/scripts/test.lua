local test = {
	simplexNoise = OpenSimplexNoise.new()
}

function test:onCreate()
	print("Test1.lua: "..self.entity:getComponent(Components.Name).name)
	print(self.simplexNoise:getNoise(5, 6))
	print('\n')
end

function test:onUpdate(deltaTime)

end

function test:onDestroy()
	print("This is destroy from lua")
end

return test

