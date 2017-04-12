UI_C := cppcms_tmpl_cc
UI_TEMPLATE_OBJECTS := $(addprefix $(UI_PATH)/, Form.cpp)
UI_CFLAGS := -I/usr/local/include -L/usr/local/lib -lcppcms -lbooster

UI_TARGET := $(addprefix $(BIN_PATH)/, Ui)
UI_TEMPLATES := $(addprefix $(UI_PATH)/, Form.tmpl)
UI_SOURCES := $(addprefix $(UI_PATH)/, Main.cpp Form.cpp Trie.cpp Ui.cpp)
UI_INCLUDES := $(addprefix $(UI_PATH)/, Content.h Trie.h Ui.h)
UI_CONFIG := $(addprefix $(UI_PATH)/, Config.js)

.PHONY: UI_all UI_clean

$(UI_TARGET): $(UI_TEMPLATE_OBJECTS) $(UI_SOURCES) $(UI_TEMPLATES) $(UI_INCLUDES) $(UI_CONFIG)
	$(CXX) -o $(UI_TARGET) $(UI_SOURCES) $(CXXFLAGS) $(UI_CFLAGS)

$(UI_TEMPLATE_OBJECTS): $(UI_TEMPLATES)
	$(UI_C) -o $(UI_TEMPLATE_OBJECTS) $(UI_TEMPLATES)

UI_clean:
	-rm -f $(UI_TARGET)
