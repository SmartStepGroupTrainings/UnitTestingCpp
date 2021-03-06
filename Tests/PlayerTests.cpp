#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Player.h>
#include <RollDiceGame.h>

using namespace ::testing;
using ::testing::Return;

class DiceStub : public IDice {
public:
	MOCK_METHOD0(Roll, int());
};

class PlayerCan : public ::testing::Test
{
protected: 
	Player player;
	RollDiceGame CreateRollDiceGameWithWinningScore(int winningScore) {
		std::unique_ptr<NiceMock<DiceStub>> diceStub(new NiceMock<DiceStub>());
		ON_CALL(*(diceStub.get()), Roll()).WillByDefault(Return(winningScore));
		RollDiceGame game(std::unique_ptr<IDice>(diceStub.release()));
		return game;
	}
};

TEST_F(PlayerCan, BuyChips)
{
	player.AddChips(100);

	ASSERT_EQ(100, player.GetChips());
}

TEST_F(PlayerCan, MakeBet) {
	player.AddChips(100);

	player.MakeBet(Bet(10, 6));

	ASSERT_EQ(10, player.GetCurrentBet().Amount());
	ASSERT_EQ(6, player.GetCurrentBet().Score());
}

TEST_F(PlayerCan, JoinGame) {
	player.AddChips(100);
	RollDiceGame game;
	
	game.Add(player);

	ASSERT_EQ(1, game.NumberOfPlayers());
}

TEST_F(PlayerCan, JoinGameWithOtherPlayers) {
	player.AddChips(100);
	RollDiceGame game;
	game.Add(Player());

	game.Add(player);

	ASSERT_EQ(2, game.NumberOfPlayers());
}

TEST_F(PlayerCan, LoseAGame) {
	player.AddChips(100);
	player.MakeBet(Bet(10, 1));
	RollDiceGame game = CreateRollDiceGameWithWinningScore(6);
	game.Add(player);

	game.Play();

	ASSERT_EQ(90, player.GetChips());
}
