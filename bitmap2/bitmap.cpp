#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <iterator>
#include <string>
#include <iostream>
#include <sstream>

const int maxDim = 184;
const int maxVal = 2 * maxDim + 1;

bool ** bmx;
int ** dmx;
int * bar = new int[maxDim];

int N, n, m;

void printB() {
	for(int i = 0; i < n; i++) {
		printf("%d", bmx[i][0]);
		for(int j = 1; j < m; j++) {
			printf(" %d", bmx[i][j]);
		}
		printf("\n");
	}
}

void printD() {
	for(int i = 0; i < n; i++) {
		printf("%d", dmx[i][0]);
		for(int j = 1; j < m; j++) {
			printf(" %d", dmx[i][j]);
		}
		printf("\n");
	}
}

void reset() {
	for(int x = 0; x < maxDim; x++) {
		std::copy(&bar[0], &bar[maxDim - 1], &dmx[x][0]);
	}
}


int proceed(int x, int y, int dist) {
	if(x < n && x > -1 && y < m && y > -1) {
		if(bmx[x][y]) {
			return 0;
		} else
			return dist;
	} else {
		return -1;
	}
}

void markIt(int x, int y, int pd, int * lc, int * rc, int * toGo) {
	if(pd != -1) {
		if(dmx[x][y] > pd) {
			dmx[x][y] = pd;
			if(pd > 0) {
				toGo[*rc] = pd;
				(*rc)--;
				toGo[*rc] = y;
				(*rc)--;
				toGo[*rc] = x;
				(*rc)--;
			} else {
				toGo[*lc] = x;
				(*lc)++;
				toGo[*lc] = y;
				(*lc)++;
				toGo[*lc] = pd;
				(*lc)++;
			}
		}
	}
}

void go(int x, int y, int distance) {
	int pd;
	int toGo[12];
	int lc = 0; int rc = 11;

	pd = proceed(x - 1, y, distance + 1);
	markIt(x - 1, y, pd, &lc, &rc, toGo);

	pd = proceed(x, y - 1, distance + 1);
	markIt(x, y - 1, pd, &lc, &rc, toGo);

	pd = proceed(x + 1, y, distance + 1);
	markIt(x + 1, y, pd, &lc, &rc, toGo);

	pd = proceed(x, y + 1, distance + 1);
	markIt(x, y + 1, pd, &lc, &rc, toGo);

	for(int i = lc - 1; i > 1; i-=3) {
		go(toGo[i - 2], toGo[i - 1], toGo[i]);
	}

	for(int i = rc + 1; i < 12; i+=3) {
		go(toGo[i], toGo[i + 1], toGo[i + 2]);
	}
}

int main() {

	bmx = new bool*[maxDim];
	for (int i = 0; i < maxDim; i++) {
	  bmx[i] = new bool[maxDim];
	}

	dmx = new int*[maxDim];
	for (int i = 0; i < maxDim; i++) {
		dmx[i] = new int[maxDim];
	}

	for(int i = 0; i < maxDim; i++) {
		bar[i] = maxVal;
	}

	reset();

	scanf("%d", &N);
	std::string s;
	for(int c = 0; c < N; c++) {
		scanf("%d %d", &n, &m);

		for(int i = 0; i < n; i++) {
			std::cin >> s;
			for(int j = 0; j < m; j++) {
				std::istringstream ss(s.substr(j,1)); ss >> bmx[i][j];
			}
		}

		//printB();

		int i = 0;
		while(i < n) {
			for(int j = 0; j < m; j++) {
				if(bmx[i][j]) {
					dmx[i][j] = 0;
					go(i, j, 0);
					i = maxDim;
					break;
				}
			}
			i++;
		}

		printD();
		reset();
	}
	return 0;
}
