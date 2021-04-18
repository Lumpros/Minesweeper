#include "Minefield.h"
#include "Exception.h"
#include "RNG.h"
#include "Resources.h"
#include "Renderer.h"

#define EMPTY 0
#define MINE 9
#define FALSE_FLAG 10

void Game::Minefield::GenerateMinefield(const uint32_t mine_count, SDL_Point clicked_cell)
{
	if (mine_count > MINEFIELD_COLUMNS * MINEFIELD_ROWS / 2)
		throw std::exception("Mine count exceeds the minefield limit!");

	MakeAllCellsEmpty();
	GenerateMines(mine_count, clicked_cell);
	GenerateCellHints();
}

void Game::Minefield::MakeAllCellsEmpty(void)
{
	for (uint32_t row = 0; row < MINEFIELD_ROWS; ++row)
	{
		for (uint32_t column = 0; column < MINEFIELD_COLUMNS; ++column)
		{
			cells[row][column].value = EMPTY;
		}
	}
}

void Game::Minefield::GenerateMines(const uint32_t mine_count, SDL_Point clicked_cell)
{
	for (uint32_t i = 0; i < mine_count; ++i)
	{
		while (true)
		{
			uint16_t row = RNG::RandUInt16(0, MINEFIELD_ROWS - 1);
			uint16_t column = RNG::RandUInt16(0, MINEFIELD_COLUMNS - 1);

			// Make at least a 3x3 area around the first opened cell empty
			if (abs(clicked_cell.x - row) > 1 || abs(clicked_cell.y - column) > 1)
			{
				if (cells[row][column].value != MINE)
				{
					cells[row][column].value = MINE;
					break;
				}
			}
		}
	}
}

void Game::Minefield::GenerateCellHints(void)
{
	for (signed int row = 0; row < MINEFIELD_ROWS; ++row)
	{
		for (signed int column = 0; column < MINEFIELD_COLUMNS; ++column)
		{
			if (cells[row][column].value != MINE)
				cells[row][column].value = CountMinesAroundCell(row, column);
		}
	}
}

signed int Game::Minefield::CountMinesAroundCell(signed int row, signed int column)
{
	signed int count = 0;

	for (signed int i = row - 1; i <= row + 1; ++i)
	{
		for (signed int j = column - 1; j <= column + 1; ++j)
		{
			if (!CellCoordinatesOutOfBounds(i, j))
				count += static_cast<int>(cells[i][j].value == MINE);
		}
	}

	return count;
}

bool Game::Minefield::CellCoordinatesOutOfBounds(signed int row, signed int column)
{
	return row < 0 || column < 0 || row >= MINEFIELD_ROWS || column >= MINEFIELD_COLUMNS;
}

void Game::Minefield::SetPositionInWindow(const SDL_Point point)
{
	window_position = point;
}

void Game::Minefield::Update(void)
{
	if (!game_over && !has_won)
	{
		HandleMouseClick();

		has_won = PlayerHasWon();

		if (has_won)
		{
			FlagUnopenedCells();
		}

		if (has_been_generated)
		{
			timer.Update();
		}
	}

	HandleButtonState();

	timerDisplay->SetValue(timer.GetTotalSecondsPassed());
	timerDisplay->Update();

	Renderer::Get()->SubmitToRender(this);
}

void Game::Minefield::HandleMouseClick(void)
{
	SDL_Point mouse_pos;
	Uint32 mouse_state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	HandleLeftMouseClick(mouse_state, mouse_pos);
	HandleRightMouseClick(mouse_state, mouse_pos);
}

void Game::Minefield::HandleLeftMouseClick(Uint32 mouse_state, SDL_Point mouse_pos)
{
	if ((mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) && !is_left_mouse_clicked)
	{
		is_left_mouse_clicked = true;
	}

	else if (!(mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) && is_left_mouse_clicked)
	{
		SDL_Point clicked_cell_point = GetClickedCell(mouse_pos);
		is_left_mouse_clicked = false;

		if (!CellCoordinatesOutOfBounds(clicked_cell_point.x, clicked_cell_point.y))
		{
			if (!has_been_generated)
			{
				GenerateMinefield(MINE_COUNT, clicked_cell_point);
				timer.Start();
				has_been_generated = true;
			}

			Cell& clicked_cell = cells[clicked_cell_point.x][clicked_cell_point.y];

			if (!clicked_cell.is_flagged)
			{
				if (clicked_cell.value == MINE)
				{
					DoGameOver(clicked_cell_point);
				}

				else
				{
					OpenCell(clicked_cell_point.x, clicked_cell_point.y);
				}
			}
		}
	}
}

void Game::Minefield::DoGameOver(SDL_Point mine_position)
{
	game_over = true;
	game_over_mine = CalculateCellWindowRect(mine_position.x, mine_position.y);

	for (uint16_t i = 0; i < MINEFIELD_ROWS; ++i)
	{
		for (uint16_t j = 0; j < MINEFIELD_COLUMNS; ++j)
		{
			Cell& cell = cells[i][j];

			if (cell.is_flagged && cell.value != MINE)
			{
				cell.value = FALSE_FLAG;
				cell.is_opened = true;
			}

			else if (!cell.is_flagged && cell.value == MINE)
			{
				cell.is_opened = true;
			}
		}
	}
}

void Game::Minefield::OpenCell(signed short row, signed short column)
{		
	if (CellCoordinatesOutOfBounds(row, column))
	{
		return;
	}

	Cell& cell = cells[row][column];

	if (!cell.is_flagged && !cell.is_opened)
	{
		++opened_cell_count;
		cell.is_opened = true;

		if (cell.value == EMPTY)
		{
			for (signed int i = row - 1; i <= row + 1; ++i)
			{
				for (signed int j = column - 1; j <= column + 1; ++j)
				{
					OpenCell(i, j);
				}
			}
		}
	}
}

void Game::Minefield::HandleRightMouseClick(Uint32 mouse_state, SDL_Point mouse_pos)
{
	if ((mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) && !is_right_mouse_clicked)
	{
		SDL_Point clicked_cell = GetClickedCell(mouse_pos);
		is_right_mouse_clicked = true;

		Cell& cell = cells[clicked_cell.x][clicked_cell.y];

		if (!CellCoordinatesOutOfBounds(clicked_cell.x, clicked_cell.y))
		{
			if (!cell.is_opened)
			{
				cell.is_flagged = !cell.is_flagged;
				cells_flagged += cell.is_flagged ? 1 : -1;
			}
		}
	}

	else if (!(mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) && is_right_mouse_clicked)
	{
		is_right_mouse_clicked = false;
	}
}

SDL_Point Game::Minefield::GetClickedCell(SDL_Point mouse_position)
{
	SDL_Point clicked_cell;
	// Using floor to avoid doing for example -17 / 32 = 0
	// This is because CellCoordinatesOutOfBounds will return false when it should return true
	clicked_cell.y = (int)floor((mouse_position.x - window_position.x) / 32.f);
	clicked_cell.x = (int)floor((mouse_position.y - window_position.y) / 32.f);
	return clicked_cell;
}

void Game::Minefield::Render(void)
{
	field_texture->Render({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });

	RenderMinefieldCells();

	if (game_over)
	{
		SDL_Rect clip = { 102, 1, 16, 16 };
		cells_texture->Render(game_over_mine, &clip);
	}

	button->Render();
}

void Game::Minefield::RenderMinefieldCells(void)
{
	SDL_Point mouse_pos;

	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	SDL_Point clicked_cell = GetClickedCell(mouse_pos);

	for (uint16_t row = 0; row < MINEFIELD_ROWS; ++row)
	{
		for (uint16_t column = 0; column < MINEFIELD_COLUMNS; ++column)
		{
			if (IsHoldingLeftMouseOverCell(row, column, clicked_cell))
			{
				cells_texture->Render(CalculateCellWindowRect(row, column), &cell_clips[EMPTY]);
			}

			else
			{
				RenderCell(row, column);
			}
		}
	}
}

bool Game::Minefield::IsHoldingLeftMouseOverCell(uint16_t row, uint16_t column, SDL_Point clicked_cell)
{
	return !cells[row][column].is_opened && !cells[row][column].is_flagged
		&& is_left_mouse_clicked && clicked_cell.x == row && clicked_cell.y == column;
}

void Game::Minefield::RenderCell(uint16_t row, uint16_t column)
{
	Cell& cell = cells[row][column];

	SDL_Rect clip = cell_clips[cell.value];

	if (!cell.is_opened)
	{
		clip = { 0, 1, 16, 16 };

		if (cell.is_flagged)
		{
			clip.x = 34;
		}
	}

	cells_texture->Render(CalculateCellWindowRect(row, column), &clip);
}

SDL_Rect Game::Minefield::CalculateCellWindowRect(uint16_t row, uint16_t column)
{
	SDL_Rect result;
	result.w = 32;
	result.h = 32;
	result.x = window_position.x + column * 32;
	result.y = window_position.y + row * 32;
	return result;
}

Game::Minefield::Minefield(void)
{
	z_index = -1;
	field_texture = Resources::Get()->gTextures.Get("field");
	cells_texture = Resources::Get()->gTextures.Get("cells");

	InitializeCellClips();
}

void Game::Minefield::InitializeCellClips(void)
{
	cell_clips[EMPTY] = { 17, 1, 16, 16 }; //unused
	cell_clips[MINE] = { 85, 1, 16, 16 };
	cell_clips[FALSE_FLAG] = { 119, 1, 16, 16 };

	for (int i = 1; i <= 8; ++i)
		cell_clips[i] = { 16 * (i - 1) + i - 1, 18, 16, 16 };	
}

void Game::Minefield::HandleButtonState(void)
{
	button->Update();

	if (game_over)
		button->SetClip({ 100, 1, 24, 24 });

	if (has_won)
		button->SetClip({ 75, 1, 24, 24 });

	HandleButtonClick();
	HandleButtonOnCellClick();
}

void Game::Minefield::HandleButtonOnCellClick(void)
{
	SDL_Point mouse_pos;
	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
	SDL_Point clicked_cell = GetClickedCell(mouse_pos);

	if (is_left_mouse_clicked && !CellCoordinatesOutOfBounds(clicked_cell.x, clicked_cell.y))
	{
		if (!cells[clicked_cell.x][clicked_cell.y].is_opened && 
			!cells[clicked_cell.x][clicked_cell.y].is_flagged)
		{
			button->SetClip({ 50, 1, 24, 24 });
		}
	}
}

void Game::Minefield::HandleButtonClick(void)
{
	if (button->IsHeldDown())
	{
		button->SetClip({ 25, 1, 24, 24 });
	}
	else if (!game_over && !has_won)
	{
		button->SetClip({ 0, 1, 24, 24 });
	}

	if (button->IsClicked())
	{
		DoGameReset();
	}
}

void Game::Minefield::RegisterResetButton(Button* button)
{
	this->button = button;
}

void Game::Minefield::DoGameReset(void)
{
	game_over = false;
	has_been_generated = false;
	has_won = false;
	opened_cell_count = 0;
	cells_flagged = 0;
	timer.Reset();
	timer.Stop();

	for (uint16_t row = 0; row < MINEFIELD_ROWS; ++row)
	{
		for (uint16_t column = 0; column < MINEFIELD_COLUMNS; ++column)
		{
			cells[row][column].is_flagged = false;
			cells[row][column].is_opened  = false;
		}
	}
}

int Game::Minefield::GetFlaggedCellsCount(void) const
{
	return cells_flagged;
}

void Game::Minefield::RegisterTimerDisplay(NumberDisplay* timerDisplay)
{
	this->timerDisplay = timerDisplay;
}

bool Game::Minefield::PlayerHasWon(void)
{
	return opened_cell_count == MINEFIELD_COLUMNS * MINEFIELD_ROWS - MINE_COUNT;
}

void Game::Minefield::FlagUnopenedCells(void)
{
	for (uint16_t row = 0; row < MINEFIELD_ROWS; ++row)
	{
		for (uint16_t column = 0; column < MINEFIELD_COLUMNS; ++column)
		{
			if (!cells[row][column].is_opened)
			{
				cells[row][column].is_flagged = true;
			}
		}
	}

	cells_flagged = MINE_COUNT;
}