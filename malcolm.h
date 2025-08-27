#ifndef MALCOLM_H
#define MALCOLM_H

#include "stdio.h"

// checkInput.c
int check_mac(char *str);
int check_ip(char *str);
int check_args(char **argv);

// utils.c
size_t  ft_strlen(const char *s);
int ft_isalnum(int c);


#endif // !MALCOLM_H
