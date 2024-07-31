/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amysiv <amysiv@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:20:51 by amysiv            #+#    #+#             */
/*   Updated: 2024/07/30 16:33:35 by amysiv           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo_dies(t_program *data)
{
	data->start_time = get_time();
	printf("%ld %d has taken a fork\n", (get_time() - data->start_time), 1);
	my_usleep(data->die_time, data);
	printf("%ld %d is died\n", \
		get_time() - data->start_time, 1);
	return ;
}

bool	is_died(t_philo *philo)
{
	long		diff;

	pthread_mutex_lock(&philo->program->print_mutex);
	diff = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->program->print_mutex);
	if (diff >= philo->program->die_time)
	{
		pthread_mutex_lock(&philo->program->status_mutex);
		philo->program->status = false;
		pthread_mutex_unlock(&philo->program->status_mutex);
		my_usleep(200, philo->program);
		printf("%ld %d is died\n", \
			get_time() - philo->program->start_time, philo->index);
		return (true);
	}
	return (false);
}

int	meal_check(t_program *data)
{
	int	i;
	int	counter;

	counter = 1;
	i = 0;
	while (data->p_numb > i)
	{
		if (data->philos[i].is_full == true)
		{
			counter++;
		}
		i++;
	}
	if (counter == data->p_numb)
	{
		return (1);
	}
	return (0);
}

void	monitor(t_program *data)
{
	int	i;

	i = 0;
	while (1)
	{
		if (data->philos[i].index == data->p_numb)
			i = 0;
		if (is_died(&data->philos[i]))
			return ;
		pthread_mutex_lock(&data->status_mutex);
		if ((data->philos[i].meal_count == data->eat_numb) && \
			(data->philos[i].is_full != true) && (data->eat_numb > 0))
		{
			data->philos[i].is_full = true;
		}
		pthread_mutex_unlock(&data->status_mutex);
		if (meal_check(data) && (data->eat_numb > 0))
		{
			pthread_mutex_lock(&data->status_mutex);
			data->status = false;
			printf("All philosophers are full!");
			pthread_mutex_unlock(&data->status_mutex);
			return ;
		}
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_program	arg_data;

	if ((argc == 5 || argc == 6) && check_numb(argv))
	{
		arg_data.status = true;
		atol_arguments(&arg_data, argv, argc);
		if (arg_data.p_numb == 1)
		{
			one_philo_dies(&arg_data);
			return (0);
		}
		init_philo(&arg_data);
		if (!arg_data.philos)
			return (-1);
		if (!start_simulation(&arg_data))
			return (-1);
	}
	else
	{
		printf("Failure\n");
		return (1);
	}
	return (0);
}
