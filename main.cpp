#include <boost/filesystem.hpp>
#include <iostream>
#include <cstring>
#include <fstream>
#include <future>
#include <map>
#include <thread>

using namespace std;
namespace fs = boost::filesystem;

string getFileName(const fs::path& p)
{
    fs::path filePath = p;
    fs::path textFilename = filePath.filename();
    cout << "Поиск имени файла...\t " << endl;
    return textFilename.string();
}

string getFileDate(const fs::path& p)
{
    fs::path filePath = p;
    time_t ftime = fs::last_write_time(filePath);
    cout << "Поиск последней модификации файла...\t"  << endl;
    return asctime(gmtime(&ftime));
}

double getFileSize(const fs::path& p) //получение размера файла
{
	fs::path filePath = p; // Указываем путь к файлу
 	cout << "Поиск размера файла...\t" << endl;
	return fs::file_size(filePath);
}

//логические функции для проверки состояния
bool print_name(const fs::path& p)
 {
  fs::path filePath = p;
  ofstream outfile;
  outfile.open(getenv("EXPLORER_LOG_PATH"), ofstream::out | ofstream::app);
  outfile << "File name: " << getFileName(filePath) << endl;
  outfile.close();
  return true;
}

bool print_date(const fs::path& p)
{
  fs::path filePath = p;
  ofstream outfile;
  outfile.open(getenv("EXPLORER_LOG_PATH"), ofstream::out | ofstream::app);
  outfile << "Latest modification of file: " << getFileDate(filePath) << endl;
  outfile.close();
  return true;
}

bool print_size(const fs::path& p)
{
  fs::path filePath = p;
  ofstream outfile;
  outfile.open(getenv("EXPLORER_LOG_PATH"), ofstream::out | ofstream::app);
  outfile << "File size: " << getFileSize(filePath) / 1000 << " Kb" << endl;
  outfile.close();
  return true;
}


bool print_info(const fs::path& p)
{
  if (!fs::exists(p))	//Проверка существования файла по введенному адресу
  {
    throw runtime_error("File does not find!");
  }

  if (fs::is_directory(p)) 	//Проверка на то,ссылается ли введённый путь на каталог
  {
    throw runtime_error("Type of file - directory");
  }

return true;
}

int main(int argc, char* argv[])
{
  try
  {
    map <const fs::path, future<bool> > results;
    for (int i = 1; i < argc; i++)
    {
      fs::path Path = argv[i];
      auto task = packaged_task<bool()> 
      {
         bind(print_info, Path)
      };
	    auto result = task.get_future();  
	    thread task_td(move(task));
	    task_td.join();    
	    results.emplace(Path, move(result));     
    }

    for(auto & p : results)  
    {
      auto& result = p.second;
      auto ok = result.get();
      if (!ok)
      {
        cerr << "Wrong !!! " << p.first << endl;
      }
    }
  }
  catch(const exception& e)
  {
    cout << e.what() << endl;
  }
  return 0;
}
