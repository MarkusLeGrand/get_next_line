#include "get_next_line.h"
#include <fcntl.h> // Pour open()
#include <stdio.h> // Pour printf()

int main(void)
{
    int     fd;
    char    *line;

    // Ouvrir un fichier en mode lecture seule
    fd = open("pomme.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Erreur: Impossible d'ouvrir le fichier.\n");
        return (1);
    }

    // Lire chaque ligne du fichier
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line); // Libérer la mémoire allouée pour chaque ligne
    }

    // Fermer le fichier après lecture
    close(fd);
    return (0);
}
