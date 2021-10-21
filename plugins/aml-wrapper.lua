local aml = require 'aml'

local M = {}

local loop_mt = {}
loop_mt.__index = loop_mt

local function wrap_cb(loop, cb)
	return function(...)
		loop.queue[#loop.queue + 1] = {
			cb=cb,
			args={...}
		}
	end
end

function loop_mt:handler_new(fd, cb)
	return aml.handler_new(fd, wrap_cb(self, cb))
end

function loop_mt:timer_new(duration, cb)
	return aml.timer_new(duration, wrap_cb(self, cb))
end

function loop_mt:ticker_new(period, cb)
	return aml.ticker_new(period, wrap_cb(self, cb))
end

function loop_mt:signal_new(signo, cb)
	return aml.signal_new(signo, wrap_cb(self, cb))
end

function loop_mt:idle_new(cb)
	return aml.idle_new(wrap_cb(self, cb))
end

function loop_mt:run()
	self.do_run = true

	while self.do_run do
		self:poll(-1)
		self:dispatch()

		for _,entry in ipairs(self.queue) do
			entry.cb(unpack(entry.args))
		end

		self.queue = {}
	end
end

function loop_mt:exit()
	self.do_run = false
	self:interrupt()
end

function loop_mt:poll(timeout)
	return self.loop:poll(timeout)
end

function loop_mt:dispatch()
	return self.loop:dispatch()
end

function loop_mt:interrupt()
	return self.loop:interrupt()
end

function loop_mt:start(obj)
	return self.loop:start(obj)
end

function loop_mt:stop(obj)
	return self.loop:stop(obj)
end

function loop_mt:is_started(obj)
	return self.loop:is_started(obj)
end

function M.new()
	local loop = {
		loop = aml.new(),
		queue = {},
		do_run = false,
	}
	setmetatable(loop, loop_mt)
	return loop
end

setmetatable(M, { __index = aml })
return M
