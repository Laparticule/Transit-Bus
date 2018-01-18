NAME = tcat # Nom du projet

CC = gcc # Compilateur
CFLAGS = -W -Wall -ansi -pedantic --std=c99# Parametres de compilation
LDLIBS = -lm # Parametres

SRC = $(wildcard src/*.c) # On selectionne tous les fichiers en .c
OBJ = $(SRC:.c=.o) # Les fichiers .c sont compiles independamment les uns des autres

all: $(NAME) # Par defaut, on compile tcat

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME) # Commande de compilation

clean:
	rm -f $(OBJ) $(NAME) # Suppression des .o et de tcat
	rm -rf doc/output # Suppression de la documentation

doc:
	mkdir doc/output # Creation du dossier documentation
	doxygen doc/Doxyfile # Ajout de la documentation

check:
	 cppcheck --quiet --enable=all --suppress=missingIncludeSystem --std=c99  src/

.PHONY: all clean doc # Cette ligne precise à Make qu'il s'agit de commandes et non pas de fichiers
