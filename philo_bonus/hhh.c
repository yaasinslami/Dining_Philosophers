
#include "philo_bonus.h"

int main()
{
	sem_unlink(FORK_SEM);
	sem_unlink(LOG_SEM);
	return (0);
}
