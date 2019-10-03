# VCH
Virtual Controller Helper

This little plugin for Euroscope enables a "Pending Request" option for any list you like.
With that you can mark the requests for individual flight plans, so you won't forget them!
So far it has the following features:
- Request Clearence (marked green)
- Request Pushback (marked yellow)
- Request Taxi (marked red)
- No Request

This is what it does on its own:
- blinking in its respective color every 5 seconds (more or less)
- deletes the request if clearence flag is set or if the respective ground state changed
- shows which request came first
- throws out every request, that has an invalid flight plan or which ground speed exceeds 80 knots

This is what is planned for the future:
- synchronization between multiple instances of Euroscope (if you can do socket programming in C++, tell me!)
- similar dropdown menu for "hold shorts"(, with all taxiways of current airport automatically in)
- maybe some other useful features, what do you want? Let me know.

How to setup:
- create new item in the departure list
- Tag Item type: VCH / Pending request
- Header name: Req
- Width: 3
- Align to center: checked
- Left button: VCH / Request menu

Pictures:
https://imgur.com/a/EPk0ACd
