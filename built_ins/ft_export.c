/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: busmanov <busmanov@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 10:55:02 by busmanov          #+#    #+#             */
/*   Updated: 2023/03/13 11:50:34 by busmanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>


typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*next;
}	t_envp;

typedef struct s_command
{
	char				*name;
	char				**arg;
	int					delim;
	char				*file;
	struct s_command	*next;
}	t_com;


//---------------LIBFT-------//

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int		x;

	x = 0;
	if (!s || fd < 0)
		return ;
	while (s[x] != '\0')
	{
		ft_putchar_fd(s[x], fd);
		x++;
	}
}

void	ft_putendl_fd(char *s, int fd)
{
	if (!s || fd < 0)
		return ;
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}

int	ft_strlen(const char *string)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (string [i])
	{
		count++;
		i++;
	}
	return (count);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
		i++;
	if (start > i)
		len = 0;
	if (len > i - start)
		len = i - start;
	substr = (char *)malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (substr);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}


char	*ft_strdup(const char *s)
{
	char	*new;
	size_t	x;

	x = 0;
	new = (char *)malloc(sizeof(*new) * (ft_strlen(s) + 1));
	if (!new)
		return (NULL);
	while (s[x])
	{
		new[x] = s[x];
		x++;
	}
	new[x] = '\0';
	return (new);
}

char	*ft_strchr(const char *s, int c)
{
	while ((*s != '\0') && (*s != (char) c))
		s++;
	if (*s == (char) c)
		return ((char *)s);
	return (NULL);
}

int	ft_isalpha(int x)
{
	if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
	{
		return (1);
	}
	return (0);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		x;
	int		y;

	x = 0;
	y = 0;
	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	while (s1[x] != '\0')
	{
		str[x] = s1[x];
		x++;
	}
	while (s2[y] != '\0')
	{
		str[y + x] = s2[y];
		y++;
	}
	str[x + y] = '\0';
	return (str);
}

int ft_strcmp(char *s1, char *s2)
{
	int x = 0;

	if (s1[x] != '\0' && s2[x] != '\0' && s1[x] == s2[x])
		x++;
	return (s1[x] - s2[x]);
}

//------------------LIBFT end---------------------//


static void	print_export_error(char *str)
{
	if (str)
	{
		ft_putstr_fd("Myshell 🐚: export: `", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
	}
}

int	print_env_declare(t_envp *envp_list)
{
    while (envp_list)
    {
        ft_putstr_fd("declare -x ", 1);
        ft_putstr_fd(envp_list->key, 1);
        if (envp_list->value)
        {
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(envp_list->value, 1);
            ft_putstr_fd("\"", 1);
        }
        ft_putstr_fd("\n", 1);
        envp_list = envp_list->next;
    }
    return (0);
}



static char	**array_from_string(char *str, char **array)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str);
	while (str[i] != '=' && str[i] != '+')
		i++;
	if (str[i] == '+' && str[i + 1] != '=')
	{
		print_export_error(str);
		return (NULL);
	}
	array[0] = ft_substr(str, 0, i);
	if (str[i] == '+')
	{
		array[1] = ft_strdup("+=");
		i++;
	}
	else if (str[i] == '=')
		array[1] = ft_strdup("=");
	array[2] = ft_substr(str, i + 1, len - i - 1);
	array[3] = NULL;
	return (array);
}

static char	**split_for_export(char *str)
{
	char	**var;

	var = malloc(sizeof(char *) * 4);
	if (!var)
		return (NULL);
	if (ft_strchr(str, '='))
		var = array_from_string(str, var);
	else
	{
		var[0] = ft_strdup(str);
		var[1] = NULL;
		var[2] = NULL;
		var[3] = NULL;
	}
	return (var);
}

static int	check_arg_of_export(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '_')
		return (1);
	if (ft_isalpha(str[0]))
		return (1);
	return (0);
}

void	envp_add_front(t_envp **envp_list, char *key, char *value)
{
    t_envp  *new_node;
    
    new_node = (t_envp *)malloc(sizeof(t_envp));
    if (!new_node)
        exit(1); //handle errors
    new_node->key = key;
    new_node->value = value;
    new_node->next = *envp_list;
    *envp_list = new_node;
}


void	add_var_to_envp_list(t_envp **envp_list, char **array, char *str)
{
	t_envp	*tmp;

	tmp = *envp_list;
	if (!array[1])
		return ;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, array[0]))
		{
			if (ft_strchr(array[1], '+'))
			{
				free(tmp->value);
				tmp->value = ft_strjoin(tmp->value, array[2]);
			}
			else
			{
				free(tmp->value);
				tmp->value = ft_strdup(array[2]);
			}
			return ;
		}
		tmp = tmp->next;
	}
	envp_add_front(envp_list, array[0], array[2]);
}


//free array
void	free_array(char **array)
{
    int i;
    
    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}


int	builtin_export(t_com *com, t_envp **envp_list)
{
	int		i;
	char	**array;

	i = -1;
	if (!envp_list || !com)
		return (1);
	if (!com->arg)
		return (print_env_declare(*envp_list));
	while (com->arg[++i])
	{
		if (check_arg_of_export(com->arg[i]))
		{
			array = split_for_export(com->arg[i]);
			if (!array)
				return (1);
			add_var_to_envp_list(envp_list, array, com->arg[i]);
			free_array(array);
		}
		else
		{
			print_export_error(com->arg[i]);
			return (1);
		}
	}
	return (0);
}

//-----------testing-------------//

int main(void)
{
    t_com   com;
    t_envp  *envp_list;

    // initialize com and envp_list

    char *args[] = {"VAR1=value1", "VAR2=value2", "VAR3+=value3", "invalid_var", NULL};
    com.arg = args;

    int ret = builtin_export(&com, &envp_list);

    // check the return value of builtin_export and the content of envp_list

    return 0;
}
