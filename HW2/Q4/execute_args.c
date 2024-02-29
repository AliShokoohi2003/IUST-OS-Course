#include "shell.h"

int execute_args(char **args)
{
	pid_t pid;
	int status;
	char *builtin_func_list[] = {
		"exit",
		"cd"
	};
	int (*builtin_func[])(char **) = {
		&own_exit,
		&own_cd
	};

	long unsigned int i = 0;
	for (; i < sizeof(builtin_func_list) / sizeof(char *); i++)
	{
		if (strcmp(args[0], builtin_func_list[i]) == 0)
		{
			return ((*builtin_func[i])(args));
		}
	}
	pid_t pr = fork();
	if (pr == 0)
	{
		execvp(args[0], args);

		exit(errno);
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			fprintf(stderr, "Command exited with status = %d\n",WEXITSTATUS(status));
	}

	return (-1); // this status code means everything is ok
}

