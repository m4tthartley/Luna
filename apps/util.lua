
local util = {
	indent = 0
}

function tprint_inner(k, t)
	util.indent = util.indent + 1
	local tab = ""
	for i=1, util.indent do
		tab = tab .. "\t"
	end
	print(tab .. k)
	util.indent = util.indent - 1
end

function util.tprint(t)
	print("{")
	for k,v in pairs(t) do
		tprint_inner(k, v)
		-- print(k)
	end
	print("}")
end

return util