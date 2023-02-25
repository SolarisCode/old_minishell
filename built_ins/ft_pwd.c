#include <stdio.h>
#include <stdlib.h> // for the exit() function
#include <unistd.h> // for the getcwd() function

// function to free a dynamically allocated string and set its pointer to NULL
void ft_free_str(char **str)
{
    if (str && *str) // make sure the pointer is not null and the string exists
    {
        free(*str);
        *str = NULL; // set the pointer to null to avoid a dangling pointer
    }
}

int ft_strlen(char *str)
{
	int x;

	x = 0;
	while (str[x])
		x++;
	return (x);
}

int main()
{
    char *cwd = NULL;

    // get the current working directory
    cwd = getcwd(cwd, 0);
    if (cwd == NULL)
    {
        perror("Error");
        return (EXIT_FAILURE);
    }
    else if (printf("%s\n", cwd) != ft_strlen(cwd) + 1) //need to implement my own ft_printf
    {
        // if printf fails, free the string and print an error message
        ft_free_str(&cwd);
        perror("\nError: printf failed");
        return (EXIT_FAILURE);
    }

    // free the string and set the pointer to null
    ft_free_str(&cwd);

    return (EXIT_SUCCESS);
}
