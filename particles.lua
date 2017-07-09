
local particles = {}

function particle(p, c, s)
	return {
		pos = p,
		life = 100,
		color = c,
		speed = s,
	}
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
		end
		if p.pos.y > window.size.y and p.speed.y > 0.0 then
			p.speed.y = p.speed.y * -0.75
		end
		if p.pos.x < 0.0 and p.speed.x < 0.0 then
			p.speed.x = p.speed.x * -0.75
		end
		if p.pos.y < 0.0 and p.speed.y < 0.0 then
			p.speed.y = p.speed.y * -0.75
		end

		set_color(p.color.x, p.color.y, p.color.z, 1.0)
		draw_rect(p.pos.x, p.pos.y, 4, 4)
	end

	local color = vec3(0.5 + math.random()*0.5,
					   0.5 + math.random()*0.5,
					   0.5 + math.random()*0.5)
	local speed = vec2(8.0 + math.random()*2.0, -8.0 + math.random()*2.0)
	local p = particle(vec2(math.random() * window.size.x, math.random() * window.size.y), color, speed)
	table.insert(particles, p)
end

function init()
end
