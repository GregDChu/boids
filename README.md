# boids
A program that simulates animal grouping behaviors using three basic principles: avoidance, alignment, and attraction. The source code is programmed in C++ using OpenGL among other complementary graphics libraries. The code was developed in Microsoft Visual Studio.

# Setup
Should you desire to test this program, you are welcome to download it and set it up. To do so, you will need to import the source code into a Microsoft Visual Studio C++ project source file. Further, you will need to link to and set up the following libraries:
* glew
* glfw
* glm
* imgui

Ensure that for glew, you also define the macro GLEW_STATIC in your preferences. Once everything is linked, the project should build properly.

# Settings
The flock simulation has several settings. Below are the description of what each setting does. All default settings are stored in flock.h as macros.

## Speed
Adjusts the maximum speed of the flock. A 0 values means no speed and a 1 value means the maximum speed allowed. The maximum speed is set by the macro MAX_SPEED.

## Speed Variation
Enables or disables variation of speed among the flock members. It is set to disabled by default with the macro VARIABLE_SPEEDS.

## Distance
The range to which each flock member can see to determine their next move. A larger range means each flock member incorporates more of its neighbors to decide its next move. A smaller range means each flock member incorporates fewer of its neighbors when deciding its next move.

## Cone of Vision
The angle from the center of the flock member's vision to the left and right of which the flock member can see. A smaller cone of vision means the flock member can't see members behind it, or next to it.

## Avoidance
The avoidance determines to what degree each flock member tries to avoid other flock members. A negative value asserts attraction to flock members closest to it.

## Alignment
The alignment determines to what degree each flock member tries to match its neighbors' heading direction. A negative value asserts that each flock member will start traveling in the opposite direction as its neighbors.

## Attraction
The attraction determines to what degree each flock member tries to steer towards its neighboring members. Only members at the boundary of its vision are considered for this factor, and members closer to the flock member are ignored. A negative value asserts attraction to flock members farther away from it.

## Randomize
The randomize button will randomize all attributes of the flock except for the flock's maximum speed.
