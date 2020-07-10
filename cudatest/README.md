This code was written to gain practice with moving images between openCV and dlib, and for testing dlib was using cuda
run the program with a collage of faces in a single jpg file on the command line, and the program will parse those faces and use them as references against which a webcam feed can be compared and people in the feed can be identified.

**Installation Details**

After many failed attempts at installing dlib, including using cmake, I removed all versions of my cuda libraries, then reinstalled only versionn 10.2 along with the appropriate cudnn library.
Installs were performed using the Nvidia installer.

After installing cuda & cudnn, I used vcpkg to also install cuda.  It  doesn't actually install cuda.  It merely findsd it on the system and makess it available to other packages.

Then I used vcpkg to install dlib[cuda].  After vcpkg finished its install, i confirmed that the CMakeCache file in vcpkg build tree for dlib successfully compiled the cuda test code and that bool DLIB_USE_CUDA = ON.
* NOTE: until I followed the steps above, i was getting DLIB_USE_CUDA = ON in the cache file but the code as not program was not using cuda.  I assume I configured something incorrectly and it took removiing everything and starting from scratch per the above to actually get dlib to use cuda*
Then, in the VS property sheet for the project, I added the paths to the cuda files.

It was not necessary to provide any path info for dlib on the property sheet since that was taken careof by vcpkg.  Nor was it necessary to add the dlib/all/source.cpp file to the project.  


