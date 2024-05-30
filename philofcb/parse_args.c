#include "philo.h"

int	ft_atoi(const char *str)
{
	int				i;
	int				s;
	unsigned int	result;

	i = 0;
	s = 1;
	result = 0;
	if (str == NULL)
		return (-1);
	if (i < 0)
		return (-1);
	while (str[i] == 32 || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			s = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
		if (result > 2147483647 && s == 1)
			return (-1);
		else if (result > 2147483647 && s == -1)
			return (0);
	}
	return ((result * s));
}

int	check_number_arg(int len)
{
	if (len < 5)
	{
		write(2, "./philo nb_philo ttd tte tts\n", 30);
		return (1);
	}
	else if (len > 6)
	{
		write(2, "./philo nb_philo ttd tte tts\n", 30);
		return (1);
	}
	return (0);
}

int	check_str(char *str)
{
	int	i;

	i = -1;
	if (str[0] == 0)
		return (1);
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[++i])
	{
		if (str[i] > '9' || str[i] < '0')
			return (1);
	}
	return (0);
}

int	check_arg_no_num(char **argv)
{
	int	i;

	i = 0;
	while (argv[++i])
	{
		if (check_str(argv[i]) == 1)
		{
			write(2, "Argument Numeric Required\n", 27);
			return (1);
		}
	}
	return (0);
}

int	check_args(int len, char **argv)
{
	if (check_number_arg(len) == 1)
		return (1);
	else if (check_arg_no_num(argv) == 1)
		return (1);
	return (0);
}