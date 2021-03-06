/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   library.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iyahoui- <iyahoui-@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 19:27:08 by iyahoui-          #+#    #+#             */
/*   Updated: 2022/06/17 18:21:51 by iyahoui-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBRARY_H
# define LIBRARY_H

# include "minishell.h"

/* String utilities */
char		*ft_strdup(const char *s1);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlen(const char *s);
char		*str_to_lower(char *str);;
char		**ft_split(char const *s, char c);
long long	ft_atoll(const char *str, bool *is_valid);
int			is_set(char const c, char const *set);
int			is_set_ret(char const c, char const *set);
char		*ft_strncpy(char *dest, char *src, size_t n);
char		*get_first_token(char *string, char delimiter);
char		*get_last_token(char *string, char delimiter);
char		*ft_strjoin_free(char *s1, char const *s2);
int			ft_isalnum(int c);
int			ft_isspace(char c);
char		*ft_itoa(int n);
int			ft_atoi(const char *str);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);

/* Writing utilities */
int			putchar_fd(char c, int fd);
int			putstr_fd(char *str, int fd);
int			putendl_fd(char *str, int fd);

/* Memory utilities */
void		*ft_memset(void *b, int c, size_t len);
void		ft_bzero(void *s, size_t n);
void		*ft_xalloc(size_t size);
void		free_split(char **split);

/* Array utilities */
int			count_array(void **array);
void		free_array(void **array);

/* Math utilities */
int			abs_val(int n);

#endif