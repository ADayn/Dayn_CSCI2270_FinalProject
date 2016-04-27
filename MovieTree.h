#ifndef MOVIETREE_H
#define MOVIETREE_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <tuple>

std::string getString();

int getInt();

struct movie {
  std::string title;

  int rank;
  int year;
  int num;
  movie * parent;
  movie * left;
  movie * right;
  bool red;
  bool isRed() {return red;}
  bool isBlack() {return !red;}
  movie() {};
  movie(std::string initTitle, int initRank, int initYear, int initNum, movie * initParent = NULL, movie * initLeft = NULL, movie * initRight = NULL) {
    red = true;
    title = initTitle;
    rank = initRank;
    year = initYear;
    num = initNum;
    parent = initParent;
    left = initLeft;
    right = initRight;
  }
};

class movieTree {
  public:
    movieTree();
    ~movieTree();
    void findMovie(std::string);
    void rent(std::string);
    void printInventory();
    void deleteMovie(std::string);
    int countInventory();
    void add(std::string, int, int, int);
    void addRB(std::string, int, int, int);
    void LOT();
    void printTree();
    void leftRotate(std::string movie);
    void rightRotate(std::string movie);
  protected:
  private:
    void printInfo(movie *);
    movie * search(std::string);
    std::tuple<int, std::string, int> ntos(movie *);
    int treeDepth();
    movie * root;
};

#endif // MOVIETREE_H
