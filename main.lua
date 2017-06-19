
local test = video.loadTexture("test.png")

function render()
	video.clear()
	--video.renderQuad(100, 100, 100, 100)

	video.color(1, 1, 1, 1)
	video.renderSprite(test, 0, 0, 64, 64, 50, 300, 4.0)

	video.color(1, 0, 1, 1)
	video.renderElipse(150, 150, 100, 100, 1.3)
end

function tick()
end

function secondstep()
end