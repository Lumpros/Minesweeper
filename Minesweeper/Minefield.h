#pragma once

#include <cstdint>
#include <SDL.h>

#include "Texture.h"
#include "Entity.h"

#define MINEFIELD_ROWS 16
#define MINEFIELD_COLUMNS 30

namespace Game
{
	class Minefield : public Entity
	{
	private:
		struct Cell
		{
			// 9 = mine, 0-8 mines around cell
			int value = 0;
			bool is_opened = false;
			bool is_flagged = false;
		};

		bool is_right_mouse_clicked = false;
		bool is_left_mouse_clicked  = false;
		bool game_over = false;

		SDL_Rect cell_clips[10];
		SDL_Point window_position = { 0, 0 };
		SDL_Rect game_over_mine;

		Texture* field_texture = nullptr, *cells_texture;
		

		SDL_Rect CalculateCellWindowRect(uint16_t row, uint16_t column);
		SDL_Point GetClickedCell(SDL_Point mouse_position);

		void GenerateMinefield(const uint32_t mine_count);
		void GenerateMines(const uint32_t mine_count);
		void GenerateCellHints(void);
		
		bool CellCoordinatesOutOfBounds(signed int row, signed int column);
		signed int CountMinesAroundCell(signed int row, signed int column);

		void InitializeCellClips(void);
		void MakeAllCellsEmpty(void);

		void HandleMouseClick(void);
		void HandleLeftMouseClick(Uint32 mouse_state, SDL_Point mouse_pos);
		void HandleRightMouseClick(Uint32 mouse_state, SDL_Point mouse_pos);

		void RenderCell(uint16_t row, uint16_t column);
		void RenderMinefieldCells(void);

		void DoGameOver(SDL_Point mine_position);

	public:
		Minefield(void);

		void Update(void) override;
		void Render(void) override;

		void SetPositionInWindow(const SDL_Point point);

		//temp
		Cell cells[MINEFIELD_ROWS][MINEFIELD_COLUMNS];
		void OpenCell(signed short row, signed short column);
	};
}

