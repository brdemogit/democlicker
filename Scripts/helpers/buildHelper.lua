buildlib = {}

buildlib.build = function (id)
	-- first we determine if it's a new building or an upgrade
	local level = string.match(id, '%d')
	local name = string.gsub(id, "b_", "")
	local rawname = string.gsub(name, '%d', "")
	
	if tonumber(level) == 1 then -- this is a new building, add it to the village scene and add the icon and label to the overlay
		local rname = "r_" 
		local lname = "l_" 
		
		if rawname == "warehouse" or rawname == "manor" or rawname == "townhall" then --handle scene buildings
			local sname = "s_" .. rawname
			pushGUIcmd(script, "overlay", "", "addAsset " .. sname)
		elseif rawname == "market" then  
			rname = rname .. "gold"
			lname = lname .. "gold"
			local sname = "s_" .. rawname
			pushGUIcmd(script, "overlay", "", "addAsset " .. sname)
			pushGUIcmd(script, "overlay", "", "addAsset " .. rname)
			pushGUIcmd(script, "overlay", "", "setLabel " .. lname .. " 0") -- since this is a new building, the resource will be zero
			pushGUIcmd(script, "overlay", "", "addLabel " .. lname)
			buildlib.addmarketbuttons(1)
		else
			rname = rname .. rawname
			lname = lname .. rawname
			pushGUIcmd(script, "overlay", "", "addAsset " .. rname)
			pushGUIcmd(script, "overlay", "", "setLabel " .. lname .. " 0") -- since this is a new building, the resource will be zero
			pushGUIcmd(script, "overlay", "", "addLabel " .. lname)
		end
		
		pushGUIcmd(script, "scene", "village", "addAsset " .. id)
		
		
		buildings[rawname] = level
		passive.updateTownhallScene()
		buildlib.updateWarehouseView()
	else  -- this is an upgrade, delete old building and add a new one in its place 
		local previd = string.gsub(id, '%d', "") .. math.floor((level - 1))
		pushGUIcmd(script, "scene", "village", "addAsset " .. id)
		pushGUIcmd(script, "scene", "village", "delAsset " .. previd)
		
		buildings[rawname] = level
		
		if rawname == "market" then
			buildlib.addmarketbuttons(tonumber(level))
		elseif rawname == "warehouse" then
			buildlib.updateWarehouseView()
		end
	end
	
	--reset the mason view
	buildlib.updateMasonView()
end


buildlib.addmasonbutton = function(name)
	if masonbuttons[name] then
		addText("mason", masonbuttons[name].x, masonbuttons[name].y, masonbuttons[name].w, masonbuttons[name].h, masonbuttons[name].id, masonbuttons[name].text, true)
		local i = 1
		for k,v in pairs(stats[name].buildcost) do
			local rname = "r_" .. k
			local res = "res" .. i
			local lab = "lab" .. i
			local rid = name .. "_" .. k -- unique id for every resource label (wood1_wood)
			addImg("mason", rname, masonbuttons[name][res].x, masonbuttons[name][res].y)
			addText("mason", masonbuttons[name][lab].x, masonbuttons[name][lab].y, 50, 30, rid, v, false)
			i = i + 1
		end
	end
end

buildlib.delmasonbutton = function(name)
	if masonbuttons[name] then
		local mname = "m_" .. name
		delText("mason", mname)
		local i = 1
		for k,v in pairs(stats[name].buildcost) do
			local rid = name .. "_" .. k
			local res = "res" .. i
			local rname = "r_" .. k
			delImg("mason", rname, masonbuttons[name][res].x, masonbuttons[name][res].y)
			delText("mason", rid)
			i = i + 1
		end
	end
end

buildlib.updateMasonView = function()
	--LIFO#############################################
	--now add everything that the current mason level allows
	for k,v in pairs(buildings) do
		--see if it's the max level or not
		local leveltoadd = math.floor(tonumber(v) + 1)
		if leveltoadd < 4 then
			if k == "mason" then --if mason then no need for checks
				buildlib.addmasonbutton(k .. leveltoadd)
			else
				if leveltoadd <= tonumber(buildings.mason) then
					buildlib.addmasonbutton(k .. leveltoadd)
				end
			end
		end
	end
	
	--first delete EVVVEERYYTHING that has lower level than current and the current ones
	for k,v in pairs(buildings) do
		local prevlevel = math.floor(tonumber(v) - 1)
		local leveltoadd = math.floor(tonumber(v) + 1)
		buildlib.delmasonbutton(k .. prevlevel)
		buildlib.delmasonbutton(k .. v)
		buildlib.delmasonbutton(k .. leveltoadd)
	end
	
	
end

buildlib.addmarketbuttons = function(level)
	if level == 1 then
	reshelper.addMarketButton("wood")
	reshelper.addMarketButton("stone")
	reshelper.addMarketButton("water")
	end
	
	if level == 2 then 
	reshelper.addMarketButton("flax")
	reshelper.addMarketButton("ore")
	reshelper.addMarketButton("timber")
	end
	
	if level == 3 then
	reshelper.addMarketButton("cobblestone")
	reshelper.addMarketButton("food")
	reshelper.addMarketButton("iron")
	end
	
	if level == 4 then
	reshelper.addMarketButton("rope")
	reshelper.addMarketButton("tools")
	reshelper.addMarketButton("cart")
	reshelper.addMarketButton("linen")
	end
end

buildlib.updateWarehouseView = function ()
	local ware = "warehouse" .. buildings.warehouse
	for k,v in pairs(capacity[ware]) do
		local level 
		if k == "gold" then level = buildings.market else level = buildings[k] end
		if tonumber(level) > 0 then
			local rname = "r_" .. k
			local text = resources[k] .. " / " .. v
			addImg("warehouse", rname, warehouseicons[k].x, warehouseicons[k].y)
			delImg("warehouse", rname, warehouseicons[k].x, warehouseicons[k].y)
			addText("warehouse", warehouseicons[k].x + 225, warehouseicons[k].y, 200, 40, k, text, false, 40)
			delText("warehouse", k)
		end
	end
end

