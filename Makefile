CC = g++
CFLAGS = -Wall -std=c++0x -Wextra -ggdb
LINKFLAGS = -std=c++0x -ggdb
RM = rm

SRC_DIR = src
BIN_DIR = bin

CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CPP_FILES))

COMPETITIE_DATASTRUCTURES_MODULES = $(BIN_DIR)/Competition.o $(BIN_DIR)/CompetitieParameters.o $(BIN_DIR)/Game.o $(BIN_DIR)/Ranking.o \
                                    $(BIN_DIR)/RankingItem.o $(BIN_DIR)/Round.o $(BIN_DIR)/Player.o $(BIN_DIR)/PlayerList.o
MISC_MODULES = $(BIN_DIR)/IOServices.o $(BIN_DIR)/PairingGenerator.o $(BIN_DIR)/GAPairingGenerator.o

all: $(BIN_DIR)/FreeKeizer $(BIN_DIR)/Unittest $(BIN_DIR)/GAtest

$(BIN_DIR)/FreeKeizer: $(BIN_DIR)/FreeKeizer.o $(COMPETITIE_DATASTRUCTURES_MODULES) $(MISC_MODULES)
	$(CC) $(LINKFLAGS) -o $(BIN_DIR)/FreeKeizer $(BIN_DIR)/FreeKeizer.o $(COMPETITIE_DATASTRUCTURES_MODULES) $(MISC_MODULES)

$(BIN_DIR)/Unittest: $(BIN_DIR)/Unittest.o  $(COMPETITIE_DATASTRUCTURES_MODULES) $(MISC_MODULES)
	$(CC) $(LINKFLAGS) -o $(BIN_DIR)/Unittest $(BIN_DIR)/Unittest.o  $(COMPETITIE_DATASTRUCTURES_MODULES) $(MISC_MODULES)

$(BIN_DIR)/GAtest: $(BIN_DIR)/GAtest.o $(BIN_DIR)/GeneticAlgorithm.o
	$(CC) $(LINKFLAGS) -o $(BIN_DIR)/GAtest $(BIN_DIR)/GAtest.o $(BIN_DIR)/GeneticAlgorithm.o

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(RM) $(BIN_DIR)/*.o
