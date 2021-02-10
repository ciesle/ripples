
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include<map>

#define rep(i,a) for(int (i)=0;(i)<(a);(i)++)
#define fs first
#define sc second
struct st {
	int val;
	int dir;
	int tim;
};
struct A {
	int i, j, val, dir, t;
};
Array<st>ar[100][100], ar2[100][100];
Rect box[100][100];
void Main()
{
	int h = 100, w = 100, sz = 20, it = 2;
	Scene::Resize(h * sz + (h - 1) * it, h * sz + (h - 1) * it);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			box[i][j] = Rect(j * (sz + it), i * (sz + it), sz, sz);
		}
	}

	using P = std::pair<int, int>;
	std::map<P, int> hash;
	Array<P> v;
	for (int i = -1; i <= 1; i++)for (int j = -1; j <= 1; j++) {
		if (i == j && j == 0) continue;
		hash[P{ i,j }] = v.size();
		v.push_back(P{ i,j });
	}
	int tim = 0, per = 2;
	double big = 16. / 17, sml = 8. / 17;
	while (System::Update()) {
		Scene::SetBackground(Palette::Black);
		if (MouseL.down()) {
			rep(i, h)rep(j, w) {
				if (box[i][j].mouseOver()) {
					ar[i][j].push_back(st{ 255,-1,tim });
				}
			}
		}
		rep(i, h)rep(j, w) {
			int sum = 0;
			for (auto g : ar[i][j]) sum += g.val;
			if (sum == 0) box[i][j].draw(Color(20, 20, 20));
			else box[i][j].draw(HSV(sum));
		}
		Array<A>ary;
		rep(i, h)rep(j, w) {
			int ss = ar[i][j].size();
			rep(t, ss) {
				auto g = ar[i][j][t];
				if (g.dir == -1) {
					if (g.val / 4 == 0) continue;
					rep(k, 8) ar[i][j].push_back(st{ g.val / 4,k,tim });
				}
			}
			for (auto g : ar[i][j]) {
				if (g.dir == -1) continue;
				if ((tim - g.tim) % per > 0) {
					ary.push_back(A{ i,j,g.val,g.dir,g.tim });
				}
				else {
					if (v[g.dir].fs == 0) {
						P k = v[g.dir];
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * big),hash[k],g.tim });
						k.fs = -1;
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * sml),hash[k],g.tim });
						k.fs = 1;
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * sml),hash[k],g.tim });
					}
					else if (v[g.dir].sc == 0) {
						P k = v[g.dir];
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * big),hash[k],g.tim });
						k.sc = -1;
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * sml),hash[k],g.tim });
						k.sc = 1;
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * sml),hash[k],g.tim });
					}
					else {
						P k = v[g.dir];
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * big),hash[k],g.tim });
						int t = 0;
						std::swap(k.fs, t);
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * sml),hash[k],g.tim });
						std::swap(k.fs, t);

						std::swap(k.sc, t);
						ary.push_back(A{ i + k.sc,j + k.fs,int(g.val * sml),hash[k],g.tim });
						std::swap(k.sc, t);
					}
				}
			}
		}

		for (auto g : ary) {
			if (g.val == 0) continue;

			P k = v[g.dir];
			if (g.i < 0) g.i = 0, k.sc = 1;
			if (g.i == h) g.i = h - 1, k.sc = -1;
			if (g.j < 0) g.j = 0, k.fs = 1;
			if (g.j == w) g.j = w - 1, k.fs = -1;
			ar2[g.i][g.j].push_back(st{ g.val,hash[k],g.t });
		}
		rep(i, h)rep(j, w) {
			ar[i][j] = ar2[i][j];
			ar2[i][j].clear();
		}
		tim++;
	}
}