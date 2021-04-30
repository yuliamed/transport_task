#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <msclr/marshal_cppstd.h>
using namespace std;


namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;//для работы с файлами
	vector <int> needs;
	vector <int> stocks;
	//int rows, columns;
	vector <vector<int> > value_of_trans;
	vector<vector <int>> plan;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ rows_text_box;
	protected:

	protected:

	protected:
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ columns_text_box;


	private: System::Windows::Forms::DataGridView^ shopsGrid;
	private: System::Windows::Forms::DataGridView^ stocksGrid;
	private: System::Windows::Forms::DataGridView^ valueOfTransGrid;




	private: System::Windows::Forms::TextBox^ textBox3;


	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ dataGridViewTextBoxColumn1;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
		   void create_problem_close(int& difference, int& rows, int& columns)
		   {

			   int sum_n = 0, sum_s = 0;
			   for (int i = 0; i < needs.size(); i++)
				   sum_n += needs[i];
			   for (int i = 0; i < stocks.size(); i++)
				   sum_s += stocks[i];
			   difference = sum_s - sum_n;
			   if (difference == 0) textBox3->Text = "Транспортная задача закрытая";
			   else
			   {
				   textBox3->Text = "Транспортная задача является открытой";
				   //дописывание строки для закрытости проги
				   if (difference > 0)
				   {
					   columns++;
					   needs.push_back(difference);
					   for (int i = 0; i < rows; i++)
					   {
						   value_of_trans[i].push_back(0);
					   }
				   }
				   else
				   {
					   rows++;
					   stocks.push_back(-difference);
					   vector<int> nul(columns, 0);
					   value_of_trans.push_back(nul);
				   }
			   }
		   }
		   void transform_line_into_vector(vector<int>& stocks, string read_str)
		   {
			   string delim(" ");
			   size_t prev = 0;
			   size_t next;
			   size_t delta = delim.length();
			   while ((next = read_str.find(delim, prev)) != string::npos)
			   {
				   string tmp = read_str.substr(prev, next - prev);

				   int number = atoi(tmp.c_str());
				   stocks.push_back(number);
				   prev = next + delta;
			   }
			   stocks.push_back(atoi((read_str.substr(prev)).c_str()));
		   }
		   bool is_v_empty(vector <int>v)
		   {
			   for (int i = 0; i < v.size(); i++)
			   {
				   if (v[i] == 0) continue;
				   else return false;
			   }
			   return true;
		   }
		   void fill_start_products(vector <vector<int>>& value_of_products_0)
		   {
			   int rows = stocks.size();
			   int columns = needs.size();
			   vector <vector<int>> value_of_products(rows, vector<int>(columns, -100));
			   while (!is_v_empty(needs) || !is_v_empty(stocks)) {
				   int i_min = 0, j_min = 0;
				   while (value_of_products[i_min][j_min] != -100)//нахождение клетки для сравнения с исключением того факта. что она может быть заполнена
				   {
					   if (i_min < stocks.size() - 1)
						   i_min++;
					   else
					   {
						   i_min = 0;
						   j_min++;
					   }
				   }
				   //поиск минимальной цены перевозки товара 
				   int min = value_of_trans[i_min][j_min];
				   for (int i = 0; i < rows; i++)
				   {
					   for (int j = 0; j < columns; j++)
					   {
						   if (min > value_of_trans[i][j] && value_of_trans[i][j] != 0 && value_of_products[i][j] == -100)
						   {
							   min = value_of_trans[i][j];
							   i_min = i;
							   j_min = j;
						   }
					   }
				   }
				   //записывание количество товара в таблицу 
				   //-1 - сюда товар не посталяется

				   int dif_product = stocks[i_min] - needs[j_min];
				   if (dif_product > 0)
				   {
					   value_of_products[i_min][j_min] = needs[j_min];
					   needs[j_min] = 0;
					   stocks[i_min] = dif_product;
					   for (int i = 0; i < rows; i++)
					   {
						   if (i != i_min && value_of_products[i][j_min] == -100)
							   value_of_products[i][j_min] = -1;
					   }
				   }
				   else if (dif_product < 0)
				   {
					   value_of_products[i_min][j_min] = stocks[i_min];
					   stocks[i_min] = 0;
					   needs[j_min] = -dif_product;
					   for (int j = 0; j < columns; j++)
					   {
						   if (j != j_min && value_of_products[i_min][j] == -100)
							   value_of_products[i_min][j] = -1;
					   }
				   }
				   else {
					   value_of_products[i_min][j_min] = stocks[i_min];
					   stocks[i_min] = 0;
					   needs[j_min] = 0;
					   for (int i = 0; i < rows; i++)
					   {
						   if (i != i_min && value_of_products[i][j_min] == -100)
							   value_of_products[i][j_min] = -1;
					   }
					   for (int j = 0; j < columns; j++)
					   {
						   if (j != j_min && value_of_products[i_min][j] == -100)
							   value_of_products[i_min][j] = -1;
					   }
				   }
			   }
			   value_of_products_0 = value_of_products;
		   }
		   void print_double_vector(vector <vector<int>>value_of_products)
		   {
			   int rows = stocks.size();
			   int columns = needs.size();
			   string str = "\r\n";
			   for (int i = 0; i < rows; i++)
			   {
				   for (int j = 0; j < columns; j++)
				   {
					   str += to_string(value_of_products[i][j]) + "\t";
					   //textBox3->Text=msclr::interop::marshal_as<String^>(value_of_products[i][j]);
				   }
				   str += "\r\n";
			   }
			   textBox3->Text += msclr::interop::marshal_as<String^>(str);
		   }
		   int count_add_basic_sells(vector <vector <int>> value_of_products)
		   {
			   int rows = stocks.size();
			   int columns = needs.size();
			   int count = 0;
			   for (int i = 0; i < rows; i++)
				   for (int j = 0; j < columns; j++)
				   {
					   if (value_of_products[i][j] != -1)
						   count++;
				   }
			   return rows + columns - 1 - count;
		   }
		   bool check_basic_sells(int min, vector <vector <int>> value_of_products, int i_0, int j_0, int rows, int columns)
		   {
			   int count_j = 0, count_i = 0;
			   for (int i = 0; i < rows; i++)
				   if (value_of_products[i][j_0] != -1) count_j++;
			   for (int j = 0; j < columns; j++)
				   if (value_of_products[i_0][j] != -1) count_i++;
			   if (count_j >= min || count_i >= min)
				   return false;
			   else return true;
		   }
		   void count_potentials(int add_basic_sells, vector <vector <int>>& value_of_products, vector <int>& stocks_potential, vector <int>& needs_potential)
		   {
			   int rows = stocks_potential.size();
			   int columns = needs_potential.size();
			   int min_basic_cells = 2;
			   vector<bool> bool_stocks_potential(rows, false);
			   vector<bool> bool_needs_potential(columns, false);
			   int temp = 1, cont = 0;
			   stocks_potential[0] = 0;
			   bool_stocks_potential[0] = true;
			   //решение вопроса с костылём по поводу итераций 

			   //int add_basic_sells = count_add_basic_sells( value_of_products);
			   do {
				   for (int i = 0; i < rows; i++)
					   if (bool_stocks_potential[i])
						   for (int j = 0; j < columns; j++)
							   if (value_of_products[i][j] != -1)
								   if (!bool_needs_potential[j])
								   {
									   needs_potential[j] = value_of_trans[i][j] - stocks_potential[i];
									   bool_needs_potential[j] = true;
									   temp++;
								   }
				   for (int j = 0; j < columns; j++)
					   if (bool_needs_potential[j])
						   for (int i = 0; i < rows; i++)
							   if (value_of_products[i][j] != -1)
								   if (!bool_stocks_potential[i])
								   {
									   stocks_potential[i] = value_of_trans[i][j] - needs_potential[j];
									   bool_stocks_potential[i] = true;
									   temp++;
								   }
				   cont++;
			   } while ((temp <= columns + rows - add_basic_sells * 2) && cont < rows * columns);

			   bool contin = false;
			   if (add_basic_sells > 0) contin = true;
			   while (contin)
			   {
				   for (int i = 0; i < rows; i++)
					   if (!bool_stocks_potential[i])
						   for (int j = 0; j < columns; j++)
							   if (bool_needs_potential[j])
							   {
								   if (add_basic_sells > 0)
									   if (value_of_products[i][j] == -1 && check_basic_sells(min_basic_cells, value_of_products, i, j, rows, columns))
									   {
										   value_of_products[i][j] = 0;
										   add_basic_sells--;
									   }
								   if (value_of_products[i][j] > -1)
								   {
									   stocks_potential[i] = value_of_trans[i][j] - needs_potential[j];
									   bool_stocks_potential[i] = true;
								   }
							   }
				   for (int j = 0; j < columns; j++)
					   if (!bool_needs_potential[j])
						   for (int i = 0; i < rows; i++)
							   if (bool_stocks_potential[i])
							   {
								   if (add_basic_sells > 0)
									   if (value_of_products[i][j] == -1 && check_basic_sells(min_basic_cells, value_of_products, i, j, rows, columns))
									   {
										   value_of_products[i][j] = 0;
										   add_basic_sells--;
									   }
								   if (value_of_products[i][j] > -1)
								   {
									   needs_potential[j] = value_of_trans[i][j] - stocks_potential[i];
									   bool_needs_potential[j] = true;
								   }
							   }
				   contin = false;
				   for (int i = 0; i < rows; i++)
					   if (!bool_stocks_potential[i])contin = true;
				   for (int j = 0; j < columns; j++)
					   if (!bool_needs_potential[j])contin = true;
				   bool_needs_potential.size();
			   }

		   }
		   bool find_cell(int& i_0, int& j_0, vector<vector<int>>value_of_products, vector<int>stocks_potential, vector<int>needs_potential)
		   {
			   for (int i = 0; i < stocks_potential.size(); i++)
				   for (int j = 0; j < needs_potential.size(); j++)
					   if (value_of_products[i][j] == -1)
						   if (stocks_potential[i] + needs_potential[j] <= value_of_trans[i][j])
							   continue;
						   else {
							   j_0 = j;
							   i_0 = i;
							   return true;
						   }
			   return false;
		   }

		   int search_gorizont(int i_0, int j_0, int i_cycle, int j_cycle, int rows, int columns, vector<vector<int>> products, vector<vector<int>>& cycle, int iteration, int min_value)
		   {
			   int rez = -1;
			   for (int j = 0; j < columns; j++)
				   if (((products[i_0][j] >= 0) && (j != j_0)) || ((j == j_cycle) && (i_0 == i_cycle) && (iteration != 0)))
				   {
					   iteration++;
					   if (iteration > 1000)
					   {
						   return -1;
					   }
					   int products_min_old = -1;
					   if ((iteration % 2) == 1)// если ячейка нечётная в цикле ( начальная- нулевая )
					   {
						   products_min_old = min_value;// Запоминаем значение минимальной поставки в цикле (на случай отката назад)
						   if (min_value < 0)min_value = products[i_0][j];
						   else if ((products[i_0][j] < min_value) && (products[i_0][j] >= 0))
						   {
							   min_value = products[i_0][j];

						   }
					   }
					   if ((j == j_cycle) && (i_0 == i_cycle) && ((iteration % 2) == 0))
					   {
						   cycle[i_cycle][j_cycle] = min_value;// Значение минимальной поставки, на величину которой будем изменять
						   return min_value;
					   }
					   else rez = search_vertical(i_0, j, i_cycle, j_cycle, rows, columns, products, cycle, iteration, min_value);
					   if (rez >= 0)
					   {
						   if (iteration % 2 == 0)cycle[i_0][j] = cycle[i_cycle][j_cycle];
						   else  cycle[i_0][j] = -cycle[i_cycle][j_cycle];
						   break;
					   }
					   else //откат назад в случае неудачи(круг не замкнулся):
					   {
						   iteration--;
						   if (products_min_old >= 0)
							   min_value = products_min_old;
					   }
				   }

			   return rez;
		   }
		   int search_vertical(int i_0, int j_0, int i_cycle, int j_cycle, int rows, int columns, vector<vector<int>> products, vector<vector<int>>& cycle, int iteration, int min_value)
		   {
			   int rez = -1;
			   int i;
			   for (i = 0; i < rows; i++)
				   if (((products[i][j_0] >= 0)) && (i != i_0) || ((j_0 == j_cycle) && (i == i_cycle) && (iteration != 0)))
				   {
					   iteration++;
					   if (iteration > 1000)
					   {
						   return -1;
					   }
					   int products_min_old = -1;
					   if ((iteration % 2) == 1)
					   {
						   products_min_old = min_value;
						   if (min_value < 0)min_value = products[i][j_0];
						   else if ((products[i][j_0] < min_value) && (products[i][j_0] >= 0))
							   min_value = products[i][j_0];
					   }
					   if ((i == i_cycle) && (j_0 == j_cycle) && ((iteration % 2) == 0))
					   {
						   cycle[i_cycle][j_cycle] = min_value;// Значение минимальной поставки, на величину которой будем изменять
						   return min_value;
					   }
					   else rez = search_gorizont(i, j_0, i_cycle, j_cycle, rows, columns, products, cycle, iteration, min_value);
					   if (rez >= 0)
					   {
						   if (iteration % 2 == 0) cycle[i][j_0] = cycle[i_cycle][j_cycle];
						   else  cycle[i][j_0] = -cycle[i_cycle][j_cycle];
						   break;
					   }
					   else
					   {
						   iteration--;
						   if (products_min_old >= 0)
							   min_value = products_min_old;
					   }
				   }
			   return rez;
		   }
		   void check_and_do_cycle(vector<vector<int>>& value_of_products)
		   {
			   int rows = stocks.size();
			   int columns = needs.size();
			   int i_start = -1, j_start = -1, i_0, j_0;
			   int add_basic_sells_ = count_add_basic_sells(value_of_products);
			   vector<int> needs_potential(columns);
			   vector<int> stocks_potential(rows);
			   count_potentials(add_basic_sells_, value_of_products, stocks_potential, needs_potential);
			   while (find_cell(i_0, j_0, value_of_products, stocks_potential, needs_potential))
			   {
				   i_start = i_0;
				   j_start = j_0;
				   vector<vector <int>> sells_for_cycle(rows, vector<int>(columns, 0));//двумерный массив заполненный нулевыми элементами--- 1 - плюсик цикла  --- -1 - минусик
				   bool even_sell = false;
				   int kol = search_vertical(i_0, j_0, i_0, j_0, rows, columns, value_of_products, sells_for_cycle, 0, -1);
				   if (kol == search_gorizont(i_0, j_0, i_0, j_0, rows, columns, value_of_products, sells_for_cycle, 0, -1) && kol == 0)
					   break;

				   for (int i = 0; i < rows; i++)
					   for (int j = 0; j < columns; j++)
					   {
						   if (value_of_products[i][j] == -1 && sells_for_cycle[i][j] != 0)
							   value_of_products[i][j] = sells_for_cycle[i][j];
						   else if (value_of_products[i][j] > 0 && value_of_products[i][j] == sells_for_cycle[i][j])
							   value_of_products[i][j] = -1;
						   else
							   value_of_products[i][j] += sells_for_cycle[i][j];
					   }
				   add_basic_sells_ = count_add_basic_sells(value_of_products);
				   count_potentials(add_basic_sells_, value_of_products, stocks_potential, needs_potential);

			   }
			   if (i_start == -1) textBox3->Text += "\r\nПлан перевозок является оптимальным и не нуждается в пересчёте\r\n";
		   }
		   void fill_tables_with_values() {
			   int rows = needs.size();
			   int columns = stocks.size();
			   stocksGrid->ColumnCount = 1;
			   stocksGrid->RowCount = columns;
			   stocksGrid->TopLeftHeaderCell->Value = "Склады";
			   shopsGrid->TopLeftHeaderCell->Value = "Магазины";
			   valueOfTransGrid->TopLeftHeaderCell->Value = "С\\М";

			   for (int i = 0; i < columns; i++)
				   stocksGrid->Rows[i]->Cells[0]->Value = msclr::interop::marshal_as< String^ >(to_string(stocks[i]));


			   shopsGrid->ColumnCount = rows;
			   shopsGrid->RowCount = 1;
			   for (int i = 0; i < rows; i++)
				   shopsGrid->Rows[0]->Cells[i]->Value = msclr::interop::marshal_as< String^ >(to_string(needs[i]));
			   valueOfTransGrid->ColumnCount = columns;
			   valueOfTransGrid->RowCount = rows;
			   for (int i = 0; i < rows; i++)
				   for (int j = 0; j < columns; j++)
				   {
					   valueOfTransGrid->Rows[i]->Cells[j]->Value = msclr::interop::marshal_as< String^ >(to_string(value_of_trans[i][j]));
				   }

		   }

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->rows_text_box = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->columns_text_box = (gcnew System::Windows::Forms::TextBox());
			this->shopsGrid = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridViewTextBoxColumn1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->stocksGrid = (gcnew System::Windows::Forms::DataGridView());
			this->valueOfTransGrid = (gcnew System::Windows::Forms::DataGridView());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->shopsGrid))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stocksGrid))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->valueOfTransGrid))->BeginInit();
			this->SuspendLayout();
			// 
			// rows_text_box
			// 
			this->rows_text_box->Location = System::Drawing::Point(108, 44);
			this->rows_text_box->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->rows_text_box->Name = L"rows_text_box";
			this->rows_text_box->Size = System::Drawing::Size(45, 22);
			this->rows_text_box->TabIndex = 13;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 44);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 17);
			this->label2->TabIndex = 12;
			this->label2->Text = L"Склады: ";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 17);
			this->label1->TabIndex = 11;
			this->label1->Text = L"Магазины:";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// columns_text_box
			// 
			this->columns_text_box->Location = System::Drawing::Point(108, 15);
			this->columns_text_box->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->columns_text_box->Name = L"columns_text_box";
			this->columns_text_box->Size = System::Drawing::Size(45, 22);
			this->columns_text_box->TabIndex = 10;
			this->columns_text_box->TextChanged += gcnew System::EventHandler(this, &MyForm::colunms_TextChanged);
			// 
			// shopsGrid
			// 
			this->shopsGrid->AllowDrop = true;
			this->shopsGrid->AllowUserToAddRows = false;
			this->shopsGrid->AllowUserToDeleteRows = false;
			this->shopsGrid->ColumnHeadersHeight = 32;
			this->shopsGrid->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(1) { this->dataGridViewTextBoxColumn1 });
			this->shopsGrid->Location = System::Drawing::Point(189, 78);
			this->shopsGrid->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->shopsGrid->MultiSelect = false;
			this->shopsGrid->Name = L"shopsGrid";
			this->shopsGrid->RowHeadersWidth = 55;
			this->shopsGrid->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			dataGridViewCellStyle1->Format = L"N2";
			dataGridViewCellStyle1->NullValue = nullptr;
			this->shopsGrid->RowsDefaultCellStyle = dataGridViewCellStyle1;
			this->shopsGrid->RowTemplate->Height = 24;
			this->shopsGrid->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
			this->shopsGrid->Size = System::Drawing::Size(492, 98);
			this->shopsGrid->TabIndex = 9;
			this->shopsGrid->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::shopsGrid_CellContentClick);
			// 
			// dataGridViewTextBoxColumn1
			// 
			this->dataGridViewTextBoxColumn1->MinimumWidth = 6;
			this->dataGridViewTextBoxColumn1->Name = L"dataGridViewTextBoxColumn1";
			this->dataGridViewTextBoxColumn1->Width = 125;
			// 
			// stocksGrid
			// 
			this->stocksGrid->AllowUserToAddRows = false;
			this->stocksGrid->AllowUserToDeleteRows = false;
			this->stocksGrid->AllowUserToOrderColumns = true;
			this->stocksGrid->AllowUserToResizeColumns = false;
			this->stocksGrid->AllowUserToResizeRows = false;
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			this->stocksGrid->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
			this->stocksGrid->ColumnHeadersHeight = 32;
			this->stocksGrid->Location = System::Drawing::Point(24, 201);
			this->stocksGrid->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->stocksGrid->Name = L"stocksGrid";
			this->stocksGrid->RowHeadersWidth = 55;
			this->stocksGrid->RowTemplate->Height = 24;
			this->stocksGrid->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->stocksGrid->Size = System::Drawing::Size(129, 471);
			this->stocksGrid->TabIndex = 8;
			this->stocksGrid->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::stocksGrid_CellContentClick);
			// 
			// valueOfTransGrid
			// 
			this->valueOfTransGrid->AllowUserToAddRows = false;
			this->valueOfTransGrid->AllowUserToDeleteRows = false;
			this->valueOfTransGrid->ColumnHeadersHeight = 32;
			this->valueOfTransGrid->Location = System::Drawing::Point(189, 201);
			this->valueOfTransGrid->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->valueOfTransGrid->Name = L"valueOfTransGrid";
			this->valueOfTransGrid->RowHeadersWidth = 55;
			this->valueOfTransGrid->Size = System::Drawing::Size(492, 471);
			this->valueOfTransGrid->TabIndex = 7;
			this->valueOfTransGrid->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::valueOfTrans_CellContentClick);
			// 
			// textBox3
			// 
			this->textBox3->BackColor = System::Drawing::SystemColors::Info;
			this->textBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox3->Location = System::Drawing::Point(688, 78);
			this->textBox3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->textBox3->Multiline = true;
			this->textBox3->Name = L"textBox3";
			this->textBox3->ReadOnly = true;
			this->textBox3->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox3->Size = System::Drawing::Size(519, 533);
			this->textBox3->TabIndex = 14;
			this->textBox3->Text = L"Загрузите данные в таблицу";
			this->textBox3->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox3_TextChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(24, 115);
			this->button1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(129, 61);
			this->button1->TabIndex = 15;
			this->button1->Text = L"Создание таблицы вручную";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(433, 12);
			this->button2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(248, 54);
			this->button2->TabIndex = 16;
			this->button2->Text = L"Загрузка данных из файла";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(688, 12);
			this->button3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(520, 54);
			this->button3->TabIndex = 17;
			this->button3->Text = L"Расчёт задачи";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->Filter = L"(*.txt)|*.txt";
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(189, 12);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(238, 54);
			this->button4->TabIndex = 18;
			this->button4->Text = L"Загрузка данных из таблицы";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// label3
			// 
			this->label3->Location = System::Drawing::Point(24, 74);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(129, 39);
			this->label3->TabIndex = 19;
			this->label3->Text = L"Для заполнения таблицы нажми";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(688, 616);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(519, 56);
			this->button5->TabIndex = 20;
			this->button5->Text = L"Загрузка полученного плана в файл";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->Filter = L"(*.txt)|*.txt";
			this->saveFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveFileDialog1_FileOk);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1223, 684);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->rows_text_box);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->columns_text_box);
			this->Controls->Add(this->shopsGrid);
			this->Controls->Add(this->stocksGrid);
			this->Controls->Add(this->valueOfTransGrid);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Решение транспорной задачи методом потенциалов";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->shopsGrid))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->stocksGrid))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->valueOfTransGrid))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}


	
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {


	}

private: System::Void valueOfTrans_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {

}
private: System::Void colunms_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	
}
private: System::Void shopsGrid_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	shopsGrid->RowCount = 1;
	shopsGrid->ColumnCount = Convert::ToInt32(columns_text_box->Text); stocksGrid->ColumnCount = 1;
	stocksGrid->RowCount= Convert::ToInt32(rows_text_box->Text);
	valueOfTransGrid->ColumnCount= Convert::ToInt32(columns_text_box->Text);
	valueOfTransGrid->RowCount = Convert::ToInt32(rows_text_box->Text); valueOfTransGrid->RowHeadersWidth = 55;
	stocksGrid->TopLeftHeaderCell ->Value= "Склады";
	shopsGrid->TopLeftHeaderCell->Value = "Магазины";
	valueOfTransGrid->TopLeftHeaderCell->Value = "С\\М";
	for (int i = 0; i < Convert::ToInt32(columns_text_box->Text); i++) {
		shopsGrid->Columns[i]->HeaderCell->Value = "M" + Convert::ToString(i + 1);
		valueOfTransGrid->Columns[i]->HeaderCell->Value = "M" + Convert::ToString(i + 1);
	}
	for (int i = 0; i < Convert::ToInt32(rows_text_box->Text); i++)
	{
		stocksGrid->Rows[i]->HeaderCell->Value = "C" + Convert::ToString(i + 1);
		valueOfTransGrid->Rows[i]->HeaderCell->Value = "C" + Convert::ToString(i + 1);
	}
	int rows = System::Convert::ToInt32(rows_text_box->Text);
	int columns = System::Convert::ToInt32(columns_text_box->Text);



}
private: System::Void stocksGrid_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
}
private: System::Void textBox3_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	stocks.clear();
	needs.clear();
	String^ name = "";

	if (openFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK)
	{
		name = openFileDialog1->FileName;
	}
	//конструкция для ошибок
	try {
		StreamReader^ file = File::OpenText(name); 
		String^ rows_s_, ^ columns_s_, ^ read_str_;
		int rows, columns;
		rows_s_ = file->ReadLine();
		columns_s_ = file->ReadLine();
		std::string rows_s = msclr::interop::marshal_as< std::string >(rows_s_);
		std::string columns_s = msclr::interop::marshal_as< std::string >(columns_s_);
		rows = atoi(rows_s.c_str());
		columns = atoi(columns_s.c_str());
		read_str_ = file->ReadLine();
		string read_str = msclr::interop::marshal_as< std::string >(read_str_);

		transform_line_into_vector(stocks, read_str);

		read_str_ = file->ReadLine();
		read_str.clear();
		read_str = msclr::interop::marshal_as< std::string >(read_str_);

		transform_line_into_vector(needs, read_str);
		needs.size();
		vector <vector<int> > value_of_trans_(rows);
		
		for (int i = 0; i < rows; i++)
		{
			read_str_ = file->ReadLine();
			read_str.clear();
			read_str = msclr::interop::marshal_as< std::string >(read_str_);
			
			transform_line_into_vector(value_of_trans_[i], read_str);
		}
		value_of_trans = value_of_trans_;
		value_of_trans.size();
		fill_tables_with_values();
		textBox3->Text = "Данные загружены из файла \r\n";
	}
	catch (Exception^ e)
	{
		MessageBox::Show(this, "Файл не был открыть", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) 
{
	int rows = stocks.size();
	int columns = needs.size();
	vector<vector <int>> value_of_products(rows, vector<int>(columns, -100));
	int dif;
	create_problem_close(dif, rows, columns);
	fill_start_products(value_of_products);

	value_of_products.size();
	textBox3->Text += "\r\nПлан перевозок, построенный методом минимальных значений:\r\n";
	print_double_vector(value_of_products);
	int add_basic_sells_ = count_add_basic_sells(value_of_products);
	if (add_basic_sells_ == 0 || add_basic_sells_ < 0)
	{
		textBox3->Text += "\r\nПлан перевозок не вырожденный\n";
	}
	else
	{
		textBox3->Text += "\r\nПлан перевозок вырожден, будут добавлены нулевые перевозки\n";
	}
	value_of_products.size();
	check_and_do_cycle(value_of_products);
	textBox3->Text += "\r\nПлан перевозок, являющийся оптимальным:\r\n";
	print_double_vector(value_of_products);
	plan = value_of_products;
}
private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
	stocks.clear();
	needs.clear();

	int rows = System::Convert::ToInt32(rows_text_box->Text);
	int columns = System::Convert::ToInt32(columns_text_box->Text);
	for (int i = 0; i < rows; i++)
	{
		if (stocksGrid->Rows[i]->Cells[0]->Value == nullptr)
		{
			MessageBox::Show(this, "Поле поставщиков не заполнено полностью", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		stocks.push_back(System::Convert::ToInt32(stocksGrid->Rows[i]->Cells[0]->Value));
	}
	stocks.size();
	for (int i = 0; i < columns; i++)
	{
		if (shopsGrid->Rows[0]->Cells[i]->Value==nullptr) {
			MessageBox::Show(this, "Поле потребителей не заполнено полностью", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
		needs.push_back(System::Convert::ToInt32(shopsGrid->Rows[0]->Cells[i]->Value));
	}
	needs.size();
	vector <vector<int> > value_of_trans_(rows);
	for(int i=0;i<rows;i++)
		for (int j = 0; j < columns; j++)
		{
			if (valueOfTransGrid->Rows[i]->Cells[j]->Value==nullptr) {
				MessageBox::Show(this, "Поле цен перевозок не заполнено полностью", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			value_of_trans_[i].push_back(System::Convert::ToInt32(valueOfTransGrid->Rows[i]->Cells[j]->Value));
		}

	value_of_trans = value_of_trans_;
	textBox3->Text = "Данные загружены из таблицы\r\n";
	value_of_trans.size();
}
private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {

	String^ name = "";

	if (saveFileDialog1->ShowDialog() == Windows::Forms::DialogResult::OK)
	{
		name = saveFileDialog1->FileName;
	}
	//конструкция для ошибок
	try {
		string str="";
		int rows = stocks.size();
		int columns = needs.size();
		StreamWriter^ file = gcnew StreamWriter(name); 
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				str += to_string(plan[i][j]) + "\t";
				//textBox3->Text=msclr::interop::marshal_as<String^>(value_of_products[i][j]);
			}
			str += "\r\n";
			
		}
		file->Write(msclr::interop::marshal_as<String^>(str));
		//textBox3->Text += msclr::interop::marshal_as<String^>(str);
		file->Close();
	}
	catch (Exception^ e)
	{
		MessageBox::Show(this, "Файл не был сохранён", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}
private: System::Void saveFileDialog1_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) {
}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}
