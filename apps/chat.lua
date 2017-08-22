
-- Chat app

local tprint = loadstring(load_module("tprint.lua"))()
local mlg = loadstring(load_module("mlg.lua"))()

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

local messages = {}
local users = {}
local login = {}

function gettoken(u, p)
	local res = http_post("http://138.68.149.32/luna-chat/web/app.php/api/login",
	[[{
		"username": "]]..u..[[",
		"password": "]]..p..[["
	}]])
	return loadstring(res)()
end
function getmessages()
	local res = http_post("http://138.68.149.32/luna-chat/web/app.php/api/getmessages",
		[[{
			"token": ]]..login.token..[[
		}]])
	-- res = string.gsub(res, "\n", "[N]")
	-- print(res)
	return loadstring(res)()
end
function getuser(id)
	local res = http_post("http://138.68.149.32/luna-chat/web/app.php/api/getuser",
		[[{
			"token": ]]..login.token..[[,
			"user_id": ]]..id..[[
		}]])
	return loadstring(res)()
end
function getnewmessages(id)
	local res = http_post("http://138.68.149.32/luna-chat/web/app.php/api/getnewmessages",
		[[{
			"token": ]]..login.token..[[,
			"message_id": ]]..id..[[
		}]])
	return loadstring(res)()
end
function postmessage(str)
	local res = http_post("http://138.68.149.32/luna-chat/web/app.php/api/postmessage",
	[[{
		"token": ]]..login.token..[[,
		"message": "]]..str..[["
	}]])
	print(res)
end

local STATE_LOGIN = 1
local STATE_APP = 2
local last_time = get_seconds()
local state = STATE_LOGIN
local login_error = ""

while true do
	clear_rect(0, 0, 1280, 720)

	local time = get_seconds()
	
	local event = next_event()
	while event do
		if event.type == "mouse_wheel" then
			mlg:update_scroll(event.amount)
		end
		if event.type == "mouse_down" then
			mlg:press_mouse_button(event.button)
		end
		if event.type == "key_down" then
			-- print("key down: " .. event.key)
			mlg:update_key(event.key, true)
		end
		if event.type == "key_up" then
			-- print("key up: " .. event.key)
			mlg:update_key(event.key, false)
		end
		if event.type == "text" then
			-- print(event.text)
			mlg:update_text(event.text)
		end
		event = next_event()
	end
	mlg:update_mouse_pos(mouse_pos())


	mlg:begin()

	if state == STATE_LOGIN then		
		mlg:row(200, function()
			mlg:col(100, function()
			end)
			mlg:col(200, function()
				mlg:spacer(50)
				mlg:text("jellee.ttf", 1, "Username", vec4(1, 1, 1, 1), vec2(10, 10))
				local u = mlg:text_input()
				mlg:spacer(10)
				mlg:text("jellee.ttf", 1, "Password", vec4(1, 1, 1, 1), vec2(10, 10))
				local p = mlg:text_input()

				if u.go or p.go then
					login = gettoken(u.text, p.text)
					if login.token then
						messages = getmessages()
						for i,msg in ipairs(messages) do
							if not users[msg.user_id] then
								users[msg.user_id] = getuser(msg.user_id)
							end
						end
						state = STATE_APP
						mlg:clear_inputs()
					end
				end

				mlg:spacer(10)
				if login.error then
					mlg:text("jellee.ttf", 1, login.error, vec4(1, 1, 1, 1), vec2(10, 10))
				end
			end)
		end)
	end

	if state == STATE_APP then
		if time - last_time > 5.0 then
			last_time = time
			local new = getnewmessages(messages[#messages].id)
			if #new > 0 then tprint(new) end
			for i,v in ipairs(new) do
				table.insert(messages, v)
			end
		end

		mlg:row(200, function()
			mlg:col(200, function()
				-- mlg:box(20, vec4(0.1, 0.1, 0.1, 1))
				mlg:box(20, vec4(0.6, 0.7, 0.8, 1))
				mlg:text("jellee.ttf", 2, "Online", vec4(0.6, 0.7, 0.8, 1), vec2(10, 10))

				for i,v in pairs(users) do
					mlg:text("jellee.ttf", 1, v.username, vec4(0.6, 0.7, 0.8, 1), vec2(10, 2))
				end
			end)
			mlg:col(20, function()
			end)
			mlg:col(600, function()
				mlg:box(20, vec4(0.6, 0.7, 0.8, 1))
				mlg:text("jellee.ttf", 2, "Chat", vec4(0.6, 0.7, 0.8, 1), vec2(10, 10))
				mlg:box(2, vec4(0.1, 0.1, 0.1, 1))

				mlg:row(600, function()
					mlg:col(vec2(600, 500), {scroll = true}, function()
						-- for i=(#messages-50),msg in ipairs(messages) do
						for i=math.max(#messages-10, 1), #messages do
							local msg = messages[i]
							mlg:box(20, vec4(0, 0, 0, 1))

							mlg:row(600, function()
								mlg:col(100, function()
									mlg:text("jellee.ttf", 1, users[msg.user_id].username..":", vec4(0.6, 0.7, 0.8, 1), vec2(10, 1))
								end)
								mlg:col(500, function()
									mlg:text("jellee.ttf", 1, msg.text, vec4(1, 1, 1, 1), vec2(10, 1))
									mlg:row(500, function()
										mlg:col(500, function()
											mlg:text("jellee.ttf", 0.7, msg.created_at, vec4(0.5, 0.5, 0.5, 1), vec2(10, 1))
										end)
									end)
								end)
							end)
						end

						mlg:row(600, function()
							mlg:col(600, function()
								mlg:spacer(20)
								local send = mlg:text_input()
								if send.go and #send.text > 0 then
									mlg.active_input.text = ""
									-- print("go: " .. send.text)
									postmessage(send.text)
								end
							end)
						end)
					end)
				end)
			end)
		end)
	end

	mlg:finish()

	present()
	sleep(16)
end