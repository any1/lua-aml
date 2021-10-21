local promise = require 'promise'

function async(cb)
	return promise(function(resolve)
		local co = coroutine.create(function(...)
			resolve(cb(...))
		end)

		local function await(prom)
			prom:thus(function(result)
				coroutine.resume(co, result)
			end)
			return coroutine.yield()
		end

		coroutine.resume(co, await)
	end)
end

return async
