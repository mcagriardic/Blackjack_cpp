#include "setStates.h"

extern BlackJack* blackjack;

// stateTag
// stateTransitions 
// there should at most 1 transition, i need to add a nullptr

State setInitialiseState() {
	return State(
		"initialise",
		{
			Transition("none", "dealing", nullptr)
		},
		nullptr
	);
}

State setDealingState()
{
	return State(
		"dealing",      
		{                
			Transition("dealt", "win", []() {return blackjack->isAnyParticipant21(); }),
			Transition("dealt", "playerTurn", []() {return !(blackjack->isAnyParticipant21());})
		},
		[]() {blackjack->onEnterState_dealing();}
	);
}

State setplayerTurnState()
{
	return State(
		"playerTurn",
		{
			Transition("hit", "win", []() {return blackjack->getCurrentPlayerScore() == 21; }, []() {blackjack->setisWinner(true); }),
			Transition("hit", "playerTurn", []() {return blackjack->getCurrentPlayerScore() < 21; }),
			Transition("stand", "dealerTurn", nullptr, []() {blackjack->setActivePlayer(blackjack->getNextPlayer()); }),
			Transition("hit", "loss", []() {return blackjack->getCurrentPlayerScore() > 21; }, []() {blackjack->setcanPlay(false); })
		},
		[]() {blackjack->onEnterState_playerTurn(); }
	);
}

State setdealerTurnState()
{
	return State(
		"dealerTurn",
		{
			Transition("hit", "loss", []() {return blackjack->getCurrentPlayerScore() > 21; }, []() {blackjack->setcanPlay(false); }),
			Transition("hit", "dealerTurn", []() {return blackjack->getCurrentPlayerScore() < 21; }),
			Transition("stand", "loss", []() {return blackjack->dealerHasLowestScore(); }, []() {blackjack->setcanPlay(false); }),
			Transition("stand", "win", []() {return blackjack->dealerHasHighestScore(); }, []() {blackjack->setisWinner(true); }),
			Transition("stand", "standOff", []() {return blackjack->dealerAndPlayersHasSameScore(); }),
			Transition("hit", "win", []() {return blackjack->getCurrentPlayerScore() == 21; }, []() {blackjack->setisWinner(true); })
		},
		[]() {blackjack->onEnterState_dealerTurn(); }
	);
}

State setlossState()
{
	return State(
		"loss",
		{
			Transition("replay", "restart", nullptr)
		},
		[]() {blackjack->onEnterState_loss(); }
	);
}

State setstandOffState()
{
	return State(
		"standOff",
		{
			Transition("replay", "restart", nullptr)
		},
		[]() {blackjack->onEnterState_standOff(); }
	);
}

State setwinState()
{
	return State(
		"win",
		{
			Transition("replay", "restart", nullptr)
		},
		[]() {blackjack->onEnterState_win(); }
	);
}

State setrestartState()
{
	return State(
		"restart",
		{
			Transition("yes", "dealing", nullptr),
			Transition("no", "quit", nullptr)
		},
		[]() {blackjack->onEnterState_restart(); }
	);
}

State setquitState()
{
	return State(
		"quit",
		{},
		nullptr
	);
}
