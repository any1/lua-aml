#!/usr/bin/luajit

local aml = require 'aml'
local promise = require 'promise'
local async = require 'async'

local SIGINT = 2

local loop = aml.new()

local do_run = true
local event_queue = {}

function emit_event(cb)
	event_queue[#event_queue + 1] = cb
end

function sleep(duration)
	return promise(function(resolve, reject)
		local timer = aml.timer_new(duration, function()
			emit_event(resolve)
		end)
		loop:start(timer)
	end)
end

function read_line_from_stdin()
	return promise(function(resolve, reject)
		local handler = aml.handler_new(0, function(handler)
			emit_event(function()
				resolve(io.read())
			end)
			loop:stop(handler)
		end)
		loop:start(handler)
	end)
end

function signal(signo)
	return promise(function(resolve)
		local sig = aml.signal_new(signo, function(sig)
			emit_event(resolve)
			loop:stop(sig)
		end)
		loop:start(sig)
	end)
end

async(function(await)
	print "Hi! How long should I sleep?"

	local seconds = nil
	while seconds == nil do
		seconds = tonumber(await(read_line_from_stdin()))
		if seconds == nil then
			print "That's not a number. Try again!"
		end
	end

	seconds = math.floor(seconds)

	print "OK!"

	while (seconds ~= 0) do
		print(tostring(seconds) .. "...")
		await(sleep(1000))
		seconds = seconds - 1;
	end

	print "Done!"

	do_run = false
	loop:exit()
end)

async(function(await)
	await(signal(SIGINT))

	print "Received signal. Exiting..."

	do_run = false
	loop:exit()
end)

-- Lua 5.1 can't yield across C call boundary, so callbacks are queued and
-- dispatched later from lua
while do_run do
	loop:poll(-1)
	loop:dispatch()

	for _,cb in ipairs(event_queue) do
		cb()
	end

	event_queue = {}
end
