# Params
TARGETNAME = cexpress
TARGET = app/bin/lib$(TARGETNAME).a

SOURCE = $(patsubst app/src/%.cpp, app/build/%.o, $(wildcard app/src/**/*.cpp))
GNUPARAMS = -Iapp/include

TEST = app/test/main

# Fundamental Recipes
app/build/%.o: app/src/%.cpp
	mkdir -p app/build
	g++ $(GNUPARAMS) -o $@ -c $<

app/test/%.o: app/test/%.cpp
	g++ $(GNUPARAMS) -o $@ -c $<

$(TARGET): $(SOURCE)
	mkdir -p app/bin
	ar rcs $(TARGET) $(SOURCE)

# Test Recipes
$(TEST).exe: $(TEST).o $(TARGET)
	g++ -o $@ $< -Lapp/bin -l$(TARGETNAME)

# Final recipes
.PHONY: build test clean
build: $(TARGET)
test: $(TEST).exe
	$<

clear:
	rm -rf app/build app/bin app/test/*.o app/test/*.exe