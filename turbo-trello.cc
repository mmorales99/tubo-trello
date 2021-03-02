#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

struct Date{
  int day;
  int month;
  int year;
};

struct Task{
  string name;
  Date deadline;
  bool isDone;
  int time;
};

struct List{
  string name;
  vector<Task> tasks;
};

struct Project{
  int id;
  string name;
  string description;
  vector<List> lists;
};

enum Error{
  ERR_OPTION,
  ERR_EMPTY,
  ERR_LIST_NAME,
  ERR_TASK_NAME,
  ERR_DATE,
  ERR_TIME
};

void error(Error e){
  switch(e){
    case ERR_OPTION:
      cout << "ERROR: wrong menu option" << endl;
      break;
    case ERR_EMPTY:
      cout << "ERROR: empty string" << endl;
      break;
    case ERR_LIST_NAME:
      cout << "ERROR: wrong list name" << endl;
      break;
    case ERR_TASK_NAME:
      cout << "ERROR: wrong task name" << endl;
      break;
    case ERR_DATE:
      cout << "ERROR: wrong date" << endl;
      break;
    case ERR_TIME:
      cout << "ERROR: wrong expected time" << endl;
  }
}

void showMainMenu(){
  cout << "1- Edit project" << endl
       << "2- Add list" << endl
       << "3- Delete list" << endl 
       << "4- Add task" << endl
       << "5- Delete task" << endl
       << "6- Toggle task" << endl
       << "7- Report" << endl
       << "q- Quit" << endl
       << "Option: ";
}


//////////////////////////////////////////////////////
//				Funciones auxiliares				//
//////////////////////////////////////////////////////

unsigned int ctoi(const char &c){
	return c -((int)'0');
}

bool isValidDate(const unsigned int &d,const unsigned int &m,const unsigned int &y){
	bool valid = false;
	const unsigned int diasxmes[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(m>=1 && m<=12) valid = true; 
	if(y>=2000 && y<=2100) valid = true;
	if(y%100==0 && y%400==0 && d>0 && d<=(diasxmes[m-1]+(m==2?1:0)) ) valid=true;
	else if(y%4==0 && d>0 && d<=(diasxmes[m-1]+(m==2?1:0))) valid=true; 
	else if(d>0&&d<=diasxmes[m-1]) valid=true;
	else valid = false;	
	return valid;
}

bool isValidDate(const Date &d){
	return isValidDate(d.day,d.month,d.year);
}

Date new_Date(const unsigned int &d,const unsigned int &m,const unsigned int &y){
	if(!isValidDate(d,m,y)){ error(ERR_DATE);}
	Date D;
	D.day = (int)d;
	D.month = (int)m;
	D.year = (int)y;
	return D;
}

Date new_Date(const string &date_str){

	// +----------------------------+
	// -Ya existen funciones en C++ que te hacen esto
	/*
	unsigned int total_separators = 0; // se asume que los separadores posibles son "-:/\|" para los posibles formatos ddmmyyyy|mmddyyyy|yyyymmdd|yyyyddmm
	#define SEP_LIST_TOTAL 5
	char separator_list[SEP_LIST_TOTAL]={'-',':','/','\\','|'};
	for(size_t i = 0;i<date_str.size();i++){ 
		for(size_t j = 0;j<SEP_LIST_TOTAL;j++)
			if(date_str[i]==separator_list[j]){ total_separators++; date_str[i]='/';}
	}
	if(total_separators!=2) exit(EXIT_FAILURE); // si el formate de entrada de la fecha no es correcta, se deberia lanzar una excepcion
	*/
	// +----------------------------+
	unsigned int d = 0;
	unsigned int m = 0;
	unsigned int y = 0;
	int i = -1;
	while(i++<2){
		if(date_str[i]=='/')  // asumimos que siempre vamos a tener el mismo separador
			break;
		d=d*10 + ctoi(date_str[i]);
	}
	while(i++<4){
		if(date_str[i]=='/')  // asumimos que siempre vamos a tener el mismo separador
			break;
		m=m*10 + ctoi(date_str[i]);
	}
	while(i++<8){
		if(date_str[i]=='/')  // asumimos que siempre vamos a tener el mismo separador
			break;
		y=y*10 + ctoi(date_str[i]);
	}
	return new_Date(d,m,y);
}

Task new_Task(const string &name, const Date &dl, const int &t){
	Task T;
	T.name = name;
	T.deadline = dl;
	T.isDone = false;
	T.time = t;
	return T;
}

List new_List(const string &str){
	List l;
	l.name = str;
	l.tasks.clear();
	return l;
}

void List_setName(List &l, const string &str){
	l.name = str;
}

//////////////////////////////////////////////////////
//	Funcion que busca una lista dado un nombre		//
//	param 1 -> p : proyecto donde buscar la lista	//
//	param 2 -> tofind : nombre de la lista buscada	//
//	return int -> index de la lista (-1 si no esta)	//
//////////////////////////////////////////////////////
int findList(const Project &p, const string &tofind){
	int index = -1;
	for(size_t i = 0;i<p.lists.size();i++)
		if(p.lists.at(i).name == tofind) index = i;
	return index;
}

//////////////////////////////////////////////////////
//	Funcion que busca una tarea dado un nombre		//
//	param 1 -> l : lista donde buscar la tarea		//
//	param 2 -> tofind : nombre de la tarea buscada	//
//	return int -> index de la tarea (-1 si no esta)	//
//////////////////////////////////////////////////////
int findTask(const List &l, const string &tofind){
	int index = -1;
	for(size_t i = 0;i<l.tasks.size();i++)
		if(l.tasks.at(i).name == tofind) index = i;
	return index;
}

//////////////////////////////////////////////////////
//				Fin Funciones auxiliares			//
//////////////////////////////////////////////////////


void editProject(Project &toDoList){
	string name;
	do{
		cout<<"Enter project name: ";
		getline(std::cin,name);
		if(name.empty()) error(ERR_EMPTY);
	}while(name.empty()!=false);
	toDoList.name = name;
	string description;
	cout<<"Enter project description: ";
	getline(std::cin,description);
	toDoList.description = description;
}

void addList(Project &toDoList){
	string name;
	do{
		cout<<"Enter list name: ";
		getline(std::cin,name);
		if(name.empty()) error(ERR_EMPTY);
	}while(name.empty()!=false);
	if(findList(toDoList,name)!=-1) error(ERR_LIST_NAME);
	else toDoList.lists.push_back(new_List(name));
	return;
}

void deleteList(Project &toDoList){
	string name;
	do{
		cout<<"Enter list name: ";
		getline(std::cin,name);
		if(name.empty()) error(ERR_EMPTY);
	}while(name.empty()!=false);
	int index = findList(toDoList,name);
	if( index !=-1 )	toDoList.lists.erase(toDoList.lists.begin()+index);
	else error(ERR_LIST_NAME);
	return;
}

void addTask(Project &toDoList){
	string name;
	do{
		cout<<"Enter list name: ";
		getline(std::cin,name);
		if(name.empty()) error(ERR_EMPTY);
	}while(name.empty()!=false);
	int index = findList(toDoList,name);
	if(index==-1){ error(ERR_LIST_NAME); return;}
	cout<<"Enter task name: ";
	name.clear(); getline(std::cin,name);
	string deadline;
	cout<<"Enter deadline: ";
	getline(std::cin,deadline);
	Date d = new_Date(deadline);
	if(!isValidDate(d)) return;
	string expected_time;
	cout<<"Enter expected time: ";
	getline(std::cin,expected_time);
	unsigned int exp_time=0;
	if(__cplusplus == 201103L) exp_time = stoi(expected_time.c_str());
	else exp_time = atoi(expected_time.c_str());
	if(exp_time < 1 || exp_time > 180){ error(ERR_TIME); return;}
	toDoList.lists.at(index).tasks.push_back(new_Task(name, d, exp_time));
}

void deleteTask(Project &toDoList){
	string name;
	do{
		cout<<"Enter list name: ";
		getline(std::cin,name);
		if(name.empty()) error(ERR_EMPTY);
	}while(name.empty()!=false);
	int index = findList(toDoList,name);
	if(index==-1){ error(ERR_LIST_NAME); return;}
	cout<<"Enter task name: ";
	name.clear(); getline(std::cin,name);
	unsigned int deleted = 0;
	for(size_t i = 0;i<toDoList.lists.at(index).tasks.size();i++){
		if(toDoList.lists.at(index).tasks.at(i).name == name) {
			deleted++;
			toDoList.lists.at(index).tasks.erase(toDoList.lists.at(index).tasks.begin()+i);
		}
	}
	if(deleted==0) error(ERR_TASK_NAME);
}

void toggleTask(Project &toDoList){
	string name;
	do{
		cout<<"Enter list name: ";
		getline(std::cin,name);
		if(name.empty()) error(ERR_EMPTY);
	}while(name.empty()!=false);
	int index = findList(toDoList,name);
	if(index==-1){ error(ERR_LIST_NAME); return;}
	cout<<"Enter task name: ";
	name.clear(); getline(std::cin,name);
	unsigned int toggled = 0;
	for(size_t i = 0;i<toDoList.lists.at(index).tasks.size();i++){
		if(toDoList.lists.at(index).tasks.at(i).name==name) {
			toggled++;
			toDoList.lists.at(index).tasks.at(i).isDone = !(toDoList.lists.at(index).tasks.at(i).isDone);
		}
	}
	if(toggled==0) error(ERR_TASK_NAME);
}

void report(const Project &toDoList){
	cout << "Name: " << toDoList.name <<endl;
	if(!toDoList.description.empty()){
		cout << "Description: " << toDoList.description << endl;
	}
	for(size_t i = 0;i<toDoList.lists.size();i++){
		List l = toDoList.lists.at(i); 
		cout << l.name << endl;
		if(l.tasks.size()>0){
			for(size_t j = 0;j<l.tasks.size();j++){
				Task t = l.tasks.at(j);
				cout << "[" << (t.isDone?'X' : ' ') << "] (" << t.time << ") " << t.deadline.year << "-" << t.deadline.month << "-" << t.deadline.day << " : " << t.name << endl;
			}
		}
	}
	unsigned int total_left=0, total_left_counter=0;
	if(toDoList.lists.size()>0){
		for(size_t i=0;i<toDoList.lists.size();i++){
			for(size_t j=0;j<toDoList.lists.at(i).tasks.size();j++){
				if(toDoList.lists.at(i).tasks.at(j).isDone==false){
					total_left_counter+=toDoList.lists.at(i).tasks.at(j).time;
					total_left++;
				}
			}
		}
	}
	cout << "Total left: " << total_left << " (" << total_left_counter << " minutes)" <<endl;
	unsigned int total_done=0, total_done_counter=0;
	if(toDoList.lists.size()>0){
		for(size_t i=0;i<toDoList.lists.size();i++){
			for(size_t j=0;j<toDoList.lists.at(i).tasks.size();j++){
				if(toDoList.lists.at(i).tasks.at(j).isDone==true){
					total_done_counter+=toDoList.lists.at(i).tasks.at(j).time;
					total_done++;
				}
			}
		}
	}
	cout << "Total done: " << total_done << " (" << total_done_counter << " minutes)" <<endl;

	Task highest_priority = new_Task("youngest trackable task",new_Date(31,12,2100),180);
	if(toDoList.lists.size()>0 && total_left>0){
		for(size_t i=0;i<toDoList.lists.size();i++){
			for(size_t j=0;j<toDoList.lists.at(i).tasks.size();j++){
				if(toDoList.lists.at(i).tasks.at(j).isDone==false){
					if(toDoList.lists.at(i).tasks.at(j).deadline.year <=highest_priority.deadline.year ){
						if(toDoList.lists.at(i).tasks.at(j).deadline.month <=highest_priority.deadline.month ){
							if(toDoList.lists.at(i).tasks.at(j).deadline.day <highest_priority.deadline.day )
								highest_priority = toDoList.lists.at(i).tasks.at(j);
						}
					}
				}
			}
		}
		
		cout << "Highest priority: ";
		cout << highest_priority.name << " (" << highest_priority.deadline.year <<'-'<<highest_priority.deadline.month<<'-'<<highest_priority.deadline.day<<")";
		cout << endl;
	}
}

int main(){
  Project toDoList;
  toDoList.id=1;
  char option;
  
  do{
    showMainMenu();
    cin >> option;
    cin.get();
    
    switch(option){
      case '1': editProject(toDoList);
                break;
      case '2': addList(toDoList);
                break;
      case '3': deleteList(toDoList);
                break;
      case '4': addTask(toDoList);
                break;
      case '5': deleteTask(toDoList);
                break;
      case '6': toggleTask(toDoList);
                break;
      case '7': report(toDoList);
                break;
      case 'q': break;
      default: error(ERR_OPTION);
    }
  }while(option!='q');
  
  return 0;    
}
