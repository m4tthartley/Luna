
local particles = {}
local counter = 0

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

function update()
	for i,p in ipairs(particles) do
		p.life = p.life - (10.0 * time.dt)
		if p.life < 0 then table.remove(particles, i) end

		p.pos.x = p.pos.x + (p.speed.x * time.dt) * 40.0
		p.pos.y = p.pos.y + (p.speed.y * time.dt) * 40.0
		p.speed.y = p.speed.y + (9.81*time.dt)

		if p.pos.x > window.size.x and p.speed.x > 0.0 then
			p.speed.x = p.speed.x * -0.75
			p.speed.y = p.speed.y * 0.9
		end
		if p.pos.y > window.size.y and p.speed.y > 0.0 then
			p.speed.y = p.speed.y * -0.75
			p.speed.x = p.speed.x * 0.9
		end
		if p.pos.x < 0.0 and p.speed.x < 0.0 then
			p.speed.x = p.speed.x * -0.75
			p.speed.y = p.speed.y * 0.9
		end
		if p.pos.y < 0.0 and p.speed.y < 0.0 then
			p.speed.y = p.speed.y * -0.75
			p.speed.x = p.speed.x * 0.9
		end

		set_color(p.color.x, p.color.y, p.color.z, 1.0)
		draw_rect(p.pos.x, p.pos.y, 4, 4)
	end

	counter = counter - 1
	if counter < 0 then
		local color = vec3(0.5 + math.random()*0.5,
						   0.5 + math.random()*0.5,
						   0.5 + math.random()*0.5)
		local speed = vec2(10.0 + math.random()*2.0, -10.0 + math.random()*2.0)
		local pos = vec2(window.size.x/2 + rand_signed()*30, window.size.y/2 + rand_signed()*30)
		local p = particle(pos, color, speed)
		table.insert(particles, p)
		counter = 2
	end
end

function init()
	window.title = "Matt's Particles"
end
