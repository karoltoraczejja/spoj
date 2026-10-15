#include <iostream>
#include <algorithm>

int N;

struct sellerCell {
	int buyerIdx;
	int value;
	int total;
	sellerCell * prev;
	sellerCell * next;
};

struct sellerMaster {
	int count;
	sellerCell * actual;
	sellerCell * high;
 	sellerCell * column;
};

const int citiesBuffSize = 100;
const int colBuffSize = 100;

int * buyerBuffBar = NULL;
int * sellerBuffBar = NULL;
bool * lockedBar = NULL;
int totalRoll = 0;
bool * lockedReset = NULL;
int * buyerReset = NULL;

int limitBuyers = 0;
//int * byBuyerMtx = NULL;
int limitSellers = 0;
sellerMaster * bySellerMtx = NULL;
sellerMaster * buyersExtMtx = NULL;
int total = 0;
int rTotal = 0;

sellerCell * getHigh(int buyerToOmit, sellerCell * pds) {
	if(pds == NULL) return NULL;
	sellerCell * sc = pds->prev;
	while(sc != NULL) {
		if(sc -> buyerIdx != buyerToOmit) return sc;
		sc = sc -> prev;
	}
	return NULL;
}

sellerCell * getMe(int idx, sellerMaster * sM) {
	sellerCell * c = sM -> column;
	for(int i = 0; i < sM -> count; i++) {
		if(c[i].buyerIdx == idx) return &c[i];
	}
	return NULL;
}

void raise(sellerCell * sc, sellerMaster * thisOne) {
	if(thisOne -> high == NULL) {
		thisOne -> high = sc;
		thisOne -> actual = sc;
		return;
	}

	if(sc -> next == NULL) {
		if(thisOne->high != sc) sc -> prev = thisOne -> high ;
		thisOne -> high -> next = sc;
	    thisOne -> high = sc;
	    thisOne -> actual = sc;
	    return;
	}

	sellerCell * above = thisOne -> high;
	if(above -> prev == sc) above -> prev = sc -> prev;
	if(sc -> prev != NULL) sc -> prev -> next = sc -> next;
	above -> next = sc;
	sc -> prev = above;
	sc -> next = NULL;
	thisOne -> high = sc; thisOne -> actual = sc;
}

void update(sellerCell * sc, sellerMaster * thisOne) {
	sc -> total = rTotal;
	if(thisOne -> high == NULL) {
		thisOne -> high = sc;
		return;
	}

	sellerCell * above = thisOne -> high;
	while(above -> prev != NULL && above -> total > sc -> total) {
		above = above -> prev;
	}
	if(above->prev == sc)
		return;
	if(above -> prev != NULL) {
		sc->prev = above->prev;
		above->prev->next = sc;
	}
	above -> prev = sc;
	sc -> next = above;
}

void accepttIt(int sm, int newTotal, sellerCell * sc, int * buyerBuff, sellerMaster * m) {
	sc -> total = newTotal;
	//if(m[sm].actual != NULL)
	//	buyerBuff[m[sm].actual -> buyerIdx] = -1;
	buyerBuff[sc -> buyerIdx] = sm;
	raise(sc, &m[sm]);

}

void accept(int sm, int newTotal, sellerCell * sc, int * buyerBuff) {
	if(buyerBuff == buyerBuffBar) {
		accepttIt(sm, newTotal, sc, buyerBuff, bySellerMtx);
		//...
		sellerCell * scb = getMe(sm, &buyersExtMtx[sc -> buyerIdx]);
		accepttIt(sc -> buyerIdx, newTotal, scb, sellerBuffBar, buyersExtMtx);
	} else {
		accepttIt(sm, newTotal, sc, buyerBuff, buyersExtMtx);
		//...
		sellerCell * scb = getMe(sm, &bySellerMtx[sc -> buyerIdx]);
		accepttIt(sc -> buyerIdx, newTotal, scb, buyerBuffBar, bySellerMtx);
	}
}

void acceptLocked(int sm, int newTotal, sellerCell * sc, int * buyerBuff, bool * locked) {
	if(!locked[sm]) {
		accept(sm, newTotal, sc, buyerBuff);
		locked[sm] = true;
	}
}

bool determin(bool cond, int tot, int sm, sellerCell * sc, int * buyerBuff, bool * locked) {
	if(cond) {
		 acceptLocked(sm, tot, sc, buyerBuff, locked);
		 return true;
	}
	return false;
}


void acceptBar() {
	//std::copy(&buyerBuffBar[0], &buyerBuffBar[0] + limitBuyers, &byBuyerMtx[0]);
	int tot = 0;
	for (int i = 0; i < limitSellers; i++) {
		if(bySellerMtx[i].actual != NULL) tot += bySellerMtx[i].actual -> value;
	}
	total = tot;
}

void smallReset() {
	//std::copy(&byBuyerMtx[0], &byBuyerMtx[0] + limitBuyers, &buyerBuffBar[0]);
	std::copy(&lockedReset[0], &lockedReset[0] + limitSellers, &lockedBar[0]);
}

void determin(bool cond, int tot, int sm, sellerCell * sc, int * buyers) {
	if(cond) {
		accept(sm, tot, sc, buyers);
		acceptBar();
	} else {
		rTotal = tot;
		update(sc, &bySellerMtx[sm]);
		sellerCell * scb = getMe(sm, &buyersExtMtx[sc -> buyerIdx]);
		update(scb, &buyersExtMtx[sc -> buyerIdx]);
	}
}

int currentVal(int col) {
	if(col == -1) return 0;
	if(bySellerMtx[col].actual != NULL) return bySellerMtx[col].actual -> value;
	return 0;
}

int digDeep(int si, sellerCell * from, int init, int toOmit, int * buyerBuff, bool * locked) {
	int myTotal = init;
	sellerCell * pds = getHigh(toOmit, from);
	if(pds != NULL) {
		int ap = buyerBuff[pds -> buyerIdx];
		myTotal = myTotal - currentVal(si) + pds -> value;
		if(ap == -1) {
			if(determin(myTotal >= total, myTotal, si, pds, buyerBuff, locked)) return myTotal;
			rTotal = myTotal;
			return -1;
		} else {
			int x = digDeep(ap, bySellerMtx[ap].actual, myTotal, toOmit, buyerBuff, locked);
			while(x == -1 && pds -> prev != NULL) {
				myTotal = myTotal - pds -> value + pds -> prev -> value;
				pds = getHigh(toOmit, pds);
				x = digDeep(buyerBuff[pds->buyerIdx], bySellerMtx[buyerBuff[pds->buyerIdx]].actual, myTotal, toOmit, buyerBuff, locked);
			}
			if(determin(x >= total, x, si, pds, buyerBuff, locked)) return x;
			return -1;
		}
	} else {
		myTotal = myTotal - currentVal(si);
		if(myTotal > total) {
			if(bySellerMtx[si].actual != NULL) {
				buyersExtMtx[sellerBuffBar[si]].actual = NULL;
				sellerBuffBar[si] = -1;
				buyerBuff[bySellerMtx[si].actual -> buyerIdx] = -1;
			}
			bySellerMtx[si].actual = NULL;
			return myTotal;
		}
		rTotal = myTotal;
		return -1;
	}
}

int digDeepB(int bi, sellerCell * from, int init, int toOmit, int * sellerBuff, bool * locked) {
	int myTotal = init;
	sellerCell * pds = getHigh(toOmit, from);
	if(pds != NULL) {
		int ap = sellerBuff[pds -> buyerIdx];
		myTotal = myTotal - currentVal(bi) + pds -> value;
		if(ap == -1) {
			if(determin(myTotal >= total, myTotal, bi, pds, sellerBuff, locked)) return myTotal;
			rTotal = myTotal;
			return -1;
		} else {
			int x = digDeepB(ap, buyersExtMtx[ap].actual, myTotal, toOmit, sellerBuff, locked);
			while(x == -1 && pds -> prev != NULL) {
				myTotal = myTotal - pds -> value + pds -> prev -> value;
				pds = getHigh(toOmit, pds);
				x = digDeepB(sellerBuff[pds->buyerIdx], buyersExtMtx[sellerBuff[pds->buyerIdx]].actual, myTotal, toOmit, sellerBuff, locked);
			}
			if(determin(x >= total, x, bi, pds, sellerBuff, locked)) return x;
			return -1;
		}
	} else {
		myTotal = myTotal - currentVal(bi);
		if(myTotal > total) {
			if(buyersExtMtx[bi].actual != NULL) {
				bySellerMtx[buyerBuffBar[bi]].actual = NULL;
				buyerBuffBar[bi] = -1;
				sellerBuff[buyersExtMtx[bi].actual -> buyerIdx] = -1;
			}
			buyersExtMtx[bi].actual = NULL;
			return myTotal;
		}
		rTotal = myTotal;
		return -1;
	}
}

void cgm(int si, int asi) {
	sellerCell * sc = &(bySellerMtx[si].column)[asi];
	int nTotal; // = total - currentVal(si) + sc -> value;
	int ab = buyerBuffBar[sc -> buyerIdx];
	if(ab == -1) {
		nTotal = total - currentVal(si) + sc -> value;
		int x = nTotal;
		if(bySellerMtx[si].actual != NULL)
			x = digDeepB(sellerBuffBar[si], buyersExtMtx[sellerBuffBar[si]].actual, nTotal, si, sellerBuffBar, lockedBar);
		determin(x != -1, x, si, sc, buyerBuffBar);
	} else {
		nTotal = total + sc -> value;
		int x = digDeep(ab, bySellerMtx[ab].actual, nTotal, sc -> buyerIdx, buyerBuffBar, lockedBar);
		determin(x != -1, x, si, sc, buyerBuffBar);
	}
	smallReset();
}

void reset() {

}

void allocBuff() {
	//std::cout << "allocating..." << '\n';
	//byBuyerMtx = new int[citiesBuffSize];

	bySellerMtx = new sellerMaster[citiesBuffSize];
	buyersExtMtx = new sellerMaster[citiesBuffSize];
	buyerReset = new int [citiesBuffSize];
	lockedBar = new bool[citiesBuffSize];
	lockedReset = new bool[citiesBuffSize];

	for(int i = 0; i < citiesBuffSize; i++) {
		buyerReset[i] = -1;
		lockedReset[i] = false;
		bySellerMtx[i].column = new sellerCell[colBuffSize];
		buyersExtMtx[i].column = new sellerCell[colBuffSize];
	}

	buyerBuffBar = new int[citiesBuffSize];
	std::copy(&buyerReset[0], &buyerReset[0] + citiesBuffSize, &buyerBuffBar[0]);

	sellerBuffBar = new int[citiesBuffSize];
	std::copy(&buyerReset[0], &buyerReset[0] + citiesBuffSize, &sellerBuffBar[0]);
}

void printSc(sellerCell * sc, int seller) {
	std::cout << "seller: " << seller + 1 << " buyer: " << sc -> buyerIdx + 1 << " goods: " << sc -> value << '\n';
}

void readMtx() {
	int c1, c2, g;
	std::cin >> limitSellers;
	std::cin >> limitBuyers;

	for(int i = 0; i < limitSellers; i++) {
		bySellerMtx[i].actual = NULL;
		bySellerMtx[i].high = NULL;
		bySellerMtx[i].count = 0;
	}

	for(int i = 0; i < limitBuyers; i++) {
		buyersExtMtx[i].actual = NULL;
		buyersExtMtx[i].high = NULL;
		buyersExtMtx[i].count = 0;
	}

	std::cin >> c1; std::cin >> c2; std::cin >> g;
	while(c1 != 0) {
		c1--;
		c2--;
		//std::cout << "c1: " << c1 << " cnt: "<< bySellerMtx[c1].count << '\n';
		sellerCell * c = bySellerMtx[c1].column;
		sellerCell * sc = &c[bySellerMtx[c1].count];
		bySellerMtx[c1].count++;
		sc->buyerIdx = c2;
		sc->value = g;
		sc->total = 0;
		sc->prev = NULL;
		sc->next = NULL;
		//...
		sellerCell * cb = buyersExtMtx[c2].column;
		sellerCell * scb = &cb[buyersExtMtx[c2].count];
		buyersExtMtx[c2].count++;
		scb->buyerIdx = c1;
		scb->value = g;
		scb->total = 0;
		scb->prev = NULL;
		scb->next = NULL;
		//printSc(&c[bySellerMtx[c1].count - 1], c1);
		std::cin >> c1; std::cin >> c2; std::cin >> g;
	}
	std::copy(&buyerReset[0], &buyerReset[0] + limitBuyers, &buyerBuffBar[0]);
	std::copy(&buyerReset[0], &buyerReset[0] + limitBuyers, &sellerBuffBar[0]);
}

void go() {
	for(int i = 0; i < limitSellers; i++) {
		for(int j = 0; j < bySellerMtx[i].count; j++) {
			cgm(i, j);
		}
	}
}

int main() {
	allocBuff();

	std::cin >> N;

	for(int x = 0; x < N; x++) {
		readMtx();
		go();
		std::cout << total << '\n';
		total = 0;
	}

	return 0;
}
