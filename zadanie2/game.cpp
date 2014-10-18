#include "game.hpp"
#include <iostream>
#include <algorithm>
#include "models.hpp"
#include "render.hpp"

Game::GameState Game::game_state = Game::GAME_STATE_NONE;
std::vector<Card> Game::cards;
int Game::pairs_left, Game::rounds;
int Game::card_testedA = -1, Game::card_testedB = -1;
double Game::game_start_time, Game::game_finish_time;
constexpr unsigned int Game::board_height, Game::board_width;

void Game::StartNewGame(){
	cards.clear();
	// Prepare cards
	for(unsigned int i = 0; i < cards_no/2; i++){
		cards.push_back( Card(i) ); // Add 2 cards of i-th model.
		cards.push_back( Card(i) );
	}
	//std::random_shuffle(cards.begin(), cards.end()); // Shuffle the deck!
	pairs_left = cards_no/2;
	rounds = 0;
	game_state = GAME_STATE_READY_TO_START;
}

void Game::TestCard(int n){
	if(cards[n].removed || cards[n].uncovered || card_testedB != -1 || n == card_testedA) return;
	cards[n].animation_start = Render::GetTime();
	cards[n].animation_mode = Card::ANIM_MODE_UNCOVER;
	if(card_testedA == -1){ // first card from a tested pair
		card_testedA = n;
	}else{ 					// second card from a tested pair
		card_testedB = n;
	}
	if(game_state == GAME_STATE_READY_TO_START){
		game_state = GAME_STATE_IN_PROGRESS;
		game_start_time = Render::GetTime();
	}
}

double Game::GetGameTime(){
	if(game_state == GAME_STATE_READY_TO_START) return 0.0;
	else if(game_state == GAME_STATE_FINISHED) return game_finish_time - game_start_time;
	else return Render::GetTime() - game_start_time;
}

void Game::OnCardAnimationFinished(int n){
	if(cards[n].animation_mode == Card::ANIM_MODE_UNCOVER){
		// Finalize card uncovering
		cards[n].uncovered = true;
		cards[n].animation_mode = -1;
		if(card_testedA == n){
			// this is the first card in a pair
		}else if(card_testedB == n){
			// this is the second card in a pair
			rounds++;
			// Do these two uncovered cards match each other?
			if( cards[card_testedB].model % CARD_MODELS == cards[card_testedA].model % CARD_MODELS){
				// Yes, they match!
				cards[card_testedB].removed = true;
				cards[card_testedA].removed = true;
				cards[card_testedA].animation_mode = Card::ANIM_MODE_REMOVED;
				cards[card_testedB].animation_mode = Card::ANIM_MODE_REMOVED;
				cards[card_testedA].animation_start = Render::GetTime();
				cards[card_testedB].animation_start = Render::GetTime();
			//	std::cout << card_testedA << " " << card_testedB << std::endl;
				card_testedB = -1;
				card_testedA = -1;
				pairs_left -= 1;
				if(pairs_left == 0){
					game_finish_time = Render::GetTime();
					game_state = GAME_STATE_FINISHED;
				}
			}else{
				// Cards are not matching. Let the player see them for a while
				cards[card_testedA].animation_mode = Card::ANIM_MODE_WAIT;
				cards[card_testedB].animation_mode = Card::ANIM_MODE_WAIT;
				cards[card_testedA].animation_start = Render::GetTime();
				cards[card_testedB].animation_start = Render::GetTime();
			}
		}else{
			std::cerr << "ERROR: This card was is not being tested, it shold not be uncovered!" << std::endl;
		}
	}else if(cards[n].animation_mode == Card::ANIM_MODE_WAIT){
		// Card has waited, time to cover it (and the other tested one too).
		cards[card_testedA].uncovered = false;
		cards[card_testedB].uncovered = false;
		// Start covering these cards.
		cards[card_testedA].animation_mode = Card::ANIM_MODE_COVER;
		cards[card_testedB].animation_mode = Card::ANIM_MODE_COVER;
		cards[card_testedA].animation_start = Render::GetTime();
		cards[card_testedB].animation_start = Render::GetTime();
	}else if(cards[n].animation_mode == Card::ANIM_MODE_COVER){
		// Cards covered. Stop their animation
		cards[card_testedA].animation_mode = -1;
		cards[card_testedB].animation_mode = -1;
		card_testedA = -1;
		card_testedB = -1;
	}else if(cards[n].animation_mode == Card::ANIM_MODE_REMOVED){
		// Cards removed. Stop their animation
		cards[n].animation_mode = -1;
		cards[n].animation_mode = -1;
	}
}
