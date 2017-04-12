INDEX_TARGET := $(addprefix $(BIN_PATH)/, Index)
INDEX_SOURCES := $(addprefix $(INDEX_PATH)/, Main.cpp Trie.cpp)
INDEX_INCLUDES := $(addprefix $(INDEX_PATH)/, Trie.h)

.PHONY: INDEX_all INDEX_clean

$(INDEX_TARGET): $(INDEX_SOURCES) $(INDEX_INCLUDES)
	$(CXX) -o $(INDEX_TARGET) $(CXXFLAGS) $(INDEX_SOURCES)

INDEX_clean:
	-rm -f $(INDEX_TARGET)
