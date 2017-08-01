
-- Chat app

local tprint = loadstring(file_request("tprint.lua"))()
local mlg = loadstring(file_request("mlg.lua"))()

function vec4(x, y, z, w)
	return {
		x = x or 0.0,
		y = y or 0.0,
		z = z or 0.0,
		w = w or 0.0,

		r = x or 0.0,
		g = y or 0.0,
		b = z or 0.0,
		a = w or 0.0,
	}
end

tprint(_mlg)

while true do
	clear_rect(0, 0, 1280, 720)
	-- draw_rect(math.random()*1000, math.random()*1000, 10, 10)

	mlg:begin()

	mlg:begin_row(1000)
		mlg:column(8)
		mlg:set_pad(10)
		mlg:box(50, vec4(1, 0, 1, 1))
		mlg:box(150, vec4(1, 1, 0, 1))

		mlg:column(4)
		mlg:set_pad(10)
		mlg:box(100, vec4(0, 1, 1, 1))
		mlg:box(100, vec4(0.5, 0.5, 1, 1))
		mlg:box(100, vec4(1, 0.5, 0.5, 1))

		mlg:column(6)
		mlg:box(150, vec4(0.5, 1, 0.5, 1))
		mlg:box(100, vec4(1, 0, 1, 1))

		mlg:column(6)
		mlg:set_pad(10)
		mlg:text("jellee.ttf", 1.0, "Hello, this the MLG library.", vec4(1, 1, 1, 1))
		mlg:text("jellee.ttf", 1.0, "Do you like it?", vec4(1, 1, 1, 1))
		mlg:text("jellee.ttf", 1.0, "I'm gonna write lots of text now, just so I can test this stuff. See if it wraps properly and is sapced properly, you know the usual stuff.", vec4(1, 1, 1, 1))
		-- mlg:box(100, vec4(0.5, 1, 0.5, 1))

		mlg:column(8)
		mlg:box(50, vec4(1, 0, 1, 1))
		mlg:box(150, vec4(1, 1, 0, 1))
	mlg:end_row()

	mlg:begin_row(1000)
		mlg:column(4)
		mlg:set_pad(10)
		mlg:box(100, vec4(0, 1, 1, 1))
		mlg:begin_row(200)
			mlg:column(4)
			mlg:set_pad(10)
			mlg:box(30, vec4(1, 0, 0, 1))
			mlg:column(4)
			mlg:set_pad(10)
			mlg:box(30, vec4(0, 1, 0, 1))
			mlg:column(4)
			mlg:set_pad(10)
			mlg:box(30, vec4(0, 0, 1, 1))
			mlg:column(4)
			mlg:set_pad(10)
			mlg:box(30, vec4(1, 1, 1, 1))
		mlg:end_row()
		-- mlg:box(100, vec4(0.5, 0.5, 1, 1))
		-- mlg:box(100, vec4(1, 0.5, 0.5, 1))

		mlg:column(4)
		mlg:set_pad(10)
		mlg:text("jellee.ttf", 2.0, "Lovely row and column layouts!", vec4(1, 1, 1, 1))
		mlg:text("jellee.ttf", 2.0, "Nested things!", vec4(1, 1, 1, 1))
	mlg:end_row()

	present()
	sleep(16)
end