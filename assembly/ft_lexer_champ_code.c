/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_champ_code.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshanaha <lshanaha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 20:12:32 by lshanaha          #+#    #+#             */
/*   Updated: 2019/04/18 18:49:08 by lshanaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

char	*ft_lexer_champ_code(t_asm_data *asm_data, char *line, int j)
{
	int		i;
	int		word_len;
	t_token	*token;
	t_list	*chain;

	i = 0;
	ft_putendl(line);
	if (!MACHINE_NEW_LINE)
		ft_add_new_str_token(asm_data);
	while (line[i])
	{
		word_len = ft_get_word_len(&line[i]);
		if (line[i] == COMMENT_CHAR)
			return (line);
		chain = ft_lstnew(NULL, sizeof(t_token));
		token = ft_token_init();
		chain->content = token;
		asm_data->token_size += 1;
		ft_lstadd_last((asm_data->tokens), chain);
		token->row = ASM_NUM_ROW;
		token->col = i + j;
		ft_fill_token(asm_data, ft_strsub(line, i, word_len), token);
		i += word_len;
	}
	return (line);
}
