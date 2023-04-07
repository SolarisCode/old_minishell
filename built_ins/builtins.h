/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: busmanov <busmanov@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 18:43:24 by busmanov          #+#    #+#             */
/*   Updated: 2023/04/07 18:43:25 by busmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef BUILTINS_H
# define BUILTINS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>


//#define PATH_MAX 1000

//int 	g_status;




char	*ft_strdup(const char *s);
int		ft_strncmp(const char *st1, const char *st2, size_t x);
size_t	ft_strlcat(char *to, const char *from, size_t tosize);
int		ft_strncmp(const char *st1, const char *st2, size_t x);
char	*ft_substr(char const *s, unsigned int start, size_t len);
//char	*ft_strjoin(char const *s1, char const *s2);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
int		ft_strequ(const char *s1, const char *s2);
int 	ft_strlen(char *string);
void	*ft_realloc(void *ptr, size_t size);
void	*ft_memcpy(void *to, const void *from, size_t n);


//utils3

int		check_existence(char **env, char *str);
int		check_name(char *str);
void	print_name(char *str);
void	print_value(char *str);

//utils3


// env_utils2.c

// char** init_envp(int* env_size);
// int find_env_var(char** envp, int env_count, char* str);
// char** double_envp_size(char** envp, int* env_size);
// int add_env_var(char** envp, int* env_count, int* env_size, char* str);
// int replace_env_var(char** envp, int index, char* str);
// int ft_putenv(char *str);

// env_utils2.c

#endif