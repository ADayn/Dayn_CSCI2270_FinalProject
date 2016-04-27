#include "MovieTree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string.h>

int numOptions;

void showMenu() {
  std::vector<std::string> optionList = {"Find a movie", "Rent a movie", "Print the inventory", "Delete a movie", "Count the movies", "Add a movie (Red-Black)", "Add a movie (regular)", "LOT", "ASCII Print", "Left rotate movie", "Right rotate movie", "Quit"};
  std::cout<<"======Main Menu======"<<std::endl;
  // Fill numOptions with the number of options in optionList
  numOptions = optionList.size();
  for(int i = 0; i < numOptions; ++i){
    std::cout<<i + 1<<". "<<optionList.at(i)<<std::endl;
  }
  return;
}


/* - Takes a database file in the form of:
 *     1,Shawshank Redemption,1994,45
 *     2,The Godfather,1972,34
 *     Rank,Movie Title,Year,Inventory
 *   and makes a binary tree based database from it
 *
 * - File name should be given as only command line argument,
 *   otherwise defaults to "Assignment6Movies.txt"
 */

// FIXME FIXME FIXME FIXME
// Comment everything
int main(int argc, char * argv[]){
  // default file name to "Assignment6Movies.txt"
  bool rb = false;
  std::string file = "Assignment6Movies.txt";
  if (argc == 2){
    if(strcmp(argv[1], "-rb") == 0){
      rb = true;
    }
    else{
      file = argv[1];
    }
  }
  else if (argc == 3){
    if (strcmp(argv[1], "-rb") == 0){
      file = argv[2];
      rb = true;
    }
    else if (strcmp(argv[2], "-rb") == 0){
      file = argv[1];
      rb = true;
    }
    else {
      std::cout<<"Error in command line arguments, using defaults"<<std::endl;
    }
  }
  else if (argc > 3){
    std::cout<<"Usage:\n";
    std::cout<<" Call with no arguments to default to the file \"Assignment6Movies.txt\", with one command line argument of the name of the file to parse, and add \"-rb\" to build the initial tree using a red black tree algorithm.\n";
    std::cout<<" ~$"<<argv[0]<<" MovieDatabaseFile.txt"<<std::endl;
    return -1;
  }
  std::ifstream database(file.c_str());
  if (!database.is_open()) {
    std::cout<<"File ("<<file<<") could not be opened"<<std::endl;
    return -1;
  }
  movieTree movieDatabase;
  std::string tmpString, title, rank, year, num;
  std::stringstream mss;
  int i = 0;
  // Read the file and populate the tree
  while(std::getline(database, tmpString)) {
    ++i;
    mss.str("");
    mss.clear();
    mss<<tmpString;
    std::getline(mss, rank, ',');
    std::getline(mss, title, ',');
    std::getline(mss, year, ',');
    std::getline(mss, num, ',');
    try{
      if (std::stoi(rank) < 0){
        std::cout<<"Movie \""<<title<<"\" has malformed input, rank must be positive (line "<<i<<")\nExiting"<<std::endl;
        return -1;
      }
      if (std::stoi(num) < 0){
        std::cout<<"Movie \""<<title<<"\" has malformed input, number in inventory must be 0 or greater (line "<<i<<")\nExiting"<<std::endl;
        return -1;
      }
      if(rb) {
        movieDatabase.addRB(title, std::stoi(rank), std::stoi(year), std::stoi(num));
      }
      else {
          movieDatabase.add(title, std::stoi(rank), std::stoi(year), std::stoi(num));
        }
    }
    catch(std::invalid_argument& e){
      std::cout<<"Malformed database file (line "<<i<<"):\n";
      std::cout<<"Expected: Rank,Movie Title 1,Year,Inventory\n";
      std::cout<<"Recieved: "<<tmpString<<'\n';
      std::cout<<"Exiting"<<std::endl;
      return -1;
    }
    title = rank = year = num = tmpString = "";
  }
  int option = 0;
  bool running = true;
  std::cin.exceptions(std::ifstream::failbit|std::ifstream::badbit);
  tryAgain:
  try{
    while(running) {
      showMenu();
      do{
        option = getInt();
        if (option < 1 || option > numOptions) std::cout<<"Enter a number between 1 and "<<numOptions<<":"<<std::endl;
      } while (option < 1 || option > numOptions);
      switch (option){
        case 1:
        // Find Movie
        std::cout<<"Enter title:"<<std::endl;
        std::getline(std::cin, title);
        movieDatabase.findMovie(title);
        break;

        case 2:
        // Rent Movie
        std::cout<<"Enter title:"<<std::endl;
        std::getline(std::cin, title);
        movieDatabase.rent(title);
        break;

        case 3:
        // Print Inventory
        movieDatabase.printInventory();
        break;

        case 4:
        // Delete Movie
        std::cout<<"Enter title:"<<std::endl;
        std::getline(std::cin, title);
        movieDatabase.deleteMovie(title);
        break;

        case 5:
        // Print count
        std::cout<<"Tree contains: "<<movieDatabase.countInventory()<<" movies."<<std::endl;
        break;

        case 6:
        // Add a movie (RB)
        std::cout<<"Enter title:"<<std::endl;
        std::getline(std::cin, title);
        std::cout << "Enter year:" << std::endl;
        std::getline(std::cin, year);
        std::cout<<"Enter rank:"<<std::endl;
        std::getline(std::cin, rank);
        std::cout << "Enter inventory:" << std::endl;
        std::getline(std::cin, num);
        movieDatabase.addRB(title, std::stoi(year), std::stoi(rank), std::stoi(num));
        break;

        case 7:
        // Add a movie
        std::cout<<"Enter title:"<<std::endl;
        std::getline(std::cin, title);
        std::cout << "Enter year:" << std::endl;
        std::getline(std::cin, year);
        std::cout<<"Enter rank:"<<std::endl;
        std::getline(std::cin, rank);
        std::cout << "Enter inventory:" << std::endl;
        std::getline(std::cin, num);
        movieDatabase.add(title, std::stoi(year), std::stoi(rank), std::stoi(num));
        break;

        case 8:
        // LOT
        movieDatabase.LOT();
        break;

        case 9:
        // ASCII print
        movieDatabase.printTree();
        break;

        case 10:
        // Left rotate
        std::cout<<"Enter title:"<<std::endl;
        std::getline(std::cin, title);
        movieDatabase.leftRotate(title);
        break;

        case 11:
        // Right rotate
        std::cout<<"Enter title:"<<std::endl;
        std::getline(std::cin, title);
        movieDatabase.rightRotate(title);
        break;

        case 12:
        // Quit
        goto quit;

        default:
        std::cout<<"ERROR:\nReceived option: "<<option<<" (not an option)"<< std::endl;
        break;
      }
    }
  }
  catch (std::ifstream::failure e){
    std::cerr<<"Error recieved: "<<e.what()<<std::endl;
  }
  catch(std::invalid_argument& e){
    std::cout<<"Invalid number recieved for year, rank, or Inventory\n";
    goto tryAgain;
  }
  quit:
  std::cout<<"Goodbye!"<<std::endl;
  return 0;
}
