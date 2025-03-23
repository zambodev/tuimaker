#Version
VERSION = 0.4.0

#Compiler settings
CC = 
CFLAGS = -std=c++20 -Werror

#Folders
INCLUDES = headers
SRC = src
OUT = out
BUILD = $(OUT)/build
LIB = $(OUT)/lib
RELEASE = $(OUT)/release
INCLUDE = $(OUT)/include
TEST = $(OUT)/test
CFL := $(OUT) $(BUILD) $(LIB) $(RELEASE) $(INCLUDE) $(TEST)	# Create Folders List

#Files
LIBA = $(LIB)/libtuimaker
LIBNAME = tuimaker
TESTFILE = $(filter %.cpp, $(MAKECMDGOALS))
SRCS := $(wildcard $(SRC)/*)
OBJS := $(addprefix $(BUILD)/, $(notdir $(SRCS:.cpp=.o)))
CHI := $(wildcard $(INCLUDES)/*)
HEADERS := $(notdir $(CHI))

# Executables
EXE = 

# OS (Run on OS)
ifeq ($(OS), win64)
	LIBA := $(addsuffix .dll, $(LIBA))
	CC = /mnt/c/mingw64/bin/g++.exe
	EXE = $(basename $(notdir $(TESTFILE))).exe
else ifeq ($(OS), linux64)
	LIBA := $(addsuffix .so, $(LIBA))
	CC = g++
	EXE = $(basename $(notdir $(TESTFILE)))
endif

# Aesthetics
GREEN = \033[0;32m
RESET = \033[0m

all: $(CFL) $(LIBA)

# Build cpp files into object files
$(BUILD)/%.o: $(SRC)/%.cpp
	@ echo -n "Compiling $^: "
	@ $(CC) -c -fPIC $(CFLAGS) $^ -o $@
	@ echo  "  $(GREEN)Done$(RESET)"

# Create shared library archive
$(LIBA): $(OBJS)
	@ echo -n "Copying $(CHI) into $(INCLUDE): "
	@ cp $(CHI) $(INCLUDE)/
	@ echo  "  $(GREEN)Done$(RESET)"
	@ echo -n "Creating archive: "
	@ $(CC) -shared -o $(LIBA) $^
	@ echo  "  $(GREEN)Done$(RESET)"

# Compile the test file
$(TESTFILE): all
	@ echo -n "Linking: "
	$(CC) $(CFLAGS) -o $(TEST)/$(EXE) $(TESTFILE) -L$(LIB)/ -I$(INCLUDE)/ -l$(LIBNAME)
	@ echo  "  $(GREEN)Done$(RESET)"
	@ echo -n "Starting program..."
	LD_LIBRARY_PATH=$(LIB)/ ./$(TEST)/$(EXE)

run:
ifneq (,$(wildcard $(TEST)/*))
	LD_LIBRARY_PATH=$(LIB)/ $(TEST)/*
else
	@ echo "File does not exits! Compile an example from 'examples'"
endif

# Check if all needed directory exists, if not, creates it
$(CFL):
ifeq ("$(wildcard $@)", "")
	@ echo -n "Creating $@ folder: "
	@ mkdir $@
	@ echo  "  $(GREEN)Done$(RESET)"
endif

# Clear folders
.PHONY: clean
clean:
	@ echo -n "Cleaning directories: "
	@ rm $(INCLUDE)/* $(BUILD)/* $(LIB)/* $(RELEASE)/* $(TEST)/* 2> /dev/null || true
	@ echo  "  $(GREEN)Done$(RESET)"

# Clear folders and delete the directories
cleanall:
	@ echo -n "Deleting directories: "
	@ rm -r $(OUT)
	@ echo  "  $(GREEN)Done$(RESET)"

zip: sysinfo $(CFL) $(CHI) $(LIBA)
ifeq ($(OS), win64)
	zip -r $(RELEASE)/$(VERSION)_win64.zip $(INCLUDE) $(LIB)
else
	tar -cvf $(RELEASE)/$(VERSION)_linux_x64.tar.gz -C $(INCLUDE) --transform 's|^|include/|' $(HEADERS)
	tar -rf $(RELEASE)/$(VERSION)_linux_x64.tar.gz -C $(LIB) .
endif