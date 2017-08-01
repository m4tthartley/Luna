
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

return mlg