/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_number.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfitahin <nfitahin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 06:45:38 by nfitahin          #+#    #+#             */
/*   Updated: 2026/09/14 06:45:39 by nfitahin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

static int	ft_str_is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static long	ft_atol(const char *nptr)
{
	int				sign;
	unsigned int	i;
	long			result;

	i = 0;
	result = 0;
	sign = 1;
	if (!nptr)
		return (0);
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = sign * (-1);
		i++;
	}
	while (ft_isdigit(nptr[i]))
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	ft_is_in_range(long n)
{
	if ((n > 2147483647) || (n < -2147483648))
		return (0);
	return (1);
}

long	ft_check_number(const char *str)
{
	long	number;

	number = ft_atol(str);
	if (!ft_str_is_numeric(str))
		return (-1);
	if (!ft_is_in_range(number))
		return (-1);
	return (number);
}
