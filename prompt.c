/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/02/24 16:58:22 by melkholy         ###   ########.fr       */
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
#define PROMPT "\033[35mminihell\033[36m$\033[0m "

typedef struct	s_term
{
	struct termios	save_attr;
}				t_term;

t_term	term_attr;

void	ft_quit_ignore(int sig)
{
	write (1, "\n", 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
	return ((void)sig);
}

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

int	ft_read_prompt()
{
	char	*str;

	while (true)
	{
		str = readline(PROMPT);
		if (!str || !strcmp(str, "exit"))
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
