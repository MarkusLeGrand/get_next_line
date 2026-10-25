# get_next_line

> Une fonction C qui lit un fichier (ou n'importe quel descripteur) ligne par ligne, quelle que soit la taille du buffer de lecture.

![Langage C](https://img.shields.io/badge/langage-C-blue)
![École 42](https://img.shields.io/badge/école-42-black)

## À propos

`get_next_line` est un projet de l'école 42. Il s'agit d'écrire une fonction qui, à chaque appel, renvoie la ligne suivante lue depuis un descripteur de fichier :

```c
char *get_next_line(int fd);
```

La difficulté : `read` lit un nombre fixe d'octets (`BUFFER_SIZE`) qui ne correspond jamais exactement à une ligne. Il faut donc conserver entre deux appels ce qui a été lu « en trop », sans fuite mémoire, avec une seule variable statique.

## Fonctionnalités

- Renvoie la ligne lue **en incluant le `\n`** final (sauf pour la dernière ligne si le fichier ne se termine pas par un saut de ligne).
- Renvoie `NULL` en fin de fichier ou en cas d'erreur (`fd` négatif, `BUFFER_SIZE <= 0`, échec de `read` ou de `malloc`).
- Fonctionne avec n'importe quelle valeur de `BUFFER_SIZE` (testé de 1 à la valeur par défaut, 42), définissable à la compilation.
- Fonctionne sur un fichier, l'entrée standard ou tout autre descripteur lisible.
- Partie obligatoire uniquement : un seul descripteur suivi à la fois (le bonus multi-fd n'est pas implémenté).

## Compilation & utilisation

```bash
make        # compile l'archive get_next_line.a
make clean  # supprime les fichiers objets
make fclean # supprime les objets et l'archive
make re     # recompile entièrement
```

Le Makefile compile avec `cc -Wall -Wextra -Werror`. La taille du buffer vaut 42 par défaut (`get_next_line.h`) et peut être changée à la compilation :

```bash
make
cc -Wall -Wextra -Werror main.c get_next_line.a -o gnl
./gnl

# ou avec une autre taille de buffer
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 main.c get_next_line.c get_next_line_utils.c -o gnl
```

Le `main.c` fourni ouvre `pomme.txt` et l'affiche ligne par ligne :

```c
while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
```

```
Hello, World!
This is a test file.
It contains multiple lines.qqqq...
...
End of file.
```

## Approche technique

Le cœur de la solution est une variable `static char *reste` qui persiste entre les appels et contient tout ce qui a été lu mais pas encore renvoyé. Chaque appel se déroule en trois étapes :

1. **`read_and_store`** : tant que `reste` ne contient pas de `\n` et que `read` ne renvoie pas 0 (fin de fichier), on lit `BUFFER_SIZE` octets dans un buffer temporaire, puis on les concatène à `reste` avec `ft_strjoin` (qui libère l'ancienne chaîne pour éviter les fuites).
2. **`extract_line`** : on copie dans une nouvelle chaîne tout ce qui précède le premier `\n`, `\n` compris. C'est la valeur renvoyée à l'appelant.
3. **`update_reste`** : on remplace `reste` par ce qui suit le `\n`. S'il n'y a plus rien, `reste` est libéré et remis à `NULL`.

```
read() ──► buffer[BUFFER_SIZE] ──strjoin──► reste ──► extract_line ──► ligne renvoyée
                                              │
                                              └──► update_reste ──► reste (pour l'appel suivant)
```

Choix notables :

- Le buffer de lecture est **alloué dynamiquement** (`BUFFER_SIZE + 1`), ce qui permet d'utiliser de grandes valeurs de `BUFFER_SIZE` sans risque pour la pile.
- La lecture s'arrête **dès qu'un `\n` est présent** dans `reste` : on ne lit jamais plus que nécessaire.
- Les utilitaires (`ft_strchr`, `ft_strlen`) **tolèrent un pointeur `NULL`**, ce qui simplifie le premier appel où `reste` n'existe pas encore.

## Structure du projet

```
get_next_line/
├── Makefile
├── get_next_line.h        # prototype, BUFFER_SIZE par défaut
├── get_next_line.c        # read_and_store, extract_line, update_reste, get_next_line
├── get_next_line_utils.c  # ft_strchr, ft_strjoin, ft_strlen
├── main.c                 # programme de test
└── pomme.txt              # fichier de test
```

## Ce que j'ai appris

- Utiliser une **variable statique** pour conserver un état entre plusieurs appels de fonction.
- Comprendre le fonctionnement de **`read`** et des descripteurs de fichiers, et gérer la fin de fichier et les erreurs.
- Gérer la **mémoire dynamique** au fil des appels : libérer l'ancienne chaîne à chaque concaténation, rendre à l'appelant une ligne qu'il doit lui-même libérer.
- Passer une **macro à la compilation** (`-D BUFFER_SIZE=n`) et tester les cas limites (buffer de 1 octet, ligne sans `\n` final).
