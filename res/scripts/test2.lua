local test2 = { 
	-- properties contains here 
	testivec2 = ivec2.new(1, 50),
	testvec2 = vec2.new(2.7, 6.0)
}

function test2:onCreate()
	local testNameChange = self.entity:getComponent(Components.Name)
	print("Test2.lua: "..testNameChange.name.."\n")
	print(self.testivec2.y)
	print(self.testvec2.x)
	self.testvec2 = self.testvec2 - self.testvec2
	print(self.testvec2.x)
	print('\n')
end

function test2:onUpdate(deltaTime)

end

return test2

