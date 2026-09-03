#include <iostream>
#include <algorithm>
#include <list>
#include <set>

struct state {
	int xi;
	bool isFull;
	bool isC;
};

int N, n1, n2, f, c, d;

int ibSize = 60000;
int ni_cnt1 = 0;
int ni_cnt2 = 0;
bool sw = false;
state ** iter1;
state ** iter2;

std::set<int> already;

bool contains(int x) {
	return already.find(x) != already.end();
}

bool contains(int x, bool b) {
	if(!b) return already.find(-x) != already.end();
	return already.find(x) != already.end();
}

void simulateFill(int nVal, state *&st, state *&stp, int *&cnt) {
	if(nVal == 0) return;
	if(contains(nVal, !st->isC)) return;
	if(!st->isFull) {
		stp->xi = st->xi;
		stp->isC = st->isC;
		stp->isFull = true;
	} else {
		stp->xi = nVal;
		stp->isC = !st->isC;
		stp->isFull = true;
	}
	(*cnt)++;
}

void putManual(int v, bool fl, bool is, state *&dest, int *&cnt) {
	dest->xi = v;
	dest->isC = is;
	dest->isFull = fl;
	(*cnt)++;
}

bool decideNext(state *&st, state **&iterPut, int *&cnt) {
	if(st->xi == d) return false;
	int nVal = 0;
	if(st->isC){
		if(st->xi < f && !st->isFull && !contains(-st->xi)) {
			putManual(st->xi, false, false, iterPut[*cnt + 1], cnt);
		}

		nVal = st->xi + f;
		if(nVal < c && !contains(nVal)) {
			if(!st->isFull) {
				putManual(st->xi, true, true, iterPut[*cnt + 1], cnt);
			} else {
				putManual(nVal, false, true, iterPut[*cnt + 1], cnt);
			}
		} else {
			nVal = std::max(0, f - (c - st->xi));
			simulateFill(nVal, st, iterPut[*cnt + 1], cnt);
		}

		nVal = std::max(0, st->xi - f);
		if(!st->isFull) {
			if(!contains(nVal)) putManual(nVal, true, true, iterPut[*cnt + 1], cnt);
		} else if(!contains(nVal) || !contains(-st->xi)) {
			 putManual(st->xi, false, true, iterPut[*cnt + 1], cnt);
		}
	} else {
		if(st->isFull &&!contains(st->xi)) putManual(st->xi, false, false, iterPut[*cnt + 1], cnt);

		nVal = c - (f - st->xi);
		simulateFill(nVal, st, iterPut[*cnt + 1], cnt);

		if(!st->isFull && !contains(st->xi)) {
			putManual(st->xi, false, true, iterPut[*cnt + 1], cnt);
		}
	}
	return true;
}

void sweech(state **&iter, state **&iter_p, int *&ni_cnt, int *&ni_cnt_p) {
	if(!sw) {
		iter = iter1;
		iter_p = iter2;
		ni_cnt = &ni_cnt1;
		ni_cnt_p = &ni_cnt2;
	} else {
		iter = iter2;
		iter_p = iter1;
		ni_cnt = &ni_cnt2;
		ni_cnt_p = &ni_cnt1;
	}
}

void putIt(state **iter_p, int from, int to) {
	state * dest;
	for(int i = from; i <= to; i++) {
		dest = iter_p[i];
		if(dest->isC) already.insert(dest->xi); else already.insert(-dest->xi);
	}
}

int main() {

	iter1 = (struct state **) malloc(ibSize * sizeof(struct state));
	iter2 = (struct state **) malloc(ibSize * sizeof(struct state));
	for(int i = 0; i < ibSize; i++) {
		iter1[i] = new state;
		iter2[i] = new state;
	}

	state ** iter = NULL;
	state ** iter_p = NULL;
	int * ni_cnt = NULL;
	int * ni_cnt_p = NULL;
	int steps = 0;
	bool reached = false;
	//state * dest;
	int from;


	std::cin >> N;
	for(int cs = 0; cs < N; cs++) {
		std::cin >> n1;
		std::cin >> n2;
		if(n1 > n2) {f = n2; c = n1;} else {f = n1; c = n2;}
		std::cin >> d;
		if(c == d || f == d) {std::cout << 1 << '\n'; continue;}

		already.clear();
		ni_cnt1 = -1;
		ni_cnt2 = -1;
		sw = false;
		reached = false;
		steps = 0;

		sweech(iter, iter_p, ni_cnt, ni_cnt_p);

		ni_cnt1++;
		//f full
		iter1[ni_cnt1]->xi = 0;
		iter1[ni_cnt1]->isC = true;
		iter1[ni_cnt1]->isFull = true;

		ni_cnt1++;
		//c full
		iter1[ni_cnt1]->xi = 0;
		iter1[ni_cnt1]->isC = false;
		iter1[ni_cnt1]->isFull = true;

		already.insert(0);
		already.insert(c);
		already.insert(-f);
		steps++;

		while(*ni_cnt >= 0 && !reached) {

			for(int i = 0; i <= *ni_cnt; i++) {
				from = *ni_cnt_p + 1;
				if(!decideNext(*&(iter)[i], iter_p, ni_cnt_p)) {reached = true; break;}
				putIt(iter_p, from, *ni_cnt_p);
			}

			/*for(int i = 0; i < *ni_cnt_p; i++) {
				dest = iter_p[i];
				if(dest->isC) already.insert(dest->xi); else already.insert(-dest->xi);
			}*/

			sw = !sw;
			sweech(iter, iter_p, ni_cnt, ni_cnt_p);
			*ni_cnt_p = -1;
			if(!reached) steps++;
		}

		if(reached) {std::cout << steps << '\n';} else {std::cout << -1 << '\n';}
	}

	return 0;
}
