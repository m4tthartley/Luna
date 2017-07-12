
vec2 = function(x, y)
	local vec = {
		x = x or 0.0,
		y = y or 0.0,
	}
	function vec:add(a)
		self.x = self.x + a.x
		self.y = self.y + a.y
	end
	function vec:sub(s)
		self.x = self.x - s.x
		self.y = self.y - s.y
	end
	function vec:mul(m)
		self.x = self.x * m.x
		self.y = self.y * m.y
	end
	function vec:div(d)
		self.x = self.x / d.x
		self.y = self.y / d.y
	end
	function vec:len()
		return math.sqrt(self.x*self.x + self.y*self.y)
	end
	function vec:normalize()
		self:div(vec2(self:len(), self:len()))
	end

	return vec
end

function vec2_add(a, b)
	return vec2(a.x+b.x, a.y+b.y)
end
function vec2_sub(a, b)
	return vec2(a.x-b.x, a.y-b.y)
end
function vec2_mul(a, b)
	return vec2(a.x*b.x, a.y*b.y)
end
function vec2_div(a, b)
	return vec2(a.x/b.x, a.y/b.y)
end