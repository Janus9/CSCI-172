# Make Settings
SHELL := powershell.exe
.SHELLFLAGS := -NoProfile -Command
.ONESHELL:

# Compiler Flags
CXX := g++ 
R_FLAGS := -O2 																	# Release Flags
D_FLAGS := -g -O0																# Debug Flags
INCLUDE := -Iinclude -I../common/include 										# Headers
LIB := -L../common/lib -lSoil -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 	# Libraries

# Directors
SRC_DIR := src
BIN_DIR := bin
BUILD_DIR := build
BUILD_R_DIR := build\release
BUILD_D_DIR := build\debug
IMG_DIR := images
MDL_DIR := models

# Files
OUTPUT = main.exe
SRCS := $(wildcard $(SRC_DIR)/*.cpp)											# List of source files
BINS := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))					# List of binary files										# Adds main to binary list

.PHONY: debug clean

debug: $(BUILD_D_DIR)/$(OUTPUT)													

# -- DEBUG -- #

# Debug Linking	
$(BUILD_D_DIR)/$(OUTPUT): $(BINS) | $(IMG_DIR) $(BUILD_D_DIR)
	$(CXX) $(BINS) $(LIB) -o $@
	@echo " -- Binaries Linked Successfully! -- "

# Debug Compilation with src
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) -c $< $(INCLUDE) $(D_FLAGS) -o $@
	@echo " -- Compiled $< Successfully! -- "

# Makes bin directory
$(BIN_DIR):
	@echo "Missing $@ folder, creating a new one now . . . "
	New-Item -ItemType Directory -Force -Path $@ | Out-Null

# Makes images directory
$(IMG_DIR):
	@echo "Missing $@ folder, creating a new one now . . . "
	New-Item -ItemType Directory -Force -Path $@ | Out-Null
 
# Makes build directory
$(BUILD_DIR):
	@echo "Missing $@ folder, creating a new one now . . . "
	New-Item -ItemType Directory -Force -Path $@ | Out-Null
# Makes debug directory inside of build
$(BUILD_D_DIR): | $(BUILD_DIR)
	@echo "Missing $@ folder, creating a new one now . . . "
	New-Item -ItemType Directory -Force -Path $@ | Out-Null

# Makes release directory inside of build
$(BUILD_R_DIR): | $(BUILD_DIR)
	@echo "Missing $@ folder, creating a new one now . . . "
	New-Item -ItemType Directory -Force -Path $@ | Out-Null

# Makes models directory
$(MDL_DIR):
	@echo "Missing $@ folder, creating a new one now . . . "
	New-Item -ItemType Directory -Force -Path $@ | Out-Null

# Removes .o files
clean:
	Remove-Item -Force -ErrorAction SilentlyContinue "$(BIN_DIR)\*.o"
	@echo "-- Binaries Cleaned Successfully -- "
	Remove-Item -Force -ErrorAction SilentlyContinue "$(BUILD_D_DIR)\*.exe"
	@echo "-- Executable Cleaned Successfully -- "