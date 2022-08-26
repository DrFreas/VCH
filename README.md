# VCH
Virtual Controller Helper

This little plugin for Euroscope enables a "Pending Request" option for any list you like.
With that you can mark the requests for individual flight plans, so you won't forget them!
So far it has the following features:
- Request Clearence (marked green by default)
- Request Pushback (marked yellow by default)
- Request Startup (marked yellow by default) - new in v0.4.4
- Request Taxi (marked orange by default) - since v0.4.4, in older versions it is red
- Request Departure (marked red by default) - new in v0.4.4
- No Request
- Synchronization between multiple instances of Euroscope (beware that it will only synchronize correctly when all instances are open already, it won't do an automatic sync when an instance is opened after a request has been set (yet, at least it is unreliable)
- Similar dropdown menu thing for "hold shorts" where you can enter up to 5 characters as a reminder out of the scratch pad
- Since v0.5.0 it is possible to customize the colors of the tag items
- Since v0.6.0 there is an online version checking, if the version is outdated it will let you know!
- Also since v.0.6.1 there is a "CTL" flag (cleared to land) which gets shown for arriving traffic as a reminder to give a landing clearence
- since v0.6.2 you can switch off the requirement to track the aircraft to switch the flag.
- since v0.8.1 there is a "CTL" flag which is only shown when it is active
- since v0.8.1 there is a "CFT" flag which shows if the ground state is "DEPA" and within 2nm within the origin (can be changed)
- since v0.8.1 there is a "reminder" item which switches states on command and can show two different things (not synced with anyone except yourself)
- since v0.8.1 there is also a "reminder" item which only shows up if the reminder is active
- since v0.8.2 there is a tag item for requests which is shorter than the default one

This is what it does on its own:
- blinking in its respective color every 5 seconds (more or less)
- deletes the request if clearence flag is set or if the respective ground state changed
- shows which request came first
- show how many minutes have passed since the request was entered
- throws out every request, that has an invalid flight plan or which ground speed exceeds 80 knots

This is what is planned for the future:
- dropdown menu for "hold shorts" has all taxiways of current airport automatically in
- maybe some other useful features, what do you want? Let me know.

How to setup:
- create new item in the departure list
- Tag Item type: VCH / Pending request
- Header name: Req
- Width: 3
- Align to center: checked
- Left button: VCH / Request menu
- another new item
- Tag Item type: VCH / Pending request time
- Header name: RT
- Width: 3
- and yet another item
- Tag Item type: VCH / Active Hold Short
- Header name: H/S
- Width: 5
- Align to center: checked
- Left button: VCH / Hold Short menu
- for the CTL item:
- Tag Item type: VCH / Pending Landing Clearence
- Header name: CTL
- Width: 3
- Align to center: checked
- Left button: VCH / Switch Landing Clearence

What you can customize (so far):
- command ".vch blink" or ".vch blink on" or ".vch blink off" to set blinking of the request /hold short item on or off (or toggle between those states).
- command ".vch option" to set visibilities of individual items in the drop-down menu. Example: ".vch option rqd off"
- command ".vch color" to set the color of the individual tag items, color code needs to be in 9 digit rgb, example: ".vch color rqc 255000000". 001001001 will set to Euroscope default, 002002002 will set back to default plugin settings.
- command ".vch ctldist" will set the distance to the destination in nm of an aircraft to show the CTL tag. Example: ".vch ctldist 20". Default is 40nm. (former: .vch distance
- command ".vch cftdist" will set the distance from the origin in nm of an aircraft to show the CFT tag. Example: ".vch ctfdist 10". Default is 2.
- command ".vch ctltrack" will switch between surpressing the tracking requirement of the CTL flag or not. So if set to "1", you won't have to track the aircraft to change the CTL flag.
- command ".vch reminderon" will set the text for the reminder tag item if it is active. Example: ".vch reminderon transfer?". Maximum size: 20. Default: "@".
- command ".vch reminderoff" will set the text for the reminder tag item if it is not active. Example: ".vch reminderoff nothing!". Maximum size: 20. Default: "_"

List of items:
- hos -> Hold Short
- rqc -> Clearence
- rqp -> Pushback
- rqs -> Startup
- rqt -> Taxi
- rqd -> Departure
- ctl -> Cleared to Land
- nlc -> Not Cleared to Land
- cft -> Cleared for Takeoff
- rem -> Reminder on
- nrm -> Reminder off
- example: ".vch option rqd off"

Some other stuff:
- command ".vch release DLH123" which will end the tracking of the specified callsign (might be usefull if an aircraft is still tagged but out of vis range)

All settings get saved, if you confirm it in the saving dialog when closing Euroscope!

Pictures:
https://prnt.sc/xpuzho
