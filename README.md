# MonkStudio
MonkStudio is a 3D game engine developed by students at CITM. 
https://github.com/AMADE128/MonkStudio

## Developers
- Himar Bravo González (https://github.com/himar33)
- Marc Pavon Llop (https://github.com/Marckitus)
- Pol Vázquez Porcar (https://github.com/AMADE128)

## How to use
The user can move through the menus by clicking on them.
On the main menu bar at the top of the screen: (each main menu button has an option :::::::::: that opens that menu as a windows, so the user won't have to enter multiple times).
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
Mesh Renderer: Checkbox to display Vertex Normals, AABB and OOBB, of the selected mesh, drop any imported mesh. Also can delete the component.
Material: User can activate/desactivate the texture, see the path and size of the image, and set a default texure over it, drop any imported texture. Also can delete the component.

- Configuartion:
Accses to Application, Window, File System, Hardware and Input options. (also opens with Alt + 4)

Application: The user can change te App name (Monk Studio by default) Cap the FPS to anything between 60 and 1 FPS, and 3 charts with the Framerate, Miliseconds in between, and Process Memory.

Window: User can change the Brightness of the monitor, Width and Height of the engine, enable Fullscreen, making the window resizable or not, Borderless or full desktop.

File System: Shows the base path where the engine is located.

Hardware: Shows what version of SDL, OpenGl, Glew, and Devil the engine is using, the computer CPU, Ram, Caps, GPU and its brand.

Input: Shows in real time the mouse: Position, Motion, and wheel. Also logs all user inputs.

- Assets:
Displays list of all current loaded fbx, and by right-clicking on them the user can create new folders. (also opens with Alt + 5)

### Help
From help the user can open an ImGui Demo, go to our github, releases and report bugs, as well as a window about with information.

### Movement and Shortcuts
User can move with WASD and look arround freely when pressing rightclick, also go verticaly UP or DOWN with E and R

- Mouse + right Click: free look.
- W: forward.
- A: left.
- S: backward.
- D: right.
- Q: UP on Y axis.
- E: DOWN on Y axis.

Alt + right click: Orbit arround the selected object.

Mouse Wheel: Zoom in and out.

Shift: Duplicates movement.

## Loading FBX and textures
User can drag and drop .fbx files onto the engine to load them.
The same can be done with .png .dds and .jpgs to load them as texutre for the selected object.

## Camera
The editor has 2 windows one with the scene and another captured from a
gameobject with a camera component, each one has its own
framebuffer and the user can visualize both at the same time.
Camera is a component.
All meshes use a bounding volume (AABB) and can be discarted using Frustum Culling.
User can activate AABB and OBB on the inspector.

## Custome File Format
Models, meshes and textures are saved to our own format under “Library” folder.

## Resource Manager

- There is an “Assets” window that shows all user assets * it can be a simple tree
viewer
- The user can drop/import new files. Assets window reacts properly
- Asset files can be deleted through the window and /Library entries are removed
- Upon start, all resources not managed inside Assets are generated in Library *
/Library folder should be regenerated from /Assets + meta content
- All resources use reference counting (e.g. a texture / mesh is only once in memory
regardless of how many gameobject use it). This requires this process to be
visualized in the editor.


# Additional Funcionalities
- The option to open all main menu buttons as windows pressing the first option that is (::::::::::::) a line of dots. (Such as Maya 2020 does).
- Render menu where the user can enable/disable Depth Test, Cull Face, Lighting, Color Material, Textures and Wireframe mode.
- Changed style.


## Fork
ModuleViewPortFrameBuffer.cpp, ModuleViewPortFrameBuffer.h, ModuleCamera3D.cpp, ModuleCamera3D.h forked from our teachers GitHub project Engine3D: https://github.com/solidajenjo/Engine3D






 