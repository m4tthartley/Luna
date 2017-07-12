
assert(loadstring(file_request("math.lua")))()

local particles = {}
local counter = 0
local size = 4 * 10

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
		local spawn_speed = 0.1

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
				p.pos.x = window.size.x - size
			end
			if p.pos.y + size > window.size.y and p.speed.y > 0.0 then
				-- p.speed.y = p.speed.y * -0.75
				-- p.speed.x = p.speed.x * 0.9
				p.speed:mul(vec2(0.9, -0.75))
				p.pos.y = window.size.y - size
			end
			if p.pos.x < 0.0 and p.speed.x < 0.0 then
				-- p.speed.x = p.speed.x * -0.75
				-- p.speed.y = p.speed.y * 0.9
				p.speed:mul(vec2(-0.75, 0.9))
				p.pos.x = 0.0
			end
			if p.pos.y < 0.0 and p.speed.y < 0.0 then
				-- p.speed.y = p.speed.y * -0.75
				-- p.speed.x = p.speed.x * 0.9
				p.speed:mul(vec2(0.9, -0.75))
				p.pos.y = 0.0
			end

			for j,p2 in ipairs(particles) do
				if i ~= j then
					local dist = vec2_sub(p.pos, p2.pos):len()

					if dist < size then
						vec2_sub(p.pos, p2.pos)
						-- p.speed:add(vec2_mul(p2.speed, vec2_sub(p2.speed, p.speed)))

						-- p.speed:add(vec2_sub(p2.speed, p.speed))
						-- local s = vec2(size, size)
						-- local diff = vec2_sub(p.pos, p2.pos)
						-- diff:normalize()
						-- diff:mul(s)
						-- p.pos = vec2_add(p2.pos, diff)

						-- local temp = vec2(p.speed.x, p.speed.y)
						-- p.speed = vec2_mul(vec2(p2.speed.x, p2.speed.y), vec2(0.99, 0.99))
						-- p2.speed = vec2_mul(temp, vec2(0.99, 0.99))

						local move = (size - dist) / 2.0
						local diff = vec2_sub(p.pos, p2.pos)
						diff:normalize()
						p.pos = vec2_add(p.pos, vec2_mul(diff, vec2(move, move)))
						p2.pos = vec2_add(p2.pos, vec2_mul(diff, vec2(-move, -move)))
					end

					-- if vec2_sub(p.pos, p2.pos):len() < 10.0 then
					-- 	if p.pos.x + size > p2.pos.x and
					-- 	   p.pos.y + size > p2.pos.y and
					-- 	   p.pos.x < p2.pos.x + size and
					-- 	   p.pos.y < p2.pos.y + size then
					-- 		local x1 = (p.pos.x + size) - p2.pos.x
					-- 		local x2 = (p2.pos.x + size) - p.pos.x
					-- 		local y1 = (p.pos.y + size) - p2.pos.y
					-- 		local y2 = (p2.pos.y + size) - p.pos.y
					-- 		local min = math.min(x1, x2, y1, y2)
					-- 		if min == x1 then
					-- 			p.pos.x = p2.pos.x - size
					-- 			p.speed:mul(vec2(-0.75, 0.9))
					-- 			p2.speed:mul(vec2(-0.75, 0.9))
					-- 		elseif min == x2 then
					-- 			p.pos.x = p2.pos.x + size
					-- 			p.speed:mul(vec2(-0.75, 0.9))
					-- 			p2.speed:mul(vec2(-0.75, 0.9))
					-- 		elseif min == y1 then
					-- 			p.pos.y = p2.pos.y - size
					-- 			p.speed:mul(vec2(0.9, -0.75))
					-- 			p2.speed:mul(vec2(0.9, -0.75))
					-- 		elseif min == y2 then
					-- 			p.pos.y = p2.pos.y + size
					-- 			p.speed:mul(vec2(0.9, -0.75))
					-- 			p2.speed:mul(vec2(0.9, -0.75))
					-- 		end
					-- 	end
					-- end
				end
			end

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

		-- set_tex_coords(0, 0, 1, 0, 1, 1, 0, 1)
		-- draw_rect_texture(image, 10, 100, 600, 400)

		swap_buffers()
	end
end

function init()
	window.title = "Matt's Particles"
end
