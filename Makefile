# Params
TARGETNAME = cexpress
TARGET = app/bin/lib$(TARGETNAME).a

SOURCE = $(patsubst app/src/%.cpp, app/build/%.o, $(wildcard app/src/*.cpp) $(wildcard app/src/**/*.cpp))
GNUPARAMS = -Iapp/include

NLOHMANN_JSON = app/include/nlohmann/json.hpp
DEPENDENCIES = $(NLOHMANN_JSON)

TEST = app/test/main

# Fundamental Recipes
app/build/%.o: app/src/%.cpp
	mkdir -p $(dir $@)
	g++ $(GNUPARAMS) -o $@ -c $<

app/test/%.o: app/test/%.cpp
	g++ $(GNUPARAMS) -o $@ -c $<

$(TARGET): $(SOURCE) $(DEPENDENCIES)
	mkdir -p $(dir $@)
	ar rcs $(TARGET) $(SOURCE)

# Dependencies
$(NLOHMANN_JSON):
	curl https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp --create-dirs -o $(NLOHMANN_JSON)

# Test Recipes
$(TEST).exe: $(TEST).o $(TARGET)
	g++ -o $@ $< -Lapp/bin -l$(TARGETNAME)

# Final recipes
.PHONY: build test clean
install: $(DEPENDENCIES)
build: $(TARGET)
test: $(TEST).exe
	$<

clear:
	rm -rf app/build app/bin app/test/*.o app/test/*.exe