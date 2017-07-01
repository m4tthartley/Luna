
local test = load_texture("test.png")

function init()
	window.size = vec2(800, 600)
	window.title = "JELLYMOON TEST"
end

rotation = 0.0

function update()
	rotation = rotation + 0.02
	rotate(rotation)
	set_color(0.8, 0.5, 1.0, 1.0)
	draw_rect(10, 10, 100, 100)
	set_color(0.5, 0.0, 0.5, 1.0)
	draw_rect(120, 10, 100, 100)

	set_color(1.0, 0.0, 0.0, 1.0)
	draw_circle(10, 120, 100, 100)
	set_color(0.0, 0.0, 1.0, 1.0)
	draw_circle(60, 120, 100, 100)

	rotate(rotation)
	set_color(1.0, 0.0, 1.0, 1.0)
	draw_circle(10, 230, 210, 100)

	set_tex_coords(0.0, 0.0,
				   0.5, 0.0,
				   0.5, 0.5,
				   0.0, 0.5)
	rotate(rotation)
	draw_rect_texture(test, 230, 10, 200, 200)

	draw_rect_texture(test, 440, 10, 200, 200)


	-- print(mouse.position.x, mouse.position.y, mouse.position_delta.x, mouse.position_delta.y)
	-- print(mouse.position_delta.x, mouse.position_delta.y)
	-- print(mouse.left.down, mouse.left.pressed, mouse.left.released)
	-- print(mouse.right.down, mouse.right.pressed, mouse.right.released)
	-- print(mouse.left.down, mouse.left.pressed, mouse.right.down, mouse.right.pressed, mouse.middle.down, mouse.middle.pressed)
	-- print(mouse.wheel_delta)
	--print(keyboard.n2.down, keyboard.n4.down)

	if keyboard.w.pressed then print("W") end
	if keyboard.a.pressed then print("A") end
	if keyboard.s.pressed then print("S") end
	if keyboard.d.pressed then print("D") end

	if keyboard.control.pressed then print("control") end
	if keyboard.enter.pressed then print("enter") end
	if keyboard.tab.pressed then print("tab") end
	if keyboard.shift.pressed then print("shift") end

	-- print(time.dt, time.seconds)
end