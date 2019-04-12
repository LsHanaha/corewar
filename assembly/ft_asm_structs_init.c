/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_asm_structs_init.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshanaha <lshanaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 15:00:35 by lshanaha          #+#    #+#             */
/*   Updated: 2019/04/12 16:09:53 by lshanaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

t_champ_data	*ft_champ_data_init(void)
{
	t_champ_data	*champ_data;

	champ_data = (t_champ_data *)malloc(sizeof(t_champ_data));
	if (!champ_data)
		exit(1);
	champ_data->comment = ft_token_init();
	champ_data->name = ft_token_init();
	champ_data->flag = 0;
	return (champ_data);
}

t_marks			*ft_marks_init(void)
{
	t_marks		*marks;

	marks = (t_marks *)malloc(sizeof(t_marks));
	if (!marks)
		exit(1);
	marks->name = NULL;
	marks->count_number_in_data = 0;
	return (marks);
}

t_token			*ft_token_init(void)
{
	t_token		*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		exit(1);
	token->error = 0;
	token->num_clomun = 0;
	token->num_string = 0;
	token->token = NULL;
	return (token);
}

t_lex_str		*ft_lex_str_init(void)
{
	t_lex_str	*lex_str;

	lex_str = (t_lex_str *)malloc(sizeof(t_lex_str));
	if (!lex_str)
		exit(1);
	lex_str->num = 0;
	lex_str->command = NULL;
	lex_str->arg_1 = NULL;
	lex_str->delimetr1 = 0;
	lex_str->arg_2 = NULL;
	lex_str->delimetr2 = 0;
	lex_str->arg_3 = NULL;
	lex_str->new_line = 0;
	return (lex_str);
}

t_asm			*ft_asm_init(void)
{
	t_asm	*asm_data;

	asm_data = (t_asm *)malloc(sizeof(t_asm));
	if (!asm_data)
		exit(1);
	asm_data->string_with_data = 0;
	asm_data->string_current = 0;
	asm_data->champ_data = ft_champ_data_init();
	asm_data->marks = NULL;
	asm_data->lex_tree = ft_rbtree_root_init();
	asm_data->errors = NULL;
	asm_data->error_size = 0;
	asm_data->current_state = 0;
	return (asm_data);
}
