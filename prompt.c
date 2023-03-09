/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/03/09 02:47:44 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "./Libft/libft.h"
#define PROMPT "\033[35mminihell\033[36m$\033[0m "
#define INPUT 1
#define HEREDOC 2
#define OUTPUT 4
#define APPEND 8

/* Node to store the commands in a linked list */
typedef struct s_cmds
{
	char			*cmd;
	char			**args;
	char			*from_file;
	char			*hdocs_end;
	char			*to_file;
	int				redirect;
	struct s_cmds	*next;
}				t_cmds;

/* A global variable to store the term attributes and exit status */
typedef struct s_term
{
	struct termios	save_attr;
}				t_term;

t_term	g_term_attr;

/* Used to ignore the Control + C signal
 and istead just displayed a new prompt on a new line */
void	ft_quit_ignore(int sig)
{
	write (1, "\n", 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	return ((void)sig);
}

/* Used to save the original terminal attributes, 
 so I can modify them to disable ^C from appreaing on the screen */
int	ft_set_terminal(void)
{
	struct termios	attr_new;
	int				error;

	error = 0;
	error = tcgetattr(STDIN_FILENO, &g_term_attr.save_attr);
	attr_new = g_term_attr.save_attr;
	attr_new.c_lflag &= ~ECHOCTL;
	error = tcsetattr(STDIN_FILENO, TCSANOW, &attr_new);
	return (error);
}

/* Used to ignore all white spaces and return the first idnex after them */
int	ft_isnspace_indx(char *in_put)
{
	int	count;

	count = 0;
	while (in_put[count] && ((in_put[count] >= 9 && in_put[count] <= 13)
			|| in_put[count] == 32))
		count ++;
	return (count);
}

/* Used to find and cut the input into words sperated by spaces or comas */
char	*ft_find_word(char *in_put)
{
	char	*word;
	int		count;
	char	divid;

	count = 0;
	divid = ' ';
	if (!in_put[count])
		return (NULL);
	if (in_put[count] == '"')
	{
		count ++;
		divid = '"';
	}
	else if (in_put[count] == '\'')
	{
		count ++;
		divid = '\'';
	}
	while (in_put[count] && in_put[count] != divid)
		count ++;
	if (divid == '\'' || divid == '"')
		word = ft_substr(in_put, 0, count + 1);
	else
		word = ft_substr(in_put, 0, count);
	return (word);
}

/* Used to reallocate memory for the double pointer string */
char	**ft_double_realloc(char **str, int old_size, int new_size)
{
	char	**tmp;
	int		count;

	tmp = (char **)ft_calloc(new_size, sizeof(char *));
	count = 0;
	while (count < old_size - 1)
	{
		tmp[count] = ft_strdup(str[count]);
		free(str[count]);
		count ++;
	}
	free(str);
	return (tmp);
}

/* Used to parse and create the command with its arguments in a node */
t_cmds	*ft_lexer(char *in_put)
{
	t_cmds	*cmd;
	int		count;
	int		len;

	cmd = (t_cmds *)ft_calloc(1, sizeof(t_cmds));
	cmd->args = (char **)ft_calloc(2, sizeof(char *));
	len = 0;
	len = ft_isnspace_indx(in_put);
	cmd->cmd = ft_find_word(in_put + len);
	len += ft_strlen(cmd->cmd);
	len += ft_isnspace_indx(in_put + len);
	if (!*(in_put + len))
		return (cmd);
	count = 0;
	while (*(in_put + len))
	{
		cmd->args[count] = ft_find_word(in_put + len);
		len += ft_strlen(cmd->args[count]);
		count ++;
		len += ft_isnspace_indx(in_put + len);
		cmd->args = ft_double_realloc(cmd->args, count + 1, count + 2);
	}
	cmd->args[count] = NULL;
	cmd->next = NULL;
	return (cmd);
}

/* Used to free any double string */
void	ft_free_dstr(char **str)
{
	int	count;

	count = 0;
	while (str[count])
	{
		free(str[count]);
		count ++;
	}
	free(str);
}

int	ft_in_redirection(char *in_put)
{
	int	count;
	int	result;

	count = 0;
	result = 0;
	if (!in_put[count])
		return (0);
	if (in_put[count] == '<')
		result = INPUT;
	if (in_put[count + 1] && in_put[count + 1] == '<')
		result = HEREDOC;
	if (!in_put[count + 1] || !in_put[count + 2])
		printf("\033]35mminihell\033]0m :\
				syntax error near unexpected token `newline'");
	return (result);
}

int	ft_get_args(t_cmds *cmd, char *in_put)
{
	int	count;
	int	len;

	len = 0;
	count = 0;
	if (cmd->cmd)
		len = ft_strlen(cmd->cmd);
	len += ft_isnspace_indx(&in_put[len]);
	if (!in_put[len])
		return (0);
	cmd->args = (char **)ft_calloc(2, sizeof(char *));
	while (*(in_put + len))
	{
		cmd->args[count] = ft_find_word(in_put + len);
		len += ft_strlen(cmd->args[count]);
		count ++;
		len += ft_isnspace_indx(in_put + len);
		cmd->args = ft_double_realloc(cmd->args, count + 1, count + 2);
	}
	return (0);
}

t_cmds	*ft_redirect_cmd(char *in_put)
{
	t_cmds	*cmd;
	int		len;
	int		redirect;

	len = 0;
	redirect = ft_in_redirection(in_put);
	len += redirect;
	if (!in_put[len])
		return (NULL);
	cmd = (t_cmds *)ft_calloc(1, sizeof(t_cmds));
	len += ft_isnspace_indx(&in_put[len]);
	if (redirect == INPUT)
	{
		cmd->to_file = ft_find_word(&in_put[len]);
		len += ft_strlen(cmd->to_file);
	}
	else if (redirect == HEREDOC)
	{
		cmd->hdocs_end = ft_find_word(&in_put[len]);
		len += ft_strlen(cmd->hdocs_end);
	}
	len += ft_isnspace_indx(&in_put[len]);
	cmd->cmd = ft_find_word(&in_put[len]);
	ft_get_args(cmd, &in_put[len]);
	cmd->next = NULL;
	cmd->redirect |= redirect;
	return (cmd);
}
/* Used to split the mutiple commands and create 
 a linked list for them and their arguments */
t_cmds	*ft_many_cmd(char *in_put)
{
	char	**many_cmd;
	t_cmds	*cmds;
	t_cmds	*tmp;
	int		count;

	many_cmd = ft_split(in_put, '|');
	count = 0;
	if (many_cmd[count][ft_isnspace_indx(many_cmd[count])] == '<')
		cmds = ft_redirect_cmd(&many_cmd[count][ft_isnspace_indx(many_cmd[count])]);
	else
		cmds = ft_lexer(many_cmd[count]);
	tmp = cmds;
	count ++;
	while (many_cmd[count])
	{
		if (many_cmd[count][ft_isnspace_indx(many_cmd[count])] == '<')
			tmp->next = ft_redirect_cmd(&many_cmd[count][ft_isnspace_indx(many_cmd[count])]);
		else
			tmp->next = ft_lexer(many_cmd[count]);
		tmp = tmp->next;
		count ++;
	}
	ft_free_dstr(many_cmd);
	return (cmds);
}

/* Used to check the input and pass it to the parsing and cutting 
 functions to get back either a linked list with all the command original
 just one command in a node */
void	ft_parse_input(char *in_put)
{
	t_cmds	*cmd;
	t_cmds	*tmp;
	int		count;

	count = 0;
	count += ft_isnspace_indx(in_put);
	if (!in_put[count])
		return ;
	if (ft_strchr(&in_put[count], '|'))
		cmd = ft_many_cmd(&in_put[count]);
	else if (in_put[count] == '<')
		cmd = ft_redirect_cmd(&in_put[count]);
	else
		cmd = ft_lexer(&in_put[count]);
	/* The rest of the function is for demonstration purposes 
	  to make sure the lexer is working well*/
	tmp = cmd;
	while (tmp)
	{
		count = 0;
		printf("Command: %s\n", tmp->cmd);
		while (tmp->args && tmp->args[count])
		{
			printf("Arg %d: %s\n", count, tmp->args[count]);
			count ++;
		}
		tmp = tmp->next;
	}
}

/* Used to display the prompt and read the input from the user */
int	ft_read_prompt(void)
{
	char	*str;

	while (true)
	{
		str = readline(PROMPT);
		if (!str || !ft_strncmp(str, "exit", 4))
		{
			tcsetattr(STDIN_FILENO, TCSANOW, &g_term_attr.save_attr);
			if (!str)
			{
				rl_on_new_line();
				rl_redisplay();
			}
			write(1, "exit\n", 5);
			exit(0);
		}
		add_history(str);
		ft_parse_input(str);
		free(str);
	}
}

int	main(void)
{
	signal(SIGINT, ft_quit_ignore);
	signal(SIGQUIT, SIG_IGN);
	if (ft_set_terminal())
		exit(1);
	ft_read_prompt();
}
