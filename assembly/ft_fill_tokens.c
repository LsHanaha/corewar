/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshanaha <lshanaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 18:40:56 by lshanaha          #+#    #+#             */
/*   Updated: 2019/04/16 21:54:43 by lshanaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void	ft_fill_token_command(t_asm_data *asm_data, char *line, t_token *token)
{
	if (line[0] == ':' && line[1] != '\0')
	{
		token->type = Label_arg;
		return ;
	}
	if (ft_line_is_command(line))
	{
		token->type = Command;
		return ;
	}
	token->type = Unknown;
	ft_error_add(asm_data, ft_strjoin_orig("Unknown type of token ",\
	token->str), token->col, 1);
}

void	ft_fill_token_reg(t_asm_data *asm_data, char *line, t_token *token)
{
	long long	temp;

	temp = ft_atol(&line[1]);
	if (line[0] == 'r')
	{
		token->type = Unknown;
		if (!line[1])
			ft_error_add(asm_data, ft_strjoin_orig("No value in reg ",\
			token->str), token->col, 1);
		else if (!ft_string_contain_only_num(&line[1]))
			ft_error_add(asm_data, ft_strjoin_orig("Detected Char in reg  ",\
			token->str), token->col, 1);
		else if (temp < 0 || temp > REG_NUMBER)
			ft_error_add(asm_data, ft_strjoin_orig("Wrong reg num ",\
			token->str), token->col, 1);
		else
			token->type = Register;
		return ;
	}
	ft_fill_token_command(asm_data, line, token);
}

void	ft_fill_token_direct(t_asm_data *asm_data, char *line, t_token *token)
{
	if (line[0] == DIRECT_CHAR)
	{
		ft_printf("token str = %s\n", token->str);
		if (line[1] == LABEL_CHAR)
			ft_fill_token_direct_labels(asm_data, &line[2], token);
		else if (ft_isdigit(line[1]) || line[1] == '+' || line[1] == '-')
			ft_fill_token_direct_digits(asm_data, &line[1], token);
		else if (line[1] == 'r')
			ft_fill_token_direct_regs(asm_data, &line[2], token);
		else
		{
			token->type = Unknown;
			ft_error_add(asm_data, ft_strjoin_orig("Unknown value in \
DIRECT_ADDR ", token->str), token->col, 1);
		}
		return ;
	}
	ft_fill_token_reg(asm_data, line, token);
}

void	ft_fill_token_digits(t_asm_data *asm_data, char *line, t_token *token)
{
	if (ft_isdigit(line[0]) || line[0] == '+' || line[0] == '-')
	{
		token->type = Unknown;
		if (line[0] == '-')
			ft_error_add(asm_data, ft_strjoin_orig("Detected minus in number ",\
			token->str), token->col, 1);
		else if (ft_atol(line) > MEM_SIZE)
			ft_error_add(asm_data, ft_strjoin_orig("Value is greater than \
MEM_SIZE ", token->str), token->col, 1);
		else if (!ft_string_contain_only_num(line))
			ft_error_add(asm_data, ft_strjoin_orig("Detected chars in number ",\
			token->str), token->col, 1);
		else
			token->type = Number;
		return ;
	}
	ft_fill_token_direct(asm_data, line, token);
}

void	ft_fill_token(t_asm_data *asm_data, char *line, t_token *token)
{
	(line[0] != ' ') ? ft_printf("{%s} ", line) : 0;
	token->str = line;
	if (line[0] == ' ' || line[0] == '\t')
	{
		token->type = Whitespace;
		return ;
	}
	if (line[0] == SEPARATOR_CHAR)
	{
		token->type = Separator;
		return ;
	}
	if (ft_find_sym(line, LABEL_CHAR) && line[0] != DIRECT_CHAR)
	{
		if (ft_check_label_symbols(line))
			token->type = Label;
		else
		{
			token->type = Unknown;
			ft_error_add(asm_data, ft_strjoin_orig("Wrong symbols in label ",\
			token->str), token->col, 1);
		}
		return ;
	}
	ft_fill_token_digits(asm_data, line, token);
}