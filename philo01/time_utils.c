#include "philo.h"



// long	get_current_time_in_ms(void)
// {
// 	struct timeval	tv;

// 	gettimeofday(&tv, NULL);
// 	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
// }

// void	usleep_ms(long time)
// {
// 	long t;

// 	t = get_current_time_in_ms();
// 	while ((get_current_time_in_ms() - t) < time)
// 		usleep(1000);
// }

//
time_t	get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	philo_sleep(time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		usleep(100);
	}
}

void	sim_start_delay(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		continue ;
}
//

bool	start_grim_reaper_threads(t_table *table)
{
	if (pthread_create(&table->gluttony_reaper, NULL,
			&global_gluttony_reaper, table) != 0)
		return(NULL);
	if (pthread_create(&table->famine_reaper, NULL,
			&global_famine_reaper, table) != 0)
		return(NULL);
	return (true);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_utoa(unsigned int nb, size_t len)
{
	char	*ret;

	ret = malloc(sizeof * ret * (len + 1));
	if (!ret)
		return (NULL);
	ret[len] = '\0';
	len--;
	while (nb % 10)
	{
		ret[len--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (ret);
}

char	*ft_strcat(char	*dst, const char *src)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dst[i])
		i++;
	j = 0;
	while (src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst);
}