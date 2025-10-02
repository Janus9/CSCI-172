DFLAGS = -g -O0 -Wall 
RFLAGS = -O2
LKFLAGS = -L../common/lib -lSoil -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32
IFLAGS = -I../common/include


debug: clean
	g++ main.cpp \
	$(DFLAGS) \
	$(IFLAGS) \
	$(LKFLAGS) \
	-o main_debug.exe 
	@echo "--- Debug Built Successfully ! ---"

release: clean
	g++ main.cpp \
	$(RFLAGS) \
	$(IFLAGS) \
	$(LKFLAGS) \
	-o main_release.exe 
	@echo "--- Release Built Successfully ! ---"

clean:
	del *.exe