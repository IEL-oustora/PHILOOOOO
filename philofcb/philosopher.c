#include "philo.h"

static t_philo **init_philosophers(t_table *table)
{
    t_philo **philos;
    unsigned int i;
	char id_str[10];

    philos = malloc(sizeof(t_philo *) * (table->nb_philos + 1));
    if (!philos)
        return (NULL);
    i = 0;
    while (i < table->nb_philos)
    {
        philos[i] = malloc(sizeof(t_philo));
        if (!philos[i])
            return (NULL);
        philos[i]->table = table;
        philos[i]->id = i;
        philos[i]->sem_meal_name = malloc(20);
        if (!philos[i]->sem_meal_name)
            return (NULL);
        ft_strcpy(philos[i]->sem_meal_name, "/sem_meal_");//
        itoa(i + 1, id_str, 10);//
        ft_strcat(philos[i]->sem_meal_name, id_str);
        philos[i]->sem_meal = sem_open(philos[i]->sem_meal_name, O_CREAT, 0644, 1);//(philosopher.c:26)
        if (philos[i]->sem_meal == SEM_FAILED)
            return (NULL);
        sem_unlink(philos[i]->sem_meal_name);
        philos[i]->times_ate = 0;
        philos[i]->nb_forks_held = 0;
        philos[i]->ate_enough = false;
        i++;
    }
    return (philos);
}


static bool	init_global_semaphores(t_table *table)
{
    unlink_global_sems();
    table->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT, 0644, table->nb_philos);
    table->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT, 0644, 1);
    table->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT, 0644, table->nb_philos);
    table->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT, 0644, table->nb_philos);
    table->sem_stop = sem_open(SEM_NAME_STOP, O_CREAT, 0644, 1);
    return (true);
}

t_table	*init_table(int ac, char **av, int i)
{
    t_table	*table;

    table = malloc(sizeof(t_table) * 1);
    if (!table)
        return (0);
    table->nb_philos = ft_atoi(av[i++]);
    table->time_to_die = ft_atoi(av[i++]);
    table->time_to_eat = ft_atoi(av[i++]);
    table->time_to_sleep = ft_atoi(av[i++]);
    table->must_eat_count = -1;
    table->philo_full_count = 0;
    table->stop_sim = false;
    if (ac - 1 == 5)
        table->must_eat_count = ft_atoi(av[i]);
    if (!init_global_semaphores(table))
        return (NULL);
    table->philos = init_philosophers(table);
    if (!table->philos)
        return (NULL);
    table->pids = malloc(sizeof(*table->pids) * table->nb_philos);
    if (!table->pids)
        return (NULL);
    return (table);
}

static bool	philo_open_global_semaphores(t_philo *philo)
{
    philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT, 0644, philo->table->nb_philos);
    philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT, 0644, 1);
    philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT, 0644, philo->table->nb_philos);
    philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT, 0644, philo->table->nb_philos);
    return (true);
}

void	init_philo_ipc(t_table *table, t_philo *philo)
{
    if (table->nb_philos == 1)
        return;
    if (!philo_open_global_semaphores(philo))
        child_exit(table, CHILD_EXIT_ERR_SEM);
    if (pthread_create(&philo->personal_grim_reaper, NULL, &personal_grim_reaper, table) != 0)
        child_exit(table, CHILD_EXIT_ERR_PTHREAD);
    return;
}

bool	has_simulation_stopped(t_table *table)
{
    bool	ret;

    sem_wait(table->sem_stop);
    ret = table->stop_sim;
    sem_post(table->sem_stop);
    return (ret);
}

bool	start_simulation(t_table *table)
{
    unsigned int	i;
    pid_t			pid;

    table->start_time = get_time_in_ms() + ((table->nb_philos * 2) * 10);
    i = -1;
    while (++i < table->nb_philos)
    {
        pid = fork();
        if (pid == -1)
            return (NULL);
        else if (pid > 0)
            table->pids[i] = pid;
        else if (pid == 0)
        {
            table->this_philo = table->philos[i];
            philosopher(table);
        }
    }
    if (start_grim_reaper_threads(table) == false)
        return (false);
    return (true);
}

int	get_child_philo(t_table *table, pid_t *pid)
{
    int	philo_exit_code;
    int	exit_code;

    if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
    {
        if (WIFEXITED(philo_exit_code))
        {
            exit_code = WEXITSTATUS(philo_exit_code);
            if (exit_code == CHILD_EXIT_PHILO_DEAD)
                return (kill_all_philos(table, 1));
            if (exit_code == CHILD_EXIT_ERR_PTHREAD || exit_code == CHILD_EXIT_ERR_SEM)
                return (kill_all_philos(table, -1));
            if (exit_code == CHILD_EXIT_PHILO_FULL)
            {
                table->philo_full_count += 1;
                return (1);
            }
        }
    }
    return (0);
}

void	write_status_debug(t_philo *philo, t_status status)
{
    if (status == DIED)
        print_philo_state(philo, "died");
    else if (status == EATING)
        print_philo_state(philo, "is eating");
    else if (status == SLEEPING)
        print_philo_state(philo, "is sleeping");
    else if (status == THINKING)
        print_philo_state(philo, "is thinking");
    else if (status == FORK_1)
        print_philo_state(philo, "has taken a fork");
    else if (status == FORK_2)
        print_philo_state(philo, "has taken a fork");
}

void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
    sem_wait(philo->sem_write);
    if (DEBUG_FORMATTING == true)
    {
        write_status_debug(philo, status);
        if (!reaper_report)
            sem_post(philo->sem_write);
        return;
    }
    if (status == DIED)
        print_philo_state(philo, "died");
    else if (status == EATING)
        print_philo_state(philo, "is eating");
    else if (status == SLEEPING)
        print_philo_state(philo, "is sleeping");
    else if (status == THINKING)
        print_philo_state(philo, "is thinking");
    else if (status == FORK_1 || status == FORK_2)
        print_philo_state(philo, "has taken a fork");
    if (!reaper_report)
        sem_post(philo->sem_write);
}

void	print_philo_state(t_philo *philo, char *state)
{
    long time;

    time = get_time_in_ms() - philo->table->start_time;
    printf("%lu | philo %d %s\n", time, philo->id + 1, state);
}
