passive = {}

passive.tick = function ()
	--print("tick function called")
	passive.updateWorkerYields()
end

passive.updateTownhallScene = function()
	for k,v in pairs (buildings) do
		--print(k)
		if k == "market" or k == "mason" or k == "townhall" or k == "manor" or k == "warehouse" then goto continue end
			if tonumber(v) > 0 then
				addText("townhall", townhallbuttons[k].x, townhallbuttons[k].y, townhallbuttons[k].w, townhallbuttons[k].h, townhallbuttons[k].id, townhallbuttons[k].text, true)
				delText("townhall", townhallbuttons[k].id)
				addText("townhall", (townhallbuttons[k].x + townhallbuttons[k].w + 8), townhallbuttons[k].y - 4, 60, 34, "wl_" .. k, workers[k], false, 34)
				delText("townhall", "wl_" .. k)
				local offset = 0
				for a,b in pairs(workercost[k]) do
					local rname = "r_" .. a
					addImg("townhall", rname, townhallbuttons[k].x + offset, townhallbuttons[k].y + 42)
					delImg("townhall", rname, townhallbuttons[k].x + offset, townhallbuttons[k].y + 42)
					offset = offset + 50
					addText("townhall", townhallbuttons[k].x + offset, townhallbuttons[k].y + 45, 50, 34, k .. a, b, false)
					delText("townhall", k .. a)
					offset = offset + 50
				end
			end
		::continue::
	end
end

passive.updateWorkerYields = function ()
	for k,v in pairs(workers) do
		if tonumber(v) > 0 then
			passive.workyieldbuffer[k] = passive.workyieldbuffer[k] + ((workeryield[k] * workers[k]) * buildings.townhall)
		end
	end
	
	for k,v in pairs(passive.workyieldbuffer) do
		if v >= 1 then
			reshelper.addResources(k, math.floor(v))
			passive.workyieldbuffer[k] = 0
		end
	end
end

passive.workyieldbuffer = {
	wood = 0,
	stone = 0,
	water = 0,
	flax = 0,
	ore = 0,
	timber = 0,
	cobblestone = 0,
	food = 0,
	linen = 0,
	iron = 0,
	rope = 0,
	tools = 0,
	cart = 0,
}