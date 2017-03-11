## Command Line Interface(CLI)
This directory contains implementation of CLI. It is used to control system core, test it and link all API functions together. It was used during testing, now it is replaced with GUI(however it also be updated, bu it is not in the focus now). CLI implementation consists of:
* CLI class - hold list of available commands, read line from console, parse and try to execute. 
* DevCLI.cpp - contains definitons of commands that operate with system API.
* main.cpp - contains main function for CLI app version.


## CLI manual
CLI used to test whole system while it has not GUI(however it can be included in GUI application as advanced control option), so cli commands are build to cover whole system API. Current commands(uses vairables as [...]; + to indicate multiple parameters; ? to indicate optional parameter):
* help - help command stub(currently prints reference to this document).
* echo ... - print arguments as is(used to test cli).
* ls - list command. Used to view different system compoenet state. Subcommands:
	* ls - list all connected devices.
	* ls state [id] - view device [id] state(uses low-level Device wrapper API).
	* ls mode [id] - view device [id] mode(uses low-level Device wrapper API).
	* ls coords - view defined coordinate planes.
	* ls tasks - view defined tasks.
* x - stop all running devices.
* dev - control device. Mostly uses low-level Device wrapper API. Subcommands:
	* dev roll [id] [trailer] - roll device [id] to the [trailer](1 or 2).
	* dev move [id] [x] [y] [speed] [div] - move device to (x; y) with speed [speed] steps/sec and step divisor [div].
	* dev rmove [id] [x] [y] [speed] [div] - relative to current position move device to (x + current.x; y + current.y) with speed [speed] steps/sec and step divisor [div].
	* dev stop [id] - stop device movement.
	* dev power [id]+ - turn on/off(switch) power on devices [id1], [id2]...
* coord - operate with coordinate plane. Represents high-level API. Subcommands:
	* coord new [xid] [yid] - create coordinate plane with devices ([xid]; [yid]). Prints coordinate plane id.
	* coord log [id] [prefix]? - add logger to coordinate plane [id].
	* coord map [id] [offset_x] [offset_x] [scale_x] [scale_y] -  add coordinate mapping to plane [id] (scales coordinates with ([scale_x]; [scale_y]) and adds offset ([offset_x]; [offset_y])). 
	* coord validate [id] [min_x] [min_y] [max_x] [max_y] [max_speed] - add validation to plane [id] (check coordinates to be from ([min_x]; [min_y]) to ([max_x]; [max_y]) and speed to be from 0 to [max_speed]).
	* coord move [id] [x] [y] [speed] [div] - move coordinate plane [id] to ([x]; [y]) with speed [speed] and step divisor [div].
	* coord rmove [id] [x] [y] [speed] [div] - relative to current position move coordinate plane [id] to ([x] + current.x; [y] + current.y) with speed [speed] and step divisor [div].
	* coord jump [id] [x] [y] [speed] [div] - jump coordinate plane [id] to ([x]; [y]) with speed [speed] and step divisor [div](Jump means no synchronization).
	* coord rjump [id] [x] [y] [speed] [div] - relative to current position jump coordinate plane [id] to ([x] + current.x; [y] + current.y) with speed [speed] and step divisor [div](Jump means no synchronization).
	* coord arc [id] [x] [y] [center_x] [center_y] [splitter] [speed] [div] - clockwise move coordinate plane [id] to ([x]; [y]) with arc center ([center_x]; [center_x]), splitter [splitter], speed [speed] steps/sec and step divisor [div].
	* coord carc [id] [x] [y] [center_x] [center_y] [splitter] [speed] [div] - counter-clockwise move coordinate plane [id] to ([x]; [y]) with arc center ([center_x]; [center_x]), splitter [splitter], speed [speed] steps/sec and step divisor [div].
	* coord rarc [id] [x] [y] [center_x] [center_y] [splitter] [speed] [div] - relative to current position clockwise move coordinate plane [id] to ([x] + current.x; [y] + current.y) with arc center ([center_x] + current.x; [center_x] + current.y), splitter [splitter], speed [speed] steps/sec and step divisor [div].
	* coord rcarc [id] [x] [y] [center_x] [center_y] [splitter] [speed] [div] - relative to current position counter-clockwise move coordinate plane [id] to ([x] + current.x; [y] + current.y) with arc center ([center_x] + current.x; [center_x] + current.y), splitter [splitter], speed [speed] steps/sec and step divisor [div].
	* coord cal [id] [tr] - calibrate coordinate plane [id] to trailer [tr](1 or 2).
	* coord meas [id] [tr] - measure and calibrate coorinate plane [id] to trailer [tr](1 or 2). Recommended to use before any other operations with plane.
	* coord graph [cid] [func] [offset_x] [offset_y] [scale_x] [scale_y] [x_min] [x_max] [y_min] [y_max] [step] [speed] - create function y=[func] graph on coordinate plane [cid](offset=[offset_x]x[offset_y]; scale=[scale_x]x[scale_y]) with x is from [x_min] to [x_max](or in negative direction), accepted y is from [y_min] to [y_max] with x step [step] and speed coef [speed].
* refresh - refresh device list and reinitialise them.
* task - operates with system tasks. Subcommands:
	* task new - create new empty programmable task.
	* task rm [id] - remove task and free all it's resources.
	* task add [id] - append operation step to programmable task [id]. All operations are similar coord commands, however coord id  and step divisor is not specified, but speed is specified in range (0; 1) including 1. Operations:
		* task add [id] move [x] [y] [speed]
		* task add [id] rmove [x] [y] [speed]
		* task add [id] jump [x] [y] [speed]
		* task add [id] rjump [x] [y] [speed]
		* task add [id] arc [x] [y] [center_x] [center_y] [splitter] [speed]
		* task add [id] rarc [x] [y] [center_x] [center_y] [splitter] [speed]
		* task add [id] carc [x] [y] [center_x] [center_y] [splitter] [speed]
		* task add [id] rcarc [x] [y] [center_x] [center_y] [splitter] [speed]
		* task cal [id] [tr]
	* task exec [id] [coordid] [speed] - execute task [id] on coordinate plane [coordid] and base speed [speed] steps/sec.
	* task load [center_x] [center_y] [scale_x] [scale_y] [path] - load G-Code from path using ([scale_x]; [scale_y]) to get coordinates(used to get most precise coordinates, otherwise functional may not be stable).
	* task graph [func] [offset_x] [offset_y] [scale_x] [scale_y] [x_min] [x_max] [y_min] [y_max] [step] [speed] - create function y=[func] graph task on coordinate plane(offset=[offset_x]x[offset_y]; scale=[scale_x]x[scale_y]) with x is from [x_min] to [x_max](or in negative direction), accepted y is from [y_min] to [y_max] with x step [step] and speed coef [speed].
	
## Notice
Because of some bugs at the start of CLI you should execute some commands:
```
refresh	# Reinitialise all devices.
dev power ... # Put all device id's to power them on.
```
	
	

