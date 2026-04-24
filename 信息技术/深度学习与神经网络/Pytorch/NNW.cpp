#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <cmath>
#include <random>
using namespace std;

#define sf scanf
#define pf printf
#define ll long long
#define db double
#define _for(i, a, b) for(int i = (a); i < (b); ++i)
#define _rfor(i, a, b) for(int i = (a); i <= (b); ++i)
#define _dfor(i, b, a) for(int i = (b); i >= (a); --i)
#define _fev(p, u) for(int p = head[u]; p; p = nex[p])
#define maxn 100005

template <class T>
void ap(string name, T arr[], int n, int flag = 1){
	cout<<name<<":";
	_for(i, 0, n)cout<<" "<<arr[i+flag];
	cout<<endl;
}

db idfun(db x){ return x; }
db didfun(db x){ return 1.0; } 
db tanh(db x){ return 1-2/(exp(2*x)+1); }
db dtanh(db x){ db y = 1/(exp(2*x)+1); return 4*y*(1-y); }

struct Learn{
	db *v, *pv;
	const int n;
	Learn(int n):n(n), v(new db[n]), pv(new db[n]){ _for(i, 0, n) v[i] = pv[i] = 0; }
	~Learn(){ delete[] v; delete[] pv; }
	virtual void learn(db *w, db *pw) = 0;
	void print() const{
		ap("v", v, n, 0);
		ap("pv", v, n, 0);	
	}
};

struct NesterovLearn:public Learn{
	static db alpha, beta;
	NesterovLearn(int n):Learn(n){}
	void learn(db *w, db *pw) override{
		//_for(i, 0, n) v[i] = beta*v[i] - alpha*pw[i], w[i] += v[i], pw[i] = 0;
		_for(i, 0, n) w[i] -= pw[i]*alpha, pw[i] = 0;
	}
};
db NesterovLearn::alpha = 1;
db NesterovLearn::beta = 0.75;

struct NNLayer{
	db *x, *px;
	const int n;
	NNLayer(int n):n(n), x(new db[n]), px(new db[n]){ }
	virtual void fp(NNLayer &that) = 0;
	virtual void bp(NNLayer &that) = 0;
	virtual void learn(int bth) = 0;
	virtual void print() = 0;
};

struct InputLayer:public NNLayer{
	InputLayer(int n):NNLayer(n){}
	void fp(NNLayer &that) override{ assert(false); }
	void bp(NNLayer &that) override{ assert(false); }
	void learn(int bth) override{ assert(false); }
	void print() override{ ap("x", x, n, 0); ap("px", x, n, 0); }
};

struct OutputLayer:public NNLayer{
	OutputLayer(int n):NNLayer(n){}
	~OutputLayer(){ }
	void learn(int bth) override{ assert(false); }
};

struct FCLayer:public NNLayer{
	db *w_, *b, *pz, *pw_, *pb;
	const int m;
	db (*func)(db x);
	db (*dfunc)(db x);
	const char *learn_type;
	Learn *learn_for_w;
	Learn *learn_for_b;
	
	FCLayer(int n, int m, db (*func)(db x), db (*dfunc)(db x), const char *learn_type)
	:	NNLayer(n), m(m), func(func), dfunc(dfunc), learn_type(learn_type),
		w_(new db[n*m]), pw_(new db[n*m]),
		b(new db[n]), pb(new db[n]),
		pz(new db[n])
	{
		if (strcmp(learn_type, "NesterovLearn") == 0){
			learn_for_w = new NesterovLearn(n*m);
			learn_for_b = new NesterovLearn(n);
		}
		else{
			puts("Undefined learn method!");
			assert(false);
		}
		assert(learn_for_w->n == n*m);
		assert(learn_for_b->n == n);
		_for(i, 0, n) _for(j, 0, m) pw(i, j) = 0;
		_for(i, 0, n) pz[i] = pb[i] = 0;
		xavier();
	}
	~FCLayer(){
		delete[] w_; delete[] b; delete[] pz; delete[] pw_; delete[] pb; 
		delete learn_for_w; delete learn_for_w;
	}
	void fp(NNLayer &that) override{
		assert(m == that.n);
		_for(i, 0, n){
			x[i] = pz[i] = 0;
			_for(j, 0, m) x[i] += w(i,j)*that.x[j]+b[i];
			pz[i] = dfunc(x[i]);
			x[i] = func(x[i]);
		}
	}
	void bp(NNLayer &that) override{
		assert(m == that.n);
		//pw 和 pb 不能在着清空，它们用来存一批的梯度之和
		_for(j, 0, m) that.px[j] = 0;
		_for(i, 0, n){
			db tmp = pz[i]*px[i];
			_for(j, 0, m){
				that.px[j] += w(i,j)*tmp;
				pw(i,j) += that.x[j]*tmp;
			}
			pb[i] += tmp;
		}
	}
	void learn(int bth_sz) override{
		_for(i, 0, n) _for(j, 0, m) pw(i,j) /= bth_sz;
		_for(i, 0, n) pb[i] /= bth_sz;
		assert(learn_for_w->n == n*m);
		assert(learn_for_b->n == n);
		learn_for_w->learn(w_, pw_);
		learn_for_b->learn(b, pb);
	}
	void xavier(){
		default_random_engine  engine(234);
		normal_distribution<db> dstr(0.0, sqrt(2.0/(n+m)));
		_for(i, 0, n) _for(j, 0, m) w(i, j) = dstr(engine);
		_for(i, 0, n) b[i] = 0;
	}

	db &w(int i, int j){ assert(0<=i&&i<n&&0<=j&&j<m); return w_[i*m+j]; }
	db &pw(int i, int j){ assert(0<=i&&i<n&&0<=j&&j<m); return pw_[i*m+j]; }
	
	void print(){
		pf("w\n");
		_for(i, 0, n){
			_for(j, 0, m) pf("%lf ", w(i,j));
			putchar('\n');
		}
		pf("b: ");
		_for(i, 0, n) pf("%lf ", b[i]);
		pf("\n");
		pf("pw\n");
		_for(i, 0, n){
			_for(j, 0, m) pf("%lf ", pw(i,j));
			putchar('\n');
		}
		pf("pb: ");
		_for(i, 0, n) pf("%lf ", pb[i]);
		pf("\n");
		pf("pz: ");
		_for(i, 0, n) pf("%lf ", pz[i]);
		pf("\n");
		pf("x: ");
		_for(i, 0, n) pf("%lf ", x[i]);
		pf("\n");
		pf("px: ");
		_for(i, 0, n) pf("%lf ", px[i]);
		pf("\n");
	}
};


struct SoftmaxLayer:public OutputLayer{
	SoftmaxLayer(int n):OutputLayer(n){}
	void fp(NNLayer &that) override{
		assert(n == that.n);
		db sum = 0;
		_for(i, 0, n) sum += x[i] = exp(that.x[i]);
		_for(i, 0, n) x[i] /= sum;
	}
	void bp(NNLayer &that) override{
		assert(n == that.n);
		
		_for(i, 0, n){
			that.px[i] = 0;
			_for(j, 0, n) that.px[i] += px[j]*x[j];
			that.px[i] = -x[i]*that.px[i] + x[i]*px[i];
		}
	}
	void print(){
		pf("x: ");
		_for(i, 0, n) pf("%lf ", x[i]);
		pf("\n");
		pf("px: ");
		_for(i, 0, n) pf("%lf ", px[i]);
		pf("\n");
	}
};

struct Loss{
	db L, *y;
	const int n;
	Loss(int n):L(0), n(n), y(new db[n]){}
	~Loss(){ delete[] y ; }
	void clr(){
		L = 0;
	}
	virtual void fp(NNLayer &that) = 0;
	virtual void bp(NNLayer &that) = 0;
};

struct CrossEntropyLoss:Loss{
	CrossEntropyLoss(int n):Loss(n){}
	void fp(NNLayer &that) override{
		assert(n == that.n);
		
		_for(i, 0, that.n){
			L -= y[i]*log(that.x[i]);
		}
	}
	void bp(NNLayer &that) override{
		assert(n == that.n);
		_for(i, 0, that.n){
			that.px[i] = -y[i]/that.x[i];
		}
	}
};

struct Data{
	db x[3], y[2];
};

Data *train_data, *test_data;
int train_sz, test_sz;
void print(const Data &data){
	pf("x:%lf %lf %lf y:%lf %lf\n", data.x[0], data.x[1], data.x[2], data.y[0], data.y[1]);
}

void get_data(db R = 1){
	mt19937(engine);
	uniform_real_distribution<double> dstr(-1.5*R, 1.5*R);
	train_data = new Data[train_sz+1];
	test_data = new Data[test_sz+1];
	
	//sphere
//	_rfor(id, 1, train_sz){
//		db sum = 0.0;
//		_for(i, 0, 3)
//			train_data[id].x[i] = dstr(engine), sum += train_data[id].x[i]*train_data[id].x[i];
//		_for(i, 0, 2)
//			train_data[id].y[i] = (sum >= R*R) ^ i;
//		assert(ck_data(train_data[id]));
//	}
	
	_rfor(id, 1, train_sz){
		_for(i, 0, 3)
			train_data[id].x[i] = dstr(engine);
		_for(i, 0, 2)
			train_data[id].y[i] = (train_data[id].x[0] <= 0) ^ i;
	}
	
	//square
	_rfor(id, 1, test_sz){
		db sum = 0.0;
		_for(i, 0, 3)
			test_data[id].x[i] = dstr(engine), sum += test_data[id].x[i]*test_data[id].x[i];
		_for(i, 0, 2)
			test_data[id].y[i] = (sum <= R*R) ^ i;
	}
}

FCLayer *pfc = 0;

struct MyNNW{
	int length;
	int *n;
	NNLayer **nnw;
	Loss *loss;
	MyNNW(){
		//length = 8; n = new int[length]{3, 10, 10, 10, 10, 10, 2, 2};
		//length = 13; n = new int[length]{3, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 2, 2};
		length = 3; n = new int[length]{3, 2, 2};
		nnw = new NNLayer*[length];
		nnw[0] = new InputLayer(n[0]);
		_for(i, 1, length-1)
			nnw[i] = pfc = new FCLayer(n[i], n[i-1], tanh, dtanh, "NesterovLearn");
		nnw[length-1] = new SoftmaxLayer(n[length-1]);

		loss = new CrossEntropyLoss(n[length-1]);
	}
	void run(const Data &data){
		//print(data);
		_for(i, 0, n[0]) nnw[0]->x[i] = data.x[i];
		_for(i, 0, loss->n) loss->y[i] = data.y[i];
		_for(i, 1, length) nnw[i]->fp(*nnw[i-1]);
		loss->fp(*nnw[length-1]);
		loss->bp(*nnw[length-1]);
		_dfor(i, length-1, 1) nnw[i]->bp(*nnw[i-1]);	
	}
	void ud_para(int bth_sz){
		_for(i, 1, length-1) nnw[i]->learn(bth_sz);
	}
	void train(int train_sz, const Data *train_data){
		int bth_cnt = 0, bth_sz = 128;
		
		NesterovLearn::alpha = 1;
		NesterovLearn::beta = 0.75;
		
		int steps = 1024*10000, print_sz = bth_sz*1000;
		_rfor(s, 1, steps){
			if (s % print_sz == 0){
				pf("train in step%d\n", s);
			}
			bth_cnt = bth_cnt%train_sz+1;
			run(train_data[bth_cnt]);
			

			if (s % print_sz == 0){
				
				pf("Loss L:%lf\n", loss->L/bth_sz);
//				
				_for(i, 1, length){
					pf("Layer:%d\n", i);
					nnw[i]->print();
					puts("");
				}
				putchar('\n');
				print(train_data[bth_cnt]);
			}
			
			if (bth_cnt % bth_sz == 0){
				ud_para(bth_sz);
				loss->clr();
			}
			
		}
	}
};

int main(){
	//freopen("sample.in", "r", stdin);
	
	MyNNW mynnw;

	train_sz = 2048; test_sz = 128;
	get_data();
	pf("Datas loaded\n"); 
	mynnw.train(train_sz, train_data);


	return 0;
} 



