# Monk Studio
Monk Studio is a 3D game engine developed by students at CITM. https://github.com/AMADE128/MonkStudio

## Developers
- Himar Bravo González (https://github.com/himar33)
- Marc Pavon Llop (https://github.com/Marckitus)
- Pol Vázquez Porcar (https://github.com/AMADE128)

## How to use
The user can move through the menus by clicking on them.
On the main menu bar at the top of the screen:
(each main menu button has an option :::::::::: that opens that menu as a windows, so the user won't have to enter multiple times).
### File:
 Quit: Exit the game. (can also be done pressing ESC).

### Render:
 Buttons to enable/disable: Depth Test, Cull Face, Lighting, Color Material, Textures and Wireframe mode. 

### Game Object:
 Creates some basic objects like: Cube, Sphere, Pyramid, Cylinder, Plane.

### View:
 Buttons to open windows:
- Console:
User can see the logs in engine. (also opens with Alt + 1).

- Hierarchy:
User can select and move objects by clicking in them and droping over others, right clicking on objects opens a submenu to delete, create an empty child or move it up and right on the child list. (also opens with Alt + 2).

- Inspector:
Shows the properties from the selected object, those can be: (also opens with Alt + 3)
Changing the name of the object. 
Local Transformation: The user can see and change the Position, Rotation and scale of the selected object by scrolling or writing the variables.
Mesh Renderer: Checkbox to see Vertex Normals.
Material: User can activate/desactivate the texture, see the path and size of the image, and set a default texure over it.

- Configuartion:
Accses to Application, Window, File System, Hardware and Input options. (also opens with Alt + 4)

Application: The user can change te App name (Monk Studio by default) Cap the FPS to anything between 60 and 1 FPS, and 3 charts with the Framerate, Miliseconds in between, and Process Memory.

Window: User can change the Brightness of the monitor, Width and Height of the engine, enable Fullscreen, making the window resizable or not, Borderless or full desktop.

File System: Shows the base path where the engine is located.

Hardware: Shows what version of SDL, OpenGl, Glew, and Devil the engine is using, the computer CPU, Ram, Caps, GPU and its brand.

Input: Shows in real time the mouse: Position, Motion, and wheel. Also logs all user inputs.

### Help
From help the user can open an ImGui Demo, go to our github, releases and report bugs, as well as a window about with information.

### Movement and Shortcuts
User can move with WASD and look arround freely when pressing rightclick, also go verticaly UP or DOWN with E and R
When right clicking:
- Mouse: look arround.
- W: forward.
- A: left.
- S: backward.
- D: right.
- E: UP on Y axis.
- R: DOWN on Y axis.

Alt + left click: Orbit arround the selected object.

Alt + pressing mouse wheel: scroll the camera on a plane.

Mouse Wheel: Zoom in and out.

F: Focus on the selected object.

Shift: Duplicates movement.

## Loading FBX and textures
User can drag and drop .fbx files onto the engine to load them.
The same can be done with .png .dds and .jpgs to load them as texutre for the selected object.

# Additional Funcionalities
- The option to open all main menu buttons as windows pressing the first option that is (::::::::::::) a line of dots. (Such as Maya 2020 does).
- Render menu where the user can enable/disable Depth Test, Cull Face, Lighting, Color Material, Textures and Wireframe mode.
- User can move around objects playing with the Local Transformation on the inspector, moving, rotating and scaling the object.
- Docking between the diferent windows displayed on the engine.
- Aditional moving functions such as: E and R to move up and down, or ALT + pressing mouse wheel to move in a plane the same way that unity does.




 