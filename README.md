# Light Input
A light-weight input handler with minimal dependencies for GLFW applications!

## Current Version: 1.0
## Changelog:
- Initial project release, features detailed below

# Description
Light Input (LI) is a wrapper and system for GLFW applications to create a simple and adaptable input interface with minimal overhead or dependencies!

## Dependencies
- GLFW/glad
- Nlohmann Json (for the serialisation of input profiles)

# Features
- Input Registration to named actions! Multiple keys can be bound and listened to by a name rather than a key code
- Input Profiles that bundle single button actions, 1 dimensional and 2 dimensional axes!

# Planned features
- Gamepad support!
- Decoupling of glm dependency to just use a custom Vec2 type!

# Setup!
- The setup is simple! Drag and include the header/source file in your project
- When you create an instance of the InputListener, also ensure to attach a function that accesses the current frame count of your application!
- Then you can call the UpdateInputState() function once per frame, and pass in that frame's deltaTime, this is used for the calculation of Mouse Velocity