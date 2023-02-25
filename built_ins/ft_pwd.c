#include <stdio.h>
#include <stdlib.h> // for the exit()
#include <unistd.h> // for the getcwd()

// this will free a dynamically allocated str and set its pointer to NULL
// make sure the pointer is not null and the string exists
// at last, set the pointer to null to avoid a dangling pointer
void ft_free_str(char **str)
{
    if (str && *str) 
    {
        free(*str);
        *str = NULL; 
    }
}

// find the len of a string
int ft_strlen(char *str)
{
	int x;

	x = 0;
	while (str[x])
		x++;
	return (x);
}

// get the current working directory
//need to implement my own ft_printf
// if printf fails, free the string and print an error message
//at last, free the string and set the pointer to null
int main()
{
    char *cwd = NULL;

    
    cwd = getcwd(cwd, 0);
    if (cwd == NULL)
    {
        perror("Error");
        return (EXIT_FAILURE);
    }
    else if (printf("%s\n", cwd) != ft_strlen(cwd) + 1) 
    {
        ft_free_str(&cwd);
        perror("\nError: printf failed");
        return (EXIT_FAILURE);
    }

    ft_free_str(&cwd);

    return (EXIT_SUCCESS);
}
