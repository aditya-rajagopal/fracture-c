# Release Notes

## v0.1.0
 * Logging system and assertions in Fracture.
 * Created a platform layer library to abstract platform specific commands.
 * Move entry point to the engine and modify logging to have different prefixes depending on the log source
 * Create the main application loop in the engine and create a application handle to manage client application lifetime.
 * Add a basic memory system into the engine that tracks memory usage by tagged allocation.
 * Added darray container API
 * Added an immediate mode events system
 * Added an input system to handle processing platform messages and to store states of mouse and keyboard for polling
 * Added key and button specific events so you dont have to listen to all key presses and releases.