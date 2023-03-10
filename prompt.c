/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/03/10 23:54:33 by melkholy         ###   ########.fr       */
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
	char			*tmp_cmd;
	char			*from_file;
	char			*hdocs_end;
	char			*to_file;
	int				redirect;
	int				skip_char;
	struct s_cmds	*next;
}				t_cmds;

/* A global variable to store the term attributes and exit status */
typedef struct s_term
{
	struct termios	save_attr;
	int				status;
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

void	ft_find_cmd(t_cmds *cmd, char *in_put);

void	ft_invalid_input(t_cmds *cmd, char *in_put, int index, char divid)
{
	char	*str;
	int		valid;
	int		count;

	valid = 0;
	count = 0;
	// modify this function to handle non starting qouts
	// if (in_put[0] != divid)
	// {
	// 	while (in_put[index + 1] && in_put[index + 1] != divid)
	// 		index ++;
	// 	index ++;
	// 	count = -1;
	// }
	cmd->skip_char += index + 1;
	if (!cmd->cmd)
	{
		while (++count < index)
			if (in_put[count] != divid)
				valid ++;
		if (valid && cmd->tmp_cmd)
		{
			str = ft_substr(in_put, 1, index - 1);
			cmd->tmp_cmd = ft_strjoin_free(cmd->tmp_cmd, str);
			free(str);
		}
		else if (valid)
			cmd->tmp_cmd = ft_substr(in_put, 1, index - 1);
	}
}

void	ft_add_cmd(t_cmds *cmd, char *in_put, int index)
{
	char	*str;
	int		valid;
	int		count;

	valid = 0;
	count = -1;
	cmd->skip_char += index;
	if (!cmd->cmd)
	{
		while (++count < index)
		{
			if (in_put[count] != ' ')
				valid ++;
		}
		if (valid)
		{
			str = ft_substr(in_put, 0, index);
			cmd->cmd = ft_strjoin_free(cmd->tmp_cmd, str);
			free(str);
		}
		else
			cmd->cmd = cmd->tmp_cmd;
	}
}

void	ft_find_cmd(t_cmds *cmd, char *in_put)
{
	int		count;
	char	divid;

	// count = 0;
	count = -1;
	// divid = ' ';
	// if (!in_put[count])
	// 	return ;
	while (in_put[++count])
		if (in_put[count] == ' ' || in_put[count] == '"' || in_put[count] == '\'')
		{
			divid = in_put[count];
			break ;
		}
	// if (in_put[count] == '"')
	// {
	// 	count ++;
	// 	divid = '"';
	// }
	// else if (in_put[count] == '\'')
	// {
	// 	count ++;
	// 	divid = '\'';
	// }
	if (count > 1)
		count = 0;
	while (in_put[count] && in_put[count] != divid)
		count ++;
	if (divid == '\'' || divid == '"')
		ft_invalid_input(cmd, in_put, count, divid);
	else if (divid == ' ' || !in_put)
		ft_add_cmd(cmd, in_put, count);
	if (!cmd->cmd)
		ft_find_cmd(cmd, &in_put[count + 1]);
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
	ft_find_cmd(cmd, in_put);
	len += cmd->skip_char;
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
	if (in_put[count] == '>')
		result = OUTPUT;
	if (in_put[count + 1] && in_put[count + 1] == '>')
		result = APPEND;
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
	len += cmd->skip_char;
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

int	ft_add_inredirect(char *in_put, t_cmds *cmd, int redirect)
{
	int	len;

	len = 0;
	if (redirect == INPUT)
	{
		cmd->from_file = ft_find_word(in_put);
		len += ft_strlen(cmd->from_file);
	}
	else if (redirect == HEREDOC)
	{
		cmd->hdocs_end = ft_find_word(in_put);
		len += ft_strlen(cmd->hdocs_end);
	}
	else if (redirect == OUTPUT || redirect == APPEND)
	{
		cmd->to_file = ft_find_word(in_put);
		len += ft_strlen(cmd->to_file);
	}
	cmd->redirect |= redirect;
	return (len);
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
	len += ft_add_inredirect(&in_put[len], cmd, redirect);
	len += ft_isnspace_indx(&in_put[len]);
	ft_find_cmd(cmd, &in_put[len]);
	ft_get_args(cmd, &in_put[len]);
	cmd->next = NULL;
	return (cmd);
}

t_cmds	*ft_many_redirection(char *one_cmd)
{
	int	spaces;

	spaces = ft_isnspace_indx(one_cmd);
	if (one_cmd[spaces] == '<' || one_cmd[spaces] == '>')
		return (ft_redirect_cmd(&one_cmd[spaces]));
	return (NULL);
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
	cmds = ft_many_redirection(many_cmd[count]);
	if (!cmds)
		cmds = ft_lexer(many_cmd[count]);
	tmp = cmds;
	count ++;
	while (many_cmd[count])
	{
		tmp->next = ft_many_redirection(many_cmd[count]);
		if (!tmp->next)
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
	else if (in_put[count] == '<' || in_put[count] == '>')
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
