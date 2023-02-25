#include "utils.h"

void		ft_pwd(void)
{
	char	*pwd;
	int		ret;
	int exit_status;

	ret = EXIT_SUCCESS;
	pwd = getcwd(0, 1024);
	ft_putendl_fd(pwd, 1);
	free(pwd);
	exit_status = 0;
}

int main(void)
{
    ft_pwd();
    return 0;
}