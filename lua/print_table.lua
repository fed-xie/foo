local print = print
local tostring = tostring
local type = type
local pairs = pairs
local insert_table = table.insert
local concat_table = table.concat
local rep_str = string.rep

local function is_table(t, name)
	if "table" ~= type(t) then
		if name then
			print(tostring(name) .. " = [" .. tostring(t) .. "]")
		else
			print(t)
		end
		return false
	end
	return true
end

local function print_rm(t, name)	
	if not is_table(t, name) then return end
	
	local cache = { [t] = "." } --To filter cycle reference
	local output = {}
	local maxlines = 30
	local space = "   "
	local outtop = 1
	local function insert_output(str)
		output[outtop] = str
		outtop = outtop + 1
		if maxlines < outtop then
			print(concat_table(output, "\n"))
			outtop = 1
		end
	end
	local function _dump(t, depth, name)
		for k, v in pairs(t) do
			local kname = tostring(k)
			local text
			if cache[v] then --Cycle reference
				insert_output(rep_str(space, depth) .. 
				"|- " .. kname .. "\t{" .. cache[v] .. "}")
			elseif "table" == type(v) then
				local new_table_key = name .. "." .. kname
				cache[v] = new_table_key
				insert_output(rep_str(space, depth) .. "|- " .. kname)
				_dump(v, depth + 1, new_table_key)
			else
				insert_output(rep_str(space, depth) .. 
				"|- " .. kname .. "\t[" .. tostring(v) .. "]")
			end
		end
		local mt = getmetatable(t)
		if mt then
			if cache[mt] then
				insert_output(rep_str(space, depth) .. 
				"|- {__metatable}\t{" .. cache[v] .. "}")
			else
				local new_table_key = name .. ".{__metatable}"
				cache[mt] = new_table_key
				insert_output(rep_str(space, depth) .. "|- {__metatable}")
				_dump(mt, depth + 1, new_table_key)
			end
		end
	end
	if name then print(name) end
	_dump(t, 0, "")
	print(concat_table(output, "\n", 1, outtop - 1))
end

local function print_r(t, name)
	if not is_table(t, name) then return end
	
	local cache = { [t] = "." } --To filter cycle reference
	local output = {}
	local maxlines = 30
	local space = "   "
	local function insert_output(str)
		insert_table(output, str)
		if maxlines <= #output then --flush output
			print(concat_table(output, "\n"))
			output = {}
		end
	end
	local function _dump(t, depth, name)
		for k, v in pairs(t) do
			local kname = tostring(k)
			local text
			if cache[v] then --Cycle reference
				insert_output(rep_str(space, depth) .. 
				"|- " .. kname .. "\t{" .. cache[v] .. "}")
			elseif "table" == type(v) then
				local new_table_key = name .. "." .. kname
				cache[v] = new_table_key
				insert_output(rep_str(space, depth) .. "|- " .. kname)
				_dump(v, depth + 1, new_table_key)
			else
				insert_output(rep_str(space, depth) .. 
				"|- " .. kname .. "\t[" .. tostring(v) .. "]")
			end
		end
	end
	if name then
		print(name)
	end
	_dump(t, 0, "")
	print(concat_table(output, "\n"))
end

local function print_m(t, name)
	if not is_table(t, name) then return end
	
	if name then print(name) end
	for k, v in pairs(t) do
		print("|- " .. tostring(k) ..  "\t[" .. tostring(v) .. "]")
	end
	meta = getmetatable(t)
	if meta then
		print("|- {__metatable}")
		for k, v in pairs(meta) do
			print("   |- " .. tostring(k) ..  "\t[" .. tostring(v) .. "]")
		end
	end
end

local function print_t(t, name)
	if not is_table(t, name) then return end
	
	if name then print(name) end
	for k, v in pairs(t) do
		print("|- " .. tostring(k) ..  " [" .. tostring(v) .. "]")
	end
end

local function print_c(c, name) --Print table as a class
	if not is_table(c, name) then return end
	
	local funcs = {}
	local vars = {}

	local function _dump(t)
		for k, v in pairs(t) do
			if "function" == type(v) and not funcs[k] then
				funcs[k] = v
			elseif not vars[k] then
				vars[k] = v
			end
		end
		local mt = getmetatable(t)
		if mt then
			_dump(mt)
			if "table" == type(mt.__index) then
				return _dump(mt.__index)
			end
		end
	end
	
	_dump(c)
	vars.__index = undef --Compatibility for Lua5.4
	
	if name then print(name) end
	print("methods")
	for k in pairs(funcs) do
		print("|- " .. tostring(k))
	end
	print("vars")
	for k, v in pairs(vars) do
		print("|- " .. tostring(k) .. "\t[" ..tostring(v) .. "]")
	end
	return vars
end

table.print_rm = print_rm
table.print_r = print_r
table.print_m = print_m
table.print_c = print_c
table.print = print_t
