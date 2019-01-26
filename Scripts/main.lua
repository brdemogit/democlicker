-- FIRST LUA FILE CALLED BY THE PROGRAM, IT NEEDS TO CONTAIN ALL THE NECESSARY FUNCTIONS derp


require ("Scripts\\helpers\\clickHandler")
require ("Scripts\\helpers\\promptHandler")
require ("Scripts\\helpers\\timerHandler")
require ("Scripts\\helpers\\resourceHelpers")
require ("Scripts\\helpers\\passiveHandler")
require ("Scripts\\helpers\\buildHelper")

function handleCMD(address, id, msg)
	if address == "click" then
		clicklib.handleClick(id, msg)
	elseif address == "prompt" then
		promptlib.handlePrompt(id, msg)
	elseif address == "timer" then
		timerlib.handleTimer(id, msg)
	else
		--print("Address ", address, " not recognized")
	end
end
	
function addText(scene, x, y, w, h, id, text, button, fontSize)
	local cmd = "addText "
	cmd = cmd .. x .. " " .. y .. " " .. w .. " " .. h .. " "
	local fsize = fontSize or 24
	cmd = cmd .. fsize .. " "
	local isButton
	if button then isButton = 1 else isButton = 0 end
	cmd = cmd .. isButton .. " " .. id .. " " .. text
	pushGUIcmd(script, "scene", scene, cmd)
end

function addImg(scene, id, x, y)
	local cmd = "addImg " .. id .. " " .. x .. " " .. y
	pushGUIcmd(script, "scene", scene, cmd)
end

function delText(scene, id)
	local cmd = "delText " .. id
	pushGUIcmd(script, "scene", scene, cmd)
end

function delImg(scene, id, x, y)
	local cmd = "delImg " .. id .. " " .. x .. " " .. y
	pushGUIcmd(script, "scene", scene, cmd)
end