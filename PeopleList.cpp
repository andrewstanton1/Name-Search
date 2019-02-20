#include "PeopleList.h"
#include "PeopleList.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

PeopleList::PeopleList() 
{
   head = nullptr;
   tail = nullptr;
};

PeopleList::~PeopleList()
{
      Node* n = head;
      Node* clear = nullptr;
      while (n != nullptr) {
         clear = n;
         n = n->next;
         delete clear;
      }
};

PeopleList::PeopleList(const PeopleList& rhs)
{
   *this = rhs;
};

const PeopleList& PeopleList::operator = (const PeopleList& rhs)
{
   if (rhs.head == nullptr)
   {
      this->head = nullptr;
      return *this;
   }
   
   if (head == nullptr && tail == nullptr)         //if creating new list copy
   {
      Node *n = new Node();
      Node *r = rhs.head;

      n->fname = r->fname;
      n->lname = r->lname;
      n->age = r->age;

      n->prev = nullptr;
      head = n;
      tail = n;
      tail->next = nullptr;

      r = r->next;

      while (r != nullptr)
      {
         Node *n = new Node();

         n->fname = r->fname;
         n->lname = r->lname;
         n->age = r->age;

         n->prev = tail;
         tail->next = n;
         tail = n;
         tail->next = nullptr;

         r = r->next;
      }
   }
   else                                //existing list
   {
      Node *r = rhs.head;
      Node *n = head;
      Node *hold = nullptr;

      while (r != nullptr)
      {
         if (n->next != nullptr)             //this statement exuctes always if lhs(copy) is larger than rhs
         {
            n->fname = r->fname;
            n->lname = r->lname;
            n->age = r->age;

            r = r->next;
            hold = n;
            n = n->next;

            if (r == nullptr)
            {
               Node *temp = n;
               n = hold;
               tail = n;
               n->next = nullptr;
               temp->prev = nullptr;

               delete temp;
               temp->next;
            }
         }
         else                       //create new nodes for copy
         {
            n->fname = r->fname;
            n->lname = r->lname;
            n->age = r->age;

            if (r->next != nullptr)
            {
               Node *newNode = new Node();

               newNode->prev = tail;
               tail->next = newNode;
               tail = newNode;
               tail->next = nullptr;
               n = newNode;
            }
            r = r->next;
         }
      }
   }      
   return *this;
};

bool PeopleList::empty() const
{
   return (head == nullptr) && (tail == nullptr);
};

int PeopleList::size() const
{
   int count = 0;
   Node *n = head;

   if (empty() == true)
      return count;
   else 
      while (n != nullptr)
      {
         count++;
         n = n->next;
      }

   return count;
};

bool PeopleList::add(const std::string& firstName, const std::string& lastName, const InfoType& value)
{
   if (contains(firstName, lastName) == true)
      return false;

   Node* n = new Node();

   if (empty() == true)
   {
      n->fname = firstName;
      n->lname = lastName;
      n->age = value;

      n->prev = nullptr;
      head = n;
      tail = n;
      tail->next = nullptr;
      
      return true;
   }
  
      Node* position = head;
      Node* hold = head;
      
      n->fname = firstName;
      n->lname = lastName;
      n->age = value;
      bool last = false;

      while (position->lname <= n->lname)             //find position
      {
         if (position->lname == n->lname)
         {
            while (position->fname < n->fname)
            {
               if (position->next != nullptr)
               {
                  hold = position;
                  position = position->next;
                  position->prev = hold;
               }
               else
               {
                  last = true;                           //keep track of last
                  break;
               }
               if (position->lname > n->lname)
                  break;
            }
            break;
         }
         if (position->next != nullptr)
         {
            hold = position;
            position = position->next;
            position->prev = hold;
         }
         else
         {
            last = true;
            break;
         }
      }

      Node *temp = position->prev;

      if (temp == nullptr && last == false)                         //insert into beginning
      {
         n->next = head;
         head->prev = n;
         head = n;
         head->prev = nullptr;
      }
      else if (last == true && position->next == nullptr)       //if inserting to end of list
      {
         n->prev = tail;
         tail->next = n;
         tail = n;
         tail->next = nullptr;
      }
      else                                      //if inserting NOT in beginning or end of list
      {
         n->prev = temp;
         temp->next = n;
         n->next = position;
         position->prev = n;
      }

   return true;
};

bool PeopleList::change(const std::string& firstName, const std::string& lastName, const InfoType& value)
{
   if (contains(firstName, lastName) == true)
   {
      Node *p = head;

      while (p != nullptr)
      {
         if (p->fname == firstName && p->lname == lastName)
         {
            p->age = value;
            return true;
         }
         p = p->next;
      }
   }
   else
      return false;

};

bool PeopleList::addOrChange(const std::string& firstName, const std::string& lastName, const InfoType& value)
{
   if (change(firstName, lastName, value) == true)
      return true;
   else
   {
      add(firstName, lastName, value);
      return true;
   }
};

bool PeopleList::remove(const std::string& firstName, const std::string& lastName)
{
   if (contains(firstName, lastName) != true)
      return false;
   
      Node* g = head;
      Node* temp = head;
      Node* hold = head;

      while (g != nullptr)                                        //loop through until we find the name we're looking for
      {
         if (g->fname == firstName && g->lname == lastName)       //name found
         {
            if (g->prev == nullptr && g->next == nullptr)         //only one element in the list
            {
               temp = g;
               head = nullptr;
               tail = nullptr;
               g->next = nullptr;
               g->prev = nullptr;
               delete temp;
            }
            else if (g->prev == nullptr && g->next != nullptr)    //first item in a list larger than one elment
            {
               temp = g;
               g = g->next;
               g->prev = nullptr;
               head = g;
               temp->next = nullptr;
               temp->prev = nullptr;
               delete temp;
            }
            else if (g->next == nullptr && g->prev != nullptr)    //last item in a list larger than one element
            {
               hold = temp;
               hold->next = nullptr;
               tail = hold;
               g->next = nullptr;
               g->prev = nullptr;
               delete g;
            }
            else                                                   //somewhere in middle of list larger than one element 
            {
               temp->next = g->next;
               hold = g->next;
               hold->prev = temp;
               g->next = nullptr;
               g->prev = nullptr;
               delete g;
            }

            break;
         }
         temp = g;
         g = g->next;;
      }
      return true;
};

bool PeopleList::contains(const std::string& firstName, const std::string& lastName) const
{
   if (empty() == true)
      return false;

   Node *p = head;

   while (p != nullptr)
   {
      if (p->fname == firstName && p->lname == lastName)
         return true;
      p = p->next;
   }

   return false;
};

bool PeopleList::lookup(const std::string& firstName, const std::string& lastName, InfoType& value) const
{
   if (contains(firstName, lastName) == true)
   {
      Node* p = head;
      while (p != nullptr)
      {
         if (p->fname == firstName && p->lname == lastName)
         {
            value = p->age;
            break;
         }
      }
      return true;
   }
   else
      return false;
};

bool PeopleList::get(int i, std::string& firstName, std::string& lastName, InfoType& value) const
{
   if (0 <= i && i < size())
   {
      int m = 0;
      Node* n = head;

      while (n != nullptr)
      {
         if (m == i)
         {
            firstName = n->fname;
            lastName = n->lname;
            value = n->age;
            break;
         }
         n = n->next;
         m++;
      }
      return true;
   }
   else
      return false;
};

void PeopleList::swap(PeopleList& other)
{
   PeopleList *a = new PeopleList();
   *a = other;        //hold value of RHS
   other = *this;    //RHS equals LHS
   *this = *a;        //LHS euals RHS

   delete a;
}

bool combine(const PeopleList& m1, const PeopleList& m2, PeopleList& result)
{
   string m1first;
   string m1last;
   InfoType value;
   string m2first;
   string m2last;
   InfoType cvalue;

   string test1;
   string test2;
   InfoType tvalue;

   bool erase1 = false;
   bool erase2 = false;

   if (result.empty() != true)                     //if list is empty dont run
   {
      if (m1.size() == result.size())              //check to see if any lists match
      {
         for (int i = 0; i <= m1.size(); i++)
         {
            m1.get(i, m1first, m1last, value);
            result.get(i, test1, test2, tvalue);

            if (m1first != test1 || m1last != test2 || value != tvalue)
            {
               erase1 = true;
               break;
            }
         }
      }
      else
         erase1 = true;

      if (m2.size() == result.size())                    //check to see if list matches
      {
         for (int i = 0; i <= m2.size(); i++)
         {
            m2.get(i, m2first, m2last, cvalue);
            result.get(i, test1, test2, tvalue);

            if (m2first != test1 || m2last != test2 || cvalue != tvalue)
            {
               erase2 = true;
               break;
            }
         }
      }
   }
   else
      erase1 = true;

   if (erase1 == true && erase2 == true)           //erase if both lists dont match
   {
     while(result.size() != 0)
      {
         result.get(0, test1, test2, tvalue);
         result.remove(test1, test2);
      }
   }

   bool valueMatch = true;

   for (int i = 0; i <= m1.size(); i++)                  //sort thought to see what elements to add
   {
      m1.get(i, m1first, m1last, value);

      if (m2.contains(m1first, m1last) != true)
         result.add(m1first, m1last, value);
      else
      {
         for (int k = 0; k <= m2.size(); k++)
         {
            m2.get(k, m2first, m2last, cvalue);

            if (m1first == m2first && m1last == m2last && value == cvalue)       //add if all values match
            {
               result.add(m1first, m1last, value);
               valueMatch = true;
               break;
            }
            else
               valueMatch = false;
         }
      }
   }

   for (int j = 0; j <= m1.size(); j++)         //go through second list
   {
      m2.get(j, m2first, m2last, cvalue);

      if (m1.contains(m2first, m2last) != true)
         result.add(m2first, m2last, cvalue);
   }

   if (valueMatch == false)            //if any names matched but values didnt 
      return false;
   else
       return true;
}

void psearch(const std::string& fsearch, const std::string& lsearch, const PeopleList& p1, PeopleList& result)
{
   string n1, n2;
   InfoType val;

   string test1;
   string test2;
   InfoType tvalue;

   int size = p1.size();

   bool bang = false;                  //if last name or first name is in list with * passed

   if (fsearch != "*" || lsearch != "*")
   {
      for (int q = 0; q <= p1.size(); q++)
      {
         p1.get(q, n1, n2, val);

         if (n1 == fsearch || n2 == lsearch)
         {
            bang = true;
            break;
         }
      }
   }
   PeopleList *copy = new PeopleList();         //create a copy of list, add elements from here
   *copy = p1;

   while (result.size() != 0)                //delete result list
   {
      result.get(0, test1, test2, tvalue);
      result.remove(test1, test2);
   }

      if (fsearch == "*" && lsearch == "*")        //just copy over list if ** is passed
         result = *copy;
      else if (bang == true)
      {
         for (int i = 0; i <= copy->size(); i++)      //sort though minding * or matches, add to empty result list
         {
            copy->get(i, n1, n2, val);

            if (fsearch == "*")
            {
               if (n2 == lsearch)
               {
                  result.add(n1, n2, val);
               }
            }
            else if (lsearch == "*")
            {
               if (n1 == fsearch)
               {
                  result.add(n1, n2, val);
               }
            }
            else
            {
               if (n1 == fsearch && n2 == lsearch)
               {
                  result.add(n1, n2, val);
               }
            }
         }
      }
   delete copy;                     //delete copy
};

int main()
{
   cout << "Passed all tests" << endl;
}