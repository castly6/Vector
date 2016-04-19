#ifndef MATRIX_HPP
#define MATRIX_HPP



#include <vector>
#include <algorithm>
#include <utility>


//Класс матрица
template<typename T>
class Matrix
{
public:
	//Псевдонимы используемых типов
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef const T& const_reference;
	typedef const T* const_pointer;
	typedef std::vector<value_type> rows_type;
	typedef std::vector< rows_type > container_type;
	typedef size_t size_type;


	/////////////////////////////////////////////////////////
	//конструктор
	//принимает в качестве параметров размеры матрицы
	Matrix(size_type height, size_type width)
		: m_matrix(height, rows_type(width, value_type()))
		, m_width(width)
		, m_height(height)
	{
	}


	/////////////////////////////////////////////////////////
	//функция-член, возвращающая ширину матрицы
	size_type getWidth() const
	{
		return m_width;
	}


	/////////////////////////////////////////////////////////
	//функция-член, возвращающая высоту матрицы
	size_type getHeight() const
	{
		return m_height;
	}


	/////////////////////////////////////////////////////////
	//не константная операторная функция-член перегруженного оператора []
	//возвращает указатель на "строку матрицы"
	pointer operator[](size_type height)
	{
		return &(m_matrix[height][0]);
	}


	/////////////////////////////////////////////////////////
	//константная версия перегруженного оператора []
	const_pointer operator[](size_type height) const
	{
		//снимаем константность с this и вызываем не константную версию,
		//тем самым мы избавляемся от дублирования кода и
		//избегаем необходимости править код в двух местах при правках
		return const_cast<Matrix*>(this)->operator[](height);
	}


	/////////////////////////////////////////////////////////
	//Функция поиска индексов максимальных элементов
	//Возвращает вектор с парами значений, где
	//first - вертикальная координата, а
	//second - горизонтальная координата
	std::vector< std::pair<size_t, size_t> > find_max_elements() const
	{
		T max = m_matrix[0][0];//Задаем элемент[0][0] как максимальный
		std::vector< std::pair<size_t, size_t> > result;//Вектор для результата
		//Проходим по всей матрице
		for (size_t i = 0; i<m_height; ++i){
			for (size_t j = 0; j<m_width; ++j){
				//Если элемент больше максимума или равен ему,
				if (m_matrix[i][j] >= max){//то
					if (m_matrix[i][j] != max){//если элемент не равен максимальному,
						//значит он больше максимального,
						//а значит
						max = m_matrix[i][j];//обновляем максимальных элемент
						result.clear();//и очищаем вектор с результатами
					}
					result.push_back(std::make_pair(i, j));//Добавляем координаты элемента в вектор результатов
				}
			}
		}
		return result;//возвращаем вектор
	}



	/////////////////////////////////////////////////////////
	//Функция поиска индексов минимальных элементов
	//Возвращает вектор с парами значений, где
	//first - вертикальная координата, а
	//second - горизонтальная координата
	std::vector< std::pair<size_t, size_t> > find_min_elements() const
	{
		//Всё аналогично поиску максимальных элементов
		T min = m_matrix[0][0];
		std::vector< std::pair<size_t, size_t> > result;
		for (size_t i = 0; i<m_height; ++i){
			for (size_t j = 0; j<m_width; ++j){
				if (m_matrix[i][j] <= min){//кроме знака в этом условии
					if (m_matrix[i][j] != min){
						min = m_matrix[i][j];
						result.clear();
					}
					result.push_back(std::make_pair(i, j));
				}
			}
		}
		return result;
	}


	/////////////////////////////////////////////////////////
	//Функция поиска всех экстремумов
	//Возвращает вектор с парами значений, где
	//first - вертикальная координата, а
	//second - горизонтальная координата
	std::vector< std::pair<size_t, size_t> > find_extremums() const
	{
		std::vector< std::pair<size_t, size_t> > result(find_min_elements());//Ищем координаты минимальных элементов
		std::vector< std::pair<size_t, size_t> > maxs(find_max_elements());//Ищем кооринаты максимальных элементов
		result.insert(result.end(), maxs.begin(), maxs.end());//Добавляем координаты максимальных элементов в вектор с коодринатами минимальных элементов
		return result;
	}


	//Функция поворота матрицы на 45 градусов
	//Возвращает повернутую матрицу
	Matrix getRotate45() const
	{
		size_t new_size = m_height + m_width - 1;//Новая матрица будет квадратной её размер равен суммарному размеру обоих матриц, уменьшенному на единицу
		Matrix new_matrix(new_size, new_size);//Создаем новую матрицу заданного размера
		size_t Fst = m_height - 1;//Смещение по горизонтали от начала матрицы. Изначально равно высоте исходной матрицы уменьшеной на единицу
		//проходим по всей матрице
		for (size_t i = 0; i < m_height; ++i){
			for (size_t j = 0; j < m_width; ++j){
				//В новую матрицу записываем значение элементов,
				//оставляя пропуски
				new_matrix[i + j][Fst + j] = m_matrix[i][j];
			}
			--Fst;//уменьшаем смещение на единицу
		}
		return new_matrix;//возвращаем результат
	}



private:
	container_type m_matrix;//матрица
	//кешированные значения высоты и ширины матрицы
	size_type m_width;
	size_type m_height;
};




#endif // MATRIX_HPP
