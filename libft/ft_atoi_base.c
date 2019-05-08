/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gedemais <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 13:16:04 by gedemais          #+#    #+#             */
/*   Updated: 2019/04/11 14:09:37 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		check_num_str(char *str)
{

	int		i;

	i = 0;
	while (str[i])
	{

		if (((str[i] >= '\0' && str[i] < '0') ||(str[i] > '9' && str[i] < 'A')
					|| (str[i] > 'F')) && str[i] != '-')
			return ;
		i++;
	}
}

int				atoi_base(char *str, char *base)
{

	int		dec;
	int		i;
	int		j;
	size_t		y;
	int		sgn;

	j = 0;
	dec = 0;
	sgn = 1;
	check_num_str(str);
	i = ft_strlen(str) - 1;
	while (str[i] && str[i] != '-' && str[i] != '+' && i >= 0)
	{

		y = 0;
		while (base[y] && str[i] != base[y])
			y++;
		if (y == ft_strlen(base))
			y = 0;
		dec += y * ft_iterative_power(ft_strlen(base), j++);
		i--;
	}
	while (str[i] && i >= 0)
		if (str[i--] == '-')
			sgn *= -1;
	return (dec * sgn);
}
