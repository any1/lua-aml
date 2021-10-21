#!/usr/bin/luajit

local aml = require 'aml-wrapper'
local promise = require 'promise'
local async = require 'async'

local SIGINT = 2

local loop = aml.new()

function sleep(duration)
	return promise(function(resolve)
		local timer = loop:timer_new(duration, function()
			resolve()
		end)
		loop:start(timer)
	end)
end

function read_line_from_stdin()
	return promise(function(resolve)
		local handler = loop:handler_new(0, function(handler)
			resolve(io.read())
			loop:stop(handler)
		end)
		loop:start(handler)
	end)
end

function signal(signo)
	return promise(function(resolve)
		local sig = loop:signal_new(signo, function(sig)
			resolve()
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

	loop:exit()
end)

async(function(await)
	await(signal(SIGINT))

	print "Received signal. Exiting..."

	loop:exit()
end)

loop:run()
