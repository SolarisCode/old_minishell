
int	ft_atoi(const char *str)
{
	long int	result;
	int			x;
	int			sign;

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
		result = result * 10 + str[x] - '0';
		x++;
	}
	return (result * sign);
}

int	ft_isdigit(int n)
{
	if (n >= '0' && n <= '9')
	{
		return (1);
	}
	return (0);
}

static int is_numeric(char *param)
{
	int counter;

	counter = -1;
	while (param[++counter])
	{
		if (!ft_isdigit((param[counter]) && param[counter] != '+' && param[counter] != '-'))
			return (0);
	}
	return (1);
}

//result = return
int ft_exit(char **exec)
{
	int result;

	result = 0;
	if (exec[1] && exec[2])
	{
		error("exit", -4, 1);
		return (1);
	}
	if (exec[1] && is_numeric(exec[1]))
	{
		result = ft_atoi(exec[1]);
		if (result < 0 || result > 255)
			result = 128;
	}
	error("exit", 1, result);
	return (0);
}
