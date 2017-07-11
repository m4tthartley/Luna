
local util = dofile("util.lua")

function init()
end

local pos = vec2(10, 10)
local speed = vec2(10, 10)

function run()
	util.tprint(pos)
	util.tprint(speed)

	while true do
		update()
		get_input()

		pos.x = pos.x + speed.x * time.dt * 60.0
		pos.y = pos.y + speed.y * time.dt * 60.0
		if (pos.x > window.size.x - 100) then speed.x = -10 end
		if (pos.y > window.size.y - 100) then speed.y = -10 end
		if (pos.x < 0) then speed.x = 10 end
		if (pos.y < 0) then speed.y = 10 end

		set_color(1, 0, 1, 1)
		draw_rect(pos.x, pos.y, 100, 100)

		swap_buffers()
		-- sleep(60)
	end
end