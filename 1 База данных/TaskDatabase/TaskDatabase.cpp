#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

struct temperature
{
    int day;
    double val;
    bool is_deleted;

    temperature(int day, double val, bool is_del=false)
    {
        this->day = day;
        this->val = val;
        is_deleted = is_del;
    }
    temperature(){}
};

void show(const vector<temperature>& temperatures)
{
    cout << setw(10) << "День" << setw(15) << "Температура" << endl;
    for (auto& t : temperatures)
    {
        if (!t.is_deleted)
            cout << setw(10) << t.day << setw(15) << t.val << endl;
    }
}

int find_temp(const vector<temperature>& temperatures, int day)
{
    for (int i = 0; i < temperatures.size(); i++)
    {
        if (temperatures[i].day == day)
            return i;
    }
    return -1;
}

void save(const vector<temperature>& temperatures)
{
    ofstream file("temp_info.txt");
    for (int i = 0; i < temperatures.size(); i++)
    {
        file << temperatures[i].day << " " << temperatures[i].val << " " << temperatures[i].is_deleted;
        if (i < temperatures.size() - 1)
            file << endl;
    }
    file.close();
}

void load(vector<temperature>& temperatures)
{
    ifstream file("temp_info.txt");
    if (!file.is_open())
        return;

    int day;
    double val;
    bool is_del;

    while (!file.eof())
    {
        file >> day >> val >> is_del;
        temperature t(day, val, is_del);
        temperatures.push_back(t);
    }
    file.close();
}

bool check_deleted(vector<temperature>& temperatures, int& count_del) {
    count_del = 0;
    for (int i = 0; i < temperatures.size(); i++)
    {
        if (temperatures[i].is_deleted)
            count_del++;
    }
    if (count_del > temperatures.size() / 2)
        return true;
    else
        return false;
}

void remove_deleted(vector<temperature>& temperatures)
{
    for (int i = 0; i < temperatures.size(); i++)
    {
        if (temperatures[i].is_deleted)
        {
            temperatures.erase(temperatures.begin() + i);
            i--;
        }
    }
}

void count_day_more_sred(const vector<temperature>& temperatures)
{
    double sum = 0;
    for (int i = 0; i < temperatures.size(); i++)
    {
        sum += temperatures[i].val;
    }
    double avg = sum / temperatures.size();

    cout << "Среднее = " << avg << endl;

    for (int i = 0; i < temperatures.size(); i++)
    {
        if (temperatures[i].val > avg)
            cout << temperatures[i].day << " ";
    }

    cout << endl;

    /*int count = 0;
    for (int i = 0; i < temperatures.size(); i++)
    {
        if (temperatures[i].val > avg)
            count++;
    }
    return count;*/
}

void long_otrezok(vector<temperature>& temperatures)
{
    //сортируем по дням
    for (int i = 0; i < temperatures.size() - 1; i++)
    {
        for (int j = 0; j < temperatures.size() - 1 - i; j++)
        {
            if (temperatures[j].day > temperatures[j + 1].day)
            {
                swap(temperatures[j], temperatures[j + 1]);
            }
        }
    }

    int find_pos = -1;
    int pos1 = -1;
    int pos2 = -1;
    int max_len = 0;

    for (int i = 0; i < temperatures.size(); i++)
    {
        if (temperatures[i].val < 0)
        {
            if (pos1 == -1)
                pos1 = i;
            else
            {
                pos2 = i;
                if ((pos2 - pos1) > max_len)
                {
                    max_len = pos2 - pos1;
                    find_pos = pos1;
                }
                pos1 = i;
            }
        }
    }

    cout << "Кол-во дней = " << max_len - 1 << endl;
    cout << "от " << temperatures[find_pos].day << " до " << temperatures[find_pos + max_len].day << endl;
}

int main()
{
    setlocale(LC_ALL, "rus");

    vector<temperature> temperatures;
    load(temperatures);
    int choose;
    int day;
    double temp;

    temperature last_temp;
    int pos_last = -1;

    do
    {
        cout << "1.Добавление новой записи" << endl;
        cout << "2.Просмотр записей" << endl;
        cout << "3.Редактирование записи" << endl;
        cout << "4.Удаление записи" << endl;
        cout << "5.Сохранение записей" << endl;
        cout << "6.Отмена последнего удаления" << endl;
        cout << "7.Определять: дни, в которые температура поднималась выше средней за месяц" << endl;
        cout << "8.Самый длинный отрезок между днями с отрицательной температурой." << endl;
        cout << "0.Выход" << endl;
        cin >> choose;

        if (choose == 1)
        {
            cout << "1.В начало" << endl;
            cout << "2.По индексу" << endl;
            cout << "3.В конец" << endl;

            int p;
            cin >> p;

            cout << "Введите номер дня: ";
            cin >> day;
            cout << "Введите температуру: ";
            cin >> temp;

            temperature t(day, temp);

            if (find_temp(temperatures, day) != -1)
            {
                cout << "Такой день уже есть!" << endl;
            }
            else
            {
                if (p == 1)
                    temperatures.insert(temperatures.begin(), t);
                else if (p == 2)
                {
                    int index;
                    cout << "Введите индекс: ";
                    cin >> index;
                    temperatures.insert(temperatures.begin() + index, t);
                }
                else
                    temperatures.push_back(t);
            }
        }
        else if (choose == 2)
        {
            show(temperatures);
        }
        else if (choose == 3)
        {
            cout << "Введите номер дня: ";
            cin >> day;

            int pos = find_temp(temperatures, day);
            if (pos == -1)
                cout << "Нет такого элемента" << endl;
            else
            {
                cout << "Введите новую температуру: ";
                cin >> temp;
                temperatures[pos].val = temp; 
                cout << "Редактирование успешно завершено" << endl;
            }
        }
        else if (choose == 4)
        {
            cout << "Введите номер дня: ";
            cin >> day;

            int pos = find_temp(temperatures, day);
            if (pos == -1)
                cout << "Нет такого элемента" << endl;
            else
            {
                pos_last = pos;
                last_temp = temperatures[pos];
                temperatures[pos].is_deleted = true;
                int count_del;

                if (check_deleted(temperatures, count_del))
                {
                    remove_deleted(temperatures);
                    pos_last -= (count_del - 1);
                }
                //temperatures.erase(temperatures.begin() + pos);
                cout << "Удаление успешно завершено" << endl;
            }
        }
        else if (choose == 5)
            save(temperatures);
        else if (choose == 6)
        {
            if (pos_last != -1)
            {
                int pos = find_temp(temperatures, last_temp.day);

                if (pos == -1)
                    temperatures.insert(temperatures.begin() + pos_last, last_temp);
                else
                    temperatures[pos].is_deleted = false;

                pos_last = -1;
            }
        }
        else if (choose == 7)
        {
            count_day_more_sred(temperatures);
        }
        else if (choose == 8)
        {
            long_otrezok(temperatures);
        }
    } 
    while (choose != 0);
}