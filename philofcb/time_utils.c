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

void itoa(int value, char *str, int base) 
{
    char *rc;
    char *ptr;
    char *low;
    
    // Set '-' for negative decimals.
    if (base == 10 && value < 0) 
	{
        *str++ = '-';
        value = -value;
    }
    // Remember where the numbers start.
    rc = ptr = str;
	// The actual conversion.
    do
	{
        *ptr++ = "0123456789abcdef"[value % base];
        value /= base;
    } 
	while (value);
    // Terminate the string.
    *ptr-- = '\0';

    // Reverse the string.
    for (low = rc; low < ptr; ++low, --ptr)
	{
        char tmp = *low;
        *low = *ptr;
        *ptr = tmp;
    }
}

char *ft_strcat(char *dst, const char *src)
{
    size_t i;
    size_t j;

    i = 0;
    while (dst[i] != '\0')
        i++;
    j = 0;
    while (src[j] != '\0')
    {
        dst[i + j] = src[j];
        j++;
    }
    dst[i + j] = '\0';
    return dst;
}
char *ft_strcpy(char *dest, const char *src)
{
    char *temp = dest;
    while ((*dest++ = *src++) != '\0');
    return (temp);
}