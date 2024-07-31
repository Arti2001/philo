/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amysiv <amysiv@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 15:42:31 by amysiv            #+#    #+#             */
/*   Updated: 2024/07/30 16:32:16 by amysiv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo(t_program *data)
{
	int			i;
	t_philo		*philo;

	data->philos = NULL;
	philo = malloc(sizeof(t_philo) * data->p_numb);
	if (philo == NULL)
		return ;
	i = 0;
	while (i < data->p_numb && data->p_numb > 1)
	{
		philo[i].is_full = false;
		philo[i].program = data;
		philo[i].index = i + 1;
		philo[i].meal_count = 0;
		philo[i].last_meal = get_time();
		philo[i].l_fork = &philo[i + 1].r_fork;
		if (philo[i].index == data->p_numb)
			philo[i].l_fork = &philo[0].r_fork;
		i++;
	}
	data->philos = philo;
}

int	philo_fork_init(t_program *data)
{
	int	i;
	int	fork_count;

	fork_count = 0;
	i = 0;
	while (i < data->p_numb)
	{
		if (fork_init(&data->philos[i], i, fork_count) == -1)
			return (-1);
		fork_count++;
		if (thread_init(&data->philos[i], i, fork_count) == -1)
			return (-1);
		i++;
	}
	return (1);
}

int	thread_init(t_philo *philo, int thread_count, int fork_count)
{
	if (pthread_create(&philo->thread, NULL, &routine, philo) == -1)
	{
		if (destroy_fork_mutex(philo->program, fork_count) == -1)
			return (-1);
		if (thread_count > 0)
		{
			if (join(thread_count, philo->program) == -1)
				return (-1);
		}
		free(philo->program->philos);
		return (-1);
	}
	return (1);
}

int	fork_init(t_philo *philo, int thread_count, int fork_count)
{
	if (pthread_mutex_init(&philo->r_fork, NULL) == -1)
	{
		if (thread_count > 0)
		{
			if (destroy_fork_mutex(philo->program, fork_count) == -1)
				return (-1);
			if (join(thread_count, philo->program) == -1)
				return (-1);
		}
		free(philo->program->philos);
		return (-1);
	}
	return (1);
}

int	init_extra_mutex(t_program *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) == -1)
	{
		free(data->philos);
		return (-1);
	}
	if (pthread_mutex_init(&data->status_mutex, NULL) == -1)
	{
		free(data->philos);
		return (-1);
	}
	if (pthread_mutex_init(&data->meal_mutex, NULL) == -1)
	{
		free(data->philos);
		return (-1);
	}
	return (1);
}
