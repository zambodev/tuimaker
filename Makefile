# Version
VERSION = 0.2.0

# Compiler settings
CC = g++
CFLAGS = -std=c++20 -Werror

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
EXE = $(basename $(TESTFILE))

# OS
ifeq ($(OS), Windows_NT)
	OS = Windows
else
	OS = Linux
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
	@ cp -p *.a $(LIB)/
	@ rm *.a
	@echo -e '	$(GREEN)Done$(RESET)'

# Compile the test file
$(TESTFILE): $(LIBA)
	@ $(CC) $(CFLAGS) -o $(BIN)/$(EXE) $(TEST)/$(TESTFILE) -l$(LIBNAME) -I$(INCLUDE)/ -L$(LIB)/ && ./$(BIN)/$(EXE)

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
ifeq ($(OS), Linux)
	tar -czvf $(RELEASE)/$(VERSION)_linux_x64.tar.gz $(INCLUDE) $(LIB)
else
	@echo 'To DO: Windows zip'
endif
