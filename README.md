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

Running
===
Make sure the application is run from within the `zadanie2` dirctory, as it
loads some additional files on runtime.

<pre>
cd zadanie2
./zadanie2
</pre>
