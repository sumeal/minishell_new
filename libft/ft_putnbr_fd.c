/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchen2 <jchen2@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:43:03 by jchen2            #+#    #+#             */
/*   Updated: 2024/11/15 12:29:19 by jchen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
PRINT OUT THE INTERGER USING WRITE FUNC
*/
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	c = n % 10 + '0';
	write(fd, &c, 1);
}
/*
int main()
{
    int fd = 1; // standard output
    ft_putnbr_fd(12345, fd);  // Outputs: 12345
    ft_putnbr_fd(-12345, fd); // Outputs: -12345
    ft_putnbr_fd(0, fd);      // Outputs: 0
    ft_putnbr_fd(-2147483648, fd); // Outputs: -2147483648
    return 0;
}
*/