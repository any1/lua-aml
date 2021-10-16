function async(cb)
	local co = coroutine.create(cb)

	local function await(prom)
		prom:thus(function(result)
			coroutine.resume(co, result)
		end)
		return coroutine.yield()
	end

	coroutine.resume(co, await)
end

return async
