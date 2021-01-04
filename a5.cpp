#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
using namespace std;

//Display the chess map
void display(vector<int> temp)
{
	cout << "Current Reversi:\n";
	int sizeIs = size(temp);
	for (int i = 0; i < sizeIs; i++)
	{
		if (i % 8 == 0)
			cout << "|";

		if (temp[i] == 0)
			cout << "O|";
		else if (temp[i] == 1)
			cout << "*|";
		else
			cout << " |";

		if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			cout << endl;
	}
	cout << endl;
}

//Return 0: computer wins, 1: player wins, 2: both draw
int result(vector<int> temp)
{
	int win = -1;
	int countCom = 0;
	int countPlayer = 0;
	for (int i = 0; i < 64; i++)
	{
		if (temp[i] == 0)
			countCom++;
		else if (temp[i] == 1)
			countPlayer++;
	}
	if (countCom > countPlayer)
		win = 0;
	else if (countCom < countPlayer)
		win = 1;
	else
		win = 2;

	return win;
}

//Reverse all the relevent chesses in the position and check this position is legal
int inverse(int index, vector<int>& temp)
{
	int legal = 0;
	int chessman = index - 1;
	int player = temp[chessman];
	int row = chessman / 8 + 1;
	int column = chessman % 8 + 1;

	// Change in colum
	for (int i = chessman - 8; i >= 0; i-=8)
	{
		if (temp[i] == -1 || temp[chessman - 8] == temp[chessman])
			break;
		if (temp[i] == temp[chessman])
		{
			for (int j = i; j < chessman; j += 8)
				temp[j] = player;
			legal = 1;
			break;
		}
	}
	for (int i = chessman + 8; i < 64; i += 8)
	{
		if (temp[i] == -1 || temp[chessman + 8] == temp[chessman])
			break;
		if (temp[i] == temp[chessman])
		{
			for (int j = i; j > chessman; j -= 8)
				temp[j] = player;
			legal = 1;
			break;
		}
	}

	// Change in row
	for (int i = chessman + 1; i < row * 8; i++)
	{
		if (temp[i] == -1 || temp[chessman + 1] == temp[chessman])
			break;
		if (temp[i] == temp[chessman])
		{
			for (int j = i; j > chessman; j--)
				temp[j] = player;
			legal = 1;
			break;
		}
	}
	for (int i = chessman - 1; i >= (row - 1) * 8; i--)
	{
		if (temp[i] == -1 || temp[chessman - 1] == temp[chessman])
			break;
		if (temp[i] == temp[chessman])
		{
			for (int j = i; j < chessman; j++)
				temp[j] = player;
			legal = 1;
			break;
		}
	}

	//Change in diagonal
	//up left
	int previous = chessman;
	for (int i = chessman - 9; i > 0; i-= 9)
	{
		if (temp[i] == -1 || (i / 8 + 1) < (previous / 8 + 1) - 1 || temp[chessman - 9] == temp[chessman])
			break;

		if (temp[i] == temp[chessman])
		{
			for (int j = i; j < chessman; j += 9)
				temp[j] = player;
			legal = 1;
			break;
		}
		previous = i;
	}
	previous = chessman;
	//up right
	for (int i = chessman - 7; i > 0; i -= 7)
	{
		if (temp[i] == -1 || (i / 8 + 1) == (previous / 8 + 1) || temp[chessman - 7] == temp[chessman])
			break;
		if (temp[i] == temp[chessman])
		{
			for (int j = i; j < chessman; j += 7)
				temp[j] = player;
			legal = 1;
			break;
		}
		previous = i;
	}
	previous = chessman;
	//down left
	for (int i = chessman + 7; i < 64; i += 7)
	{
		if (temp[i] == -1 || (i / 8 + 1) == (previous / 8 + 1) || temp[chessman + 7] == temp[chessman])
			break;
		if (temp[i] == temp[chessman])
		{
			for (int j = i; j > chessman; j -= 7)
				temp[j] = player;
			legal = 1;
			break;
		}
		previous = i;
	}
	previous = chessman;
	//down right
	for (int i = chessman + 9; i < 64; i += 9)
	{
		if (temp[i] == -1 || (i / 8 + 1) > (previous / 8 + 1) + 1 || temp[chessman + 9] == temp[chessman])
			break;
		if (temp[i] == temp[chessman])
		{
			for (int j = i; j > chessman; j -= 9)
				temp[j] = player;
			legal = 1;
			break;
		}
		previous = i;
	}
	return legal;
}

//Check the current chess map is legal for player or computer
bool checkLegal(vector<int> play1, int turn)
{
	for (int i = 0; i < 64; i++)
	{
		vector<int> play(play1);
		if (play[i] == -1)
		{
			play[i] = turn;
			if (inverse(i + 1, play) == 1)
			{
				return true;
			}
			play[i] = -1;
		}
	}
	return false;
}

//child function of playouts
int playout(vector<int> &temp, int &grade)
{
	srand(time(NULL));
	int times = 1;
	while (1)
	{
		vector<int> temp1;
		for (int i = 0; i < 64; i++)
		{
			if (temp[i] == -1)
			{
				vector<int> play(temp);
				play[i] = times % 2;
				if (inverse(i + 1, play) == 1)
				{
					temp1.push_back(i);
				}
			}
		}
		if (size(temp1) > 0)
		{
			int random = rand() % (size(temp1));
			temp[temp1[random]] = times % 2;
			inverse(temp1[random] + 1, temp);
			times++;
			if (checkLegal(temp, 1) == false && checkLegal(temp, 0) == false)
			{
				int res = result(temp);
				if (res == 0 || res == 2)
					return ++grade;
				else
					return --grade;
			}
		}
		else
		{
			if (checkLegal(temp, (times + 1) % 2) == false)
				return ++grade;
			else
				times++;
		}
	}
}

//Struct that has two variables
struct use
{
	int one, two;
};

//Main playouts function (100 times random playout)
void playouts(vector<int>& temp)
{
	int count = 0;
	vector<use>determine;
	for (int i = 0; i < 64; i++)
	{
		if (temp[i] == -1)
		{
			count++;
			determine.push_back({ i, -1 });
		}
	}
	int iter1 = 0;
	while (iter1 < count)
	{
		int wad = 0;
		vector<int> temp1(temp);
		temp1[determine[iter1].one] = 0;
		if (inverse(determine[iter1].one + 1, temp1) == 1)
		{
			for (int i = 0; i < 100; i++)
			{
				vector<int> temp1(temp);
				temp1[determine[iter1].one] = 0;
				wad = playout(temp1, wad);
			}
		}
		determine[iter1].two = wad;
		iter1++;
	}

	int finalres = 0;
	int comp = -10000;
	for (int i = 0; i < size(determine); i++)
	{
		if (determine[i].two > comp)
		{
			comp = determine[i].two;
			finalres = determine[i].one;

		}
	}

	temp[finalres] = 0;
	inverse(finalres+1, temp);
	cout << "Computer Turen: Position " << finalres + 1 << endl;
	display(temp);
}


int main()
{
	cout << "Index of Reversi:\n";
	for (int i = 0; i < 64; i++)
	{
		if (i < 9)
			cout << i + 1 << ' ' << '|';
		else
			cout << i + 1 << '|';

		if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55 || i == 63)
			cout << endl;
	}
	cout << "Your chess will be black -- *, and go first."<< endl;

	vector<int> play(64, -1);
	play[27] = 0;
	play[28] = 1;
	play[35] = 1;
	play[36] = 0;
	display(play);
	int game = 0;
	int win = -1;
	cout << inverse(1, play) << endl;
	while (game == 0)
	{
		if (checkLegal(play, 1))
		{
			bool enter = false;
			while (enter == false)
			{
				int playposition;
				cout << "Please enter position you want to put(1 to 64), with integer: ";
				cin >> playposition;
				vector<int> temp(play);
				temp[playposition - 1] = 1;
				if (play[playposition - 1] != -1)
					cout << "Cannot swap an existing chess, please enter again\n";
				else if (inverse(playposition, temp) == 0)
					cout << "Not a legal position, please enter again\n";
				else
				{
					play[playposition - 1] = 1;
					inverse(playposition, play);
					enter = true;
					display(play);
				}
			}
		}
		else
			cout << "No legal position\n";

		if (checkLegal(play, 0))
		{
			//check the overall playouts time
			clock_t start, end;
			start = clock();

			playouts(play);

			end = clock();
			double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
			cout << "Computer used : " << fixed
				<< time_taken << " sec " << endl;

			cout << "Your turn:\n";
		}

		if (checkLegal(play, 1) == false && checkLegal(play, 0) == false)
		{
			win = result(play);
			game = 1;
		}
	}

	if (win == 0)
		cout << "Computer wins!\n";
	else if (win == 1)
		cout << "You win!\n";
	else
		cout << "You draw!\n";

	system("pause");
	return 0;
}