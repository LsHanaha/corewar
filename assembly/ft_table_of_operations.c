/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_table_of_operations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshanaha <lshanaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 19:59:19 by lshanaha          #+#    #+#             */
/*   Updated: 2019/04/18 20:29:00 by lshanaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

int				g_command_nums[REG_NUMBER] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,\
	11, 12, 13, 14, 15, 16};

int				g_count_of_args[REG_NUMBER] = {1, 2, 2, 3, 3, 3, 3, 3, 1, 3,\
	3, 1, 2, 3, 1, 1};

int				g_arg1_type[REG_NUMBER] = {
	T_DIR,
	T_DIR + T_IND,
	T_REG,
	T_REG,
	T_REG,
	T_REG + T_DIR + T_IND,
	T_REG + T_DIR + T_IND,
	T_REG + T_DIR + T_IND,
	T_DIR,
	T_REG + T_DIR + T_IND,
	T_REG,
	T_DIR,
	T_DIR + T_IND,
	T_REG + T_DIR + T_IND,
	T_DIR,
	T_REG
};

int				g_arg2_type[REG_NUMBER] = {
	0,
	T_REG,
	T_REG + T_IND,
	T_REG,
	T_REG,
	T_REG + T_DIR + T_IND,
	T_REG + T_DIR + T_IND,
	T_REG + T_DIR + T_IND,
	0,
	T_REG + T_DIR,
	T_REG + T_DIR + T_IND,
	0,
	T_REG,
	T_REG + T_DIR,
	0,
	0
};

int				g_arg3_type[REG_NUMBER] = {
	0,
	0,
	0,
	T_REG,
	T_REG,
	T_REG,
	T_REG,
	T_REG,
	0,
	T_REG,
	T_REG + T_DIR,
	0,
	0,
	T_REG,
	0,
	0
};

int				g_args_codes[REG_NUMBER] = {
	0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1
};

t_table_ops		*ft_table_operations_init(void)
{
	t_table_ops	*operations;

	operations = (t_table_ops *)malloc(sizeof(t_table_ops));
	if (!operations)
		exit(ft_printf("error in malloc ft_table_operations_init()\n"));
	operations->command_nums = g_command_nums;
	operations->count_of_args = g_count_of_args;
	operations->arg1_type = g_arg1_type;
	operations->arg2_type = g_arg2_type;
	operations->arg3_type = g_arg3_type;
	operations->arg_code_exists = g_args_codes;
	return (operations);
}

t_label_compile	*ft_init_label_compile(void)
{
	t_label_compile	*label;

	label = (t_label_compile *)malloc(sizeof(t_label_compile));
	if (!label)
		exit(1);
	label->points_to_row = 0;
	label->label_text = NULL;
	label->row_num = 0;
	return (label);
}

t_syntax_row	*ft_syn_row_init(void)
{
	t_syntax_row	*row;

	row = (t_syntax_row *)malloc(sizeof(t_syntax_row));
	if (!row)
		exit(ft_printf("error in malloc ft_syn_row_init()\n"));
	row->row_num = 0;
	row->command_num = 0;
	row->command_size = 0;
	row->num_current_arg = 1;
	row->wait_separator = 0;
	row->newline = 0;
	row->prev_arg_type = -5;
	row->args_text = NULL;
	row->arg_types_code = 0;
	return (row);
}

t_list		*ft_collect_labels(t_asm_data *asm_data, int i)
{
	t_list			*current;
	t_list			*root;
	t_list			*chain;
	t_label_compile	*label;

	root = NULL;
	current = asm_data->tokens;
	while (++i < ASM_TOKEN_SIZE)
	{
		if (((t_token *)(current->content))->type == Label)
		{
			label = ft_init_label_compile();
			label->row_num = ASM_NUM_ROW;
			label->label_text = ft_strdup(((t_token *)(current->content))->str);
			chain = ft_lstnew(NULL, sizeof(t_label_compile));
			chain->content = label;
			chain->content_size = i++;
			if (!root)
				root = chain;
			else
				ft_lstadd_last(root, chain);
		}
		current = current->next;
	}
	return (root);
}