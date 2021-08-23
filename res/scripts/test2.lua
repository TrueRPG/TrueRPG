local test2 = { 
	-- properties & fields contains here
	testivec2 = glm.ivec2.new(1, 50),
	testvec2 = glm.vec2.new(2.7, 6.0)
}

function test2:onCreate()
	print("Test2.lua: "..self.entity:getName().name)
	self.lol = self.entity:getRigidBody()

	print('\n')
end

function test2:onUpdate(deltaTime)

end

return test2

