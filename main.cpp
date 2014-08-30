#include<iostream>
#include<string>
#include"sqlite3.h"
#include<time.h>
#include<sstream>


using namespace std;


void createTables();
void addUser(string u_n, string f, string l);
void addFood(string fn, int cal, int cat_id);
void printUsers();		//example functionality, remove from final program
bool checkUser(string u);
int dispMain();
void dispLog(string u_n);
void dispCreateNewUser(string u_n);		
int dispPostLog(const string& u_n);
void showCategories();
bool checkFood(string food);
void dispAddFood();
void updateCalorie(string food_name, int new_cal);
void dispFoodDate(const string& u_n);
void addFoodByDate(string fn, string date, const string& u_n);
int getFoodDateQuantity(const string& u_n, string fn, const string& da);
void updateQuant(const string& u_n, const string& fn, int quantity, const string& da);
int getTodayCalorie(const string& u_n, const string& da);
string getCurrentDate();
int getCalByCat(const string& u_n, const string& da);
void getInfoAboutDay(const string& u_n);
void getFoodOnDate(const string& u_n, const string& date);
void calBetweenDates(const string& u_n);
void getDateInfo(const string& u_n);
void insertCategories(string cat_name);
string db_path;


								//dips creat new	disp log	check user	adduser
int main(){
	
	db_path = "CC.sqlite";
	createTables();	
	string date = getCurrentDate();
	int choice = 0;
	string user_name;
	system("cls");
	choice = dispMain();
	if(choice == 1)
		dispLog(user_name);
	else if(choice == 2)
		dispCreateNewUser(user_name);
	else
		return 0;	
	choice = 0;
	choice = dispPostLog(user_name);
	
	while(true){
		cout<<"Here"<<endl;
		if(choice == 1){
			dispAddFood();			
			choice = dispPostLog(user_name);			
		}
		else if(choice == 2){
			dispFoodDate(user_name);			
			choice = dispPostLog(user_name);
		}
		else if(choice == 3){			
			system("cls");
			cout<<"Calorie count for today: "<<getTodayCalorie(user_name, date)<<" calories."<<endl;
			system("pause");
			choice = dispPostLog(user_name);
		}
		else if(choice == 4){
			getInfoAboutDay(user_name);
			system("pause");
			choice = dispPostLog(user_name);
		}
		else if(choice == 5){
			/*You should be able to query the database to find out how many calories by category 
			and how many total calories the user has eaten in between any two dates.*/ 
			system("cls");
			calBetweenDates(user_name);
			system("pause");
			choice = dispPostLog(user_name);
		}
		else if(choice == 6){
			//enter date see all days when food was eaten (and what else was eaten on that day)
			getDateInfo(user_name);
			system("pause");
			choice = dispPostLog(user_name);
		}
		else if(choice == 7){
			dispLog(user_name);
			choice = dispPostLog(user_name);
		}
		else if(choice == 8){
			return 0;
		}
	}

	system("pause");
	return 0;
}

string getCurrentDate(){
	time_t now = time(NULL);
   struct tm *t = localtime(&now);   
   int year = t->tm_year+1900;
   int month = t->tm_mon+1;   
   int day = t->tm_mday;  
   stringstream s;
   s<<year;
   string date = s.str();   
   s.str("");
   s<<month;
   date = date + "-"+ s.str();
   s.str("");
   s<<day;
   if(day > 9)
	   date = date+"-"+s.str();
   else
	   date = date+"-0"+s.str();
   return date;
}

void dispFoodDate(const string& u_n){					
	system("cls");
	cout<<"Enter food name: ";
	string food;
	cin>>food;
	if(!checkFood(food)){
		cout<<"You have entered a food not in the database. To re-enter name of food press 1."<<endl
			<<"To add this food to the databse press 2."<<endl;
		int choice = 0;
		cin>>choice;
		if(choice == 1)
			dispFoodDate(u_n);
		else
			dispAddFood();
	}
	string date;
	cout<<"Enter the date you ate this food (yyyy-mm-dd): ";
	cin>>date;	
	addFoodByDate(food,date,u_n);

}					

void dispAddFood(){
	system("cls");
	showCategories();

	cout<<"Enter food name: ";
	string fn;
	cin>>fn;
	cout<<"Enter calories: ";
	int calories;
	cin>>calories;
	cout<<"Enter category id (see above): ";
	int catid;
	cin>>catid;
	
	int choice = 0;
	if(!checkFood(fn))
		addFood(fn,calories,catid);
	else{
		cout<<"Food already in database, would you like to enter another food (press 1) or update "<<endl
			<<"calorie info for this food (press 2)"<<endl;
		cin>>choice;
		while(choice != 1 && choice != 2){
			cout<<"Enter a 1 or a 2"<<endl;
			cin>>choice;
		}
		if(choice == 1)
			dispAddFood();
		else{				
			updateCalorie(fn, calories);

		}				
	}
}

int dispMain(){
	cout<<"1.) Log In"<<endl<<"2.) Create New User"<<endl<<"3.) Exit Program"<<endl;
	int x = 0;
	cin>>x;
	while(x != 1 && x != 2 && x != 3){
		cout<<"Please enter a 1, 2, or 3: "<<endl;
		cin>>x;
	}
	return x;
}

void dispLog(string u_n){
	system("cls");
	int choice = 0;
	cout<<"Enter your user name: ";
	cin>>u_n;
	while(!checkUser(u_n)){
		cout<<"Invalid user name. Press 1 to re-enter user name, or "<<endl
			<<"press 2 to create a new user"<<endl;
		cin>>choice;
		while(choice != 1 && choice != 2){
			cout<<"Please enter a 1 or a 2"<<endl;
			cin>>choice;
		}
		if(choice == 1)
			dispLog(u_n);
		else{
			dispCreateNewUser(u_n);
		}
	}
	
}

void dispCreateNewUser(string u_n){
	system("cls");

	string first, last;
	cout<<"Enter your first name: ";
	cin>>first;
	cout<<"Enter your last name: ";
	cin>>last;

	cout<<"Enter your desired user name: ";
	cin>>u_n;
	while(checkUser(u_n)){
		cout<<"User name already taken, please enter another: ";
		cin>>u_n;
	}
	addUser(u_n, first, last);
}

int dispPostLog(const string& u_n){
	//insert menus for after logging in here
	system("cls");
	cout<<"Hello "<<u_n<<" hope the diet is going well"<<endl<<endl;
	cout<<"1.) Add A Food To Memory"<<endl<<"2.) Enter Food Eaten On A Day"<<endl<<"3.) Show Today's Calorie Count"<<endl
		<<"4.) Get Info About a Day"<<endl<<"5.) Find Calories Between Dates"<<endl<<"6.) Find When Food Was Eaten"<<endl
		<<"7.) Switch Users"<<endl<<"8.) Exit Program"<<endl;
	int choice = 0;
	cin>>choice;
	while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7 && choice != 8){
		cout<<"Please enter a number 1 through 7"<<endl;
		cin>>choice;
	}
	return choice;
}

void createTables(){
	
	sqlite3 *db;

	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		cout<<"Successfully opened the databse"<<endl;
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "CREATE TABLE User (UserName TEXT PRIMARY KEY  NOT NULL  UNIQUE , First TEXT, Last TEXT)",
			-1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a prepared statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);

			if(status_of_step == SQLITE_DONE){
				cout<<"Succesfully inserted into the databse"<<endl;
			}
			else
				cout<<"Problem inserting into the databse"<<endl;
		}
		else
			cout<<"Problem creating prepared statement"<<endl;


		status_of_prep = sqlite3_prepare_v2(db, "CREATE TABLE FoodEatenByUser (UserName TEXT NOT NULL , FoodName TEXT NOT NULL , Date DATETIME NOT NULL , Quantity INTEGER, PRIMARY KEY (UserName, FoodName, Date))",
			-1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a prepared statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);

			if(status_of_step == SQLITE_DONE){
				cout<<"Succesfully inserted into the databse"<<endl;
			}
			else
				cout<<"Problem inserting into the databse"<<endl;
		}
		else
			cout<<"Problem creating a prepared statement"<<endl;

		status_of_prep = sqlite3_prepare_v2(db, "CREATE TABLE Food (FoodName TEXT PRIMARY KEY  NOT NULL , Calories INTEGER, CategoryID INTEGER)",
			-1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a prepared statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);

			if(status_of_step == SQLITE_DONE){
				cout<<"Succesfully inserted into the databse"<<endl;
			}
			else
				cout<<"Problem inserting into the databse"<<endl;
		}
		else
			cout<<"Problem creating a prepared statement"<<endl;


		status_of_prep = sqlite3_prepare_v2(db, "CREATE TABLE Category (CategoryID INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , Name TEXT)",
			-1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a prepared statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);

			if(status_of_step == SQLITE_DONE){
				cout<<"Succesfully inserted into the databse"<<endl;
			}
			else
				cout<<"Problem inserting into the databse"<<endl;
		}
		else
			cout<<"Problem creating a prepared statement"<<endl;

	}
	else
		cout<<"Problem opening the databse"<<endl;

	string cats[6] = {"Vegetables", "Fruits", "Grains", "Dairy", "Meats", "Sweets"};
	for(int i = 0; i < 6; i++){
		insertCategories(cats[i]);
	}
}

void getDateInfo(const string& u_n){
	 
	cout<<"Enter a food to see when you've eaten it: ";
	string food_name;
	cin>>food_name;
	sqlite3* db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);
	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;
		int status_of_prep = sqlite3_prepare_v2(db, "SELECT FoodName, Date, Quantity FROM FoodEatenByUser WHERE Date IN ( SELECT Date FROM FoodEatenByUser WHERE FoodName = ?1)",
			-1, &my_statement, NULL);
		sqlite3_bind_text(my_statement, 1, food_name.c_str(), -1, SQLITE_STATIC);
		if(status_of_prep == SQLITE_OK){
			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				string food = ((char*)sqlite3_column_text(my_statement, 0));
				string date = ((char*)sqlite3_column_text(my_statement, 1));
				int quantity = sqlite3_column_int(my_statement, 2);
				cout<<"On "<<date<<" you ate "<<quantity<<" servings of "<<food<<"."<<endl;
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
		else
			cout<<"problem 1"<<endl;
	}
	else
		cout<<"problem 2"<<endl;
	sqlite3_close(db);
}	

int getCalByCat(const string& u_n, const string& da){	
	int total_cal = 0;
	sqlite3* db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);
	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;
		int status_of_prep = sqlite3_prepare_v2(db, "SELECT Category.Name, Food.Calories*FoodEatenByUser.Quantity FROM Category, Food, FoodEatenByUser WHERE Category.CategoryID = Food.CategoryID AND FoodEatenByUser.Date = ?2 AND Food.FoodName = FoodEatenByUser.FoodName AND FoodEatenByUser.UserName = ?1 AND Category.CategoryID IN(SELECT CategoryID FROM Category)",
			-1, &my_statement, NULL);
		sqlite3_bind_text(my_statement, 1, u_n.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 2, da.c_str(), -1, SQLITE_STATIC);
		if(status_of_prep == SQLITE_OK){
			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				string cat_name = ((char*)sqlite3_column_text(my_statement, 0));
				int cals = sqlite3_column_int(my_statement, 1);
				cout<<"You took in "<<cals<<" calories of "<<cat_name<<"."<<endl;
				total_cal += cals;
				status_of_step=sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
		else
			cout<<"problem 1"<<endl;
	}
	else
		cout<<"problem 2"<<endl;
	sqlite3_close(db);
	return total_cal;
}		//sort by descending calorie count

void getInfoAboutDay(const string& u_n){
	system("cls");
	cout<<"Enter a date to get info for (yyyy-mm-dd): ";
	string date;
	cin>>date;
	//Enter in date and show all food eaten on that date, cal count by cat (see below) and total cal count for the day
	getFoodOnDate(u_n, date);	
	int total_calories = getCalByCat(u_n, date);
	cout<<"For a total of "<<total_calories<<" calories."<<endl;
}

void getFoodOnDate(const string& u_n, const string& date){
	sqlite3* db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);
	if(status_of_open == SQLITE_OK){
		cout<<"Succesfully opene the databse"<<endl;
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "SELECT FoodName, Quantity FROM FoodEatenByUser WHERE Date = ?1 AND UserName = ?2", -1, &my_statement, NULL);
		sqlite3_bind_text(my_statement, 1, date.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 2, u_n.c_str(), -1, SQLITE_STATIC);
		if(status_of_prep == SQLITE_OK){
			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				string name = ((char*)sqlite3_column_text(my_statement, 0));					
				int quant = sqlite3_column_int(my_statement, 1);
				cout<<"You ate "<<quant<<" servings of "<<name<<endl;
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
		else
			cout<<"Problem 1"<<endl;
	}
	else
		cout<<"Problem 2"<<endl;	
	sqlite3_close(db);
}

void updateQuant(const string& u_n, const string& fn, int quantity, const string& da){
	int quant = quantity+1;
	sqlite3* db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);
	if(status_of_open == SQLITE_OK){
		cout<<"Succesfully opened the database"<<endl;
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "UPDATE FoodEatenByUser SET Quantity = ?1 WHERE  UserName = ?2 AND FoodName = ?3 AND Date = ?4",-1, &my_statement, NULL);
		sqlite3_bind_int(my_statement, 1, quant);
		sqlite3_bind_text(my_statement, 2, u_n.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 3, fn.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 4, da.c_str(), -1, SQLITE_STATIC);
		if(status_of_prep == SQLITE_OK){
			cout<<"Sucessfully inserted into the database"<<endl;
			int status_of_step = sqlite3_step(my_statement);
			if(status_of_step == SQLITE_DONE){
				cout<<"Succesfully inserted into the database"<<endl;
			}
			else
				cout<<"Problem inserting into the database"<<endl;
		}
	}
	else
		cout<<"Couldn't open the databse"<<endl;
}

void addFoodByDate(string fn, string date, const string& u_n){
	int quant = getFoodDateQuantity(u_n, fn, date);
	if(quant == 0){
		sqlite3* db;
		int status_of_open = sqlite3_open(db_path.c_str(), &db);				//add a check for quantity in here then put it into a variable then update quantity		if(status_of_open == SQLITE_OK){
			cout<<"Succesfully opened the databse"<<endl;
			sqlite3_stmt* my_statement;

			int status_of_prep = sqlite3_prepare_v2(db, "INSERT INTO FoodEatenByUser (UserName,FoodName,Date,Quantity) VALUES (?1,?2,?3,?4)", -1, &my_statement, NULL);

			sqlite3_bind_text(my_statement, 1, u_n.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(my_statement, 2, fn.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(my_statement, 3, date.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_int(my_statement, 4, 1);
			if(status_of_prep == SQLITE_OK){
				cout<<"Sucessfully inserted into the databse"<<endl;
				int status_of_step = sqlite3_step(my_statement);
				if(status_of_step == SQLITE_DONE){
					cout<<"Succesfully inserted into the databse"<<endl;
				}
				else
					cout<<"Problem inserting into the databse"<<endl;
			}
			else
				cout<<"Couldn't open the databse"<<endl;
		}
	else{
		//update quantity
		updateQuant(u_n, fn, quant, date);
	}	
	
}

void addFood(string fn, int cal, int cat_id){
	sqlite3* db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		cout<<"Succesfully opened the database"<<endl;
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "INSERT INTO Food (FoodName,Calories,CategoryID) VALUES (?1,?2,?3)", -1, &my_statement, NULL); //functionality will need to be added to get category id from a category name, or display category id on the side perhaps

		sqlite3_bind_text(my_statement, 1, fn.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(my_statement, 2, cal);
		sqlite3_bind_int(my_statement, 3, cat_id);

		if(status_of_prep == SQLITE_OK){
			cout<<"Successfully inserted into the database"<<endl;
			int status_of_step = sqlite3_step(my_statement);

			if(status_of_step == SQLITE_DONE){
				cout<<"Successfully inserted into the databse"<<endl;
			}
			else
				cout<<"Problem inserting into the database"<<endl;
		}
		else
			cout<<"Couldn't open the database"<<endl;
	}
}

void addCategories(string category){
	sqlite3* db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		cout<<"Successfully opened the databse"<<endl;
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "INSERT INTO Category (Name) VALUES (?1)", -1, &my_statement, NULL);

		sqlite3_bind_text(my_statement, 1, category.c_str(), -1, SQLITE_STATIC);

		if(status_of_prep == SQLITE_OK){
			cout<<"Successfully inserted into the databse"<<endl;
			int status_of_step = sqlite3_step(my_statement);

			if(status_of_step == SQLITE_DONE){
				cout<<"Successfully inserted into the databse"<<endl;
			}
			else
				cout<<"Problem inserting into the databse"<<endl;		
		}
		else
			cout<<"Couldn't open the databse"<<endl;
	}
}

void updateCalorie(string food_name, int new_cal){
	sqlite3* db;

	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		cout<<"Succesfully opened the database"<<endl;
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "UPDATE Food SET Calories = ?1 WHERE  FoodName = ?2", -1, &my_statement, NULL);

		sqlite3_bind_int(my_statement, 1, new_cal);
		sqlite3_bind_text(my_statement, 2, food_name.c_str(), -1, SQLITE_STATIC);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a preprated statement"<<endl;
			int status_of_step = sqlite3_step(my_statement);
			if(status_of_step == SQLITE_DONE){
				cout<<"Sucessfully inserted into the databse"<<endl;
			}
			else
				cout<<"Problem inserting into the databse"<<endl;
		}
	}
	else
		cout<<"Couldn't open the databse"<<endl;
}

void insertCategories(string cat_name){
	sqlite3* db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);
	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;
		int status_of_prep = sqlite3_prepare_v2(db, "INSERT INTO Category (Name) VALUES (?1)", -1, &my_statement, NULL);
		sqlite3_bind_text(my_statement, 1, cat_name.c_str(), -1, SQLITE_STATIC);
		if(status_of_prep == SQLITE_OK){
			int status_of_step = sqlite3_step(my_statement);
			if(status_of_step == SQLITE_DONE){
				cout<<"success"<<endl;
			}
			else
				cout<<"problem"<<endl;
		}
	}
	else
		cout<<"couldn't open"<<endl;
}

void addUser(string u_n, string f, string l){
	sqlite3* db;

	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		cout<<"Succesfully opened the database"<<endl;
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "INSERT INTO User (UserName,First,Last) VALUES (?1,?2,?3)",
			-1, &my_statement, NULL);
		
		sqlite3_bind_text(my_statement, 1, u_n.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 2, f.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 3, l.c_str(), -1, SQLITE_STATIC);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a prepared statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);

			if(status_of_step == SQLITE_DONE){
				cout<<"Successfully inserted into the databse"<<endl;
			}
			else
				cout<<"Problem inserting into the databse"<<endl;
		}
	}
	else
		cout<<"Couldn't open the database"<<endl;
}

int getTodayCalorie(const string& u_n, const string& da){
	int cals = 0;
	sqlite3 *db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);
	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;
		int status_of_prep = sqlite3_prepare_v2(db, "SELECT SUM(Food.Calories*FoodEatenByUser.Quantity) FROM Food, FoodEatenByUser WHERE FoodEatenByUser.FoodName = Food.FoodName AND FoodEatenByUser.UserName = ?1 AND FoodEatenByUser.Date = ?2",
			-1, &my_statement, NULL);
		sqlite3_bind_text(my_statement, 1, u_n.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 2, da.c_str(), -1, SQLITE_STATIC);
		if(status_of_prep == SQLITE_OK){
			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				cals = sqlite3_column_int(my_statement, 0);
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
	}
	return cals;
}

void calBetweenDates(const string& u_n){
	int total_cals = 0;
	cout<<"Enter starting date (yyyy-mm-dd): ";
	string date1, date2;
	cin>>date1;
	cout<<"Enter ending date (yyyy-mm-dd): ";
	cin>>date2;
	sqlite3* db;
	int status_of_open = sqlite3_open(db_path.c_str(), & db);
	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;
		int status_of_prep = sqlite3_prepare_v2(db, "SELECT Category.Name, FoodEatenByUser.Quantity*Food.Calories FROM FoodEatenByUser, Food, Category WHERE FoodEatenByUser.Date BETWEEN ?1 AND ?2 AND FoodEatenByUser.UserName = ?3 AND Food.FoodName = FoodEatenByUser.FoodName AND Food.CategoryID = Category.CategoryID ORDER BY FoodEatenByUser.Quantity*Food.Calories DESC",
			-1, &my_statement, NULL);
		sqlite3_bind_text(my_statement, 1, date1.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 2, date2.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(my_statement, 3, u_n.c_str(), -1, SQLITE_STATIC);
		if(status_of_prep == SQLITE_OK){
			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				string name = ((char*)sqlite3_column_text(my_statement, 0));
				int cals = sqlite3_column_int(my_statement, 1);
				cout<<name<<" "<<cals<<endl;
				total_cals += cals;
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
	}
	sqlite3_close(db);
	cout<<"You took in "<<total_cals<<" total calories."<<endl;
}

int getFoodDateQuantity(const string& u_n, string fn, const string& da){
	int quant = 0;
	sqlite3 *db;
	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "SELECT * FROM FoodEatenByUser", -1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a prepared statement"<<endl;
			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){				
				int x = sqlite3_column_int(my_statement, 3);	
				string user = ((char*)sqlite3_column_text(my_statement, 0));
				string food = ((char*)sqlite3_column_text(my_statement, 1));
				string date = ((char*)sqlite3_column_text(my_statement, 2));
				if(u_n == user && fn == food && da == date)
					quant = x;
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
		else
			cout<<"Problem creating a prepared statement"<<endl;
	}
	else
		cout<<"Problem opening the databse"<<endl;
	sqlite3_close(db);
	return quant;
}	   //might have issues with date in here

bool checkFood(string food){
	sqlite3 *db;
	bool found = false;

	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "SELECT FoodName FROM Food", -1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a prepared statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				string f = ((char*)sqlite3_column_text(my_statement, 0));
				if(f == food){
					found = true;
					break;
				}
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
		else
			cout<<"Problem creating a prepared statement"<<endl;
	}
	else
		cout<<"Problem opening the databse"<<endl;
	sqlite3_close(db);
	return found;
}

bool checkUser(string u){
	sqlite3 *db;
	bool found = false;

	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "SELECT UserName FROM User", -1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a preprared Statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				string use_name = ((char*)sqlite3_column_text(my_statement, 0));
				if(u == use_name){
					found = true;
					break;
				}			
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
		else
			cout<<"Problem creating a prepared statement"<<endl;
	}
	else
		cout<<"Problem opening the database"<<endl;
	sqlite3_close(db);
	return found;
}	

void showCategories(){
	sqlite3 *db;

	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "SELECT * FROM Category", -1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Succesfully created a prepared statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				int cid = sqlite3_column_int(my_statement, 0);
				string cat = ((char*)sqlite3_column_text(my_statement, 1));
				cout<<cid<<".) "<<cat<<endl;
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
		else
			cout<<"Problem creating a prepared statement"<<endl;
	}
	else 
		cout<<"Problem opening the database"<<endl;
	sqlite3_close(db);
}

void printUsers(){
	sqlite3 *db;

	int status_of_open = sqlite3_open(db_path.c_str(), &db);

	if(status_of_open == SQLITE_OK){
		sqlite3_stmt* my_statement;

		int status_of_prep = sqlite3_prepare_v2(db, "SELECT * FROM User", -1, &my_statement, NULL);

		if(status_of_prep == SQLITE_OK){
			cout<<"Successfully created a prepared statement"<<endl;

			int status_of_step = sqlite3_step(my_statement);
			while(status_of_step == SQLITE_ROW){
				string u = ((char*)sqlite3_column_text(my_statement, 0));
				string f = ((char*)sqlite3_column_text(my_statement, 1));
				string l = ((char*)sqlite3_column_text(my_statement, 2));
				
				cout<<u<<"  "<<f<<"  "<<l<<endl;
				status_of_step = sqlite3_step(my_statement);
			}
			sqlite3_finalize(my_statement);
		}
		else
			cout<<"Problem creating a prepared statement"<<endl;
	}
	else
		cout<<"Problem opening the databse"<<endl;
	sqlite3_close(db);
}






