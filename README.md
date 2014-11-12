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
where N is twice then number of your CPU cores.

Running
===
You can run the application from within the `build` or `zadanieN` directory.

<pre>
cd build
./zadanieN
</pre>

where N is the task number.
