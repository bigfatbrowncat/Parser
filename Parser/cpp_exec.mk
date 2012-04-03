################### Main part ###################

CC = gcc $(COMMON_CFLAGS) $(CFLAGS) $(CDEFINES)
CPP = g++ $(COMMON_CPPFLAGS) $(CPPFLAGS) $(CPPDEFINES)
COMMON_CPPFLAGS += $(COMMON_CFLAGS)
CPPFLAGS += $(CFLAGS)
CPPDEFINES += $(CDEFINES)

OSTYPE := $(shell uname)

TARGET_BIN = $(TARGET)/bin
TARGET_OBJ = $(TARGET)/obj

EXECUTABLE = $(TARGETNAME).exe

OBJECT_FILES_WITH_PATH = $(addprefix $(TARGET_OBJ)/,$(addsuffix .o,$(OBJECTS)))
HEADERS_WITH_PATH = $(addprefix $(INCLUDE)/,$(shell cd $(INCLUDE); find . -name \*.h))

all: executable

clean:
	@echo "[$(PROJ)] Removing target executable..."
	rm -f $(TARGET_BIN)/$(EXECUTABLE)

	@echo "[$(PROJ)] Removing object files..."
	rm -f $(OBJECT_FILES_WITH_PATH)

	@echo "[$(PROJ)] Removing empty directories..."
	find $(TARGET) -depth -empty -type d -exec rmdir {} \;

executable: $(TARGET_BIN)/$(EXECUTABLE)

################### Folders ###################

ENSURE_BIN = if [ ! -d "$(TARGET_BIN)" ]; then mkdir -p "$(TARGET_BIN)"; fi
ENSURE_OBJ = if [ ! -d "$(TARGET_OBJ)" ]; then mkdir -p "$(TARGET_OBJ)"; fi

################### Objects ###################

$(OBJECT_FILES_WITH_PATH) : $(TARGET_OBJ)/%.o : $(SOURCE)/%.cpp $(HEADERS_WITH_PATH) 
	@echo "[$(PROJ)] Compiling $@ ..."
	$(ENSURE_OBJ)
	if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	$(CPP) -c $< -o $@ -I$(INCLUDE)

################### Targets ###################

$(TARGET_BIN)/$(EXECUTABLE): $(OBJECT_FILES_WITH_PATH) 
	@echo "[$(PROJ)] Building executable $@ ..."
	$(ENSURE_BIN)
	$(CPP) $(OBJECT_FILES_WITH_PATH) $(LIBS) -o $@

############### Dependent libs ################

.PHONY: all executable clean
.SILENT: