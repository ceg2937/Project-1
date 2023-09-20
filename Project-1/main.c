/*
Name: carlos guardado
Modificaion:
1.swe add the reak statement in switch statment after the execution of case 'h'
2.We Modifiy the rin_result function so the function print_result() to print results correctly and in the same order
as the words were specified on the command line.
3.we add some other functionality as mentioned in the file name C-Enhancemens.txt.

About this program:
- This program counts words.
- The specific words that will be counted are passed in as command-line
  .
- The proargumentsgram reads words (one word per line) from standard input until EOF or
  an input line starting with a dot '.'
- The program prints out a summary of the number of times each word has
  appeared.
- Various command-line options alter the behavior of the program.

E.g., count the number of times 'cat', 'nap' or 'dog' appears.
> ./main cat nap dog
Given input:
 cat
 .
Expected output:
 Looking for 3 words
 Result:
 cat:1
 nap:0
 dog:0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smp0_tests.h"

/* B2 */
#define LENGTH(s) (sizeof(s) / sizeof(*s))

/* Structures */
typedef struct {
  char *word;
  int counter;
} WordCountEntry;

/* Complete C4 and in this function
      strtok() can be used to split a line into individual tokens.
      For the separator characters we use whitespace (space and
      tab), as well as the newline character '\n'.  We could also
      trim the buffer to get rid of the newline, instead. 
      strtok returns NULL when no more tokens are available. 
      Google strtok line to learn more about how to use it */
int process_stream(WordCountEntry entries[], int entry_count)
{
  short line_count = 0;
  char buffer[30];

  while (fgets(buffer, sizeof(buffer), stdin)) {
    if (*buffer == '.')
      break;

    char *token = strtok(buffer, " \t\n");
    while (token) {
      int i = 0;
      while (i < entry_count) {
        if (!strcmp(entries[i].word, token))
          entries[i].counter++;
        i++;
      }
      token = strtok(NULL, " \t\n");
    }
    line_count++;
  }
  // Returns the number of lines processed.
  return line_count;
}

/* Complete B5: introduce a temporary variable i and use it to access every index in entries */
void print_result(WordCountEntry entries[], int entry_count)
{
  printf("Result:\n");

  for (int i = 0; i < entry_count; i++) {
    printf("%s:%d\n", entries[i].word, entries[i].counter);
  }
}

void printHelp(const char *name)
{
  fprintf(stderr, "usage: %s [-h] <word1> ... <wordN>\n", name);
  fprintf(stderr, "-fFILENAME: Redirect output to FILENAME\n");
}

int main(int argc, char **argv)
{
  const char *prog_name = *argv;

  WordCountEntry entries[argc - 1];
  int entryCount = 0;

  FILE *output = stdout; // Default to stdout

  while (*argv != NULL) {
    if (**argv == '-') {
      switch ((*argv)[1]) {
        case 'h':
          printHelp(prog_name);
          return EXIT_SUCCESS;
        case 'f':
          if (*(argv + 1) != NULL) {
            output = fopen(*(argv + 1), "w");
            if (output == NULL) {
              fprintf(stderr, "Error opening file for writing: %s\n", *(argv + 1));
              return EXIT_FAILURE;
            }
            argv++; // Move to the next argument after '-fFILENAME'
          } else {
            fprintf(stderr, "Missing filename after -f option\n");
            return EXIT_FAILURE;
          }
          break;
        default:
          fprintf(stderr, "%s: Invalid option %s. Use -h for help.\n", prog_name, *argv);
      }
    } else {
      if (entryCount < argc - 1) {
        entries[entryCount].word = *argv;
        entries[entryCount++].counter = 0;
      }
    }
    argv++;
  }

  if (entryCount == 0) {
    fprintf(stderr, "%s: Please supply at least one word. Use -h for help.\n", prog_name);
    return EXIT_FAILURE;
  }

  fprintf(output, "Looking for %d words\n", entryCount);

  process_stream(entries, entryCount);
  print_result(entries, entryCount);

  if (output != stdout) {
    fclose(output);
  }

  return EXIT_SUCCESS;
}
