#ifndef LIBRARY_H
# define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

typedef struct s_wildcard
{
	/* General wildcard variables */
	char	*str;
	char	*path;
	char	**output;
	char	**search;
	int		*ends;

	/* Wildcard match variables */

	int		start;
	int		end;
	int		search_tot;
	char	*ret;
	char	*tmp;
	char	*p_tmp;
}	t_wildcard;


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


char **expand_wildcard(char *arg);
void	check_ends(char *str, int *ends);
char **search_directory(char **direct, char **search, int *ends);
bool	is_wildcard_match(char *direct, char **search, int *ends);
int	get_search_tot(char **search);
char **read_directory(char *path);
char **add_str_array(char **array, char *str);
void	split_path_wildcard(t_wildcard *wc, char *arg);
void	add_path_wildcard(char **results, char *path);
char	*ft_strjoin_free_rev(char *s1, char *s2);
bool free_return_bool(char *str, bool ret);

#endif