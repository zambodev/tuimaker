# Compiler settings
CC = g++
CFLAGS = -std=c++17

# Folders
SRC = src
BIN = bin
BUILD = build
LIB = lib
TEST = test
CFL := $(BIN) $(BUILD) $(LIB)	# Create Folders List

# Files
LIBA = libtui.a
INCLIB = tui
TESTFILE = $(filter-out debug, $(MAKECMDGOALS))
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(addprefix $(BUILD)/, $(notdir $(SRCS:.cpp=.o)))

# Executables
EXE = $(basename $(TESTFILE))

# Architecture
ARCH = $(shell uname -m)

# Aesthetics
GREEN = \033[0;32m
RESET = \033[0m

all: $(CFL) $(LIBA)

# Build c files into object files
$(BUILD)/%.o: $(SRC)/%.cpp
	@echo -n 'Building object $^: '
	@ $(CC) -c $(CFLAGS) $^
	@ mv *.o $(BUILD)
	@echo -e '	$(GREEN)Done$(RESET)'

# Create static library archive
$(LIBA): $(OBJS)
	@echo -n 'Creating static library archive: '
	@ ar -rcs $@ $(BUILD)/*
	@ cp -p *.a $(LIB)/
	@ rm *.a
	@echo -e '	$(GREEN)Done$(RESET)'

# Compile the test file
$(TESTFILE): all
	@ $(CC) $(CFLAGS) -o $(BIN)/$(EXE) $(TEST)/$(TESTFILE) -I$(SRC)/ -L$(LIB)/ -l$(INCLIB)
	./$(BIN)/$(EXE)

# Check if all needed directory exists, if not, creates it
$(CFL):
ifeq ("$(wildcard $@)", "")
	@echo -n 'Creating $@ folder: '
	@ mkdir $@
	@echo -e '			$(GREEN)Done$(RESET)'
endif

# Clear folders
clean:
	rm $(BIN)/* $(BUILD)/* $(LIB)/*

# Clear folders and delete the directories
cleanall:
	rm -r $(BIN)/ $(BUILD)/ $(LIB)/
