
local texture = load_texture('kitten3.png')

local x = 10
local y = 10
local sx = 1
local sy = 1
local r = 0
while true do
	-- set_color(1, 0, 1, 1)
	-- draw_rect(x, y, 100, 100)
	-- set_color(0, 1, 1, 1)
	-- draw_rect(x + 50, y, 100, 100)

	

	clear_color(0.0, 0.1, 0.3, 1.0)
	clear_rect(0, 0, 1280, 720)

	set_color(1, 0, 1, 1)
	draw_rect(0, 0, 50, 50)
	draw_line_rect(100, 0, 50, 50)
	draw_triangle(200, 0, 200, 50, 250, 50)
	draw_line_triangle(300, 0, 300, 50, 350, 50)
	set_color(0, 1, 1, 1)
	draw_circle(400, 0, 50, 50)
	draw_line_circle(500, 0, 50, 50)
	draw_line(600, 0, 650, 50)

	set_color(1, 1, 1, 1)
	draw_rect_texture(texture, 0, 100, 250, 250)
	set_tex_coords(0.25, 0.0,
				   0.75, 0.0,
				   0.75, 0.5,
				   0.25, 0.5)
	rotate(r)
	draw_rect_texture(texture, 300, 100, 250, 250)

	x = x + sx * 5
	y = y + sy * 5
	if x > 1280-50 then sx = -1 end
	if x < 0 then sx = 1 end
	if y > 720-50 then sy = -1 end
	if y < 0 then sy = 1 end
	set_color(1, 1, 0, 1)
	draw_rect(x, y, 50, 50)

	local w_key = key_state('w')
	-- print("w " .. (w_key and "true" or "false"))

	r = r + 0.05

	present()
	sleep(16)
end