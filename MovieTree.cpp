#include "MovieTree.h"

/* getInt waits for the user to input an integer and returns it (inorder)
 */
int getInt(){
  int number = 0;
  std::string input;
  std::stringstream numStream;
  // TODO Use exceptions for getline TODO
  while(std::getline(std::cin, input)){
    numStream.str("");
    numStream.clear();
    numStream<<input;
    if(numStream>>number && !(numStream>>input)) return number;
    else std::cout<<"Only enter a number:"<<std::endl;
    std::cin.clear();
  }
  return -1;
}

/* movieTree Constructor
 */
movieTree::movieTree() {
  root = NULL;
}

void addRB(std::string, int, int, int) {
  
}

/* movieTree Destructor
 */
movieTree::~movieTree() {
  movie * tmp = root, * tmpP = NULL;
  while (tmp) {
    // Store the parent before deleteing the node
    tmpP = tmp->parent;
    // Go down left if exists
    if (tmp->left) {
      tmp = tmp->left;
    }
    // Else, go down right if exists
    else if (tmp->right) {
      tmp = tmp->right;
    }
    // Else, leaf hit
    // Delete it and fix its parent pointers
    // go back up
    else {
      if (tmp->parent){
        if (tmp->parent->left == tmp) tmp->parent->left = NULL;
        else tmp->parent->right = NULL;
      }
      std::cout<<"Deleting: "<<tmp->title<<std::endl;
      delete tmp;
      tmp = tmpP;
    }
  }
  return;
}


/* Uses printInfo to print the movie that has the name
 * passed inot title
 */
void movieTree::findMovie(std::string title) {
  if (!root) {
    std::cout<<"No movies in database!"<<std::endl;
    return;
  }
  movie * queryMovie = search(title);
  if (!queryMovie) std::cout<<"Movie not found."<<std::endl;
  else printInfo(queryMovie);
  return;
}

/* If movie is not sold out (num is not 0), prints the
 * info for it using printInfo() and subtracts 1 from
 * its num member. Otherwise prints out of stock
 */

void movieTree::rent(std::string title) {
  if (!root) {
    std::cout<<"No movies in database."<<std::endl;
    return;
  }
  movie * queryMovie = search(title);
  if (!queryMovie) std::cout<<"Movie not found."<<std::endl;
  // Check if in stock, then remove one from inventory
  else if (queryMovie->num) {
    --(queryMovie->num);
    std::cout<<"Movie has been rented."<<std::endl;
    printInfo(queryMovie);
    if(queryMovie->num == 0){
      deleteMovie(title);
    }
  }
  else std::cout<<"Movie out of stock."<<std::endl;
  return;
}

/* Goes through the tree and prints the movies and
 * the inventory of each in alphabetical order
 */
// Inorder tree traversal without recursion
void movieTree::printInventory() {
  if (!root) {
    std::cout<<"No movies in database."<<std::endl;
    return;
  }
  movie * tmp = root;
  while (tmp) {
    // If left exists, traverse it
    if(tmp->left) {
      tmp = tmp->left;
      continue;
    }
    // If right and no left print movie, then traverse right
    else if(tmp->right) {
      std::cout<<"Movie: "<<tmp->title<<" "<<tmp->num<<std::endl;
      tmp = tmp->right;
    }
    // If neither exist, leaf hit
    // Print it
    // Go back up, printing roots, until a right branch needs to be traversed
    else {
      std::cout<<"Movie: "<<tmp->title<<" "<<tmp->num<<std::endl;
      while(tmp->parent){
        // If came up from left:
        // Print root
        if (tmp->parent->left == tmp){
          std::cout<<"Movie: "<<tmp->parent->title<<" "<<tmp->parent->num<<std::endl;
          // Go down right if exists, else keep going up
          if (tmp->parent->right){
            tmp = tmp->parent->right;
            break;
          }
          else{
            tmp = tmp->parent;
            continue;
          }
        }
        // tmp is right of parent, parent has been printed, keep going up until we go up from the left
        else {
          tmp = tmp->parent;
        }
      }
      // If while broke because !tmp->parent, then we hit the root and are done
      if(tmp == root){
        return;
      }
    }
  }
  return;
}

void movieTree::deleteMovie(std::string removieTitle){
  movie * removie = search(removieTitle), * minMovie = NULL;
  if(!removie){
    std::cout<<"Movie not found."<<std::endl;
    return;
  }

  // No children
  if (!removie->left && !removie->right){
    // Root case
    if(!removie->parent){
      root = NULL;
    }
    // Removie is left child of parent
    else if(removie->parent->left == removie){
      removie->parent->left = NULL;
    }
    // Removie is right child of parent
    else{
      removie->parent->right = NULL;
    }
  }

  // Only one child
  else if(!removie->right || !removie->left){
    // Only left child
    if(!removie->right){
      minMovie = removie->left;
    }
    // Only right child
    else{
      minMovie = removie->right;
    }
    // Root case
    if(!removie->parent){
      root = minMovie;
    }
    // Removie is left child of parent
    else if(removie->parent->left == removie){
      removie->parent->left = minMovie;
    }
    // Removie is right child of parent
    else{
      removie->parent->right = minMovie;
    }
    minMovie->parent = removie->parent;
  }

  // Both left and right children
  else{
    minMovie = removie->right;
    // Find replacement movie
    while(minMovie->left){
      minMovie = minMovie->left;
    }
    // Root case
    if(!removie->parent){
      root = minMovie;
    }
    // Removie is left child of parent
    else if(removie->parent->left == removie){
      removie->parent->left = minMovie;
    }
    // Removie is right child of parent
    else{
      removie->parent->right = minMovie;
    }
    // minMovie is not directly after removie
    if(minMovie != removie->right){
      removie->right->parent = minMovie;
      minMovie->parent->left = minMovie->right;
      if(minMovie->right){
        minMovie->right->parent = minMovie->parent;
      }
      minMovie->right = removie->right;
    }
    minMovie->parent = removie->parent;
    minMovie->left = removie->left;
    minMovie->left->parent = minMovie;
  }
  delete removie;
}

/* Goes through the tree and counts movies
 * (the number of different movies, not the
 * number of total coppies), then returns
 * the count
 */
// Inorder tree traversal without recursion
int movieTree::countInventory() {
  if (!root) {
    return 0;
  }
  movie * tmp = root;
  int cnt = 0;
  while (tmp) {
    // If left exists, traverse it
    if(tmp->left) {
      tmp = tmp->left;
      continue;
    }
    // If right and no left count movie, then traverse right
    else if(tmp->right) {
      ++cnt;
      tmp = tmp->right;
    }
    // If neither exist, leaf hit
    // Count it
    // Go back up, counting roots, until a right branch needs to be traversed
    else {
      ++cnt;
      while(tmp->parent){
        // If came up from left:
        // count root
        if (tmp->parent->left == tmp){
          ++cnt;
          // Go down right if exists, else keep going up
          if (tmp->parent->right){
            tmp = tmp->parent->right;
            break;
          }
          else{
            tmp = tmp->parent;
            continue;
          }
        }
        // tmp is right of parent, parent has been printed, keep going up until we go up from the left
        else {
          tmp = tmp->parent;
        }
      }
      // If while broke because !tmp->parent, then we hit the root and are done
      if(tmp == root){
        return cnt;
      }
    }
  }
  return cnt;
}

/* Checks if a movie by the name given is in the tree
 * if it is not, adds the movie to the tree, where it
 * belongs alphabeticaly
 */
void movieTree::add(std::string initTitle, int initRank, int initYear, int initNum) {
  movie * newMovie = new movie(initTitle, initRank, initYear, initNum, root);
  if(!root) {
    root = newMovie;
    return;
  }
  movie * tmp = root;
  // Keep going until end of path is hit
  while(tmp) {
    // If initTitle comes after tmp->title go right
    if(initTitle.compare(tmp->title) > 0) {
      newMovie->parent = tmp;
      tmp = tmp->right;
    }
    // If initTitle comes before tmp->title go left
    else if (initTitle.compare(tmp->title) < 0) {
      newMovie->parent = tmp;
      tmp = tmp->left;
    }
    // else, initTitle = tmp-> title, so movie is already in tree
    else {
      std::cout<<"Movie \""<<initTitle<<"\" already in database"<<std::endl;
      delete newMovie;
      return;
    }
  }
  if (initTitle.compare(newMovie->parent->title) > 0) {
    newMovie->parent->right = newMovie;
  }
  else newMovie->parent->left = newMovie;
  return;
}

void movieTree::printInfo(movie * queryMovie) {
  std::cout<<"Movie Info:"<<std::endl;
  std::cout<<"==========="<<std::endl;
  std::cout<<"Ranking:"<<queryMovie->rank<<std::endl;
  std::cout<<"Title:"<<queryMovie->title<<std::endl;
  std::cout<<"Year:"<<queryMovie->year<<std::endl;
  std::cout<<"Quantity:"<<queryMovie->num<<std::endl;
  return;
}

/* Searches throught the tree for the movie with
 * the given title and returns the pointer to it
 * if it is found, else returns NULL
 */
movie * movieTree::search(std::string title) {
  movie * tmp = root;
  while(tmp) {
    if (!title.compare(tmp->title)) return tmp;
    if (title.compare(tmp->title) > 0) {
      tmp = tmp->right;
    }
    else {
      tmp = tmp->left;
    }
  }
  return tmp;
}

// Level Order Travesal
// Print nodes in level order
// No queue of nodes at each level

void movieTree::LOT(){
  int olvl = 0, ilvl = 0;
  movie * node = root;
  bool found = true;
  if (!node){
    std::cout<<"No movies in tree"<<std::endl;
    return;
  }
  for(node = root; node;){

    // Have not reached the current printing level in the tree
    if(ilvl < olvl){
      // Go down left if can
      if(node->left){
        node = node->left;
        ++ilvl;
        continue;
      }
      // Go down right if can
      else if(node->right){
        node = node->right;
        ++ilvl;
        continue;
      }
      // Go back up if leaf hit
      else{
        // keep going up while node is parent->right or there is not parent->right
        while(node->parent && (node->parent->right == node || !node->parent->right)){
          node = node->parent;
          --ilvl;
        }
        // hit root after going through tree
        if(!ilvl){
          // If no nodes printed at olvl, reached end of tree
          if(!found){
            std::cout<<std::endl;
            return;
          }
          // Else. continue to next level of tree
          else{
            olvl++;
            found = false;
            node = root;
            std::cout<<"\b\b"<<std::endl;
            continue;
          }
        }
        // node->parent->right hasn't been visited
        else{
          node = node->parent->right;
        }
      }
    }

    // Now at a node on the printing level
    else if(ilvl == olvl){
      found = true;
      // Print node
      if(node->parent && node->parent->left == node){
        std::cout << std::setw(2) << node->parent->rank << "->L: ";
      }
      else if(node->parent){
        std::cout << std::setw(2) << node->parent->rank << "->R: ";
      }
      std::cout << std::setw(2) << node->rank << "  ";
      // Go back up to next branch to be traversed
      // keep going up while node is parent->right or there is not parent->right
      while(node->parent && (node->parent->right == node || !node->parent->right)){
        node = node->parent;
        --ilvl;
      }
      // hit root after going through tree
      if(!ilvl){
        olvl++;
        found = false;
        node = root;
        std::cout<<"\b\b"<<std::endl;
        continue;
      }
      // node->parent->right hasn't been visited
      else{
        node = node->parent->right;
      }
    }

    // ilvl somehow went over olvl, error
    else{
      std::cout<<"Error occured, ilvl ("<<ilvl<<") passed olvl ("<<olvl<<")."<<std::endl;
      return;
    }
  }
}


/* find's the maximum depth of the tree */
int movieTree::treeDepth(){
  movie * node = root;
  int olvl = 0, ilvl = 0;
  if(!node) return -1;
  while(node){

    // Check left tree
    if(node->left){
      node = node->left;
      ++ilvl;
      if(ilvl > olvl){
        olvl = ilvl;
      }
    }

    // Check right tree
    else if (node->right){
      node = node->right;
      ++ilvl;
      if(ilvl > olvl){
        olvl = ilvl;
      }
    }

    // Leaf hit, go back up looking for next unvisited tree
    else {
      // keep going up while node is parent->right or there is not parent->right
      while(node->parent && (node->parent->right == node || !node->parent->right)){
        node = node->parent;
        --ilvl;
      }
      // hit root after going through tree
      if(!ilvl){
        break;
      }
      // node->parent->right hasn't been visited
      else{
        node = node->parent->right;
      }
    }
  }
  ++olvl;
  return olvl;
}


std::tuple<int, std::string, int> movieTree::ntos(movie * node){
  if(!node) {
    return std::make_tuple(0, "", 0);
  }
  std::string str = "(";
  int left = 0, right = 0;
  str += std::to_string(node->rank);
  str += ")";
  if (node->left) {
    left = std::to_string(node->left->rank).length();
    str.insert(0, ".-");
    for (int i = 0; i < left; ++i) {
      str.insert(0, " ");
    }
    left += 2;
  }
  if (node->right) {
    right = std::to_string(node->right->rank).length();
    for (int i = 0; i < right; ++i) {
      str += "-";
    }
    str += ". ";
    right  += 2;
  }
  return std::make_tuple(left, str, right);
}

void movieTree::printTree(){
  // olvl: current level being printed
  // ilvl: current level of node
  // left: unaccounted for chars to left of node (branch and spaces)
  // right: unaccounted for chars to right of node (branch)
  // index: index of node in upper string/ level
  // lvl: array of levels represented as strings
  int olvl = 0, ilvl = 0, depth = treeDepth(), left = 0, right = 0;
  size_t index = 0;
  std::string * lvl = new std::string[depth];
  std::string tmpS;
  movie * node = root;

  if (!node){
    std::cout<<"No movies in tree"<<std::endl;
    return;
  }

  // Go until last level of tree
  while(olvl < depth){

    // Have not reached the current printing level in the tree
    if(ilvl < olvl){
      // Go down left if can
      if(node->left){
        node = node->left;
        ++ilvl;
        continue;
      }
      // Go down right if can
      else if(node->right){
        node = node->right;
        ++ilvl;
        continue;
      }
      // Go back up if leaf hit and not at level
      else{
        // keep going up while node is parent->right or there is not parent->right
        while(node->parent && (node->parent->right == node || !node->parent->right)){
          node = node->parent;
          --ilvl;
        }
        // hit root after going through tree
        if(!ilvl){
          olvl++;
          node = root;
        }
        // node->parent->right hasn't been visited
        else{
          node = node->parent->right;
        }
      }
    }

    // Now at a node on the inserting level
    else if(ilvl == olvl){
      // Printing root
      if(!olvl){
        std::tie(std::ignore, lvl[olvl], std::ignore) = ntos(node);
        ++olvl;
        continue;
      }
      // Print node:
      // find index of next node in lvl[olvl - 1] (next period)
      // insert lvl[olvl][index - 1] char at index left number of times
      // insert lvl[olvl][index + 1 + left] char at index + left right number of times
      std::tie(left, tmpS, right) = ntos(node);
      index = lvl[olvl - 1].find(".", lvl[olvl].length());
      // If a node was found without corresponding pointer to it, error occured
      if(index == std::string::npos){
        std::cout<<"Error: node found with no position to place it in\n";
        std::cout<<"level: "<<olvl<<", node: "<<node->rank<<std::endl;
        break;
      }
      // Add to left of new node
      for(int i = olvl - 1; i >= 0; --i){
        if(index <= lvl[i].length()){
          lvl[i].insert(index, left, lvl[i][index - 1]);
        }
      }
      // Add to right of node
      for(int i = olvl - 1; i >= 0; --i){
        if(index + 1 + left <= lvl[i].length()){
          lvl[i].insert(index + 1 + left, right, lvl[i][index + 1 + left]);
        }
      }
      index = index - tmpS.find(")") + left + 1;
      if(lvl[olvl].length() < index){
        lvl[olvl].resize(index, ' ');
      }
      lvl[olvl].insert(index, tmpS);
      // Go back up to next branch to be traversed
      // keep going up while node is parent->right or there is not parent->right
      while(node->parent && (node->parent->right == node || !node->parent->right)){
        node = node->parent;
        --ilvl;
      }
      // hit root after going through tree
      if(!ilvl){
        olvl++;
        node = root;
      }
      // node->parent->right hasn't been visited
      else{
        node = node->parent->right;
      }
    }

    // ilvl somehow went over olvl, error
    else{
      std::cout<<"Error occured, ilvl ("<<ilvl<<") passed olvl ("<<olvl<<")."<<std::endl;
      return;
    }
  }
  for (int i = 0; i < depth; ++i) {
    std::cout<<"Level "<<std::setw(2)<<i + 1<<": ";
    std::cout<<lvl[i]<<std::endl;
  }
  delete[] lvl;
}

void movieTree::leftRotate(movie * x){
  if (!x || !x->right){
    // Throw runtime exception
  }
  movie * y = x->right;
  if (x == root){
    root = y;
  }

}

void movieTree::rightRotate(movie * x){
  if (!x || !x->left){
    // Throw runtime exception
  }
  movie * y = x->left;
  if (x == root){
    root = y;
  }
}
