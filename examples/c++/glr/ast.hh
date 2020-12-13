#include <iostream>

#if __cplusplus < 201103L
# define nullptr 0
#endif

class Node
{
public:
  Node ()
    : parents_ (0)
  {}

  virtual ~Node ()
  {}

  void free ()
  {
    parents_ -= 1;
    /* Free only if 0 (last parent) or -1 (no parents).  */
    if (parents_ <= 0)
      delete this;
  }

  virtual std::ostream& print (std::ostream& o) const = 0;

protected:
  friend class Nterm;
  friend class Term;
  int parents_;
};


static std::ostream&
operator<< (std::ostream& o, const Node &node)
{
  return node.print (o);
}

class Nterm : public Node
{
public:
  Nterm (char const *form,
         Node *child0 = nullptr, Node *child1 = nullptr, Node *child2 = nullptr)
    : form_ (form)
  {
    children_[0] = child0;
    if (child0)
      child0->parents_ += 1;
    children_[1] = child1;
    if (child1)
      child1->parents_ += 1;
    children_[2] = child2;
    if (child2)
      child2->parents_ += 1;
  }

  ~Nterm ()
  {
    for (int i = 0; i < 3; ++i)
      if (children_[i])
        children_[i]->free ();
  }

  std::ostream& print (std::ostream& o) const
  {
    o << form_;
    if (children_[0])
      {
        o << '(' << *children_[0];
        if (children_[1])
          o << ", " << *children_[1];
        if (children_[2])
            o << ", " << *children_[2];
        o << ')';
      }
    return o;
  }

private:
  char const *form_;
  Node *children_[3];
};

class Term : public Node
{
public:
  Term (const std::string &text)
    : text_ (text)
  {}

  std::ostream& print (std::ostream& o) const
  {
    o << text_;
    return o;
  }

private:
  std::string text_;
};
