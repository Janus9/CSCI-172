debug:
	echo "Building as Debug . . . "
	g++ main.cpp \
	-g -O0 -Wall \
	-I../common/include \
	-L../common/lib -lSoil -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 \
	-o main_debug.exe

release:
	echo "Building as Release . . . "
	g++ main.cpp \
	-O2 \
	-I../common/include \
	-L../common/lib -lSoil -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 \
	-o main_release.exe

clean:
	echo "Cleaning builds . . . "
	del *.exe