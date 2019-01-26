clicklib = {}

clicklib.handleClick = function (id, msg)
	-- handle left click
	if msg == "left" then
		if string.find(id, "s_") then
			clicklib.handleSceneClick(id)
		elseif string.find(id, "b_") then
			clicklib.handleBldgClick(id)
		elseif string.find(id, "m_") then
			clicklib.handleMasonClick(id)
		elseif string.find(id, "g_") then
			clicklib.handleMarketClick(id)
		elseif string.find(id, "t_") then
			clicklib.handleTownhallClick(id)
		else
			--print("click type not recognized")
		end
	elseif msg == "right" then
		if string.find(id, "b_") then
			clicklib.rightBldgClick(id)
		end
	end
	buildlib.updateWarehouseView()
end

clicklib.handleSceneClick = function (id)
	local scene = string.gsub(id, "s_", "")
	local cmd = "setCurrentScene " .. scene

	pushGUIcmd(script, "window", "", cmd)
	end
	
clicklib.handleBldgClick = function (id)

	--get helper names
	local name = string.gsub(id, "b_", "")    --changes eg b_wood1 into wood1       name
	local rawname = string.gsub(name, '%d', "")  --extracts the generic name from name var  eg wood1 into wood
	
	--is it a scene building?
	if not resources[rawname] then
		local cmd = "setCurrentScene " .. rawname
		pushGUIcmd(script, "window", "", cmd)
		return
	end
	
	-- if it's a resource building, add the timer
	-- does the player has required materials?
	if clicklib.checkReqs(id) then
		for k,v in pairs (stats[name].cost) do
			reshelper.subResources(k, v)       -- if he does, substract them and add the timer
		end
		
		local cmd = "addTimer " .. id     --add a timer to the village scene eg. b_wood1
		local timer = stats[name].timer
		cmd = cmd .. " " .. timer
		
		pushGUIcmd(script, "scene", "village", cmd)
	end
end
	
clicklib.checkReqs = function (id)

	--what type of requirement is to be checked
	if string.find(id, "b_") then            			--resource production
		local name = string.gsub(id, "b_", "")   
		-- cycle through the cost table of the stats data
		for k,v in pairs(stats[name].cost) do
			if v > tonumber(resources[k]) then
				return false
			end
		end
		-- return true if cycled through all costs
		return true
		
	elseif string.find(id, "m_") then
		local name = string.gsub(id, "m_", "")   
		-- cycle through the cost table of the stats data
		for k,v in pairs(stats[name].buildcost) do
			if v > tonumber(resources[k]) then
				return false
			end
		end
		return true
		
	elseif string.find(id, "g_") then
		local rawname = string.gsub(id, "g_", "")
		rawname = string.gsub(rawname, '%d', "")
		if tonumber(resources[rawname]) >= tonumber(exchange[rawname]) then
			return true
		end
	
	
	elseif string.find(id, "t_") then
		local rawname = string.gsub(id, "t_", "")
		rawname = string.gsub(rawname, '%d', "")
		for k,v in pairs(workercost[rawname]) do
			if v > tonumber(resources[k]) then
				return false
			end
		end
		return true
	end
end

clicklib.rightBldgClick = function (id)

	local name = string.gsub(id, "b_", "")
	local cmd = "addPrompt " .. stats[name].info
	pushGUIcmd(script, "window", "", cmd)	
end

clicklib.handleMasonClick = function (id)
	--get helper names
	local name = string.gsub(id, "m_", "")   
	local bname = "b_" .. name

	if clicklib.checkReqs(id) then
		for k,v in pairs(stats[name].buildcost) do
			reshelper.subResources(k, v)
		end
		
		buildlib.build(bname)
	end
end

clicklib.handleMarketClick = function(id)
	--get rawname
	local rawname = string.gsub(id, "g_", "")
	if clicklib.checkReqs(id) then
		reshelper.subResources(rawname, exchange[rawname])
		reshelper.addResources("gold", 1)
	end
end

clicklib.handleTownhallClick = function(id)
	--get rawname
	local rawname = string.gsub(id, "t_", "")
	if clicklib.checkReqs(id) then
		for k,v in pairs(workercost[rawname]) do
			reshelper.subResources(k, v)
		end
		workers[rawname] = math.floor(workers[rawname] + 1)
		passive.updateTownhallScene()
	end
end
