timerlib = {}

timerlib.handleTimer = function (id, msg)
	if string.find(id, "b_") then
		timerlib.handleBldgTimer(id, msg)
	else
		--print("timer not recognized")
	end
end

timerlib.handleBldgTimer = function (id, msg)
	local name = string.gsub(id, "b_", "")    --will get eg. wood1
	local rawname = string.gsub(name, '%d', "")	
	reshelper.addResources(name)
	timers[rawname] = 0
end

timerlib.checkCapacity = function (rawname, amount)
	local warehouse = "warehouse" .. buildings[warehouse]
	local maxcap = capacity[warehouse][rawname]
	
end