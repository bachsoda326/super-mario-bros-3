#pragma once
#include <vector>

using std::vector;

class CPlayerInfo
{
	static CPlayerInfo* __instance;

	int life;
	int money;
	int score;
	int form;
	vector<int> cards;

public:
	CPlayerInfo();

	int GetLife() { return life; }
	int GetMoney() { return money; }
	int GetScore() { return score; }
	int GetForm() { return form; }
	vector<int> GetCards() { return cards; }

	void AdjustLife(int num) { life += num; }
	void AdjustMoney(int num) { money += num; }
	void AdjustScore(int num) { score += num; }
	void SetForm(int level) { form = level; }
	void AddCard(int card) { cards.push_back(card); }
	void ClearCard() { cards.clear(); }

	static CPlayerInfo* GetInstance();
};

