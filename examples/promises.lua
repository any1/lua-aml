#!/usr/bin/luajit

local promise = require 'promise'
local aml = require 'aml'

local loop = aml.new()

function sleep(duration)
	return promise(function(resolve, reject)
		local timer = aml.timer_new(duration, function()
			resolve()
		end)
		loop:start(timer)
	end)
end

print 'I promise this will only take a second...'

sleep(1000):thus(function()
	print 'Done!'
	loop:exit()
end)

loop:run()
