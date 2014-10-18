#ifndef GAME_HPP
#define GAME_HPP
#include <vector>

// This class represents a single card on the board, tracking its state
// orientation, and current animation.
class Card{
public:
	Card(int m) : model(m) {}
	// Card model number.
	int model;
	bool uncovered = false;
	bool removed = false;
	mutable double animation_start = 0.0;
	enum{
		ANIM_MODE_UNCOVER = 0,
		ANIM_MODE_COVER,
		ANIM_MODE_WAIT
	} AnimModes;
	mutable int animation_mode = -1;
};


class Game{
private:
	Game() = delete; // static class
public:
	// Used for tracking whether there is a game in progress, or has it finished.
	typedef enum{
		GAME_STATE_NONE,
		GAME_STATE_READY_TO_START,
		GAME_STATE_IN_PROGRESS,
		GAME_STATE_FINISHED
	} GameState;
	static GameState game_state;
	// The array of cards.
	static std::vector<Card> cards;
	// How card pairs the player has yet to find? How many rounds have been played?
	static int pairs_left, rounds;
	// Used for measuring game time.
	static double game_start_time, game_finish_time;

	// Prepares everything for a new game.
	static void StartNewGame();
	// This procedure is called when the player presses space to select a card.
	static void TestCard(int n);
	// Returns time (in secs) since game start.
	static double GetGameTime();
	// Call this when animation is compeleted to finalize appropriate action.
	static void OnCardAnimationFinished(int n);

	// Here is board size defined, can be set (compile-time) to any other value.
	static constexpr unsigned int board_width = 4;
	static constexpr unsigned int board_height = 4;
	static constexpr unsigned int cards_no = board_height*board_width;

	static_assert(Game::cards_no % 2 == 0, "Cards number with these dimentions is not an even number!");

	// The ids of cards in pair that are being tested by the player.
	static int card_testedA, card_testedB;
};

#endif
