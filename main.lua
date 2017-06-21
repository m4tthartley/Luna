
-- local test = video.loadTexture("test.png")

-- function render()
-- 	video.clear()
-- 	--video.renderQuad(100, 100, 100, 100)

-- 	video.color(1, 1, 1, 1)
-- 	video.renderSprite(test, 0, 0, 64, 64, 50, 300, 4.0)

-- 	video.color(1, 0, 1, 1)
-- 	video.renderElipse(150, 150, 100, 100, 1.3)
-- end

-- function tick()
-- end

-- function secondstep()
-- end

local test = load_texture("test.png")

function init()
	window.size = vec2(800, 600)
	window.title = "JELLYMOON TEST"
	window.fullscreen = false

	print(window.size.x)
	print(window.size.y)
end

function update()
	set_tex_coords(0.0, 0.0,
				   0.5, 0.0,
				   0.5, 0.5,
				   0.0, 0.5)
	draw_rect_texture(test, 230, 10, 200, 200)

	draw_rect_texture(test, 440, 10, 200, 200)

	set_color(1.0, 0.0, 1.0, 1.0)
	draw_rect(10, 10, 100, 100)
	set_color(0.0, 1.0, 1.0, 1.0)
	draw_rect(120, 10, 100, 100)

	set_color(1.0, 0.0, 0.0, 1.0)
	draw_circle(10, 120, 100, 100)
	set_color(0.0, 0.0, 1.0, 1.0)
	draw_circle(60, 120, 100, 100)

	-- rotate(10)
	set_color(1.0, 0.0, 1.0, 1.0)
	draw_circle(10, 230, 210, 100)

	print(mouse.position.x, mouse.position.y)
end