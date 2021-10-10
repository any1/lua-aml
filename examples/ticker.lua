#!/usr/bin/luajit

local aml = require 'aml'

local loop = aml.new()

local is_tick = true;

local ticker = aml.ticker_new(1000, function(handler)
	if is_tick then
		print("tick")
		is_tick = false
	else
		print("tock")
		is_tick = true
	end
end)

loop:start(ticker)

loop:run()
