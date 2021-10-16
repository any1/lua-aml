local mt = {}
mt.__index = mt

function mt:await(prom)
	prom:thus(function(result)
		coroutine.resume(self.co, result)
	end)
	return coroutine.yield()
end

function async(cb)
	local ctx = {}
	setmetatable(ctx, mt)
	ctx.co = coroutine.create(cb)
	coroutine.resume(ctx.co, ctx)
	return ctx
end

return async
