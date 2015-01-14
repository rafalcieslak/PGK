Building
===
To build the application, use CMake. You will need at libgl-dev and xorg-dev.
Other dependencies, including glew, glfw and libfreetype2 are either provided
with the source, or will be automatically downloaded and build by cmake.

<pre>
mkdir build
cd build
cmake ..
make
</pre>

To speed up the process, use: `make -jN`
where N is twice the number of your CPU cores.

It is possible to force cmake scripts to never detect system libraries (glew,
glfw, freetype and glm). In order to do so, run cmake with `ForceNoLibs` option
enabled, e.g. `cmake -DForceNoLibs=ON ..`

Running
===
You can run the application from within the `build` or `zadanieN` directory.

<pre>
cd build
./zadanieN
</pre>

where N is the task number.
