RANK_TARGET := $(addprefix $(BIN_PATH)/, PageRank)
RANK_SOURCES := $(addprefix $(RANK_PATH)/, Main.cpp Graph.cpp)
RANK_INCLUDES := $(addprefix $(RANK_PATH)/, Graph.h)

.PHONY: RANK_all RANK_clean

$(RANK_TARGET): $(RANK_SOURCES) $(RANK_INCLUDES)
	$(CXX) -o $(RANK_TARGET) $(RANK_SOURCES) $(CXXFLAGS)

RANK_clean:
	-rm -f $(RANK_TARGET)
