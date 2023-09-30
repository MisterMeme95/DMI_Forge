. . . 
. . .
https://docs.conan.io/2/integrations/clion.html
https://www.jetbrains.com/help/clion/how-to-create-toolchain-in-clion.html#custom-targets-toolchain
To install this properly, you need to download:

	A) MingW (for gcc and g++ compilers)
	B) Download Cmake
	C) Download Clion
	D) Conan (make sure the paths are included in the system) - to get libpng and zlib added to the project.
	E) File > Setting > Plugins > download Conan (in Clion)
	F) View > Tool Windows > Conan -- then install libpng (zlib is included automatically)