# Version
VERSION = 0.3.0

# Compiler settings
CC = 
CFLAGS = -std=c++20 -Werror -static -static-libgcc -static-libstdc++

# Folders
SRC = src
INCLUDE = include
BIN = bin
BUILD = build
LIB = lib
TEST = test
RELEASE = release
CFL := $(BIN) $(BUILD) $(LIB) $(RELEASE)	# Create Folders List

# Files
LIBA = libtui.a
LIBNAME = tui
TESTFILE = $(filter-out sysinfo clean cleanall zip, $(MAKECMDGOALS))
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(addprefix $(BUILD)/, $(notdir $(SRCS:.cpp=.o)))

# Executables
EXE = 

# OS
ifeq ($(OS), win64)
	CC = x86_64-w64-mingw32-g++
	EXE = $(basename $(TESTFILE)).exe
else ifeq ($(OS), linux64)
	CC = g++
	EXE = $(basename $(TESTFILE))
endif

# Aesthetics
GREEN = \033[0;32m
RESET = \033[0m

all: sysinfo $(CFL) $(LIBA)

# Build c files into object files
$(BUILD)/%.o: $(SRC)/%.cpp
	@echo -n 'Building object $^: '
	@ $(CC) -c $(CFLAGS) $^
	@ mv *.o $(BUILD)
	@echo -e '	$(GREEN)Done$(RESET)'

# Create static library archive
$(LIBA): $(OBJS)
	@echo -n 'Creating static library archive: '
	@ ar -rcs $@ $^
	@ cp -p $@ $(LIB)/
	@ rm $@
	@echo -e '	$(GREEN)Done$(RESET)'

# Compile the test file
$(TESTFILE): all
	@ $(CC) $(CFLAGS) -o $(BIN)/$(EXE) $(TEST)/$(TESTFILE) -I$(INCLUDE)/ -L$(LIB)/ -l$(LIBNAME) && ./$(BIN)/$(EXE)

# Check if all needed directory exists, if not, creates it
$(CFL):
ifeq ("$(wildcard $@)", "")
	@echo -n 'Creating $@ folder: '
	@ mkdir $@
	@echo -e '			$(GREEN)Done$(RESET)'
endif

# Print architecture info
sysinfo:
	@echo 'Building for $(OS)'

# Clear folders
clean:
	rm $(BIN)/* $(BUILD)/* $(LIB)/*

# Clear folders and delete the directories
cleanall:
	rm -r $(BIN)/ $(BUILD)/ $(LIB)/

zip: $(CFL)
	@echo OS: $(OS)
ifeq ($(OS), win64)
	@echo 'To DO: Windows zip'
else
	tar -czvf $(RELEASE)/$(VERSION)_linux_x64.tar.gz $(INCLUDE) $(LIB)
endif
