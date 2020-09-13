#include <cstdio>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <climits>
#include <cstring>

struct Scanner {

	int c;
	char buffer[1 << 16];
	size_t pos;
	size_t size;
	
	Scanner() {
		pos = 0;
		size = 0;
		nextChar();
	}
	
	void nextChar() {
		if (pos >= size) {
			size = fread(buffer, sizeof(buffer[0]), sizeof(buffer) / sizeof(buffer[0]), stdin);
			pos = 0;
			if (size == pos) {
				c = EOF;
				return;
			}
		}
		assert(pos < size);
		c = buffer[pos] & 0xFF;
		pos++;
	}
	
	int nextInt() {
		if (c == '-') {
			nextChar();
			assert('1' <= c && c <= '9');
			int n = -(c - '0');
			nextChar();
			while ('0' <= c && c <= '9') {
				int d = c - '0';
				assert(0 <= d && d <= 9);
				nextChar();
				assert(INT_MIN / 10 <= n);
				n *= 10;
				assert(n < 0 && n >= INT_MIN + d);
				n -= d;
			}
			return n;
		} else {
			if (c == '0') {
				nextChar();
				assert(!('0' <= c && c <= '9'));
				return 0;
			}
			assert('1' <= c && c <= '9');
			int n = c - '0';
			nextChar();
			while ('0' <= c && c <= '9') {
				int d = c - '0';
				assert(0 <= d && d <= 9);
				nextChar();
				assert(n <= INT_MAX / 10);
				n *= 10;
				assert(n > 0 && n <= INT_MAX - d);
				n += d;
			}
			return n;
		}
	}
	
	int nextInt(int low, int high) {
		int value = nextInt();
		assert(low <= value && value <= high);
		return value;
	}
	
	bool isSpace() const {
		return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == EOF;
	}
	
	std::vector<char> wordBuffer;
	
	std::string next() {
		if (wordBuffer.size() > 0) {
			wordBuffer.clear();
			assert(wordBuffer.capacity() > 0);
		}
		assert(!isSpace());
		while (!isSpace()) {
			wordBuffer.push_back((char)c);
			nextChar();
		}
		return std::string(wordBuffer.begin(), wordBuffer.end());
	}

	void skipSpace() {
		assert(c == ' ');
		nextChar();
	}

	void skipEndl() {
		assert(c == '\n');
		nextChar();
	}

	void skipEoln() {
		skipEndl();
	}
	
	void checkEof() {
		assert(c == EOF);
	}
};

struct Query {

	const std::string *s;
	
	long long *purity;
	
};

void addSingle(
	const std::string &gene,
	const int purity,
	const std::vector<Query> &q
) {
	std::vector<int> prefix(gene.size(), 0); // prefix[i] -> size of proper prefix equal to proper suffix of gene[0..i] (string of size i+1)
	int p = 0; // size of current proper prefix
	for (int i = 1; i < (int)gene.size(); i++) {
		while (true) {
			if (gene[i] == gene[p]) {
				p++;
				break;
			}
			if (p == 0) {
				break;
			}
			p = prefix[p - 1];
		}
		prefix[i] = p;
	}
	for (const Query &sq : q) {
		long long sum = 0;
		p = 0;
		for (char c : *sq.s) {
			while (true) {
				if (c == gene[p]) {
					p++;
					break;
				}
				if (p == 0) {
					break;
				}
				p = prefix[p - 1];
			}
			if (p == (int)gene.size()) {
				sum += purity;
				p = prefix[p - 1];
			}
		}
		*sq.purity += sum;
	}
}

const int N_LETTERS = 26;

struct Vertex {
	int next[N_LETTERS];
	long long puritySum;
	int parent;
	int suffixLink;
	int parentChar;
};

Vertex a[1 + 2 * 1000 * 1000];
int aSize;

void addGene(const std::string &gene, const int purity) {
	int v = 0;
	for (int i = 0; i < (int)gene.size(); i++) {
		int c = gene[i] - 'a';
		assert(0 <= c && c < N_LETTERS);
		if (a[v].next[c] == -1) {
			std::memset(a[aSize].next, -1, sizeof(a[aSize].next));
			a[aSize].puritySum = 0;
			a[aSize].parent = v;
			a[aSize].suffixLink = -1;
			a[aSize].parentChar = c;
			a[v].next[c] = aSize;
			aSize++;
		}
		v = a[v].next[c];
	}
	a[v].puritySum += purity;
}

int go(int v, int c);

long long getPuritySum(int v);

int getSuffixLink(int v) {
	if (a[v].suffixLink == -1) {
		if (v == 0 || a[v].parent == 0) {
			a[v].suffixLink = 0;
		} else {
			a[v].suffixLink = go(getSuffixLink(a[v].parent), a[v].parentChar);
			a[v].puritySum += getPuritySum(a[v].suffixLink);
		}
	}
	return a[v].suffixLink;
}

int go(int v, int c) {
	assert(0 <= c && c < N_LETTERS);
	if (a[v].next[c] == -1) {
		if (v == 0) {
			a[v].next[c] = 0;
		} else {
			a[v].next[c] = go(getSuffixLink(v), c);
		}
	}
	return a[v].next[c];
}

long long getPuritySum(int v) {
	if (a[v].suffixLink == -1) {
		getSuffixLink(v); // fills a[v].puritySum
	}
	return a[v].puritySum;
}

void addMulti(
	const std::vector<std::string> &genes,
	const std::vector<int> &purity,
	const int firstGeneId,
	const int afterGeneId,
	const std::vector<Query> &q
) {
	std::memset(a[0].next, -1, sizeof(a[0].next));
	a[0].puritySum = 0;
	a[0].parent = -1;
	a[0].suffixLink = -1;
	aSize = 1;
	for (int i = firstGeneId; i < afterGeneId; i++) {
		addGene(genes[i], purity[i]);
	}
	for (const Query &sq : q) {
		long long sum = 0;
		int v = 0;
		for (char c : *sq.s) {
			v = go(v, c - 'a');
			sum += getPuritySum(v);
		}
		*sq.purity += sum;
	}
}

int main() {
	Scanner in;
	int nGenes = in.nextInt(1, 100 * 1000);
	in.skipEndl();
	std::vector<std::string> genes;
	for (int i = 0; i < nGenes; i++) {
		if (i > 0) {
			in.skipSpace();
		}
		std::string gene = in.next();
		for (char c : gene) {
			assert('a' <= c && c <= 'z');
		}
		genes.push_back(gene);
	}
	in.skipEndl();
	std::vector<int> purity;
	for (int i = 0; i < nGenes; i++) {
		if (i > 0) {
			in.skipSpace();
		}
		purity.push_back(in.nextInt(0, 10 * 1000 * 1000));
	}
	in.skipEndl();
	int nQueries = in.nextInt(1, 100 * 1000);
	in.skipEndl();
	std::vector<std::string> queries;
	std::vector<int> qLeft;
	std::vector<int> qRight;
	for (int i = 0; i < nQueries; i++) {
		int left = in.nextInt(0, nGenes - 1);
		in.skipSpace();
		int right = in.nextInt(0, nGenes - 1);
		assert(left <= right);
		in.skipSpace();
		std::string query = in.next();
		for (char c : query) {
			assert('a' <= c && c <= 'z');
		}
		qLeft.push_back(left);
		qRight.push_back(right);
		queries.push_back(query);
		if (i != nQueries - 1) { // That's HackerRank way to trim.
			in.skipEndl();
		}
	}
	in.checkEof();
	int lenSum = 0;
	for (const std::string &s : genes) {
		lenSum += (int)s.size();
	}
	for (const std::string &s : queries) {
		lenSum += (int)s.size();
	}
	assert(1 <= lenSum && lenSum <= 2 * 1000 * 1000);
	fprintf(stderr, "lenSum=%d\n", lenSum);
	std::vector<long long> qPurity(nQueries, 0);
	const int SMALL_GROUP_SIZE = 36;
	const int SMALL_GROUPS_PER_LARGE = 36;
	const int LARGE_GROUP_SIZE = SMALL_GROUP_SIZE * SMALL_GROUPS_PER_LARGE;
	{ // block used to free memory of 'single' before filling 'small'
		std::vector<std::vector<Query>> single(nGenes);
		for (int i = 0; i < nQueries; i++) {
			int left = qLeft[i];
			int right = qRight[i];
			int leftSmallGroup = (left + SMALL_GROUP_SIZE - 1) / SMALL_GROUP_SIZE;
			int rightSmallGroup = (right + 1) / SMALL_GROUP_SIZE - 1;
			if (leftSmallGroup > rightSmallGroup) {
				for (int j = left; j <= right; j++) {
					single[j].push_back(Query{ &queries[i], &qPurity[i] });
				}
			} else {
				for (int j = left, after = leftSmallGroup * SMALL_GROUP_SIZE; j < after; j++) {
					single[j].push_back(Query{ &queries[i], &qPurity[i] });
				}
				for (int j = (rightSmallGroup + 1) * SMALL_GROUP_SIZE; j <= right; j++) {
					single[j].push_back(Query{ &queries[i], &qPurity[i] });
				}
			}
		}
		for (int i = 0; i < nGenes; i++) {
			addSingle(genes[i], purity[i], single[i]);
		}
	}
	{ // block used to free memory of 'small' before filling 'large'
		std::vector<std::vector<Query>> small(nGenes / SMALL_GROUP_SIZE);
		for (int i = 0; i < nQueries; i++) {
			int left = qLeft[i];
			int right = qRight[i];
			int leftSmallGroup = (left + SMALL_GROUP_SIZE - 1) / SMALL_GROUP_SIZE;
			int rightSmallGroup = (right + 1) / SMALL_GROUP_SIZE - 1;
			int leftLargeGroup = (left + LARGE_GROUP_SIZE - 1) / LARGE_GROUP_SIZE;
			int rightLargeGroup = (right + 1) / LARGE_GROUP_SIZE - 1;
			if (leftLargeGroup > rightLargeGroup) {
				for (int j = leftSmallGroup; j <= rightSmallGroup; j++) {
					small[j].push_back(Query{ &queries[i], &qPurity[i] });
				}
			} else {
				for (int j = leftSmallGroup, after = leftLargeGroup * SMALL_GROUPS_PER_LARGE; j < after; j++) {
					small[j].push_back(Query{ &queries[i], &qPurity[i] });
				}
				for (int j = (rightLargeGroup + 1) * SMALL_GROUPS_PER_LARGE; j <= rightSmallGroup; j++) {
					small[j].push_back(Query{ &queries[i], &qPurity[i] });
				}
			}
		}
		for (int i = 0; i < nGenes / SMALL_GROUP_SIZE; i++) {
			int first = i * SMALL_GROUP_SIZE;
			int after = first + SMALL_GROUP_SIZE;
			addMulti(genes, purity, first, after, small[i]);
		}
	}
	{
		std::vector<std::vector<Query>> large(nGenes / LARGE_GROUP_SIZE);
		for (int i = 0; i < nQueries; i++) {
			int left = qLeft[i];
			int right = qRight[i];
			int leftLargeGroup = (left + LARGE_GROUP_SIZE - 1) / LARGE_GROUP_SIZE;
			int rightLargeGroup = (right + 1) / LARGE_GROUP_SIZE - 1;
			for (int j = leftLargeGroup; j <= rightLargeGroup; j++) {
				large[j].push_back(Query{ &queries[i], &qPurity[i] });
			}
		}
		for (int i = 0; i < nGenes / LARGE_GROUP_SIZE; i++) {
			int first = i * LARGE_GROUP_SIZE;
			int after = first + LARGE_GROUP_SIZE;
			addMulti(genes, purity, first, after, large[i]);
		}
	}
	long long min = qPurity[0];
	long long max = qPurity[0];
	for (long long p : qPurity) {
		min = std::min(min, p);
		max = std::max(max, p);
	}
	printf("%lld %lld\n", min, max);
	return 0;
}
