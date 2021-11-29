#include <unistd.h>
#include <stdio.h>
int		main(int argc, char **argv, char **envp)
{
	char	*arg[3];

	arg[0] = "/bin/echo";
	arg[1] = "saluuuuuuut";
	arg[2] = NULL;

	execve("/bin/echo", arg, envp);
	printf("error\n");
	return (0);
}
