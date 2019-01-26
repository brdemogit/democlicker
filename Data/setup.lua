-- lua set up script

require ("Data\\data")



function setup()   -- remember to addscenes and set current scene at the end! LIFO
	
	-- WAREHOUSE SCENE #####################################################
	buildlib.updateWarehouseView()	
	-- TOWNHALL SCENE #######################################################
	passive.updateTownhallScene()
	-- MARKET SCENE #########################################################
	
	if tonumber(buildings.market) == 4 then
		goto four
	elseif tonumber(buildings.market) == 3 then
		goto three
	elseif tonumber(buildings.market) == 2 then
		goto two
	elseif tonumber(buildings.market) == 1 then
		goto one
	elseif tonumber(buildings.market) == 0 then
		goto zero
	end
	
	::four::
	reshelper.addMarketButton("linen")
	reshelper.addMarketButton("rope")
	reshelper.addMarketButton("tools")
	reshelper.addMarketButton("cart")
	::three::
	reshelper.addMarketButton("cobblestone")
	reshelper.addMarketButton("food")
	reshelper.addMarketButton("iron")
	::two::
	reshelper.addMarketButton("flax")
	reshelper.addMarketButton("ore")
	reshelper.addMarketButton("timber")
	::one::
	reshelper.addMarketButton("wood")
	reshelper.addMarketButton("stone")
	reshelper.addMarketButton("water")
	::zero::
	
	-- MASON SCENE ###########################################################
	
	--[[for k,v in pairs(buildings) do
		if tonumber(v) < 4 then
			local number = (tonumber(v) + 1)
			if number  buildings.mason
			local buttonname = k .. (tonumber(v) + 1)
			buildlib.addmasonbutton(buttonname)
		end
	end]]--
	buildlib.updateMasonView()
	
	-- VILLAGE SCENE #####################################################################################
	-- if there were any timers currently added, add them now (remember to setTimer, and then addTimer cmd)
	for k,v in pairs(timers) do
		if tonumber(v) > 0 then
			local assetName = "b_" .. k .. buildings[k]
			local timerName = k .. buildings[k]
			pushGUIcmd(script, "scene", "village", "addToTimer " .. assetName .. " " ..v)
			pushGUIcmd(script, "scene", "village", "addTimer " .. assetName .. " " .. stats[timerName].timer)
		end
	end
	
		-- add built buildings to the main scene and setup overlay(resources)
	for k,v in pairs (buildings) do
		if tonumber(v) > 0 then
			local name = "b_" .. k .. v
			
			local levname = name .. v   --prepare for future, will be b_bldg1 or b_wood2
			
			--check if the building adds a resource or a scene
			if not resources[k] then
				pushGUIcmd(script, "scene", "village", "addAsset " .. name)
				if k == "market" then
					pushGUIcmd(script, "overlay", "", "addAsset r_gold")
					pushGUIcmd(script, "overlay", "", "setLabel l_gold " .. tonumber(resources["gold"]))
					pushGUIcmd(script, "overlay", "", "addLabel l_gold")
				end
				local lname = "s_" .. k
				pushGUIcmd(script, "overlay", "", "addAsset " .. lname)
			else
				local rname = "r_" .. k
				pushGUIcmd(script, "scene", "village", "addAsset " .. name)
				pushGUIcmd(script, "overlay", "", "addAsset " .. rname)
				local lname = "l_" .. k
				pushGUIcmd(script, "overlay", "", "setLabel " .. lname .. " " .. tonumber(resources[k]))
				pushGUIcmd(script, "overlay", "", "addLabel " .. lname)		
			end
		end
	end
	
	pushGUIcmd(script, "overlay", "", "addAsset s_village")
	
	
	-- add backgrounds to scenes 
	pushGUIcmd(script, "scene", "village", "addAsset background")
	pushGUIcmd(script, "overlay", "", "addAsset overlayup")
	pushGUIcmd(script, "overlay", "", "addAsset overlaydown")
	
	
	-- ADD SCENES (THIS HAS TO BE AT THE BOTTOM OF THIS FUNCTION) ####################################################
	
	for i,v in ipairs(scenes) do
		
		--print (i, v)
		if i == 1 then
			--print ("setting current scene ", v)
			local cm = "setCurrentScene " .. v
			pushGUIcmd(script, "window", " ", cm)
		end
		
		--print ("adding scene ", v)
		local cmd = "addScene " .. v
		pushGUIcmd(script, "window", " ", cmd)
	end
			
	--print("TODO - this will set up the scenes, label values, and assets at the startup")
end


-- DEBUG INFO #####################
for k,v in pairs(buildings) do
	--print(k, v)
end

for k,v in pairs(resources) do
	--print(k, v)
end

for i,v in ipairs(scenes) do
	--print(i, v)
end


-- call the function #################################
setup()