-- States: pending, fulfilled, rejected

local mt = {}
mt.__index = mt

local promise

function is_promise(v)
	return v ~= nil and type(v) == 'table' and getmetatable(v) == mt
end

function mt:resolve(result)
	if (self.state ~= 'pending') then
		error("Promise is not pending")
	end

	if (is_promise(result)) then
		local p = result
		rawset(p, 'on_fulfilled', function(r)
			self:resolve(r)
		end)
		return
	end

	rawset(self, 'state', 'fulfilled')
	rawset(self, 'result', result)

	if (self.on_fulfilled) then
		self.on_fulfilled(result)
	end
end

function mt:reject()
	if (self.state ~= 'pending') then
		error("Promise is not pending")
	end

	rawset(self, 'state', 'rejected')

	if (self.on_rejected) then
		self:on_rejected()
	end
end

function mt:thus(cb)
	return promise(function(resolve, reject)
		rawset(self, 'on_fulfilled', function(result)
			resolve(cb(result))
		end)
	end)
end

function create_promise()
	local p = {}

	p.state = 'pending'

	setmetatable(p, mt)

	return p
end

promise = function(cb)
	local p = create_promise()

	cb(function(result) p:resolve(result) end, function() p:reject() end)

	return p
end

return promise
