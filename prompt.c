/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/02/25 19:09:15 by melkholy         ###   ########.fr       */
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

/* Node to store the commands in a linked list */
typedef struct	s_cmds
{
	char	*cmd;
	char	*opt;
	char	*args;
	int		to_pipe;
	int		to_file;
}				t_cmds;

/* A global variable to store the term attributes and exit status */
typedef struct	s_term
{
	struct termios	save_attr;
}				t_term;

t_term	term_attr;

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
int	ft_set_terminal()
{
	struct termios	attr_new;
	int	error;

	error = 0;
	error = tcgetattr(STDIN_FILENO, &term_attr.save_attr);
	attr_new = term_attr.save_attr;
	attr_new.c_lflag &= ~ECHOCTL;
	error = tcsetattr(STDIN_FILENO, TCSANOW, &attr_new);
	return (error);
}

void	ft_many_cmd(char *in_put)
{
	char	**cmds;

	cmds = ft_split(in_put, '|');
}

void	ft_parse_input(char *in_put)
{
	int	count;

	count = 0;
	while ((in_put[count] >= 9 && in_put[count] <= 13) || in_put[count] == 32)
		count ++;
	if (ft_strchr(&in_put[count], '|'))
		ft_many_cmd(&in_put[count]);
}

/* Used to display the prompt and read the input from the user */
int	ft_read_prompt()
{
	char	*str;

	while (true)
	{
		str = readline(PROMPT);
		if (!str || !ft_strncmp(str, "exit", 4))
		{
			tcsetattr(STDIN_FILENO, TCSANOW, &term_attr.save_attr);
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
