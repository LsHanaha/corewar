/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmcclure <mmcclure@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 18:36:36 by mmcclure          #+#    #+#             */
/*   Updated: 2019/04/15 19:35:27 by mmcclure         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "visu.h"

static void		render_status_img(t_window *window, t_vm *vm)
{
	SDL_Rect	dst;

	dst = (SDL_Rect){9, 15, SCREEN_WIDTH * 0.75, SCREEN_HEIGHT - 30};
	if (WIN_STATUS == STAT_START)
	{
		SDL_RenderCopy(WIN_REND, BACK_START, NULL, &dst);
		print_str(window, "Press SPACE to start", 380, 800);
		print_str(window, "** Starting **", 1245, 40);
		print_start_names(window, vm);
	}
	if (WIN_STATUS == STAT_PAUSE)
	{
		print_str(window, "Press SPACE to continue", 330, 800);
		print_str(window, "** Paused **", 1258, 40);
	}
	if (WIN_STATUS == STAT_END)
	{
		SDL_RenderCopy(WIN_REND, BACK_END, NULL, &dst);
		print_str(window, "Press ANY KEY to exit", 371, 800);
		print_str(window, vm->winner->name,
						609 - ft_strlen(vm->winner->name) * 15, 550);
		print_str(window, "** Finished **", 1245, 40);
	}
}

static void		render_status_val(t_window *window, t_vm *vm)
{
	int			height;
	int			i;

	i = 0;
	height = 80;
	print_nbr(window, WIN_SPEED, 1459, height += 35);
	print_nbr(window, VM_CYCLE, 1287, height += 35);
	print_nbr(window, VM_P_TOTAL, 1324, height += 35);
	print_nbr(window, VM_CYCLE_TO_DIE, 1358, height += 35);
	print_nbr(window, CYCLE_DELTA, 1352, height += 35);
	print_nbr(window, NBR_LIVE, 1322, height += 35);
	print_nbr(window, MAX_CHECKS, 1352, height += 35);
	height = 380;
	while (VM_CHAMPS[i] != NULL)
	{
		print_nbr(window, VM_CHAMPS[i]->last_live, 1325, height += 60);
		print_nbr(window, VM_CHAMPS[i]->lives_in_period, 1435, height += 30);
		height += 30;
		i++;
	}
}

static void		change_run(t_window *window, t_vm *vm)
{
	static int	n;

	VM_CYCLE += 1;
	vm->process->cycles_to_wait -= 1;
	vm->process->next->cycles_to_wait -= 1;
	if (vm->process->cycles_to_wait < 0)
	{
		vm->process->next->cycles_to_wait = 50;
		vm->process->next->position += 2;
		vm->process->next->pos_written += 15;
		vm->process->cycles_to_wait = 50;
		vm->process->position++;
		vm->process->pos_written += 10;
	}
	if (VM_CYCLE_TO_DIE <= 0)
	{
		VM_WINNER = VM_CHAMPS[0];
		VM_CYCLE_TO_DIE = 0;
		WIN_STATUS = 3;
		return ;
	}
	if (VM_CYCLE >= 1460 * (n + 1))
	{
		n++;
		VM_CYCLE_TO_DIE -= CYCLE_DELTA;
	}
}

static void		make_car_color(t_window *window, t_proccess *proc)
{
	FONT_COLOR = (SDL_Color){COL_BLACK};
	SDL_SetRenderDrawColor(WIN_REND, COL_GREY);
	if (proc->cycles_to_wait == 0 && proc->player_id == -1)
		FONT_COLOR = (SDL_Color){COL_L_GREY};
	else if (proc->cycles_to_wait == 0)
		FONT_COLOR = get_player_color(proc->player_id);
	else if (proc->player_id == -1)
	{
		FONT_COLOR = (SDL_Color){COL_GREY};
		SDL_SetRenderDrawColor(WIN_REND, COL_L_GREY);
	}
	else if (proc->player_id == 0)
		SDL_SetRenderDrawColor(WIN_REND, COL_GREEN);
	else if (proc->player_id == 1)
		SDL_SetRenderDrawColor(WIN_REND, COL_BLUE);
	else if (proc->player_id == 2)
		SDL_SetRenderDrawColor(WIN_REND, COL_RED);
	else if (proc->player_id == 3)
		SDL_SetRenderDrawColor(WIN_REND, COL_YELOW);
}

static void		render_car_source(t_window *window, t_vm *vm, t_proccess *proc)
{
	int			i;
	char		*hex;
	char		str[3];

	hex = "0123456789abcdef";
	if (proc->pos_written >= 0)
	{
		str[2] = '\0';
		i = -1;
		if (proc->cycles_to_wait == 25)
			FONT_COLOR = get_player_color(proc->player_id + 4);
		else if (proc->cycles_to_wait == 0)
			FONT_COLOR = get_player_color(proc->player_id);
		if (proc->cycles_to_wait == 25 || proc->cycles_to_wait == 0)
			while (++i < 4)
			{
				str[0] = hex[VM_MEMORY[proc->pos_written + i] / 16];
				str[1] = hex[VM_MEMORY[proc->pos_written + i] % 16];
				print_str(window, str,
							13 + (proc->pos_written + i) % 64 * 18.7,
							20 + 13.5 * (int)((proc->pos_written + i) / 64));
			}
	}
}

static void		render_carrier(t_window *window, t_vm *vm)
{
	SDL_Rect	rect;
	t_proccess	*proc;
	int			pos;
	char		str[3];
	char		*hex;

	hex = "0123456789abcdef";
	proc = vm->process;
	str[2] = '\0';
	SDL_SetRenderTarget(WIN_REND, WIN_BACK);
	while (proc)
	{
		pos = proc->position;
		rect = (SDL_Rect){13 + pos % 64 * 18.7,
							20 + 13.5 * (int)(pos / 64), 15, 15};
		make_car_color(window, proc);
		SDL_RenderFillRect(WIN_REND, &rect);
		str[0] = hex[VM_MEMORY[pos] / 16];
		str[1] = hex[VM_MEMORY[pos] % 16];
		print_str(window, str, 13 + pos % 64 * 18.7,
							20 + 13.5 * (int)(pos / 64));
		render_car_source(window, vm, proc);
		proc = proc->next;
	}
	SDL_SetRenderTarget(WIN_REND, NULL);
}

void			render_image(t_window *window, t_vm *vm, t_op op_tab[17])
{
	SDL_RenderCopy(WIN_REND, WIN_BACK, NULL, NULL);
	SDL_RenderSetScale(WIN_REND, (WIN_WID / (float)SCREEN_WIDTH),
									(WIN_HEIG / (float)SCREEN_HEIGHT));
	FONT_CURR = FONT_STAT;
	FONT_COLOR = (SDL_Color){COL_WHITE};
	render_status_val(window, vm);
	FONT_CURR = FONT_PAUSE;
	if (WIN_STATUS == STAT_START || WIN_STATUS == STAT_PAUSE
										|| WIN_STATUS == STAT_END)
		render_status_img(window, vm);
	else
	{
		print_str(window, "** Running **", 1244, 40);
		FONT_CURR = FONT_ARENA;
		render_carrier(window, vm);
		// change_run(window, vm);
		do_cyrcle(vm, op_tab);
	}
}
