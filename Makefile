# Determine the OS
UNAME_S := $(shell uname -s)

# Common settings
TARGETNAME = cexpress
TARGET = app/bin/lib$(TARGETNAME).a

SOURCE = $(patsubst app/src/%.cpp, app/build/%.o, $(wildcard app/src/*.cpp) $(wildcard app/src/**/*.cpp))

GNUPARAMS = -Iapp/include

ifeq ($(UNAME_S),Linux)
	create_dir = $(shell mkdir -p $1)
	OSLIBS =
else
	create_dir = $(shell mkdir $(subst /,\,$1))
    GNUPARAMS += -std=c++17

	OSLIBS = -lws2_32
endif

NLOHMANN_JSON = app/include/nlohmann/json.hpp
DEPENDENCIES = $(NLOHMANN_JSON)

TEST = app/test/main

# Fundamental Recipes
app/build/%.o: app/src/%.cpp
	$(call create_dir, $(dir $@))
	g++ $(GNUPARAMS) -o $@ -c $<

app/test/%.o: app/test/%.cpp
	$(call create_dir, $(dir $@))
	g++ $(GNUPARAMS) -o $@ -c $<

$(TARGET): $(DEPENDENCIES) $(SOURCE)
	$(call create_dir, $(dir $@))
	ar rcs $(TARGET) $(SOURCE)

# Dependencies
$(NLOHMANN_JSON):
	curl -k https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp --create-dirs -o $(NLOHMANN_JSON)

# Test Recipes
$(TEST).exe: $(TARGET) $(TEST).o
	g++ -o $@ $(TEST).o -Lapp/bin -l$(TARGETNAME) $(OSLIBS)

# Final recipes
.PHONY: build test clean install
install: $(DEPENDENCIES)
build: $(TARGET)
test: $(TEST).exe
	$<

ifeq ($(UNAME_S),Linux)
clear:
	rm -rf app/build app/bin app/test/*.o app/test/*.exe
else
clear:
	@if exist app\build (rmdir /S /Q app\build)
	@if exist app\bin (rmdir /S /Q app\bin)
	del /Q app\test\*.o app\test\*.exe
endif
