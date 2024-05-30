#include "philo.h"

static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;
	long 	time;//
	
	time = (get_time_in_ms() - philo->last_meal);
	sem_wait(philo->sem_meal);
	time_to_think = (philo->table->time_to_die - time - philo->table->time_to_eat) / 2;
	sem_post(philo->sem_meal);
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_sleep(time_to_think);
}

static void	lone_philo_routine(t_philo *philo)
{
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT, S_IRUSR | S_IWUSR, philo->table->nb_philos);
	sem_wait(philo->sem_philo_full);
	sim_start_delay(philo->table->start_time);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		exit(CHILD_EXIT_PHILO_FULL);
	}
	if (DEBUG_FORMATTING == true)
		print_philo_state(philo, "has taken a fork");
	else
		print_philo_state(philo, "has taken a fork");
	philo_sleep(philo->table->time_to_die);
	if (DEBUG_FORMATTING == true)
		print_philo_state(philo, "died");
	else
		print_philo_state(philo, "died");
	free_table(philo->table);
	exit(CHILD_EXIT_PHILO_DEAD);
}
//
void	grab_fork(t_philo *philo)
{
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	if (philo->nb_forks_held <= 0)
		write_status(philo, false, FORK_1);
	if (philo->nb_forks_held == 1)
		write_status(philo, false, FORK_2);
	philo->nb_forks_held += 1;
	sem_post(philo->sem_meal);
}

static void	eat_sleep_routine(t_philo *philo)
{
	grab_fork(philo);
	grab_fork(philo);
	write_status(philo, false, EATING);
	sem_wait(philo->sem_meal);
	philo->last_meal = get_time_in_ms();
	sem_post(philo->sem_meal);
	philo_sleep(philo->table->time_to_eat);
	write_status(philo, false, SLEEPING);
	sem_post(philo->sem_forks);
	sem_post(philo->sem_forks);
	sem_wait(philo->sem_meal);
	philo->nb_forks_held -= 2;
	philo->times_ate += 1;
	sem_post(philo->sem_meal);
	philo_sleep(philo->table->time_to_sleep);
}

static void	philosopher_routine(t_philo *philo)
{
	if (philo->id % 2)
		think_routine(philo, true);
	while (1)
	{
		eat_sleep_routine(philo);
		think_routine(philo, false);
	}
}

void	philosopher(t_table *table)
{
	t_philo	*philo;

	philo = table->this_philo;
	if (philo->table->nb_philos == 1)
		lone_philo_routine(philo);
	init_philo_ipc(table, philo);
	if (philo->table->must_eat_count == 0)
	{
		sem_post(philo->sem_philo_full);
		child_exit(table, CHILD_EXIT_PHILO_FULL);
	}
	if (philo->table->time_to_die == 0)
	{
		sem_post(philo->sem_philo_dead);
		child_exit(table, CHILD_EXIT_PHILO_DEAD);
	}
	sem_wait(philo->sem_meal);
	philo->last_meal = philo->table->start_time;
	sem_post(philo->sem_meal);
	sim_start_delay(philo->table->start_time);
	philosopher_routine(philo);
}
