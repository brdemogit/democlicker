reshelper = {}

reshelper.addResources = function (name, amount)
	local rawname = string.gsub(name, '%d', "")   -- will get eg. wood
	local lname = "l_" .. rawname
	local amount = amount or stats[name].yield
	
	--check if there is enough storage for the resource in the warehouse
	local warehouse = "warehouse" .. buildings["warehouse"]
	local maxcap = capacity[warehouse][rawname]
	
	--if not enough space then set resource amount to maxcap
	if (resources[rawname] + amount) > maxcap then
		resources[rawname] = math.floor(maxcap)
		--and show a prompt
		local promptcmd = "addPrompt "
		local promptmsg = "Maximum amount of " .. rawname .. " you \ncan store is " .. math.floor(maxcap) .. ".\nUpgrade your warehouse"
		--pushGUIcmd(script, "window", "", promptcmd .. promptmsg)
	else -- add the full amount
		resources[rawname] = math.floor(resources[rawname] + amount)
	end
	
	local command = "setLabel" .. " " .. lname .. " " .. resources[rawname]
	pushGUIcmd(script, "overlay", "", command)
	
	buildlib.updateWarehouseView()
end

reshelper.subResources = function (rawname, amount)
	local lname = "l_" .. rawname
	resources[rawname] = math.floor(resources[rawname] - amount)
	local command = "setLabel" .. " " .. lname .. " " .. resources[rawname]
	pushGUIcmd(script, "overlay", "", command)
	buildlib.updateWarehouseView()
end

reshelper.addMarketButton = function (rawname)
	addText("market", marketbuttons[rawname].x, marketbuttons[rawname].y, marketbuttons[rawname].w, marketbuttons[rawname].h, marketbuttons[rawname].id, marketbuttons[rawname].text, true)
end