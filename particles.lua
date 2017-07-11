
vec2 = function(x, y)
	local vec = {
		x = x or 0.0,
		y = y or 0.0,
	}
	function vec:add(v)
		self.x = self.x + v.x
		self.y = self.y + v.y
	end
	function vec:sub(v)
		self.x = self.x - v.x
		self.y = self.y - v.y
	end
	function vec:mul(v)
		self.x = self.x * v.x
		self.y = self.y * v.y
	end
	function vec:len()
		return math.sqrt(self.x*self.x + self.y*self.y)
	end

	return vec
end

function vec_sub(a, b)
	return vec2(a.x-b.x, a.y-b.y)
end

local particles = {}
local counter = 0
local size = 4

local font = load_font("jellee.ttf", 1.5)
local image = load_texture("kitten2.jpg")

function particle(p, c, s)
	return {
		pos = p,
		life = 100,
		color = c,
		speed = s,
	}
end

function rand_signed()
	return -1.0 + math.random()*2.0
end

function run()
	print(file_request('josh.lua'))
	local printlua = loadstring(file_request('print.lua'))
	-- print("printlua " .. printlua)
	printlua()

	while true do
		update()
		get_input()

		local speed_mul = 1.0
		local spawn_speed = 1.0

		for i,p in ipairs(particles) do
			p.life = p.life - (10.0 * time.dt)
			-- if p.life < 0 then table.remove(particles, i) end

			p.pos.x = p.pos.x + (p.speed.x * time.dt) * 40.0 * speed_mul
			p.pos.y = p.pos.y + (p.speed.y * time.dt) * 40.0 * speed_mul
			p.speed.y = p.speed.y + (9.81*time.dt) * speed_mul

			if p.pos.x + size > window.size.x and p.speed.x > 0.0 then
				-- p.speed.x = p.speed.x * -0.75
				-- p.speed.y = p.speed.y * 0.9
				p.speed:mul(vec2(-0.75, 0.9))
			end
			if p.pos.y + size > window.size.y and p.speed.y > 0.0 then
				-- p.speed.y = p.speed.y * -0.75
				-- p.speed.x = p.speed.x * 0.9
				p.speed:mul(vec2(0.9, -0.75))
			end
			if p.pos.x < 0.0 and p.speed.x < 0.0 then
				-- p.speed.x = p.speed.x * -0.75
				-- p.speed.y = p.speed.y * 0.9
				p.speed:mul(vec2(-0.75, 0.9))
			end
			if p.pos.y < 0.0 and p.speed.y < 0.0 then
				-- p.speed.y = p.speed.y * -0.75
				-- p.speed.x = p.speed.x * 0.9
				p.speed:mul(vec2(0.9, -0.75))
			end

			-- for j,p2 in ipairs(particles) do
			-- 	if i ~= j then
			-- 		-- if vec_sub(p.pos, p2.pos):len() < 10.0 then
			-- 			if p.pos.x + size > p2.pos.x and
			-- 			   p.pos.y + size > p2.pos.y and
			-- 			   p.pos.x < p2.pos.x + size and
			-- 			   p.pos.y < p2.pos.y + size then
			-- 				local x1 = (p.pos.x + size) - p2.pos.x
			-- 				local x2 = (p2.pos.x + size) - p.pos.x
			-- 				local y1 = (p.pos.y + size) - p2.pos.y
			-- 				local y2 = (p2.pos.y + size) - p.pos.y
			-- 				local min = math.min(x1, x2, y1, y2)
			-- 				if min == x1 then
			-- 					p.pos.x = p2.pos.x - size
			-- 					p.speed:mul(vec2(-0.75, 0.9))
			-- 					p2.speed:mul(vec2(-0.75, 0.9))
			-- 				elseif min == x2 then
			-- 					p.pos.x = p2.pos.x + size
			-- 					p.speed:mul(vec2(-0.75, 0.9))
			-- 					p2.speed:mul(vec2(-0.75, 0.9))
			-- 				elseif min == y1 then
			-- 					p.pos.y = p2.pos.y - size
			-- 					p.speed:mul(vec2(0.9, -0.75))
			-- 					p2.speed:mul(vec2(0.9, -0.75))
			-- 				elseif min == y2 then
			-- 					p.pos.y = p2.pos.y + size
			-- 					p.speed:mul(vec2(0.9, -0.75))
			-- 					p2.speed:mul(vec2(0.9, -0.75))
			-- 				end
			-- 			end
			-- 		-- end
			-- 	end
			-- end

			set_color(p.color.x, p.color.y, p.color.z, 1.0)
			draw_rect(p.pos.x, p.pos.y, size, size)
		end

		counter = counter + spawn_speed
		if counter >= 1.0 then
			local color = vec3(0.5 + math.random()*0.5,
							   0.5 + math.random()*0.5,
							   0.5 + math.random()*0.5)
			local speed = vec2(10.0 + math.random()*2.0, -10.0 + math.random()*2.0)
			local pos = vec2(window.size.x/2 + rand_signed()*30, window.size.y/2 + rand_signed()*30)
			local p = particle(pos, color, speed)
			table.insert(particles, p)
			counter = counter - 1
		end

		set_color(1.0, 1.0, 1.0, 1.0)
		draw_font(font, "particles " .. #particles, 10, 0)

		set_tex_coords(0, 0, 1, 0, 1, 1, 0, 1)
		draw_rect_texture(image, 10, 100, 600, 400)

		swap_buffers()
	end
end

function init()
	window.title = "Matt's Particles"
end
