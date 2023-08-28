#Version
VERSION = 0.4.0

#Compiler settings
CC = 
CFLAGS = -std=c++20 -Werror -static -static-libgcc -static-libstdc++ -lpthread -g -Og

#Folders
SRC = src
INCLUDE = include
BIN = bin
BUILD = build
LIB = lib
TEST = test
RELEASE = release
CFL := $(BIN) $(BUILD) $(LIB) $(RELEASE)	# Create Folders List

#Files
LIBA = libtui
LIBNAME = tui
TESTFILE = $(filter %.c %.cpp, $(MAKECMDGOALS))
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(addprefix $(BUILD)/, $(notdir $(SRCS:.cpp=.o)))

# Executables
EXE = 

# LOS (Library for OS)
ifeq ($(CCS), win)
	LIBA := $(addsuffix .lib, $(LIBA))
else ifeq ($(COS), linux)
	LIBA := $(addsuffix .a, $(LIBA))
endif

# OS (Run on OS)
ifeq ($(OS), win64)
	CC = /mnt/c/mingw64/bin/g++.exe
	EXE = $(basename $(TESTFILE)).exe
else ifeq ($(OS), linux64)
	CC = g++
	EXE = $(basename $(TESTFILE))
endif

# Aesthetics
GREEN = \033[0;32m
RESET = \033[0m

all: clean sysinfo $(CFL) $(LIBA)

# Build c files into object files
$(BUILD)/%.o: $(SRC)/%.cpp
	@echo -n "Compiling $^: "
	@ $(CC) -c $(CFLAGS) $^
	@ mv *.o $(BUILD)
	@echo  "  $(GREEN)Done$(RESET)"

# Create static library archive
$(LIBA): $(OBJS)
	@echo -n "Creating archive: "
	@ ar -rcs $@ $^
	@ mv $@ $(LIB)/
	@echo  "  $(GREEN)Done$(RESET)"

# Compile the test file
$(TESTFILE): all
	@echo -n "Linking: "
	@ $(CC) $(CFLAGS) -o $(BIN)/$(EXE) $(TEST)/$(TESTFILE) -I$(INCLUDE)/ -L$(LIB)/ -l$(LIBNAME)
	@echo  "  $(GREEN)Done$(RESET)"
	@echo -n "Starting program..."
# ./$(BIN)/$(EXE)

# Check if all needed directory exists, if not, creates it
$(CFL):
ifeq ("$(wildcard $@)", "")
	@echo -n "Creating $@ folder: "
	@ mkdir $@
	@echo  "  $(GREEN)Done$(RESET)"
endif

# Print architecture info
sysinfo:
	@echo "Building for $(OS)"

# Clear folders
clean:
	@echo -n "Cleaning directories: "
	@rm $(BIN)/* $(BUILD)/* $(LIB)/* 2> /dev/null || true
	@echo  "  $(GREEN)Done$(RESET)"

# Clear folders and delete the directories
cleanall:
	@echo -n "Deleting directories: "
	@rm -r $(BIN)/ $(BUILD)/ $(LIB)/
	@echo  "  $(GREEN)Done$(RESET)"

zip: $(CFL) clean $(OBJS)
ifeq ($(OS), win64)
	@ ar -rcs libtui.lib $(OBJS)
	@ mv libtui.lib $(LIB)/
	zip -r $(RELEASE)/$(VERSION)_win64.zip $(INCLUDE) $(LIB)
else
	@ ar -rcs libtui.a $(OBJS)
	@ mv libtui.a $(LIB)/
	tar -czvf $(RELEASE)/$(VERSION)_linux_x64.tar.gz $(INCLUDE) $(LIB)
endif
