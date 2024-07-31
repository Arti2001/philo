/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amysiv <amysiv@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:57:23 by amysiv            #+#    #+#             */
/*   Updated: 2024/07/31 17:01:46 by amysiv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	taking_a_fork(t_philo *philo)
{
	//pthread_mutex_lock(&philo->program->print_mutex);
	//if ((philo->index % 2) == 1)
	//	my_usleep(philo->program->eat_time / 2, philo->program);
	//pthread_mutex_unlock(&philo->program->print_mutex);
	pthread_mutex_lock(&philo->r_fork);
	print_action("has taken a fork", philo);
	pthread_mutex_lock(philo->l_fork);
	print_action("has taken a fork", philo);
}

void	is_eating(t_philo *philo)
{
	print_action("is eating", philo);
	pthread_mutex_lock(&philo->program->print_mutex);
	philo->last_meal = get_time();
	my_usleep(philo->program->eat_time, philo->program);
	pthread_mutex_unlock(&philo->program->print_mutex);
	pthread_mutex_unlock(&philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_lock(&philo->program->meal_mutex);
	philo->meal_count += 1;
	pthread_mutex_unlock(&philo->program->meal_mutex);
}

void	is_sleeping(t_philo *philo)
{
	print_action("is sleeping", philo);
	my_usleep(philo->program->sleep_time, philo->program);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	//pthread_mutex_lock(&philo->program->print_mutex);
	if ((philo->index % 2) == 1)
		my_usleep(philo->program->eat_time / 2, philo->program);
	//pthread_mutex_unlock(&philo->program->print_mutex);
	while (1)
	{
		pthread_mutex_lock(&philo->program->status_mutex);
		if (philo->program->status == false)
			return (pthread_mutex_unlock(&philo->program->status_mutex), NULL);
		pthread_mutex_unlock(&philo->program->status_mutex);
		taking_a_fork(philo);
		is_eating(philo);
		is_sleeping(philo);
		print_action("is thinking", philo);
	}
	return (NULL);
}

int	start_simulation(t_program *data)
{
	data->start_time = get_time();
	if (!init_extra_mutex(data))
		return (-1);
	if (!philo_fork_init(data))
		return (-1);
	monitor(data);
	if (!join(data->p_numb, data))
		return (-1);
	if (destroy_fork_mutex(data, data->p_numb) == -1)
		return (-1);
	if (destroy_extra_mutex(data) == -1)
		return (-1);
	free(data->philos);
	return (1);
}
