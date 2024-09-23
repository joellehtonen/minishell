/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:46:57 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/23 15:50:44 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	cd_exec(t_shell *shell);

int	builtins(t_shell *shell)
{
	if (ft_strncmp(shell->user_input, "cd", 2) == 0)
		cd_exec(shell);	// int return?
/* 	if (ft_strncmp(shell->user_input, "EXIT", 2) == 0)
		exit_exec(shell);
	if (ft_strncmp(shell->user_input, "ENV", 2) == 0)
		env_exec(shell);
	if (ft_strncmp(shell->user_input, "EXPORT", 2) == 0)
		export_exec(shell);
	if (ft_strncmp(shell->user_input, "UNSET", 2) == 0)
		unset_exec(shell);
	if (ft_strncmp(shell->user_input, "ECHO", 2) == 0)
		echo_exec(shell);
	if (ft_strncmp(shell->user_input, "PWD", 2) == 0)
		pwd_exec(shell); */
	return (0);
}

static int	cd_exec(t_shell *shell)
{
	char	*old_path;
	char	*new_path;
	int		len;
	
	old_path = (char *)malloc(B_SIZE * sizeof(char));
	if (getcwd(old_path, 1024) == NULL)
	{
		perror("getcwd error");
		//free_and_exit();
	}
	printf("old path - %s\n", old_path);
	len = 0;
	//if (ft_strlen(shell->user_input == 2)
		//go to home dir
		//what if "cd ." or "cd .."?
	len = ft_strlen(old_path) + ft_strlen(shell->user_input + 3) + 2;
	new_path = (char *)malloc(len * sizeof(char));
	new_path = ft_strjoin_four(old_path, "/",shell->user_input + 3, "");
	free(old_path);
	printf("new path - %s\n", new_path);
	if (access(new_path, F_OK) == -1 && errno == ENOENT)
	{
		printing(shell->user_input + 3, ": No such file or directory\n", 2);
		free(new_path);
		//free_and_exit();
		//exit(127); // can add when in child process
		return (1);
	}
	if (chdir(new_path) == -1)
	{
    	perror("chdir() error");
		//free_and_exit();
	}
	//write new location to shell->pwd
	free(new_path);
	return (0);
}