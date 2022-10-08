CC = clang++
SRC_DIR = src
OBJ_NAME = main
BUILD_DIR = build
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# MacOS
FRAMEWORK_DIR_MAC = -F/Library/Frameworks -framework SDL2 -F/Library/Frameworks -framework SDL2_image -F/Library/Frameworks -framework SDL2_mixer  -F/Library/Frameworks -framework SDL2_ttf
INCLUDE_PATHS_MAC = -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers -I/Library/Frameworks/SDL2_mixer.framework/Headers -I/Library/Frameworks/SDL2_ttf.framework/Headers

mac:
	$(CC) $(INCLUDE_PATHS_MAC) $(FRAMEWORK_DIR_MAC) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)