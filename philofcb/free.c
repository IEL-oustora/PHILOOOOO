#include "philo.h"

void	child_exit(t_table *table, int exit_code)
{
	sem_post(table->this_philo->sem_meal);
	pthread_join(table->this_philo->personal_grim_reaper, NULL);
	sem_close(table->this_philo->sem_forks);
	sem_close(table->this_philo->sem_philo_full);
	sem_close(table->this_philo->sem_philo_dead);
	sem_close(table->this_philo->sem_write);
	sem_close(table->this_philo->sem_meal);
	free_table(table);
	exit(exit_code);
}

void *free_table(t_table *table)
{
    unsigned int i;

    if (!table)
        return (NULL);

    if (table->philos)
    {
        for (i = 0; i < table->nb_philos; i++)
        {
            if (table->philos[i])
            {
                if (table->philos[i]->sem_meal_name)
                {
                    free(table->philos[i]->sem_meal_name);
                }
                if (table->philos[i]->sem_meal != SEM_FAILED)
                {
                    sem_close(table->philos[i]->sem_meal);
                }
                free(table->philos[i]);
            }
        }
        free(table->philos);
    }
    if (table->pids)
        free(table->pids);
    free(table);
    return (NULL);
}

int	sem_error_cleanup(t_table *table)
{
	sem_close(table->sem_forks);
	sem_close(table->sem_write);
	sem_close(table->sem_philo_full);
	sem_close(table->sem_philo_dead);
	sem_close(table->sem_stop);
	unlink_global_sems();
	return(0);
}

int	table_cleanup(t_table *table, int exit_code)
{
	if (table != NULL)
	{
		pthread_join(table->famine_reaper, NULL);
		pthread_join(table->gluttony_reaper, NULL);
		sem_close(table->sem_forks);
		sem_close(table->sem_write);
		sem_close(table->sem_philo_full);
		sem_close(table->sem_philo_dead);
		sem_close(table->sem_stop);
		unlink_global_sems();
		free_table(table);
	}
	return (exit_code);
}

void	unlink_global_sems(void)
{
	sem_unlink(SEM_NAME_FORKS);
	sem_unlink(SEM_NAME_WRITE);
	sem_unlink(SEM_NAME_FULL);
	sem_unlink(SEM_NAME_DEAD);
	sem_unlink(SEM_NAME_STOP);
}
