
print("MLG")

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
}

function mlg:begin()
	self.row_pos = vec2(0, 0)
	self.row_size = vec2(0, 0)
	self.col_pos = vec2(0, 0)
	self.col_size = 0
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

function mlg:col(width, callback)
	self.col_pos.y = 0
	self.col_size = width
	callback()
	self.col_pos.x = self.col_pos.x + width
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
	local pos = mlg:box(30, vec4(0, 0, 0, 0), vec2(0, 0))
	local mouse = mouse_pos()
	if self:is_in_box(mouse, pos) then
		set_color(1, 1, 1, 1)
	else
		set_color(1, 1, 1, 0.5)
	end
	draw_line_rect(pos.x, pos.y, pos.w, pos.h)
	set_color(1, 1, 1, 1)
	draw_font("jellee.ttf", 1.0, "testing", pos.x+10, pos.y+4, pos.w - 20)
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