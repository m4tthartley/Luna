
file_request("url:mattsblog.net/particles.lua")
file_request("url:mattsblog.net/math.lua")

assert(loadstring(file_request("math.lua")))()

local particles = {}
local counter = 0
local size = 6 * 1

-- local font = load_font("jellee.ttf", 1.5)
local image = load_texture("kitten2.jpg")

-- from http://lua-users.org/wiki/SimpleRound
function round(num, numDecimalPlaces)
	local mult = 10^(numDecimalPlaces or 0)
	return math.floor(num * mult + 0.5) / mult
end

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

-- local grid_size = 100
-- local cell_size = size * 4
-- local grid = {}
-- table.setn(grid, grid_size*grid_size)
-- function coord_to_grid(x, y)
-- 	return y*grid_size + x
-- end

function run()
	print(file_request('josh.lua'))
	local printlua = loadstring(file_request('print.lua'))
	-- print("printlua " .. printlua)
	printlua()

	local frame_time = 0

	while true do
		local start_time = get_seconds()
		update()
		get_input()
		time.dt = 1.0 / 60.0

		local speed_mul = 1.0
		local spawn_speed = 0.1

		for i,p in ipairs(particles) do
			p.life = p.life - (10.0 * time.dt)
			-- if p.life < 0 then table.remove(particles, i) end

			p.pos.x = p.pos.x + (p.speed.x * time.dt) * 40.0 * speed_mul
			p.pos.y = p.pos.y + (p.speed.y * time.dt) * 40.0 * speed_mul
			p.speed.y = p.speed.y + (9.81*time.dt) * speed_mul

			set_color(p.color.x, p.color.y, p.color.z, 1.0)
			draw_rect(p.pos.x, p.pos.y, size, size)
		end

		for i,p in ipairs(particles) do
			for j,p2 in ipairs(particles) do
				if i ~= j then
					local dist = vec2_sub(p.pos, p2.pos):len()

					if dist < size then
						-- vec2_sub(p.pos, p2.pos)

						--[[
							avel = (elastic_constant*b.mass * (b.vel - a.vel) + a.mass*a.vel + b.mass*b.vel) / (a.mass+b.mass)
							bvel = (elastic_constant*a.mass * (a.vel - b.vel) + a.mass*a.vel + b.mass*b.vel) / (a.mass+b.mass)

							if mass == 1
							avel = (elastic_constant * (b.vel-a.vel) + a.vel + b.vel) / 2.0
							bvel = (elastic_constant * (a.vel-b.vel) + a.vel + b.vel) / 2.0
						]]--

						local mass = 1
						local elastic = 0.9

						local avel = vec2(elastic, elastic)
						avel:mul(vec2_sub(p2.speed, p.speed))
						avel:add(p.speed)
						avel:add(p2.speed)
						avel:div(vec2(2.0, 2.0))

						local bvel = vec2(elastic, elastic)
						bvel:mul(vec2_sub(p.speed, p2.speed))
						bvel:add(p.speed)
						bvel:add(p2.speed)
						bvel:div(vec2(2.0, 2.0))

						p.speed = avel
						p2.speed = bvel

						-- local temp = vec2(p.speed.x, p.speed.y)
						-- p.speed = vec2_mul(vec2(p2.speed.x, p2.speed.y), vec2(0.99, 0.99))
						-- p2.speed = vec2_mul(temp, vec2(0.99, 0.99))

						local move = (size - dist) / 2.0
						local diff = vec2_sub(p.pos, p2.pos)
						diff:normalize()
						p.pos = vec2_add(p.pos, vec2_mul(diff, vec2(move, move)))
						p2.pos = vec2_add(p2.pos, vec2_mul(diff, vec2(-move, -move)))
					end
				end
			end

			if p.pos.x + size > window.size.x then
				p.pos.x = window.size.x - size
				if p.speed.x > 0.0 then
					p.speed:mul(vec2(-0.75, 0.9))
				end
			end
			if p.pos.y + size > window.size.y then
				p.pos.y = window.size.y - size
				if p.speed.y > 0.0 then
					p.speed:mul(vec2(0.9, -0.75))
				end
			end
			if p.pos.x < 0.0 then
				p.pos.x = 0.0
				if p.speed.x < 0.0 then
					p.speed:mul(vec2(-0.75, 0.9))
				end
			end
			if p.pos.y < 0.0 then
				p.pos.y = 0.0
				if p.speed.y < 0.0 then
					p.speed:mul(vec2(0.9, -0.75))
				end
			end
		end

		-- if keyboard.space.down then
			counter = counter + spawn_speed
			if counter >= 1.0 then
				local color = vec3(0.0,--0.5 + math.random()*0.5,
								   0.0 + math.random()*0.5,
								   0.8 + math.random()*0.2)
				local speed = vec2(10.0 + math.random()*2.0, -10.0 + math.random()*2.0)
				local pos = vec2(window.size.x/2 + rand_signed()*30, window.size.y/2 + rand_signed()*30)
				local p = particle(pos, color, speed)
				table.insert(particles, p)
				counter = counter - 1
			end
		-- end

		set_color(1.0, 1.0, 1.0, 1.0)
		draw_font("jellee.ttf", 4.0, "particles " .. #particles, 10, 0)

		set_color(1.0, 0.0, 1.0, 1.0)
		local tx, ty = font_dimensions("arial.ttf", 4.0, "frame time " .. round(frame_time, 3))
		-- draw_rect(10, 70, tx, ty)
		set_color(1.0, 1.0, 1.0, 1.0)
		-- draw_font("arial.ttf", 4.0, "frame time " .. round(frame_time, 3), 10, 70)
		draw_font("jellee.ttf", 4.0, "Daisy, Daisy, give me your answer do. I'm half crazy all for the love of you. It won't be a stylish marriage. I can't afford a carriage. But you'll look sweet upon the seat. Of a bicycle built for two.", 10, 70, 1000)

		-- set_color(1.0, 0.0, 1.0, 1.0)
		-- draw_line(100, 200, 150, 250)
		-- draw_line_rect(200, 200, 50, 50)
		-- draw_line_circle(300, 200, 150, 150)
		-- draw_triangle(500, 100, 550, 150, 500, 200)
		-- draw_line_triangle(600, 100, 650, 150, 625, 200)
		-- local tri = vec2(600, 100)
		-- tri:add(vec2(650, 150))
		-- tri:add(vec2(625, 200))
		-- tri:div(vec2(3.0, 3.0))
		-- draw_rect(tri.x, tri.y, 2, 2)

		--[[
		
		draw_line(float x, float y, float x2, float y2)
		draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3)
		draw_line_triangle(float x1, float y1, float x2, float y2, float x3, float y3)
		draw_line_rect(float x, float y, float width, float height)
		draw_line_circle(float x, float y, float w, float h)

		]]--

		-- set_tex_coords(0, 0, 1, 0, 1, 1, 0, 1)
		-- draw_rect_texture(image, 10, 100, 600, 400)

		frame_time = get_seconds() - start_time

		swap_buffers()
	end
end

function init()
	window.title = "Matt's Particles"
end
