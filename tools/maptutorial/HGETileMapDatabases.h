#ifndef DB_H
#define DB_H

#include <string>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////
// Class pointer database

#define DATABASE_MAX 10240

template <class CLS>
class CPDatabase {
private:
	
	CLS *objlist[DATABASE_MAX];
	int cobject;

public:
	
	CPDatabase() {
		cobject = 0;
		
		for (int i=0; i<100; i++)
			objlist[i] = NULL;
	}


	CLS *Add(CLS *Obj) {
		objlist[cobject] = Obj;
		cobject ++;
		return Obj;
	}

	
	bool Remove(CLS *Obj) {
		for (int i=0; i<cobject; i++) {
			if (Obj == objlist[i]) {
				cobject --;

				for (int j=i; j<cobject; j++)
					objlist[j] = objlist[j+1];

				objlist[cobject] = NULL;
				return true;
			}
		}
		return false;
	}
	

	CLS *Fetch(int idx) {
		return objlist[idx];
	}

	int Max() { return cobject; }
	int size() { return cobject; }

	void clear() { cobject = 0; }

	CLS *operator [] (unsigned idx) { return objlist[idx]; }

};

//////////////////////////////////////////////////////////////////////////
// struct database

#define SDATABASE_MAX 4096

template <class CLS>
class CSDatabase {
private:
	
	CLS objlist[SDATABASE_MAX];
	int cobject;

public:
	
	CSDatabase() {
		cobject = 0;
	}


	void Add(CLS Obj) {
		objlist[cobject] = Obj;
		cobject ++;
	}

	bool Remove(int idx) {
		for (int i=0; i<cobject; i++) {
			if (idx == i) {
				cobject -= 1;

				for (int j=i; j<cobject; j++)
					objlist[j] = objlist[j+1];

				return true;
			}
		}
		return false;
	}

	CLS Fetch(int idx) {
		return objlist[idx];
	}

	CLS *FetchPointer(int idx) {
		return &objlist[idx];
	}

	int Max() { return cobject; }

	void Clear() { cobject = 0; }

};

#endif
