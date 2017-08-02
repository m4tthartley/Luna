
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

function mlg_test_stuff()
	mlg:begin()

	mlg:row(500, function()
		mlg:col(100, function()
			mlg:box(50, vec4(1, 0, 1, 1))
			mlg:box(50, vec4(0, 1, 1, 1))
		end)
		mlg:col(50, function()
			mlg:box(75, vec4(1, 1, 0, 1))
			mlg:box(50, vec4(0.5, 0.5, 1, 1))
		end)
		mlg:col(50, function()
			mlg:box(50, vec4(1, 0.5, 0.5, 1))
			mlg:box(50, vec4(0.5, 1, 0.5, 1))
			mlg:box(50, vec4(1, 0, 1, 1))
		end)
		mlg:col(200, function()
			mlg:row(100, function()
				mlg:col(25, function()
					mlg:box(25, vec4(1, 0, 0, 1))
					mlg:row(10, function()
						mlg:col(10, function()
							mlg:box(10, vec4(0, 1, 0, 1))
							mlg:box(10, vec4(1, 0, 0, 1))
							mlg:box(10, vec4(1, 1, 0, 1))
						end)
						mlg:col(10, function()
							mlg:box(10, vec4(0, 0, 1, 1))
						end)
					end)
				end)
				mlg:col(25, function()
					mlg:box(25, vec4(0, 1, 0, 1))
				end)
				mlg:col(25, function()
					mlg:box(25, vec4(0, 0, 1, 1))
				end)
				mlg:col(25, function()
					mlg:box(25, vec4(1, 1, 1, 1))
				end)
			end)
			mlg:box(50, vec4(1, 1, 0, 1))
		end)
	end)

	mlg:row(500, function()
		mlg:col(100, function()
			mlg:box(50, vec4(1, 0, 0, 1))
			mlg:row(100, function()
				mlg:col(50, function()
					mlg:box(25, vec4(0, 1, 0, 1))
					mlg:row(50, function()
						mlg:col(15, function()
							mlg:box(10, vec4(1, 1, 0, 1))
							mlg:row(50, function()
								mlg:col(5, function()
									mlg:box(5, vec4(1, 0, 0, 1))
								end)
								mlg:col(5, function()
									mlg:box(5, vec4(0, 1, 0, 1))
								end)
							end)
						end)
						mlg:col(15, function()
							mlg:box(10, vec4(0, 1, 1, 1))
						end)
					end)
				end)
				mlg:col(50, function()
					mlg:box(25, vec4(0, 0, 1, 1))
				end)
			end)
			mlg:box(20, vec4(0, 0, 1, 1))
		end)
		mlg:col(100, function()
			mlg:box(50, vec4(0, 1, 0.5, 1), 0)
			mlg:box(50, vec4(1, 1, 0, 1), 0)
		end)
		mlg:col(100, function()
			mlg:text("jellee.ttf", 1.0, "This love has taken it's toll on me, she said goodbye, too many times before", vec4(1, 1, 1, 1), 5)
		end)
	end)

	mlg:row(100, function()
		mlg:col(100, function()
			mlg:box(50, vec4(0, 0, 0, 1))
		end)
	end)

	mlg:row(500, function()
		mlg:col(100, function()
			mlg:box(150, vec4(1, 0, 0, 1))
			mlg:box(150, vec4(0, 1, 0, 1))
		end)
		mlg:col(100, function()
			mlg:box(50, vec4(0, 0, 1, 1))
			mlg:row(100, function()
				mlg:col(100, function()
					mlg:text("jellee.ttf", 1.0, "Her heart is breaking in front of me, and I have no choice, cause I can't say goodbye anymore", vec4(1, 1, 1, 1), 10)
				end)
			end)
			mlg:box(50, vec4(1, 1, 0, 1))
		end)
		mlg:col(100, function()
			mlg:box(150, vec4(1, 0, 1, 1))
			mlg:box(150, vec4(0, 1, 1, 1))
		end)
	end)
end

while true do
	clear_rect(0, 0, 1280, 720)
	-- draw_rect(math.random()*1000, math.random()*1000, 10, 10)

	--[[
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
	]]--

	-- mlg_test_stuff()

	local messages = {
		{
			user = "jtparrett",
			text = "Hello?",
		},
		{
			user = "spartanslayer117",
			text = "sup",
		},
		{
			user = "matthartley",
			text = "hi",
		},
		{
			user = "jtparrett",
			text = "Pub?",
		},
		{
			user = "matthartley",
			text = ": )",
		},
		{
			user = "spartanslayer117",
			text = "I was so high, I did not recognize\nThe fire burning in her eyes\nThe chaos that controlled my mind\nWhispered goodbye as she got on a plane\nNever to return again\nBut always in my heart",
		},
	}

	mlg:begin()
	mlg:row(200, function()
		mlg:col(200, function()
			mlg:box(20, vec4(0.1, 0.1, 0.1, 1))
			mlg:text("jellee.ttf", 2, "Online", vec4(0.6, 0.7, 0.8, 1), vec2(10, 10))

			mlg:text("jellee.ttf", 1, "jtparrett", vec4(0.6, 0.7, 0.8, 1), vec2(10, 2))
			mlg:text("jellee.ttf", 1, "spartanslayer117", vec4(0.6, 0.7, 0.8, 1), vec2(10, 2))
			mlg:text("jellee.ttf", 1, "matthartley", vec4(0.6, 0.7, 0.8, 1), vec2(10, 2))
			mlg:text("jellee.ttf", 1, "jellybot", vec4(0.6, 0.7, 0.8, 1), vec2(10, 2))
		end)
		mlg:col(20, function()
		end)
		mlg:col(600, function()
			mlg:box(20, vec4(0.1, 0.1, 0.1, 1))
			mlg:text("jellee.ttf", 2, "Chat", vec4(0.6, 0.7, 0.8, 1), vec2(10, 10))
			mlg:box(2, vec4(0.1, 0.1, 0.1, 1))

			for i,msg in ipairs(messages) do
				mlg:box(20, vec4(0, 0, 0, 1))
				-- mlg:box(1, vec4(0.1, 0.1, 0.1, 1), vec2(10, 0))
				-- mlg:text("jellee.ttf", 1, msg.user..":", vec4(0.6, 0.7, 0.8, 1), vec2(10, 1))
				-- mlg:text("jellee.ttf", 1, msg.text, vec4(1, 1, 1, 1), vec2(10, 1))
				mlg:row(600, function()
					mlg:col(200, function()
						mlg:text("jellee.ttf", 1, msg.user..":", vec4(0.6, 0.7, 0.8, 1), vec2(10, 1))
					end)
					mlg:col(400, function()
						mlg:text("jellee.ttf", 1, msg.text, vec4(1, 1, 1, 1), vec2(10, 1))
					end)
				end)
			end
		end)
	end)

	present()
	sleep(16)
end