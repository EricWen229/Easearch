SPLIT_TARGET := $(addprefix $(BIN_PATH)/, Split)
SPLIT_SOURCES := $(addprefix $(SPLIT_PATH)/, Main.cpp)

.PHONY: SPLIT_all SPLIT_clean

$(SPLIT_TARGET): $(SPLIT_SOURCES)
	$(CXX) -o $(SPLIT_TARGET) $(SPLIT_SOURCES) $(CXXFLAGS)

SPLIT_clean:
	-rm -f $(SPLIT_TARGET)
