#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <chrono>

using namespace std;

long * position;
int * stc;

int c, n;

long distanceP(int l, int h) {
	return position[h] - position[l];
}

bool higherPosition(int ta, int idx) {
	return position[idx] >= position[ta];
}

int half(int l, int r) {
	if(l <= r) return (l + r) / 2;
	return -1;
}

long drr(int h, int l) {
	return position[h] - position[l];
}

long minN(int h, int l, int ll) {
	long dr = drr(h, l);
	long dl = drr(l, ll);
	if(dl < dr) return dl; else return dr;
}

int placeIt(int idx, int lb, bool (*test)(int, int)) {
	int ll = lb;
	int rl = idx;
	int h = half(ll, rl);
	int i;
	bool end = false;

	while(h != -1 && !end) {
		if(test(h, idx)) {
			ll = h + 1;
			if(h == idx) end = true;
		} else {
			rl = h;
			if(h == lb) end = true;
		}
		i = h;
		h = half(ll, rl);
		if(h == i) end = true;
	}

	if(i >= idx) return -1;
	return i;
}

void insertIt(int idx, int at, long * arr) {
	long e = arr[idx];
	copy(&arr[at], &arr[idx], &arr[at + 1]);
	arr[at] = e;
}

void doIt(int idx, int lb, long * arr, bool (*test)(int, int)) {
	if(idx == lb) return;
	int p = placeIt(idx, lb, test);
	if(p != -1) insertIt(idx, p, arr);
}

int fitIt(int lower, long dist) {
	int ll = lower;
	int rl = n - 1;
	int h = half(ll, rl);
	int i = 0;

	while(h != -1) {
		if(drr(h, lower) > dist) {
			rl = h;
		} else {
			ll = h + 1;
		}

		i = h;
		h = half(ll, rl);
		if(h == i) break;
	}

	return i;
}

void minimize(long * minn, long test) {
	if(test <= *minn) {*minn = test;}
}

int main() {
	position = new long[100000];
	stc = new int[100000];
	int N;

	scanf("%d", &N);
	for(int ex = 0; ex < N; ex++) {
		scanf("%d %d", &n, &c);
		for(int st = 0; st < n; st++) {
			scanf("%ld", &position[st]);
			doIt(st, 0, position, higherPosition);
		}

		if(c == 2) {
			printf("%ld\n", distanceP(0, n - 1));
			continue;
		}

		long min = 0;

		int ll = 0;
		int rl = position[n - 1];
		int h = half(ll, rl);
		int ph = 0;
		int idx = 0;
		bool cont = true;
		bool hit = false;
		bool start = true;

		while(cont) {
			long minI = 1000000000l;
			long seed = h;
			//cout << "seed " << seed << endl;
			int i = 0;
			int ni = 0;

			for(int x = 0; x < c - 1; x++) {
				ni = fitIt(i, seed);
				minimize(&minI, distanceP(i, ni));
				if(minI <= min) break;
				i = ni;
			}

			if(start) {min = minI; start = false;}

			if(minI <= min) {
				stc[idx] = h + 1;
				idx++;
				stc[idx] = rl;
				idx++;
				rl = h;
				//cout << "lewa" << endl;
			} else {
				//cout << "prawa" << endl;
				hit = true;
				ll = h + 1;
			}

			ph = h;
			h = half(ll, rl);
			if(h == -1 || ph == h) {
				if(hit) cont = false;
				else {
					idx--;
					rl = stc[idx];
					idx--;
					ll = stc[idx];
					ph = 0;
					h = ll;
				}
			}
			if(minI > min) min = minI;
		}

		printf("%ld\n", min);
	}
}
