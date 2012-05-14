GCC = g++
CFLAGS = -Wall -Wextra -ggdb
LINKFLAGS = -ggdb

SRC_DIR = src
BIN_DIR = bin

CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CPP_FILES))

COMPETITIE_DATASTRUCTURES_MODULES = $(BIN_DIR)/Competitie.o $(BIN_DIR)/CompetitieParameters.o $(BIN_DIR)/Partij.o $(BIN_DIR)/Ranglijst.o \
                                    $(BIN_DIR)/RanglijstItem.o $(BIN_DIR)/Ronde.o $(BIN_DIR)/Speler.o $(BIN_DIR)/Spelerslijst.o
MISC_MODULES = $(BIN_DIR)/KeizerIO.o

all: $(BIN_DIR)/FreeKeizer $(BIN_DIR)/Unittest $(BIN_DIR)/GAtest

$(BIN_DIR)/FreeKeizer: $(BIN_DIR)/FreeKeizer.o $(COMPETITIE_DATASTRUCTURES_MODULES) $(MISC_MODULES)
	g++ $(LINKFLAGS) -o $(BIN_DIR)/FreeKeizer $(BIN_DIR)/FreeKeizer.o $(COMPETITIE_DATASTRUCTURES_MODULES) $(MISC_MODULES)

$(BIN_DIR)/Unittest: $(BIN_DIR)/Unittest.o  $(COMPETITIE_DATASTRUCTURES_MODULES) $(MISC_MODULES)
	g++ $(LINKFLAGS) -o $(BIN_DIR)/Unittest $(BIN_DIR)/Unittest.o  $(COMPETITIE_DATASTRUCTURES_MODULES) $(MISC_MODULES)

$(BIN_DIR)/GAtest: $(BIN_DIR)/GAtest.o $(BIN_DIR)/GeneticAlgorithm.o
	g++ $(LINKFLAGS) -o $(BIN_DIR)/GAtest $(BIN_DIR)/GAtest.o $(BIN_DIR)/GeneticAlgorithm.o

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) -c $(CFLAGS) -o $@ $<

clean:
	rm $(BIN_DIR)/*.o
