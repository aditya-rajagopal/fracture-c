# Release Notes

## v0.1.0
 * Logging system and assertions in Fracture.
 * Created a platform layer library to abstract platform specific commands.
 * Move entry point to the engine and modify logging to have different prefixes depending on the log source
 * Create the main application loop in the engine and create a application handle to manage client application lifetime.
 * Add a basic memory system into the engine that tracks memory usage by tagged allocation.
 * Added darray container API