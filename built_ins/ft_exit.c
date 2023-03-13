
#include <unistd.h>
#include <stdlib.h>


typedef struct	s_com
{
    char        *cmd;       // command name
    char        **arg;      // command arguments
    struct s_com *next;     // pointer to next command in list
}               t_com;

typedef struct	s_envp
{
    char        *var;       // environment variable name
    char        *val;       // environment variable value
    struct s_envp *next;    // pointer to next variable in list
}               t_envp;




//-----------LIBFT------------//


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

int	ft_atoi(const char *str)
{
	long int	result;
	int			x;
	int			sign;
	long int	old_num;

	result = 0;
	sign = 1;
	x = 0;
	while (str[x] == 32 || (str[x] >= 9 && str[x] <= 13))
		x++;
	if (str[x] == '-')
		sign *= -1;
	if (str[x] == '+' || str[x] == '-')
		x++;
	while (str[x] >= '0' && str[x] <= '9')
	{
		old_num = result;
		result = result * 10 + str[x] - '0';
		if ((old_num < 0 && result > 0) || (old_num > 0 && result < 0))
			return ((sign == 1) * -1);
		x++;
	}
	return (result * sign);
}


//--------------LIBFT------------//

// void	free_array(char **arr)
// {
// 	int	i;

// 	i = 0;
// 	if (arr)
// 	{
// 		if (arr[i])
// 		{
// 			while (arr[i])
// 			{
// 				free(arr[i]);
// 				i++;
// 			}
// 		}
// 		free(arr);
// 	}
// }

// void	free_envp_list(t_envp *envp_list)
// {
// 	if (!envp_list)
// 		return ;
// 	while (envp_list)
// 	{
// 		if (envp_list->var)
// 			free(envp_list->var);
// 		if (envp_list->val)
// 			free(envp_list->val);
// 		envp_list = envp_list->next;
// 	}
// 	free(envp_list);
// 	envp_list = NULL;
// }

// void	free_com_list(t_com *com)
// {
// 	t_com	*tmp;

// 	if (com)
// 	{
// 		while (com)
// 		{
// 			tmp = com->next;
// 			if (com->cmd)
// 				free(com->cmd);
// 			if (com->arg)
// 				free_array(com->arg);
// 			if (com->arg)
// 				free(com->arg);
// 			com = tmp;
// 		}
// 		if (com)
// 			free(com);
// 	}
// }

//free commands list
void free_com_list(t_com *com)
{
    while (com != NULL)
    {
        t_com *next = com->next;
        free(com->cmd);
        if (com->arg != NULL)
        {
            for (int i = 0; com->arg[i] != NULL; i++)
                free(com->arg[i]);
            free(com->arg);
        }
        free(com);
        com = next;
    }
}

//free envp list
void free_envp_list(t_envp *envp_list)
{
    while (envp_list != NULL)
    {
        t_envp *next = envp_list->next;
        free(envp_list->var);
        free(envp_list->val);
        free(envp_list);
        envp_list = next;
    }
}


static void	arguments_handler(char **array, t_com *com, t_envp *envp_list)
{
	int	i;

	i = 0;
	if (array[0][i] == '+' || array[0][i] == '-')
		i++;
	while (array[0][++i])
	{
		if (array[0][i] < '0' || array[0][i] > '9')
		{
			ft_putendl_fd("Myshell: exit: numeric argument required", 2);
			free_com_list(com);
			free_envp_list(envp_list);
			exit(255);
		}
	}
	if (array[1])
		ft_putendl_fd("exit \nMyshell: exit: too many arguments", 2);
	else
	{
		i = ft_atoi(array[0]);
		ft_putendl_fd("exit", 1);
		free_com_list(com);
		free_envp_list(envp_list);
		exit(i);
	}
}

void	builtin_exit(t_com *com, t_envp *envp_list)
{
	int	i;

	i = 0;
	if (!com->arg)
	{
		ft_putendl_fd("exit", 1);
		free_com_list(com);
		free_envp_list(envp_list);
		exit(0);
	}
	else
		arguments_handler(com->arg, com, envp_list);
}


//--------TESTING-------//
#include <stdio.h>

int main(void)
{
    // Allocate memory for t_com and t_envp
    t_com *com = malloc(sizeof(t_com));
    t_envp *envp_list = malloc(sizeof(t_envp));

    // Initialize command and argument
	com->arg = malloc(1 * sizeof(char *));
    com->cmd = "exit";
    com->arg = malloc(1 * sizeof(char *));
    com->arg[0] = "1";
    com->arg[1] = NULL;

    // Initialize environment variable
    envp_list->var = "PATH";
    envp_list->val = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin";
    envp_list->next = NULL;

    // Call the exit builtin function
    builtin_exit(com, envp_list);

    // Free allocated memory
   	free(com->arg);
    free(com);
    free(envp_list);

    // Exit the program
    return 0;
}


// int main(void)
// {
//     t_com *com = malloc(sizeof(t_com));
//     t_envp *envp_list = malloc(sizeof(t_envp));
//     com->cmd = "exit";
//     com->arg = malloc(2 * sizeof(char *));
//     com->arg[0] = "1";
//     com->arg[1] = NULL;
//     envp_list->var = "PATH";
//     envp_list->val = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin";
//     envp_list->next = NULL;

//     builtin_exit(com, envp_list);

//     // free memory and exit
//     return (0);
// }
