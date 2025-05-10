# StatTrak Tsuba Project 
This repo contains an upload of the archived project files from a personal project I worked on when I was a junior at UC San Diego studying computer science in late 2022. As fall quarter wound down, I shelved it to focus on finals. I eventually decided the whole thing was kind of a bad idea and moved on to other things, but I may resurrect it someday. 

## Summary
The idea for this project first emerged when I found the datasheet for the Broadcom HDSP-2530 dot-matrix display. I noticed that it closely resembled the dot-matrix display depicted on StatTrak weapons in CS:GO, and I set out to find something to attach one to. After considering some funny ideas like our apartment toaster, I decided it could be an interesting challenge to try to put a display (as well as the electronics necessary to drive it) into a 3D-printed kendo tsuba. The idea was for it to use a gyroscope to count either practice swings or impacts, depending on a user setting, similarly to a StatTrak weapon in CS:GO. 

## Background
### StatTrak 
StatTrak is a feature on some weapon skins in the Counter-Strike franchise that adds a digital dot-matrix display to the weapon skin that counts player kills. [There's a wiki about it here.](https://liquipedia.net/counterstrike/StatTrak) The original idea for this project was to take an HDSP-2530 and some supporting electronics and to add StatTrak to something IRL. 

![StatTrak_promo_image](https://github.com/user-attachments/assets/3ffcabf2-e0ef-4a53-833f-552343183807)

> Promotional image for the StatTrak feature. Source: [counter-strike.net](https://www.counter-strike.net/armsdeal)

### Tsuba 
A tsuba is a disc-shaped hand guard that slides onto the hilt of a a bamboo sword used for kendo, or Japanese fencing. They're usually made of plastic or thick leather, and come in various colours and designs. 

![Tsuba illustration](https://github.com/user-attachments/assets/ed8f5a82-6f83-4281-9b5c-326f0c707d85)

> A kendo tsuba in the wild. Modified from source: [Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Kendo_EM_2005_-_taiatari.jpg)

## Hardware 
The hardware used was as follows: 
- Broadcom HDSP-2530 display
- Arduino Nano 33 BLE
- 3.3V buck converter
- 300 mAh lithium-polymer battery
- 5v lipo battery charging circuit
- Adafruit LSM6DSO32 accelerometer/gyroscope

## Software 
The software was built to sample the accelerometer to detect the rapid acceleration that occurs when a shinai (bamboo sword) changes direction at the end of a strike. It would then increment a counter and write the new value to the display. 

The accelerometer was also used for mode selection, as any buttons or switches on the device would either be too easy to push by accident or too small to manipulate while wearing a pair of kote. The user could change the mode by bumping the pommel of the sword against something, which would register a sharp forward acceleration in the same axis as the length of the sword. I chose this motion because I thought it was an acceleration pattern not found anywhere in kendo, meaning the user wouldn't change modes by accident. 

[Andy4495's HCMS39xx Arduino library](https://github.com/Andy4495/HCMS39xx) was super helpful in writing data out to the display quickly and easily.

## Viability and state of the project 
This project never saw the light of day beyond a few iterations on a prototype device, which never made it to the dojo floor. A few of the concerns that motivated me to keep the prototype at home and eventually to shelve the project were: 

### Safety 
Tsubas are usually made of solid plastic. Nicer ones are made of thick, hardened leather. These are highly strong and flexible materials that don't easily crack or shatter due to hard impacts. On the other hand, the prototype StatTrak tsuba was housed in a relatively thin 3D printed ABS shell that likely would not have been able to endure the forces involved in keiko without shattering. This is of particular concern because kendo is practiced barefoot, meaning that the pieces of a shattered tsuba can pose a serious safety risk. 

Further, I couldn't be certain that the lithium-polymer battery inside could withstand the forces involved with keiko without deforming and potentially catching fire. In the event that it was exposed due to a shattered housing, any further impact could immediately crush it and cause a fire. 

### User acceptance
The culture at the kendo dojos that I've visited places high importance on deferring to the sensei on matters of technique, keeping one's equipment in good condition for the sake of others' safety, and avoiding flashy gear or other displays that might make you stand out. Bringing this device to a practice would've been problematic on all three fronts. 
1. In developing the software that detects swings and filters out other movements, I would implicitly draw a software-defined line distinguishing proper swings from improper ones, which is something that can't really be done by a computer, even with hundreds of highly-accurate sensors. Trying to do that with an Arduino and a single gyro/accelerometer board is just asking for trouble. It'd be a pretty bad look if someone handed it to a sensei and the software didn't count their swings right. 
2. Bringing in homebrew equipment that posed a safety risk to others was a complete non-starter unless I was trying to get sent to do hayasuburi until the end of time. 
3. Practicing with a bulky, glowing electronic tsuba would attract the wrong kind of attention.

## Conclusion
This project, though it never got far past the initial prototype phase, was pretty fun and educational. It challenged me to experiment with writing software for an environment that was vastly different from the work I'd done for my University courses or my AWS internship, which gave me valuable perspective. I also got to venture into the physical and electronic design spaces, which I couldn't fit into my CS curriculum. I'm glad that I took the time to experiment with the idea, and I may come back to it in the future if I find a way to design it to be more sturdy and discreet. 
