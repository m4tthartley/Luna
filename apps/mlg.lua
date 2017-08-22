
-- local _mlg = {
-- 	col_size = 1,
-- 	col_pos = vec2(0, 0)
-- }
-- local _mlg

function vec2(x, y)
	return {
		x = x or 0.0,
		y = y or 0.0,
	}
end

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

local mlg = {
	row_pos = vec2(0, 0),
	col_pos = vec2(0, 0),
	row_size = vec2(0, 0),
	-- col_offset = vec2(0, 0),
	col_size = 0,
	mouse_pos = vec2(0, 0),
	scroll = 0.0,
	mouse = {false, false, false},
	active_input = nil,

	cols = {},
	col_index = 1,

	inputs = {},
	input_index = 1,
}

function mlg:begin()
	self.row_pos = vec2(0, 0)
	self.row_size = vec2(0, 0)
	self.col_pos = vec2(0, 0)
	self.col_size = 0
	self.col_index = 1
	self.input_index = 1
end

function mlg:finish()
	self.mouse[1] = false
	self.mouse[2] = false
	self.mouse[3] = false
end

function mlg:update_mouse_pos(pos)
	self.mouse_pos = pos
end

function mlg:press_mouse_button(button)
	self.mouse[button] = true
	if button == 1 then
		self.active_input = nil
	end
end

function mlg:update_scroll(amount)
	self.scroll = self.scroll + amount
end

function mlg:update_key(key, state)
	if key == "return" and state then
		if self.active_input then
			self.active_input.go = true
		end
	end
	if key == "backspace" and state then
		if self.active_input then
			self.active_input.text = self.active_input.text:sub(1, -2)
		end
	end
end

function mlg:update_text(text)
	if self.active_input then
		self.active_input.text = self.active_input.text .. text
	end
end

function mlg:clear_inputs()
	self.inputs = {}
end

function mlg:row(width, callback)
	local pos = vec2(self.row_pos.x, self.row_pos.y)
	local cpos = vec2(self.col_pos.x, self.col_pos.y)
	local size = vec4(self.row_size.x, self.row_size.y)
	local csize = self.col_size--vec4(self.col_size.x, self.col_size.y)
	self.row_pos = vec2(self.row_pos.x+self.col_pos.x, self.row_pos.y+self.col_pos.y)
	self.col_pos.x = 0
	self.row_size = vec2(0, 0)
	self.col_size = 0
	callback()
	self.row_pos = vec2(pos.x, pos.y)
	self.col_pos = vec2(cpos.x, cpos.y+self.row_size.y)
	self.row_size = vec2(size.x, math.max(size.y, cpos.y+self.row_size.y))
	self.col_size = csize
end

function mlg:col(width, options, callback)
	if type(options) == "function" then
		callback = options
		options = {scroll = false}
	end

	local size
	if type(width) == "table" then size = width
	else size = vec2(width, 0) end

	if self.cols[self.col_index] then else
		self.cols[self.col_index] = { scroll = 0 }
	end

	if options.scroll then
		if self.mouse_pos.x > self.row_pos.x+self.col_pos.x and
			self.mouse_pos.x < self.row_pos.x+self.col_pos.x+size.x and
			self.mouse_pos.y > self.row_pos.y+self.col_pos.y and
			self.mouse_pos.y < self.row_pos.y+self.col_pos.y+size.y then
			draw_rect(self.row_pos.x+self.col_pos.x, self.row_pos.y+self.col_pos.y, size.x, size.y)
			self.cols[self.col_index].scroll = self.cols[self.col_index].scroll + self.scroll*5
			self.scroll = 0
		end
	end

	self.col_pos.y = self.cols[self.col_index].scroll
	self.col_size = size.x
	self.col_index = self.col_index + 1
	callback()
	self.col_pos.x = self.col_pos.x + size.x
	self.row_size.x = math.max(self.row_size.x, self.col_pos.x)
end

function mlg:box(height, color, p)
	set_color(color.r, color.g, color.b, color.a)
	local pad = p or vec2(0, 0)
	local pos = {x = self.row_pos.x+self.col_pos.x+pad.x, y = self.row_pos.y+self.col_pos.y+pad.y,
				 w = self.col_size-(pad.x*2), h = height-(pad.y*2)}
	draw_rect(pos.x, pos.y, pos.w, pos.h)
	self.col_pos.y = self.col_pos.y + height
	self.row_size.y = math.max(self.row_size.y, self.col_pos.y)
	return pos
end

function mlg:text(file, size, str, color, p)
	set_color(color.r, color.g, color.b, color.a)
	-- draw_rect(self.row_pos.x+self.col_pos.x, self.row_pos.y+self.col_pos.y, self.col_size, height)
	local pad = p or vec2(0, 0)
	local dim = font_dimensions(file, size, str, self.col_size-(pad.x*2))
	-- set_color(c.r, c.g, c.b, c.a)
	draw_font(file, size, str, self.row_pos.x+self.col_pos.x+pad.x, self.row_pos.y+self.col_pos.y+pad.y, self.col_size-(pad.x*2))
	self.col_pos.y = self.col_pos.y + dim.y + (pad.y*2)
	self.row_size.y = math.max(self.row_size.y, self.col_pos.y)
end

function mlg:spacer(amount)
	mlg:box(amount, vec4(0, 0, 0, 0))
end

function mlg:is_in_box(pos, box)
	return pos.x > box.x and pos.x < box.x+box.w and
		   pos.y > box.y and pos.y < box.y+box.h
end

function mlg:text_input()
	if self.inputs[self.input_index] then else
		self.inputs[self.input_index] = { active = false, timer = 0, text = "", go = false }
	end

	self.inputs[self.input_index].timer = (self.inputs[self.input_index].timer + 1) % 40

	local pos = mlg:box(30, vec4(0, 0, 0, 0), vec2(0, 0))
	local mouse = mouse_pos()
	if self:is_in_box(mouse, pos) then
		set_color(1, 1, 1, 1)
		if self.mouse[1] then
			self.inputs[self.input_index].active = true
			self.active_input = self.inputs[self.input_index]
		end
	else
		set_color(1, 1, 1, 0.5)
		if self.mouse[1] then
			self.inputs[self.input_index].active = false
		end
	end
	if self.inputs[self.input_index].active then
		set_color(1, 0.5, 0.5, 1)
	end
	draw_line_rect(pos.x, pos.y, pos.w, pos.h)
	set_color(1, 1, 1, 1)
	local str = self.inputs[self.input_index].text
	if self.inputs[self.input_index].active and 
	   self.inputs[self.input_index].timer < 20 then
		str = str .. "|"
	end
	draw_font("jellee.ttf", 1.0, str, pos.x+10, pos.y+4, pos.w - 20)

	local go = self.inputs[self.input_index].go
	self.inputs[self.input_index].go = false
	local text = self.inputs[self.input_index].text

	self.input_index = self.input_index + 1

	return {
		go = go,
		text = text,
	}
end

--[[
local mlg = {
	row_stack = {},
	last_row_height = 0,
}

function mlg:begin()
	self.last_row_height = 0
end

function mlg:_new_row(width)
	return {
		col_size = 1,
		col_pos = vec2(0, 0),
		pad = 0,
		pos = vec2(0, 0),
		size = 0,
		width = width,
		max_height = 0,
	}
end

function mlg:begin_row(width)
	table.insert(self.row_stack, self:_new_row(width))
	local row = self.row_stack[table.getn(self.row_stack)]
	row.col_size = 0
	row.col_pos = vec2(0, 0)
	if table.getn(self.row_stack) > 1 then
		row.pos = vec2(0, self.row_stack[table.getn(self.row_stack)-1].max_height + self.row_stack[table.getn(self.row_stack)-1].pad)
		-- self.row_stack[table.getn(self.row_stack)-1].pos.y
		-- + self.row_stack[table.getn(self.row_stack)-1].max_height
		row.size = 1280/self.row_stack[table.getn(self.row_stack)-1].col_size
	else
		row.pos = vec2(0, self.last_row_height)
		row.size = 1280
	end

	self.last_row_height = 0
end

function mlg:end_row()
	-- self.col_size = 0
	-- self.col_pos = vec2(0, 0)
	local row = self.row_stack[table.getn(self.row_stack)]
	self.last_row_height = row.max_height
	table.remove(self.row_stack, table.getn(self.row_stack))
end

function mlg:column(size)
	local row = self.row_stack[table.getn(self.row_stack)]
	row.col_pos.y = row.pos.y
	if row.col_size > 0 then
		row.col_pos.x = row.col_pos.x + (1.0 / row.col_size)
	end
	row.col_size = size
	row.pad = 0
end

function mlg:box(height, c)
	local row = self.row_stack[table.getn(self.row_stack)]
	set_color(c.r, c.g, c.b, c.a)
	-- set_color(1, 0, 0, 1)
	draw_rect(row.col_pos.x*row.size + row.pad, row.col_pos.y + row.pad, ((row.size/row.col_size) - (row.pad*2)), height)

	row.col_pos.y = row.col_pos.y + height + row.pad
	row.max_height = math.max(row.max_height, row.col_pos.y)
end

function mlg:text(file, size, str, c)
	local row = self.row_stack[table.getn(self.row_stack)]
	local dim = font_dimensions(file, size, str, row.size/row.col_size)
	-- set_color(0.2, 0.2, 0.2, 1)
	-- draw_rect(row.col_pos.x*row.size + row.pad, row.col_pos.y + row.pad, dim.x, dim.y)
	set_color(c.r, c.g, c.b, c.a)
	draw_font(file, size, str, row.col_pos.x*row.size + row.pad, row.col_pos.y + row.pad, (row.size/row.col_size) - (row.pad*2))
	row.col_pos.y = row.col_pos.y + dim.y + row.pad
	row.max_height = math.max(row.max_height, row.col_pos.y)
end

function mlg:set_pad(amount)
	local row = self.row_stack[table.getn(self.row_stack)]
	row.pad = amount
end
]]--

return mlg