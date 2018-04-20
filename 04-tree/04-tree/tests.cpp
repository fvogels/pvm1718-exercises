#define CATCH_CONFIG_MAIN
#include "Catch.h"
#include "tree-set.h"

/*

    For this assignment, you will need to create a set represented internally as a binary search tree.

    The idea behind a binary tree is simple: let's start with the linked list which you are already familiar with.
    A linked list consists of nodes. Each node points to a next node, except for the last one, which points to nullptr.

    [NODE] --> [NODE] --> [NODE] --> ... -> [NODE] --> nullptr

    Now, instead of having each node point to 1 other node, let's have each node point to 2 other nodes instead.
    
    [NODE] --> [NODE] --> [NODE] --> nullptr
       |          |          |
       |          |          +-----> nullptr
       |          |
       |          +-----> [NODE] --> nullptr
       |                     |
       |                     +-----> nullptr
       |
       +------> [NODE] --> nullptr
                   |
                   +-----> [NODE] --> nullptr
                              |
                              +-----> nullptr

    Now, nullptr is actually quite evil (https://www.lucidchart.com/techblog/2015/08/31/the-worst-mistake-of-computer-science/):
    all our code would need to take into account that a child might be nullptr (i.e., lots of ifs), and if we mistakenly
    forget one check somewhere, bam!, undefined behavior.

    So, we will make use of a simple design pattern, named the Null Object Pattern (https://www.tutorialspoint.com/design_pattern/null_object_pattern.htm),
    which by the way, is useful in any language (including Java and C#) that is infected with null pointers.
    The idea is simple: instead of using nullptr, we make use of an actual object that represents this "emptiness".
    
    For example, in the case of computing the size of linked lists, we had code something like this:

      int LinkedList::size() const
      {
        if ( next == nullptr ) return 0;
        else return 1 + next->size();
      }

    What if we had introduced a separate EndOfList class, whose size() method would simply return 0? size()s implementation would look like this:

      int LinkedList::size() const { return 1 + next->size(); }
      int EndOfList::size() const { return 0; }

    Let's now apply this to our BinaryTree. We'll call the "real nodes" Branch, and we'll use Leaf objects to replace nullptr.
    Our tree above would become

    [BRANCH] --> [BRANCH] --> [BRANCH] --> [LEAF]
       |            |            |
       |            |            +-----> [LEAF]
       |            |
       |            +-----> [BRANCH] --> [LEAF]
       |                       |
       |                       +-----> [LEAF]
       |
       +------> [BRANCH] --> [LEAF]
                   |
                   +-----> [BRANCH] --> [LEAF]
                   |
                   +-----> [LEAF]

    So, you'll need three classes: a supertype SortedTree and two subtypes named Branch and Leaf.

    LinkedList nodes carried little integers with them. For our set, we want to do something better: we wish
    the set to be able to contain any kind of values. The way to accomplish this is to make use of templates:
    a TreeSet<T> contains values of type T.

    Where do the set's elements reside exactly? Each Branch node contains exactly one T, while the leaves contain nothing:
    they merely serve as "plugs" to fill the holes in the tree.

    We don't want the T-values to be stored arbitrarily: there'd be no point in using a tree.
    We want the values to be sorted. As mentioned above, each branch refers to two nodes (its child nodes) and has its own value.
    A branch's value must be larger than all values stored in its left children but less than all values stored in its right children.
    For example (leaves are not shown):

                                [BRANCH 8]
                                 |      |
                 +---------------+      +-------------------+
                 |                                          |
            [BRANCH 4]                                  [BRANCH 11]
              |    |                                      |     |
        +-----+    +----+                           +-----+     +------+
        |               |                           |                  |
    [BRANCH 1]      [BRANCH 7]                  [BRANCH 9]         [BRANCH 15]

    Take the root node, i.e., the branch carrying 8. All values in its left node (1, 4, 7) are less than 8
    and the values in the right children (9, 11, 15) are all greater than 8. The same rule is applicable on each branch in the tree.
    Note that no duplicates are allowed: a set cannot contain the same value twice.
*/
