#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
using namespace std;

int main()
{
	int col, row, E;
	cin >> col >> row >> E;
	std::vector<std::vector<int> >array(col, std::vector<int>(row));
	for (int y = 0; y<row; ++y)
	for (int x = 0; x<col; ++x)
		array[x][y] = rand()%20;

	for (int y = 0; y<row; ++y)
	{
		for (int x = 0; x<col; ++x)
			cout << array[x][y] << " ";
		cout << endl;
	}

	for (int i = 0; i<x; ++x)
	for (int j = 0; j<y / 2; ++y)
		std::swap(array[x][y], a[i][m - j - 1]);

	for (int i = 0; i<n; ++i)
	{
		for (int j = 0; j<m; ++j)
			std::cout << a[i][j] << " ";
		std::cout << std::endl;
	}

	system("pause");
	return 0;

}
