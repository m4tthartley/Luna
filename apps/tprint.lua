
-- tprint: Print Tables
-- version: 0.1

local tprint = {
	indent = 0
}

function tprint:inner(t)
	for k,v in pairs(t) do
		local tab = ""
		for i=1, self.indent do tab = tab .. "    " end
		if type(v) == "table" then
			print(tab .. k .. " = {")
			self.indent = self.indent + 1
			self:inner(v)
			self.indent = self.indent - 1
			print(tab .. "}")
		else
			if type(v) == "string" then v = "\""..v.."\"" end
			if type(v) == "boolean" then v = (v and "true" or "false") end
			if type(v) == "function" then v = "function()" end
			print(tab .. k .. " = " .. v)
		end
	end
end

function tprint:main(t)
	if type(t) == "table" then
		print("{")
		self.indent = self.indent + 1
		self:inner(t)
		self.indent = self.indent - 1
		print("}")
	else
		print("dude, that's a " .. type(t) .. ", not a table")
	end
end

setmetatable(tprint, {
	__call = tprint.main,
})

return tprint
