/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melkholy <melkholy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 18:42:45 by melkholy          #+#    #+#             */
/*   Updated: 2023/02/21 22:12:32 by melkholy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	ft_quit_ignore(int sig)
{
	const char	prompt[] = "\033[35mminihell\033[36m$\033[0m ";
	char	*str;

	printf("\n");
	str = readline(prompt);
	printf("Here is %s\nSig: %d\n", str, sig);
	free(str);
}

int	main(void)
{
	const char	prompt[] = "\033[35mminihell\033[36m$\033[0m ";
	char		*str;
	// struct sigaction	sa;
	//
	// sigemptyset(&sa.sa_mask);
	// sa.sa_handler = ft_quit_ignore;
	// sigaction(SIGQUIT, &sa, NULL);
	signal(SIGQUIT, ft_quit_ignore);
	str = readline(prompt);
	printf("In Main: %s\n", str);
	free(str);
}
