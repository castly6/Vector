#include <iostream>
#include <iomanip>
#include <type_traits>
#include <stdexcept>
#include <random>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>

#include "matrix.h"


/////////////////////////////////////////////////////////
//функция вывода всех элементов матрицы
template<typename T>
void out_matrix(std::ostream& stream, const Matrix<T>& matrix)
{
	for (size_t i = 0; i<matrix.getHeight(); ++i){
		for (size_t j = 0; j<matrix.getWidth(); ++j){
			stream << std::setw(4 + std::is_floating_point<T>::value * 2) << std::fixed << std::setprecision(2) << matrix[i][j];
		}
		stream << std::endl;
	}
}


/////////////////////////////////////////////////////////
//функция вывода матрицы, повернутой на 45 градусов
template<typename T>
void out_rotate_matrix(std::ostream& stream, const Matrix<T>& matrix)
{
	out_matrix(stream, matrix.getRotate45());
}


//////////////////////////////////////////////////////////
//функция вывода элементов матрицы, заданных переданными координатами
template<typename T>
void out_elements(std::ostream& stream, const Matrix<T>& matrix, const std::vector< std::pair<size_t, size_t> >& indexes)
{
	for (std::vector< std::pair<size_t, size_t> >::const_iterator it = indexes.begin(); it != indexes.end(); ++it){
		stream << "[" << it->first << ", " << it->second << "] = " << matrix[it->first][it->second] << std::endl;
	}
}


//////////////////////////////////////////////////////////
//Функция генерации псевдо-случайного значения
template<typename T>
T get_random(T min, T max)
{
	//Используемый генератор
	//Семя инициализируем текущим временем
	static std::default_random_engine generator(time(0));
	//проверка параметра шаблона на этапе компиляции
	//если T - целочисленный тип, то
	//distribution_type будет псевдонимом типа std::uniform_int_distribution<T>
	//в ином случае
	//distribution_type будет псевдонимом типа std::uniform_real_distribution<T>
	typedef typename std::conditional< std::is_integral<T>::value, std::uniform_int_distribution<T>, std::uniform_real_distribution<T> >::type distribution_type;
	//создаем равномерное распределение с ограницением значения в интервале
	//[min, max - 1) для целочисленных значений
	//и [min, max - 0) для значений с плавающей точкой
	distribution_type distr(min, max - std::is_integral<T>::value);
	return distr(generator);//генерируем значение и возвращаем его
}



//////////////////////////////////////////////////////////
//Заполнение матрицы случайными числами
template<typename T>
void fill_random_matrix(Matrix<T>& matrix, typename Matrix<T>::value_type min, typename Matrix<T>::value_type max)
{
	//проходим по всей матрице
	for (size_t i = 0; i<matrix.getHeight(); ++i){
		for (size_t j = 0; j<matrix.getWidth(); ++j){
			matrix[i][j] = get_random(min, max);//генерируем число в интервале [min, max)
		}
	}
}


////////////////////////////////////////////////////////
//функция вычисления Евклидового расстояния между точками
double point_distance(const std::pair<size_t, size_t>& f, const std::pair<size_t, size_t>& s)
{
	size_t dh = std::max(f.first, s.first) - std::min(f.first, s.first);//разница координат по y
	size_t dw = std::max(f.second, s.second) - std::min(f.second, s.second);//разница координат по x
	return sqrt(dh*dh + dw*dw);//Вычисление расстояния
}


/////////////////////////////////////////////////////////
//функция поиска элементов на диагонали, отличных от заданных элементов не более, чем на E
template<typename T>
std::vector< std::pair<size_t, size_t> > find_range_e(const Matrix<T>& matrix, typename Matrix<T>::value_type E, const std::vector<typename Matrix<T>::value_type>& values)
{
	std::vector< std::pair<size_t, size_t> > result;
	for (size_t i = 0; i<matrix.getHeight() && i<matrix.getWidth(); ++i){//Проходим по главной диагонали
		for (size_t v = 0; v<values.size(); ++v){//Проходим по набору значений
			if (!(abs(matrix[i][i] - values[v]) > E)){//Если разница между значением в наборе и значением элемента матрицы не больше E, то
				result.push_back(std::make_pair(i, i));//Добавляем в вектор координаты найденного элемента
				break;//Останавливаем цикл по набору значений
			}
		}
	}
	return result;
}


////////////////////////////////////////////////////////////
//функция рассчета и вывода расстояний между точками двух наборов
void out_distances(std::ostream& stream, const std::vector< std::pair<size_t, size_t> >& f_points, const std::vector< std::pair<size_t, size_t> >& s_points)
{
	//Проходим по всем точкам в обоих контейнерах
	for (size_t i = 0; i<f_points.size(); ++i){
		for (size_t j = 0; j<s_points.size(); ++j){
			stream << "For "
				<< "[" << f_points[i].first << ", " << f_points[i].second << "] and "
				<< "[" << s_points[j].first << ", " << s_points[j].second << "] "
				//считаем дистанцию и выводим
				<< "distance equal " << point_distance(f_points[i], s_points[j]) << std::endl;
		}
	}
}


////////////////////////////////////////////////////////////
//функция обработки матрицы
template<typename T>
void process_func(std::ostream& stream, size_t Height, size_t Width, T E)
{
	//Если высота или ширина не заданы (равны нулю),
	//то генерируем их автоматически в интервале [3, 8)
	size_t M = (Height == 0) ? rand() % 5 + 3 : Height;
	size_t N = (Width == 0) ? rand() % 5 + 3 : Width;
	Matrix<T> matrix(M, N);//Создаем матрицу заданного размера

	fill_random_matrix(matrix, 10, 100);//Заполняем матрицу значенями в интервале [10, 100)

	stream << "Matrix[" << M << ", " << N << "]; " << "E = " << E << ":\n";
	out_matrix(stream, matrix);//Выводим матрицу в поток

	//Ищем координаты всех экстремумов
	//тип переменной std::vector< std::pair<size_t, size_t> >
	auto extremums = matrix.find_extremums();

	//В extremums не может быть менее двух элементов
	//Гарантировано:
	//Первый элемент в extremums всегда минимум
	//Последний элемент - всегда максимум
	auto min = matrix[extremums.front().first][extremums.front().second];
	auto max = matrix[extremums.back().first][extremums.back().second];


	stream << "\n\nExtremums:\n";
	out_elements(stream, matrix, extremums);//Выводим экстремумы в поток

	//Ищем на главной диагонали элементы, отличные от максимального или минимального не более, чем на E
	auto ranges = find_range_e(matrix, E, { min, max });

	stream << "\nRanges:\n";
	out_elements(stream, matrix, ranges);

	stream << "\nDistances:\n";
	//Выводим в поток расстояния между элементами, по принципу "каждый с каждым"
	out_distances(stream, ranges, extremums);

	stream << "\nRotate:\n";
	//Выводим повернутую на 45 градусов матрицу
	out_rotate_matrix(stream, matrix);
}


////////////////////////////////////////////////////////////
//Ручной выбор параметров
void manual()
{
	size_t M = 0;
	size_t N = 0;
	char ch = 0;
	std::string file_name;//имя файла для сохранения результата
	std::cout << "Input output file->";
	std::getline(std::cin >> std::ws, file_name);//запрос имени файла
	std::ofstream o_file(file_name.c_str());//Создаем файловый поток
	if (!o_file.is_open()){//Если файл открыть не удалось, то
		std::cout << "File is not open" << std::endl;//Выводим сообщение об ошибке
		return;//и выходим из функции
	}
	std::cout << "Input matrix size ->";
	std::cin >> M >> N;//запрашиваем размер матрицы
	std::cout << "Choose element type (1 - double, other - int) ->";
	std::cin >> ch;//запрашиваем тип элементов
	if (ch == '1'){//Если выбрали тип double, то
		double E = 0.0;
		std::cout << "Input E ->";
		std::cin >> E;//запрашиваем E
		process_func(o_file, M, N, E);//вызываем функцию обработки с ользовательскими параметрами
	}
	else{
		int E = 0.0;
		std::cout << "Input E ->";
		std::cin >> E;
		process_func(o_file, M, N, E);
	}
}


////////////////////////////////////////////////////////////
//Функция автотеста
void autotest()
{
	std::cout << "Input count tests->";
	size_t count = 0;
	std::cin >> count;//Запрос количества тестов
	for (size_t i = 0; i<count; ++i){
		std::cout << "Test №" << (i + 1) << std::endl;
		//Вызываем функцию обработки с сгенерированными параметрами
		process_func(std::cout, get_random(3, 7), get_random(5, 8), get_random(5, 30));
		std::cout << "end test " << (i + 1)
			<< "\n-------------------------------------------\n\n";
	}

}


////////////////////////////////////////////////////////////
//Функция вывода меню и обработки ответа
void menu()
{
	char ch;
	while (true){
		//Вывод меню
		std::cout << "Menu\n"
			<< "1 - Manual\n"
			<< "2 - Autotest\n"
			<< "Other - exit"
			<< std::endl;
		std::cout << "->";
		std::cin >> ch;//Запрос ответа
		switch (ch){//Если
		case '1'://выбрали 1, то
			manual();//вызываем ручной способ задания параметров
			break;
		case '2'://выбрали 2
			autotest();//вызываем автотест
			break;
		default://в ином случае
			return;//выходим из функции
		}
	}

}





//Функции тестирования и демонстрации использования отдельных функций
namespace samples
{


	template<typename T>
	Matrix<T> generate_random_matrix()
	{
		Matrix<T> matrix(get_random(2, 6), get_random(2, 6));
		fill_random_matrix(matrix, 10, 100);
		return matrix;
	}


	template<typename T>
	std::vector<T> get_main_diag(const Matrix<T>& matrix)
	{
		std::vector<T> result;
		for (size_t i = 0; i<matrix.getHeight() && i<matrix.getWidth(); ++i){
			result.push_back(matrix[i][i]);
		}
		return result;
	}


	template<typename T>
	void out_main_diag(const Matrix<T>& matrix)
	{
		auto vec = get_main_diag(matrix);
		for (auto&& e : vec){
			std::cout << e << ' ';
		}
		std::cout << std::endl;
	}


	template<typename T>
	Matrix<T> generate_random_and_out()
	{
		Matrix<T> matrix(generate_random_matrix<T>());
		std::cout << "Matrix:\n";
		out_matrix(std::cout, matrix);
		std::cout << "Main:\n";
		out_main_diag(matrix);
		return matrix;
	}


	void out_points_dist(size_t x1, size_t y1, size_t x2, size_t y2)
	{
		std::cout << "Points [" << x1 << ", " << y1 << "] - [" << x2 << ", " << y2 << "] distance: "
			<< point_distance(std::make_pair(x1, y1), std::make_pair(x2, y2)) << std::endl;
	}


	template<typename T>
	void out_extremums(const Matrix<T>& matrix)
	{
		auto vec = matrix.find_extremums();
		std::cout << "Extremums:\n";
		out_elements(std::cout, matrix, vec);
	}


	template<typename T>
	void out_maximums(const Matrix<T>& matrix)
	{
		auto vec = matrix.find_max_elements();
		std::cout << "Maxs:\n";
		out_elements(std::cout, matrix, vec);
	}


	template<typename T>
	void out_minimums(const Matrix<T>& matrix)
	{
		auto vec = matrix.find_min_elements();
		std::cout << "Mins:\n";
		out_elements(std::cout, matrix, vec);
	}


}//end of test namespace







////////////////////////////////////////////////////////////
int main()
{
	menu();//Вызов меню
	/*
	//Тестирование отдельных возможностей
	samples::generate_random_and_out<int>();

	samples::out_points_dist(1, 1, 1, 3);
	samples::out_points_dist(0, 3, 4, 2);
	samples::out_points_dist(5, 2, 0, 3);

	std::cout << "For m1 mins test:" << std::endl;
	Matrix<int> m1 = samples::generate_random_and_out<int>();
	samples::out_minimums(m1);

	std::cout << "For m2 maxs test:" << std::endl;
	Matrix<int> m2 = samples::generate_random_and_out<int>();
	samples::out_maximums(m2);

	std::cout << "For m3 extr test:" << std::endl;
	Matrix<double> m3 = samples::generate_random_and_out<double>();
	samples::out_extremums(m3);
	*/
}
