#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// cat [OPTION] [FILE]...

// -b                                            нумерует только непустые строки
// (GNU: --number-nonblank)

// -e предполагает и -v                          также отображает символы конца строки как $
// (GNU only: -E то же самое, но без применения -v)

// -n                                            нумерует все выходные строки
// (GNU: --number)

// -s                                            сжимает несколько смежных пустых строк
// (GNU: --squeeze-blank)

// -t предполагает и -v                          также отображает табы как ^I
// (GNU: -T то же самое, но без применения -v)

void my_cat(int argc, char *argv[]);

void cat_file(int count_file,  char name_file[200][200], \
               int number_nonblank, int number_all, int squeeze_blank, int tab_print, int end_str);

int main(int argc, char *argv[]) {
    s21_cat(argc, argv);
    return 0;
}

void my_cat(int argc, char *argv[]) {
    int number_nonblank = 0;
    int end_str = 0;
    int number_all = 0;
    int squeeze_blank = 0;
    int tab_print = 0;
    char name_file[200][200] = {0};
    int count_file = 0;

    int norm = 1;
    for (int i = 1; (i < argc) && (norm == 1); i++) {
        if (!strcmp(argv[i], "-b") || !strcmp(argv[i], "--number-nonblank")) {
            number_nonblank = 1;
        } else if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "-E")) {
            end_str = 1;
        } else if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--number")) {
            number_all = 1;
        } else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--squeeze-blank")) {
            squeeze_blank = 1;
        } else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "-T")) {
            tab_print = 1;
        } else {
            if (argv[i][0] == '-') {
                fprintf(stderr, "\tdon`t know this flag %s", argv[i]);
                norm = 0;
            } else {
                strcpy(name_file[count_file], argv[i]);
                count_file += 1;
            }
        }
    }

    cat_file(count_file,  name_file, number_nonblank, number_all, \
              squeeze_blank, tab_print, end_str);
}

void cat_file(int count_file,  char name_file[200][200], int number_nonblank, \
               int number_all, int squeeze_blank, int tab_print, int end_str) {
    int num = 1;
    for (int i = 0; i < count_file; i++) {
        FILE *pfile;
        pfile = fopen(name_file[i] , "r");
        if (pfile == NULL) {
            fprintf(stderr, "can not open file\n");
        } else {
            int c;
            int last = '\n';
            int flg_s = 0;
            while ((c = fgetc(pfile)) != EOF) {
                if (c == '\n') {
                    flg_s++;  // считаем количество новых строк подряд
                } else {
                    flg_s = 0;  // если это не новая строка - флаг убираем
                }
                if ((number_nonblank == 1 && last == '\n' && c != '\n') ||
                    (number_nonblank != 1 && number_all == 1 && last == '\n')) {
                    printf("%6d\t", num);
                    num++;
                }
                if (end_str == 1 && c == '\n') {
                        if (squeeze_blank != 1 || flg_s <= 2) {
                        printf("$");
                    }
                }

                if (squeeze_blank != 1 || flg_s <= 2) {
                    if (tab_print == 1 && c == '\t') {
                        printf("^I");
                    } else {
                        putchar(c);
                    }
                }

                last = c;
            }
        }
    }
    if (count_file <= 0) {
        fprintf(stderr, "\nno file");
    }
}
