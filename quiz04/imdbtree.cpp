// https://bridges-cs.herokuapp.com/assignments/1/inflmts
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include "Bridges.h"
#include "DataSource.h"
#include "BSTElement.h"
using namespace std;
using namespace bridges;

/*
  Step 1: Get the Bridges USER ID and API KEY for your program
          by creating an account and signing up (You can use any
          email): https://bridges-cs.herokuapp.com/login

  Step 2: User ID (Username) and Key (API Sha1 Key) can be
          found under Profile on the top left
*/

BSTElement<string, string> *insertIntoBST(string actor, string movie, BSTElement<string, string> *root)
{
    if (root == nullptr)
    {
        root = new BSTElement<string, string>(actor);
        root->setLabel(movie);
        return root;
    }

    if (actor > root->getKey())
    {
        root->setRight(insertIntoBST(actor, movie, root->getRight()));
    }
    else
    {
        root->setLeft(insertIntoBST(actor, movie, root->getLeft()));
    }

    return root;
}

int main(int argc, char **argv)
{
    /*
       Step 3: Test if the following print statement is being run
    */

    cout << "Bridges: IMDB Data in a BST\n";

    /*
       Step 4: Create a Bridges object by uncommenting the next line
               and inserting the values from steps 1 and 2
    */

    // use $FORCE_BRIDGES_APIKEY
    Bridges bridges(1, "inflmts", "");

    /*
       Step 5: Import IMDB data into a vector<ActorMovieIMDB> by
               referring to the Bridges IMDB documentation:
               https://bridgesuncc.github.io/tutorials/Data_IMDB.html
    */

    DataSource ds(&bridges);
    vector<ActorMovieIMDB> entries = ds.getActorMovieIMDBData(1814);
    cout << "Loaded " << entries.size() << " entries\n";

    /*
       Step 6: Open the file "insertIntoBST.txt" and copy the provided
               function for inserting an actor/movie pair into a BST.
               Paste the function into this file above your "main" function.
    */

    /*
       Step 7: Use the insertion function from the previous step to insert
               any 100 actor/movie pairs into a Bridges BST. Refer to the
               Bridges IMDB documentation:
               https://bridgesuncc.github.io/tutorials/Data_IMDB.html
    */

    BSTElement<string, string> *root = nullptr;
    for (int i = 0; i < 100; i++) {
        ActorMovieIMDB& entry = entries[114 + i * 11];
        root = insertIntoBST(entry.getActor(), entry.getMovie(), root);
    }

    /*
       Step 8: Visualize this tree by referring to the Bridges BST documentation:
               https://bridgesuncc.github.io/tutorials/BinarySearchTree.html
    */

    //bridges.setDataStructure(root);
    //bridges.visualize();

    /*
       Step 9: Color each level of the tree using a level-order traversal.
               Every node in a given level should have the same color.
               Do not use the same color in two consecutive levels. A starter
               queue has been provided in case you wish to use an iterative
               implementation of BFS.

               Refer to the Bridges BST Styling documentation:
               https://bridgesuncc.github.io/tutorials/BinarySearchTree.html
    */

    queue<BSTElement<string, string> *> q;
    Color color(0xff, 0x78, 0x00);
    q.push(root);
    q.push(nullptr);
    while (q.size() > 1) {
        auto node = q.front();
        if (node) {
            auto left = node->getLeft();
            auto right = node->getRight();
            if (left)
                q.push(left);
            if (right)
                q.push(right);
            node->setColor(color);
        } else {
            color.setValue(
                (color.getGreen() + 7) & 0xff,
                (color.getBlue() + 15) & 0xff,
                (color.getRed() + 23) & 0xff
            );
            q.push(nullptr);
        }
        q.pop();
    }

    /*
      Step 10: Visualize the updated tree. Comment out or remove the old
               visualization code from Step 8
    */

    bridges.setDataStructure(root);
    bridges.visualize();

    return 0;
}

// vim:sw=4
