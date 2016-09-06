#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <queue>
#define oo 0x7FFFFFFF

namespace naiveDB {
	namespace dataprocessor {
		// AVL. finddata(T dat)返回的是一个node指针p，p->dat = dat；
		//其余的AVL的方法均返回为空，执行所要求操作 
		template <typename T>
		class AVL {
		private:
			std::wstring name;
			int id;
			class node {
			public:
				node *l, *r;
				int h, size;
				T dat;
				int changeNum;
				std::vector<int>idset;
				node() : l(0), r(0), h(1), size(0) { idset.clear(); };
				node(T tdat) : dat(tdat), l(0), r(0), h(1), size(1) {};

				int geth() {
					if (!this) return 0;
					else return h;
				}
				int getsize() {
					if (!this) return 0;
					else return size;
				}

				void update() {
					if (this) {
						h = std::max(l->geth(), r->geth()) + 1;
						size = l->getsize() + r->getsize() + 1;
					}
				}
			}*root;
			void clear(node * p) {
				if (!p)
					return;
				clear(p->l);
				clear(p->r);
				delete p;
			}
			void Lrotate(node * &p) {
				node * t = p->r->l;
				p->r->l = p;
				p = p->r;
				p->l->r = t;
				p->l->update();
				p->update();
			}

			void Rrotate(node * &p) {
				node * t = p->l->r;
				p->l->r = p;
				p = p->l;
				p->r->l = t;
				p->r->update();
				p->update();
			}
			void maintain(node * &p) {
				int lh, rh;
				if (p->l->geth() > p->r->geth() + 1) { // To balance the left tree
					lh = p->l->l->geth();
					rh = p->l->r->geth();
					if (lh >= rh)
						Rrotate(p);
					else {
						Lrotate(p->l);
						Rrotate(p);
					}
				}
				if (p->r->geth() > p->l->geth() + 1) { // To balance the right tree
					lh = p->r->l->geth();
					rh = p->r->r->geth();
					if (rh >= lh)
						Lrotate(p);
					else {
						Rrotate(p->r);
						Lrotate(p);
					}
				}
			}
			void insert(node  * & p, T dat, int id) {
				if (!p) {
					p = new node(dat);
					p->idset.push_back(id);
					return;
				}
				if (dat == p->dat) p->idset.push_back(id);
				else if (dat < p->dat)
					insert(p->l, dat, id);
				else
					insert(p->r, dat, id);
				maintain(p);
				p->update();
			}
			void erase(node * & p, T dat) {
				if (!p) return;
				if (p->dat == dat) {
					if (p->l && p->r) {
						node * t = p->r;
						while (t->l)
							t = t->l;
						p->dat = t->dat;
						p->idset = t->idset;
						erase(p->r, t->dat);
						maintain(p);
					}
					else if (p->l) {
						p->dat = p->l->dat;
						p->idset = p->l->idset;
						p->l = 0;
					}
					else if (p->r) {
						p->dat = p->r->dat;
						p->idset = p->r->idset;
						p->r = 0;
					}
					else p = 0;
					p->update();
					return;
				}
				else if (dat < p->dat)
					erase(p->l, dat);
				else
					erase(p->r, dat);
				maintain(p);
				p->update();
			}
			node* finddata(node * &p, T dat)
			{
				if (!p)
				{
					return p;
				}
				if (p->dat == dat) return p;
				else if (p->dat > dat) return finddata(p->l, dat);
				else return finddata(p->r, dat);
			}
		public:

			AVL(std::wstring _name, int _id) : root(0), id(_id), name(_name) {};
			int GetId()
			{
				return id;
			}
			std::wstring GetName()
			{
				return name;
			}
			std::vector<int> finddata(T dat)
			{
				return finddata(root, dat)->idset;
			}
			void insert(T dat, int id) {
				insert(root, dat, id);
			}
			void erase(T dat) {
				erase(root, dat);
			}
			void erasenew(T dat, int id) {
				node * now = finddata(root, dat);
				std::vector<int>::iterator it;
				for (it = now->idset.begin(); it != now->idset.end(); it++)
				{
					if (*it == id)
					{
						now->idset.erase(it);
						break;
					}
				}
				if (now->idset.size() == 0) erase(root, now->dat);
			}
			void update(T datx, T daty, int id)
			{
				node * now = finddata(root, daty);
				if (now == NULL) {

				}
				else
				{
					erasenew(daty, id);
					insert(datx, id);
				}
			}
			void clear()
			{
				if (root != NULL)
					clear(root);
				root = NULL;
			}
			bool empty() {
				return (root->getsize() == 0);
			}
			int size() {
				return root->getsize();
			}
			node* getroot() {
				return root;
			}
		};
	}
}