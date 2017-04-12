CXX := g++
CXXFLAGS := -std=c++11
INDEX_PATH := ./Preprocess/Index
RANK_PATH := ./Preprocess/Rank
SPLIT_PATH := ./Preprocess/Split
UI_PATH := ./Ui
BIN_PATH := ./Bin
RAW_PATH := ./RawData
DATA_PATH := ./FormattedData

RAW_TEXT := $(addprefix $(RAW_PATH)/, RawText.txt)
RAW_LINK := $(addprefix $(RAW_PATH)/, RawLink.txt)
INDEX := $(addprefix $(DATA_PATH)/, Index.txt)
RANK := $(addprefix $(DATA_PATH)/, Rank.txt)

.PHONY: default all run clean clean-data

default: all

include $(INDEX_PATH)/Makefile
include $(RANK_PATH)/Makefile
include $(SPLIT_PATH)/Makefile
include $(UI_PATH)/Makefile

all: $(INDEX_TARGET) $(SPLIT_TARGET) $(RANK_TARGET) $(UI_TARGET)

$(INDEX): $(INDEX_TARGET) $(RAW_TEXT)
	$(INDEX_TARGET) $(RAW_TEXT) $(INDEX)

$(RANK): $(RANK_TARGET) $(RAW_LINK)
	$(RANK_TARGET) $(RAW_LINK) $(RANK)

$(DATA_PATH)/splitdone: $(SPLIT_TARGET) $(RAW_TEXT)
	$(SPLIT_TARGET) $(RAW_TEXT) $(DATA_PATH)/
	touch $(DATA_PATH)/splitdone

run: $(INDEX) $(DATA_PATH)/splitdone $(RANK) $(UI_TARGET) $(UI_CONFIG)
	$(UI_TARGET) -c $(UI_CONFIG)

clean: INDEX_clean RANK_clean SPLIT_clean UI_clean

clean-data:
	-rm -r $(DATA_PATH)
	mkdir $(DATA_PATH)
