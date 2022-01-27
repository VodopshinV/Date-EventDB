#include<iostream>
#include<string>
#include<vector>
#include<exception>
#include<fstream>
#include<sstream>
#include<map>
#include<set>
#include<iomanip>
using namespace std;

class Date{
public:
	explicit Date() {

	};

	explicit Date(const int& new_year,const int& new_month, const int& new_day)
	{
		year = new_year;

		if (1 > new_month || new_month > 12)
		{
			string string_err = "Month value is invalid: " +to_string(new_month);
			throw runtime_error(string_err);
		}
		else if (1 > new_day || new_day > 31)
		{
			string string_err = "Day value is invalid: " + to_string(new_day);
			throw runtime_error(string_err);
		}
		else
		{
			month = new_month;
			day = new_day;
		}
	};

	int GetYear() const
	{
		return year;
	}

	int GetMonth() const
	{
		return month;
	}

	int GetDay() const
	{
		return day;
	}

private:
	int year;
	int month;
	int day;
};

void EnsureNextSymbolAndSkip(stringstream& stream, const string& date_str)
{
	if (stream.peek() != '-')
	{
		stringstream ss;
		ss << "Wrong date format: "  + date_str;
		throw runtime_error(ss.str());
	}
	stream.ignore(1);
}

istream& operator >> (istream& stream, Date& date) {
	int year = 10000;
	int month = 10000;
	int day = 10000;

	string data_str;
	string endError = ""; 
	if (stream >> data_str) {
		stringstream d(data_str);
		d >> year;
		EnsureNextSymbolAndSkip(d, data_str);
		d >> month;
		EnsureNextSymbolAndSkip(d, data_str);
		d >> day;
		d >> endError;
		
		if (year > -1 && year < 10000 && month < 10000 && day < 10000 && endError == "") {
			date = Date(year, month, day);
			return stream;
		}
		else {
			stringstream string_error;
			string_error << "Wrong date format: " + data_str;
			throw runtime_error(string_error.str());   
		}
	}
	else {
		stringstream string_error;
		string_error << "Wrong date format: " + data_str;
		throw runtime_error(string_error.str());   
	}

}

ostream& operator<< (ostream& stream, const Date& date)
{
	stream << setw(4) << setfill('0') << date.GetYear() << '-'
		<< setw(2) << setfill('0') << date.GetMonth() << '-'
		<< setw(2) << setfill('0') << date.GetDay();
	return stream;
}

bool operator<(const Date& lhs, const Date& rhs)
{
	if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth())
	{
		return lhs.GetDay() < rhs.GetDay();
	}
	else if (lhs.GetYear() == rhs.GetYear())
	{
		return lhs.GetMonth() < rhs.GetMonth();
	}
	else 
	{
		return lhs.GetYear() < rhs.GetYear();
	}
}

class Database
{
public:
	void AddEvent(const Date& date, const string& event)
	{
		Database[date].insert(event);
	}

	void DeleteEvent(const Date& date, const string& event)
	{
		if (Database.count(date))
		{
			if (Database[date].count(event))
			{
				Database[date].erase(event);
				cout << "Deleted successfully";
			}
			else
			{
				cout << "Event not found";
			}
		}
		else
		{
			cout << "Event not found";
		}

		cout << endl;
	}

	void DeleteDate(const Date& date)
	{
		int n = 0;
		if (Database.count(date))
		{
			n = Database[date].size();
			Database.erase(date);
		}
		cout << "Deleted " << n << " events"<<endl;
	}

	void Find(const Date& date) const
	{
		if (Database.count(date))
		{
			for (auto& x : Database.at(date))
			{
				cout << x << endl;
			}
		}
	}

	void Print() const
	{
		for (auto x : Database)
		{
			for (auto y : x.second)
			{
				cout << x.first << ' ' << y << endl;
			}
		}
	}

private:
	map<Date, set<string>> Database;
};

void Command(const string& command, Database& db)
{
	stringstream stream(command);
	Date date;
	string event;
	string command_set;

	if (stream >> command_set)
	{
		if (command_set == "Add")
		{
			if (stream >> date)
			{
				if (stream >> event)
				{
					db.AddEvent(date, event);
				}
				else
				{
					stringstream ss;
					ss << "Wrong date format: " << date;
					throw runtime_error(ss.str());
				}
			}
		}
		else if (command_set == "Del")
		{
			if (stream >> date)
			{
				if (stream >> event)
				{
					db.DeleteEvent(date, event);
				}
				else
				{
					db.DeleteDate(date);
				}
			}
		}
		else if (command_set == "Find")
		{
			if (stream >> date)
			{
				db.Find(date);
			}
		}
		else if (command_set == "Print")
		{
			db.Print();
		}
		else
		{
			stringstream ss;
			ss << "Unknown command: " << command_set;
			throw runtime_error(ss.str());
		}
	}
}

int main()
{
	Database db;
	string command;

	try {
		while (getline(cin, command))
		{
			Command(command, db);
		}
	}
	catch(exception&ex)
	{
		cout << ex.what();
	}

	system("pause");
	return 0;
}