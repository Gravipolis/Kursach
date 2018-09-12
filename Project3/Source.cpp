#include <Windows.h>
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

class TennisPlayer
{
public:
	string Country;
	string FIO;
	string date_of_birth;
	int winCount;
	int speed;
	float luck;

	void Show()
	{
		cout << Country << " " << FIO << " " << date_of_birth << " " << winCount << endl;
	}

	bool Equals(TennisPlayer p2)
	{
		return Country == p2.Country
			&& FIO == p2.FIO
			&& date_of_birth == p2.date_of_birth
			&& winCount == p2.winCount;
	}
};

class Result
{
public:
	int round;
	TennisPlayer player;
	int place;
	Result() {}
	Result(int round, TennisPlayer player, int place)
	{
		this->round = round;
		this->player = player;
		this->place = place;
	}
};

class Competition;
class IClient
{
public:
	virtual void handleEvent(const Competition&) = 0;
};

vector<vector<Result>> ResultTable = vector<vector<Result>>();

class Competition // Observable class
{

	list<IClient*> _observers;

	void _Notify()
	{
		for (list<IClient*>::iterator iter = _observers.begin(); iter != _observers.end(); ++iter)
		{
			(*iter)->handleEvent(*this);
		}
	}

public:
	vector<Result> results;
	int place;
	void add(IClient& ref)
	{
		_observers.push_back(&ref);
	}

	void remove(IClient& ref)
	{
		_observers.remove(&ref);
	}

	void start(vector<TennisPlayer> players, int look, int round)
	{
		results.clear();
		results = vector<Result>();
		Result  *res = new Result[players.size()];

		for (int i = 0; i < players.size(); i++)
		{
			res[i].player = players.at(i);
			res[i].round = round;
		}

		int numofchnges = 1;
		while (true)
		{
			if (numofchnges == 0) break;
			numofchnges = 0;

			for (int i = 0; i < players.size() - 1; i++)
			{
				TennisPlayer playerA = res[i].player;
				TennisPlayer playerB = res[i + 1].player;

				if (playerA.luck * playerA.speed < playerB.luck * playerB.speed)
				{
					Result r = res[i];
					res[i] = res[i + 1];
					res[i + 1] = r;
					numofchnges++;
				}
			}
		}

		for (int i = players.size() - 1; i >= 0; i--)
		{
			res[i].place = i + 1;
			if (res[i].player.Equals(players.at(look)))
				place = i + 1;
			results.push_back(res[i]);
		}

		ResultTable.push_back(results);

		_Notify();
	}

	void finish()
	{

	}
};

class Client : public IClient // Prints the observed string into cout
{
public:
	virtual void handleEvent(const Competition& ref)
	{
		cout << "Player took " << ref.place << " place" << endl;


		if (ResultTable.size() >= 5)
		{
			Result *res = new Result[ResultTable.at(0).size()];
			for (int i = 0; i < ResultTable.at(0).size(); i++)
				res[i].player = ResultTable.at(0).at(i).player;

			for (auto r : ResultTable)
				for (int i = 0; i < r.size(); i++)
					res[i].place += r.at(i).place;

			int numofchnges = 1;
			while (true)
			{
				if (numofchnges == 0) break;
				numofchnges = 0;

				for (int i = 0; i < ResultTable.at(0).size() - 1; i++)
				{
					Result playerA = res[i];
					Result playerB = res[i + 1];

					if (playerA.place > playerB.place)
					{
						Result r = res[i];
						res[i] = res[i + 1];
						res[i + 1] = r;
						numofchnges++;
					}
				}
			}

			cout << endl;
			for (int i = 0; i < ResultTable.at(0).size(); i++) {
				ofstream f("Competition" + std::to_string(rand()));
				cout << i + 1 << " place" << " player " << res[i].player.FIO << ", " << res[i].player.Country << endl;
				f << i + 1 << " place" << " player " << res[i].player.FIO << ", " << res[i].player.Country << endl;
			}

			cout << endl;
		}
	}
};

class BigTennis {
public:
	vector<TennisPlayer> players = vector<TennisPlayer>();
	BigTennis()
	{
		ifstream ofstr;
		ofstr.open("input.txt");
		int count = 0;
		ofstr >> count;
		for (int i = 0; i < count; i++)
		{
			TennisPlayer player;
			ofstr >> player.Country;
			string F, I, O;
			ofstr >> F;
			ofstr >> I;
			ofstr >> O;
			player.FIO = F + " " + I + " " + O;
			ofstr >> player.date_of_birth;
			ofstr >> player.winCount;
			players.push_back(player);
		}
	}

	void StartCompetition()
	{
		cout << "Selet player to watch:" << endl;
		for (TennisPlayer p : players)
			p.Show();
		cout << endl;

		int index;
		cin >> index;

		Competition comp;
		Client cl;

		comp.add(cl);

		for (int i = 0; i < 5; i++) {

			srand((i + 1)*time(0));
			for (int i = 0; i < players.size(); i++)
			{
				players.at(i).speed = rand() % 50;
				players.at(i).luck = (rand() % 200) / 100.0f;
			}
			comp.start(players, index, i);
		}

		comp.finish();

	}
};

int main()
{
	BigTennis tennis = BigTennis();

	while (1)
	{
		cout <<
			"1-start" << endl
			<< "2-add player" << endl
			<< "3-exit" << endl;
		int menu = 0;
		cin >> menu;
		if (menu == 3) break;
		if (menu == 1)tennis.StartCompetition();
		if (menu == 2)
		{
			TennisPlayer player;
			cout << "input country" << endl;
			cin >> player.Country;
			string F, I, O;
			cout << "input F" << endl;
			cin >> F;
			cout << "input I" << endl;
			cin >> I;
			cout << "input O" << endl;
			cin >> O;
			player.FIO = F + " " + I + " " + O;
			cout << "input date of birth" << endl;
			cin >> player.date_of_birth;
			cout << "input number of wins" << endl;
			cin >> player.winCount;
			tennis.players.push_back(player);
		}
		ResultTable.clear();
		ResultTable = vector<vector<Result>>();
	}
	system("pause");
	return 0;
}