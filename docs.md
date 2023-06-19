**box_clear()**<br>
Wipe box from buffer and restore the background
```
void Tui::box_clear(std::string id)
```
Parameters:<br>
* **id** &ensp; Id of the box instance


**box_clear_text()**<br>
Wipe the inside part of the box from the buffer
```
void Tui::box_clear_text(std::string id)
```
Parameters:
* **id** &ensp; Id of the box instance


**box_create()**<br>
Create a new box instance
```
void Tui::box_create( 
	std::string  	id,
	int  			x1,
	int  			y1,
	int  			x2,
	int  			y2,
	std::string  	title = "" 
)
```
Parameters:
* **id** &ensp; Id of the box instance
* **x1** &ensp; x value of the leftmost point
* **y1** &ensp; y value of the leftmost point
* **x2** &ensp; x value of the rightmost point
* **y2** &ensp; y value of the rightmost point


**box_delete()**<br>
Delete box istance
```
void Tui::box_delete(std::string id)
```
Parameters:
* **id** &ensp; Id of the box instance


**box_draw()**<br>
Draw box into the buffer
```
void Tui::box_draw(std::string id) 	
```
Parameters:
* **id** &ensp;	Id of the box instance


**box_move()**<br>
Change box coordinates and size
```
void Tui::box_move 	(
	std::string		id,
	int  			x1,
	int  			y1,
	int  			x2 = -1,
	int  			y2 = -1 
)
```
Parameters:
* **id** &ensp; Id of the box instance
* **x1** &ensp; x value of the leftmost point
* **y1** &ensp; y value of the leftmost point
* **x2** &ensp; x value of the rightmost point (set only to change ratio)
* **y2** &ensp; y value of the rightmost point (set only to change ratio)


**box_write()**<br>
Write text into the box
```
void Tui::box_write(
	std::string  				id,
	std::vector<std::string>  	str_arr 
)
```
Parameters:
* **id** &emsp;&emsp;&ensp;&nbsp; Id of the box instance
* **str_arr** &ensp; Array of strings


**get_instance()**<br>
Get a unique instance of the class
```
static Tui * Tui::get_instance(std::string  title)
```
Parameters:
* **title** &ensp; Title of the Program


**get_size()**<br>
Return size of the terminal {max_x, max_y}
```
std::array< int, 2 > Tui::get_size(void)
```

**input_mode()**<br>
Change input mode for the input thread
```
void Tui::input_mode(std::string mode)
```
Parameters:
* **mode** &ensp; "input" for text mode, "command" for command mode


**refresh()**<br>
Write the buffer to screen
```
void Tui::refresh(void)
```

**selec_clear()**<br>
Wipe selectable from the buffer
```
void Tui::selec_clear(std::string id)
```
Parameters:
* **id** &ensp; Id of the selectable instance


**selec_create()**<br>
Create a new selectable instance
```
void Tui::selec_create(
	std::string  								id,
	int  										x,
	int  										y,
	bool  										is_row,
	std::vector<std::string>  					options,
	std::vector<std::function< void(void)>>  	funcs 
)
```
Parameters:
* **id** &emsp;&emsp;&emsp; Id of the selectable instance
* **x** &emsp;&emsp;&emsp;&ensp; x value of the leftmost point of the list
* **y** &emsp;&emsp;&emsp;&ensp; y value of the leftmost point of the list
* **is_row** &emsp; Selectable rotation, true if is a row, false if is a clumn
* **options** &ensp; List of option names
* **funcs** &emsp;&ensp; List of functions associated to the options


**selec_delete()**<br>
Delete selectable istance
```
void Tui::selec_delete(std::string id)
```
Parameters:
* **id** &ensp;Id of the selectable instance


**selec_draw()**<br>
Draw selectable into the buffer
```
Tui::selec_draw(std::string id)
```
Parameters:
* **id** &ensp;	Id of the selectable instance


**selec_input()**<br>
Get user input for the desired selectable It uses the auxiliary thread, not the input thread
```
void Tui::selec_input(std::stringid)
```
Parameters:
* **id** &ensp; Id of the selectable instance

