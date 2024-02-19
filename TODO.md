# Things needed in the Fracture Engine
This todo list has been taken from the [kohi engine roadmap](https://github.com/travisvroman/kohi/blob/main/TODO.md) and slightly modified for my purpose as a rough guide. I am using the [kohi engine videos](https://www.youtube.com/playlist?list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj) as a tutorial for this project as a learning experience.
The items in this list are not in any particular order. This list will be updated occasionally as development progresses.

## Engine general:
- [ ] logging
  - [x] Basic logging and asserts
  - [ ] Async logging
  - [ ] Logging to ingame/ineditor consoles
- [x] platform layer for now windows only
- [ ] event system
  - [x] Immediate mode event system
  - [ ] Deferred event system
  - [ ] Multithreaded event system
- [ ] clock
- [ ] testing framework
- [ ] math library (vector math, etc)
- [x] Memory system 
- [ ] Generic sorting function/library.
- [ ] Allocators:
  - [ ] linear allocator
  - [ ] dynamic allocator (variable-size allocations)
  - [ ] pool allocator
- [ ] Systems manager
- [ ] Resource system
- [ ] Binary resource packing
- [ ] Resource Loaders:
  - [ ] binary
  - [ ] text
  - [ ] image
  - [ ] material 
  - [ ] bitmap font 
  - [ ] system font 
  - [ ] scene
- [ ] SIMD
- [ ] Containers:
  - [x] darray
  - [ ] stack
  - [ ] hashtable
  - [ ] freelist
  - [ ] dynamic arrays  
  - [ ] ring buffer
  - [ ] queue 
  - [ ] pool 
  - [ ] bst
- [ ] quadtrees/octrees
- [ ] Threads 
- [ ] Semaphores
- [ ] Job system
  - [ ] Job dependencies
  - [ ] Job semaphores/signaling
- [ ] ThreadPools
- [ ] Multi-threaded logger
- [ ] Textures 
  - [ ] binary file format
- [ ] Renderable (writeable) textures 
- [ ] Static geometry 
- [ ] Materials 
- [ ] billboards
- [ ] particles
- [ ] Input:
  - [ ] desktop
  - [ ] touch
  - [ ] gamepad
  - [x] keymaps/keybindings
    - [ ] I18n / keyboard layouts
- [ ] In-game/In-editor Conosole
  - [ ] Console consumer interface
  - [ ] Logger as consumer
  - [ ] Debug console as consumer 
  - [ ] kvars (console variables)
  - [ ] Console commands
- [ ] Application-level configuration
- [ ] high-level string structure library (not c-strings)
- [ ] I18n strings for translations
- [ ] resource hot reloading
- [ ] prefabs
- [ ] Simple Scenes
  - [ ] Base implementation
  - [ ] Load from file 
  - [ ] Adjustable global scene properties
  - [ ] Save to file
- [ ] Renderer System (front-end/backend plugin architecture)
- [ ] Audio System (front-end)
- [ ] Physics System (front-end)
- [ ] networking
- [ ] profiling
- [ ] timeline system
- [ ] skeletal animation system
- [ ] skybox
- [ ] skysphere (i.e dynamic day/night cycles)
- [ ] water plane
- [ ] Raycasting
- [ ] Object picking 
  - [ ] Pixel-perfect picking 
  - [ ] Raycast picking
- [ ] Gizmo (in-world object manipulation)
- [ ] Viewports
- [ ] terrain
  - [ ] binary format
  - [ ] heightmap-based
  - [ ] pixel picking
  - [ ] raycast picking 
  - [ ] chunking/culling
  - [ ] LOD
    - [ ] Blending between LOD levels (geometry skirts vs gap-filling, etc.)
  - [ ] tessellation
  - [ ] holes
  - [ ] collision
- [ ] volumes 
  - [ ] visibility/occlusion
  - [ ] triggers 
  - [ ] physics volumes 
  - [ ] weather
- [ ] Multi-window applications

## Renderer:
- [ ] geometry generation (2d and 3d, e.g. cube, cylinder, etc.)
- [ ] advanced Materials
- [ ] PBR Lighting model
- [ ] batch rendering (2d and 3d)
- [ ] instanced rendering
- [ ] Per-scene vertex/index buffers
- [ ] Queue-up of data uploads during scene load:
  - Notes/ steps involved: 
    - Setup a queue in the scene to hold pending mesh data.
    - For each mesh:
      - Make sure mesh is invalidated so it doesn't attempt to render. 
      - Assign a unique id for it and add it to the queue
      - Load it from disk (multithreaded, currently done but needs some changes). Save off id, size, data, offsets, etc.
      - Reserve space in buffer freelist but _don't_ upload to GPU. Save the offset in the queue as well.
      - NOTE: that this could be used to figure out how large the buffer needs to be.
    - Repeat this for all meshes.
    - In one swoop, upload all vertex and index data to GPU at once, perhaps on a separate (transfer) queue to avoid frame drops.
      - Probably the easiest way is a single vkCmdCopyBuffer call with multiple regions (1 per object), to a STAGING buffer.
      - Once this completes, perform a copy from staging to the appropriate vertex/index buffer at the beginning of the next available frame.
    - After the above completes (may need to setup a fence), validate meshes all at once, enabling rendering.
- [ ] shadow maps
  - [ ] PCF
  - [ ] cascading shadow maps
  - [ ] Adjustable Directional Light properties
    - [ ] max shadow distance/fade (200/25)
    - [ ] cascade split multiplier (0.91)
    - [ ] shadow mode (soft/hard shadows/none)
  - [ ] Percentage Closer Soft Shadows (PCSS)
  - [ ] Point light shadows
- [ ] texture mipmapping
- [ ] Specular maps (NOTE: removed in favour of PBR)
- [ ] Normal maps 
- [ ] Phong Lighting model (NOTE: removed in favour of PBR)
- [ ] Multiple/configurable renderpass support.
- [ ] Rendergraph
  - [ ] Linear processing
  - [ ] Rendergraph Pass Dependencies/auto-resolution
  - [ ] Multithreading/waiting/signaling
  - [ ] Forward rendering specialized rendergraph
  - [ ] Deferred rendering specialized rendergraph
  - [ ] Forward+ rendering specialized rendergraph
- [ ] Forward rendering 
- [ ] Deferred rendering 
- [ ] Forward+ rendering
- [ ] Compute Shader support (frontend)

## Plugins:
 - [ ] ECS (Entity Component System)
 - [ ] Audio (OpenAL plugin)
 - [ ] Vulkan Renderer Plugin
   - [ ] multithreading
     - [ ] texture data upload
     - [ ] mesh data upload
   - [ ] pipeline statistic querying
   - [ ] compute support
 - [ ] OpenGL Renderer Plugin 


## Standard UI: (separated section because number of controls)
- [ ] Standard UI system
- [ ] Layering
- [ ] UI file format
- [ ] Load/Save UIs
- [ ] UI Editor (as a plugin to the editor)
- [ ] control focus (tab order?)
- [ ] docking
- [ ] drag and drop support
- [ ] UI Controls (one of the few engine-level areas that uses OOP):
  * [ ] Base control - all controls come from this
  * [ ] panel
  * [ ] image box
  * [ ] viewport control (world/scenes will be switched to use these as render targets)
  * [ ] rich text control (system text w/ multicolour, bold/italic, etc. and bitmap text with multicolour only)
  * [ ] button
  * [ ] checkbox
  * [ ] radio buttons
  * [ ] tabs
  * [ ] windows/modals (complete with resize, min/max/restore, open/close, etc.)
  * [ ] resizable multi-panels
  * [ ] scrollbar
  * [ ] scroll container
  * [ ] textbox/textarea
  * [ ] In-game debug console

## Editor: mosaic
- [ ] Editor application and 'runtime' executable
  - [ ] World editor
  - [ ] UI editor
  - [ ] editor logic library (dll/.so) hot reload
- [ ] Move .obj, .mtl import logic to editor (output to binary format).
- [ ] Move texture import logic to editor (output to binary format).
- [ ] DDS/KTX texture format imports
- [ ] FBX model imports 

## Level Generator: tessel
- [ ] Level Generation tool made using Fracture to generate proc generated levels
  - [ ] It can be a module that is loaded into the editor
  - [ ] Takes in 3D/2D tiles and allows rules to generate levels
  - [ ] Produces a binary format file that can be loaded up with the game engine to populate scenes.

## Other items:
- [x] Split off "core" items (defines, memory, strings, containers, etc.) into a "core" or "foundation" library so they may be used without having to pull in all of the engine.
- [x] Split off platform layers into separate libraries outside the engine.
- [ ] Auto-Generated API documentation
- [ ] Documentation
- [ ] Continuous Integration

Back to [readme](readme.md) 