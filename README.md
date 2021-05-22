# VCH
Virtual Controller Helper

This little plugin for Euroscope enables a "Pending Request" option for any list you like.
With that you can mark the requests for individual flight plans, so you won't forget them!
So far it has the following features:
- Request Clearence (marked green)
- Request Pushback (marked yellow)
- Request Startup (marked yellow) - new in 0.4.4
- Request Taxi (marked orange) - since 0.4.4, in older versions it is red
- Request Departure (marked red) - new in 0.4.4
- No Request
- Synchronization between multiple instances of Euroscope (beware that it will only synchronize correctly when all instances are open already, it won't do an automatic sync when an instance is opened after a request has been set (yet, at least it is unreliable)
- Similar dropdown menu thing for "hold shorts" where you can enter up to 5 characters as a reminder out of the scratch pad

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

What you can customize (so far):
- command ".vch blink" or ".vch blink on" or ".vch blink off" to set blinking of the request /hold short item on or off (or toggle between those states).
- command ".vch option" to set visibilities of individual items in the drop-down menu.
- command ".vch color" to set the color of the individual tag items, color code needs to be in 9 digit rgb, example: .vch color rqc 255000000. 001001001 will set to Euroscope default, 002002002 will set back to default plugin settings.

List of items:
- rqc -> Clearence
- rqp -> Pushback
- rqs -> Startup
- rqt -> Taxi
- rqd -> Departure
- example: ".vch option rqd off"

Both settings get saved, if you confirm it in the saving dialog when closing Euroscope!

Pictures:
https://prnt.sc/xpuzho
