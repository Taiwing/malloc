#include "ft_wait_lock.h"

t_mslock	g_lock = { 0 };


void	*slave(void *ptr)
{
	(void)ptr;
	ft_printf("Slave: Waiting for Master\n");
	ft_wait_master(&g_lock);
	ft_printf("Slave: Yeah!!! Now I am free!\n");
	return (NULL);
}

int	main(void)
{
	int			ret;
	t_ft_thread	slave_thread;

	ft_exitmsg("mslock_test");
	ft_mslock_init(&g_lock);
	ft_printf("Master: Locking Slave\n");
	ft_lock_slave(&g_lock);
	if ((ret = ft_thread_create(&slave_thread, NULL, slave, NULL)))
		ft_exit(EXIT_FAILURE, "ft_thread_create: %s", strerror(ret));
	sleep(1);
	ft_printf("Master: Unlocking Slave\n");
	ft_unlock_slave(&g_lock);
	ft_printf("Master: Done Unlocking Salve\n");
	ft_thread_join(&slave_thread, NULL);
	return (EXIT_SUCCESS);
}
