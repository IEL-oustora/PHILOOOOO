#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <pthread.h>

# define MAX_PHILOS	250
# define STR_MAX_PHILOS "250"

# ifndef DEBUG_FORMATTING
#  define DEBUG_FORMATTING 0
# endif

# define SEM_NAME_FORKS "/philo_global_forks"
# define SEM_NAME_WRITE "/philo_global_write"
# define SEM_NAME_FULL	"/philo_global_full"
# define SEM_NAME_DEAD	"/philo_global_dead"
# define SEM_NAME_STOP	"/philo_global_stop"
# define SEM_NAME_MEAL	"/philo_local_meal_"

# define CHILD_EXIT_ERR_PTHREAD	40
# define CHILD_EXIT_ERR_SEM		41
# define CHILD_EXIT_PHILO_FULL	42
# define CHILD_EXIT_PHILO_DEAD	43

typedef struct s_philo	t_philo;

typedef struct s_table
{
	time_t			start_time;
	unsigned int	nb_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_philo_full;
	unsigned int	philo_full_count;
	sem_t			*sem_philo_dead;
	sem_t			*sem_stop;
	bool			stop_sim;
	t_philo			**philos;
	t_philo			*this_philo;
	pid_t			*pids;
	pthread_t		gluttony_reaper;
	pthread_t		famine_reaper;
}	t_table;

typedef struct s_philo
{
	pthread_t			personal_grim_reaper;
	sem_t				*sem_forks;
	sem_t				*sem_write;
	sem_t				*sem_philo_full;
	sem_t				*sem_philo_dead;
	sem_t				*sem_meal;
	char				*sem_meal_name;//
	unsigned int		nb_forks_held;
	unsigned int		id;
	unsigned int		times_ate;
	bool				ate_enough;
	time_t				last_meal;
	t_table				*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	FORK_1 = 4,
	FORK_2 = 5
}	t_status;


bool			has_simulation_stopped(t_table *table);

bool			is_valid_input(int ac, char **av);
int				ft_atoi(const char *str);


t_table			*init_table(int ac, char **av, int i);


void			init_philo_ipc(t_table *table, t_philo *philo);

void			philosopher(t_table *table);


void			grab_fork(t_philo *philo);

time_t			get_time_in_ms(void);
void			philo_sleep(time_t sleep_time);
void			sim_start_delay(time_t start_time);

void			write_status(t_philo *philo, bool reaper, t_status status);
void			write_outcome(t_table *table);
void    		print_philo_state(t_philo *philo, char *state);

void			*global_gluttony_reaper(void *data);
void			*global_famine_reaper(void *data);
void			*personal_grim_reaper(void *data);
int				kill_all_philos(t_table *table, int exit_code);

char			*ft_utoa(unsigned int nb, size_t len);
char			*ft_strcat(char	*dst, const char *src);
size_t			ft_strlen(const char *str);
void			unlink_global_sems(void);
bool			start_grim_reaper_threads(t_table *table);

void			*free_table(t_table *table);
int				sem_error_cleanup(t_table *table);
int				table_cleanup(t_table *table, int exit_code);

void			child_exit(t_table *table, int exit_code);
int				msg(char *str, char *detail, int exit_no);
int				error_failure(char *str, char *details, t_table *table);
void			*error_null(char *str, char *details, t_table *table);

int	get_child_philo(t_table *table, pid_t *pid);
void	write_status_debug(t_philo *philo, t_status status);
bool	start_simulation(t_table *table);
#endif

